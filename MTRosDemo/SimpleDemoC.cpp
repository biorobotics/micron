/********************************************************************
Simple application demonstrating the use the MTC library to obtain
measurements from one or more cameras.
********************************************************************/

#include "string.h"
#include "stdlib.h"
#include <stdio.h>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf2_ros/transform_broadcaster.h>
#include "MTC.h" //MTC.h need to be in the local directory or include path


//Macro to check for and report MTC usage errors.
#define MTC(func) {int r = func; if (r!=mtOK) printf("MTC error: %s\n",MTLastErrorString()); };

#ifdef WIN32
int getMTHome (  char *sMTHome, int size ); //Forward declaration
#endif

/********************************************************************/
int main(int argc, char* argv[])
/********************************************************************/
{

	printf("\n Simple MicronTracker app");
	printf("\n==========================\n");

	//Connect to the available cameras, and report on what was found
	//The first camera is designated as the "current" camera - we will use its coordinate
	//space in reporting pose measurements.
	//char MTHome[512];
	char calibrationDir[512];
	char markerDir[512];
	

	int result = 0;
        int getMTHome (  char *sMTHome, int size );
    	if ( getMTHome (calibrationDir, sizeof(calibrationDir)) < 0 ) {
		// No Environment
		return result;
	} else {
		sprintf(markerDir,"%s/Markers",calibrationDir);
		sprintf(calibrationDir,"%s/CalibrationFiles",calibrationDir);
	}
        
//#ifdef WIN32
//    if ( getMTHome (MTHome, sizeof(MTHome)) < 0 ) {
//		// No Environment
//		printf("MTHome environment variable is not set!\n");
//		return 0;
//	} else {
//		sprintf(calibrationDir,"%s\\CalibrationFiles",MTHome);
//		sprintf(markerDir,"%s\\Markers",MTHome);
//	}
//#else  //Linux & Mac OSX
	//sprintf(calibrationDir,"../../CalibrationFiles");
	//sprintf(markerDir,"../../Markers");
//#endif
	printf("\nCalibration File: %s\n", calibrationDir);
	MTC( Cameras_AttachAvailableCameras(calibrationDir) ); //Path to directory where the calibration files are
	if (Cameras_Count() < 1) {
		printf("No camera found!\n");
		return 0;
	}
	mtHandle CurrCamera, IdentifyingCamera;
	int		CurrCameraSerialNum;
	MTC( Cameras_ItemGet(0, &CurrCamera) ); //Obtain a handle to the first/only camera in the array
	MTC( Camera_SerialNumberGet(CurrCamera, &CurrCameraSerialNum) ); //obtain its serial number
	printf("Attached %d camera(s). Curr camera is %d\n", Cameras_Count(), CurrCameraSerialNum);

	int x, y;
	MTC (Camera_ResolutionGet(CurrCamera, &x, &y));
	printf("the camera resolution is %d, x %d", x, y);

	//Load the marker templates (with no validation).
	MTC( Markers_LoadTemplates(markerDir) ); //Path to directory where the marker templates are
	printf("Loaded %d marker templates\n",Markers_TemplatesCount());


	ros::init(argc, argv, "micron");
	ros::NodeHandle node;
	static tf2_ros::TransformBroadcaster br;

	//Create objects to receive the measurement results
	mtHandle IdentifiedMarkers = Collection_New();
	mtHandle PoseXf = Xform3D_New();
	int i, j;
	while (node.ok()) {
		MTC( Cameras_GrabFrame(NULL) ); //Grab a frame (all cameras together)
		MTC( Markers_ProcessFrame(NULL) ); //Process the frame(s) to obtain measurements
		if (i<20) continue; //the first 20 frames are auto-adjustment frames, and would be ignored here
		//Here, MTC internally maintains the measurement results.
		//Those results can be accessed until the next call to Markers_ProcessFrame, when they
		//are updated to reflect the next frame's content.
		//First, we will obtain the collection of the markers that were identified.
		MTC( Markers_IdentifiedMarkersGet(NULL, IdentifiedMarkers) );
		printf("%d: identified %d marker(s)\n", i, Collection_Count(IdentifiedMarkers));
		//Now we iterate on the identified markers (if any), and report their name and their pose
		ros::Time t = ros::Time::now();
		for (j=1; j<=Collection_Count(IdentifiedMarkers); j++) {
			//Obtain the marker's handle, and use it to obtain the pose in the current camera's space
			//  using our Xform3D object, PoseXf.
			mtHandle Marker = Collection_Int(IdentifiedMarkers, j);
			MTC( Marker_Marker2CameraXfGet (Marker, CurrCamera, PoseXf, &IdentifyingCamera) );
			//We check the IdentifyingCamera output to find out if the pose is, indeed,
			//available in the current camera space. If IdentifyingCamera==0, the current camera's
			//coordinate space is not registered with any of the cameras which actually identified
			//the marker.
			if (IdentifyingCamera != 0) { 
				char MarkerName[MT_MAX_STRING_LENGTH];
				double	Position[3], Angle[3];
				//We will also check and report any measurement hazard
				mtMeasurementHazardCode Hazard; 
				MTC( Marker_NameGet(Marker, MarkerName, MT_MAX_STRING_LENGTH, 0) );
				MTC( Xform3D_ShiftGet(PoseXf, Position) );
				//Here we obtain the rotation as a sequence of 3 angles. Often, it is more convenient
				//(and slightly more accurage) access the rotation as a 3x3 rotation matrix.
				MTC( Xform3D_RotAnglesDegsGet(PoseXf, &Angle[0], &Angle[1], &Angle[2]) );
				MTC( Xform3D_HazardCodeGet(PoseXf, &Hazard) );
				//Print the report
				printf(">> %s at (%0.2f, %0.2f, %0.2f), rotation (degs): (%0.1f, %0.1f, %0.1f) %s\n", 
					MarkerName, 
					Position[0], Position[1], Position[2], 
					Angle[0], Angle[1], Angle[1],
					MTHazardCodeString(Hazard));
				// Publish ROS tf
				double  quarternion[4];
				MTC( Xform3D_RotQuaternionsGet(PoseXf, quarternion) );
				geometry_msgs::TransformStamped transformStamped;
				transformStamped.header.stamp = t;
				transformStamped.header.frame_id = "micron/tracker";
				std::string frame_name = std::string(MarkerName);
				frame_name = "micron/marker_" + frame_name;
				transformStamped.child_frame_id = MarkerName;
				transformStamped.transform.translation.x = Position[0] / 1000.0;
				transformStamped.transform.translation.y = Position[1] / 1000.0;
				transformStamped.transform.translation.z = Position[2] / 1000.0;
				transformStamped.transform.rotation.x = quarternion[0];
				transformStamped.transform.rotation.y = quarternion[1];
				transformStamped.transform.rotation.z = quarternion[2];
				transformStamped.transform.rotation.w = quarternion[3];
				br.sendTransform(transformStamped);

			}

		}
		ros::spinOnce();
	}
	//free up all resources taken
	Collection_Free(IdentifiedMarkers);
	Xform3D_Free(PoseXf);
	Cameras_Detach(); //important - otherwise the cameras will continue capturing, locking up this process.

	return 0;
}

//#ifdef WIN32
/********************************************************************/
int getMTHome (  char *sMTHome, int size )
/********************************************************************/
{
#ifdef _WIN32
    LONG err;
    HKEY key;
    char *mfile = "MTHome";
    DWORD value_type;
    DWORD value_size = size;

    /* Check registry key to determine log file name: */
    if ( (err = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0,
	    KEY_QUERY_VALUE, &key)) != ERROR_SUCCESS ) {
		return(-1);
	}

    if ( RegQueryValueEx( key,
			mfile,
			0,	/* reserved */
			&value_type,
			(unsigned char*)sMTHome,
			&value_size ) != ERROR_SUCCESS || value_size <= 1 ){
		/* size always >1 if exists ('\0' terminator) ? */
		return(-1);
	}
#else
	char *localNamePtr = getenv("MTHome");
	if ( localNamePtr) {
		strncpy(sMTHome, localNamePtr, size-1);
		sMTHome[size] = '\0';
	} else {
//		sprintf(sMTHome,"/home/biomed/MicronTracker");
		return(-1);
	}
	

#endif
    return(0);
}
//#endif