/*****************************************************************************
* Shared data structures and function definitions for MicronTracker classes
*
* Copyrights (c)2001-2004 by Claron Technology Inc.
******************************************************************************/
/** \file
\brief Header file for client-side MTC data structures and function declarations.

The classes and functions are documented separately in their source file.
*/

#ifndef MTC_H
#define MTC_H


#ifdef _WIN32
	#define _WIN32_WINNT  0x0400 //for SetThreadIdealProcessor
	#include	<windows.h>
    #include	<winbase.h>
	#include	<time.h>   // for the MicronTrackerLogFile
	#include    <stdlib.h>  // for the MicronTrackerLogFile
    #include    <iostream>  // for the MicronTrackerLogFile
    #include    <fstream>  // for the MicronTrackerLogFile
	#define EXT 
	#define CCONV  _stdcall
#else
	#define EXT extern
	#define CCONV  
#ifndef BYTE
	typedef unsigned char BYTE;
#endif
#ifndef UINT
	typedef unsigned int UINT;
#endif
#ifndef ULONG
	typedef unsigned long ULONG;
#endif
#ifndef LONG
	typedef long LONG;
#endif
	#define TRUE 1
	#define FALSE 0
	#define VOID void
#endif

#  ifdef __cplusplus
extern "C" {
#  endif /* __cplusplus */

/* MTC Version */
#define MTCMajorVersion 3
#define MTCMinorVersion 7
#define MTCBuild 2
#define MTCRevision 3
#define MTCVERSION_STR "3, 7, 2, 3"

/*! Any handle to an MT object
*/
#ifdef _WIN64  //Defined for applications for Win64.
typedef long long mtHandle; 
#else 
#ifdef _LINUX64
  typedef long long mtHandle; 
#else
  typedef int mtHandle;  // for WIN32
#endif
#endif


const mtHandle mtHandleNull = 0;


/*! \name Limits*/
/*\{*/
const int MaxCameras = 10; /*!< Maximum number of cameras that can be concurrently attached. */
const int MaxSliderControlledXpoints = 200;
const int MaxXPointsPerImage = 8000; /*!< Maximum number of XPs concurrently detected and processed in each image. */
const int MaxVectorsPerFrame = 2000; /*!< Maximum number of vectors concurrently detected and processed.  */
#define MT_MAX_STRING_LENGTH    400	/*!< Maximum number of characters in a single string buffer passed in/out in calls. */

/*\}*/

/*! \name Enums and constants*/
/*\{*/
/*! Values returned from most MT function calls
	\sa MTLastErrorString()
*/
enum mtCompletionCode 
{	mtOK=0, /*!< OK */
	mtInvalidHandle, /*!< Invalid object handle */
	mtReentrantAccess, /*!< Reentrant access - library is not thread-safe */
	mtInternalMTError, /*!< Internal MicronTracker software error */
	mtNullPointer, /*!< Null pointer parameter */
	mtOutOfMemory,	/*!< Out of memory*/
	mtParameterOutOfRange,	/*!< Parameter out of range*/
	mtStringParamTooLong,	/*!< String parameter too long*/
	mtOutBufferTooSmall,	/*!< Insufficient space allocated by the client to the output buffer*/
	mtCameraNotInitialized,	/*!< Camera not initialized*/
	mtCameraAlreadyInitialized,	/*!< Camera already initialized - cannot be initialized twice*/
	mtCameraInitializationFailed,	/*!< Camera initialization failed*/
	mtCompatibilityError,  /*!< MTC is incompatible with a software module it calls*/
	mtDataNotAvailable, /*< Data requested is not available */
	mtUnrecognizedCameraModel,	/*!< Calibration file error: unrecognized camera model*/
	mtPathNotSet,	/*!< Path not set*/
	mtCannotAccessDirectory,	/*!< Cannot access the directory specified*/
	mtWriteToFileFailed,	/*!< Write to file failed*/
	mtInvalidIndex,	/*!< Invalid Index parameter*/
	mtInvalidSideI,	/*!< Invalid SideI parameter*/
	mtInvalidDivisor,	/*!< Invalid Divisor parameter*/
	mtEmptyCollection,	/*!< Attempting to access an item of an empty IntCollection*/
	mtInsufficientSamples,	/*!< Insufficient samples*/
	mtInsufficientSamplesWithinTolerance, 	/*!< Insufficient samples that fit within the acceptance tolerance*/
	mtOddNumberOfSamples,	/*!< Odd number of vector samples*/
	mtLessThan2Vectors,	/*!< Less than 2 vectors*/
	mtMoreThanMaxVectorsPerFacet,	/*!< More than maximum vectors per facet*/
	mtErrorExceedsTolerance,	/*!< Error exceeds tolerance*/
	mtInsufficientAngleBetweenVectors,	/*!< Insufficient angle between vectors*/
	mtFirstVectorShorterThanSecond,	/*!< First vector is shorter than the second*/
	mtVectorLengthsTooSimilar,	/*!< Vector lengths are too similar*/
	mtNullTemplateVector,	/*!< Template vector has 0 length*/
	mtTemplateNotSet,	/*!< The template has not been created or loaded*/
	mtCorruptTemplateFile,	/*!< Template file is corrupt*/
	mtMaxMarkerTemplatesExceeded,	/*!< Maximum number of marker templates allowed exceeded*/
	mtDifferentFacetsGeometryTooSimilar,	/*!< Geometries of different facets are too similar*/
	mtNoncompliantFacetDefinition,	/*!< Noncompliant facet definition*/
	mtCollectionContainsNonVectorHandles,	/*!< The SampledVectorPairsCollection contains non-Vector handles*/
	mtEmptyBuffer,	/*!< Empty pixels buffer*/
	mtDimensionsDoNotMatch,	/*!< Dimensions do not match*/
	mtOpenFileFailed,	/*!< File open failed*/
	mtReadFileFailed,	/*!< File read failed*/
	mtWriteFileFailed,	/*!< File write failed*/
	mtCannotOpenCalibrationFile,	/*!< Cannot open calibration file (typically named [driver]_[ser num].calib */
	mtNotACalibrationFile,	/*!< Not a calibration file*/
	mtCalibrationFileCorrupt,	/*!< Calibration file contents corrupt*/
	mtCalibrationFileDoesNotMatchCamera,	/*!< Calibration file was not generated from this camera*/
	mtCalibrationFileNotLoaded,	/*!< Calibration file not loaded*/
	mtIncorrectFileVersion,	/*!< Incorrect file version*/
	mtLocationOutOfMeasurementBounds, /*!< Input image location is out of bounds of the measurement volume*/
	mtCannotTriangulate, /*!< Input image locations do not triangulate to a valid 3-D point*/
	mtUnknownXform, /*!< Transform between coordinate spaces is unknown*/
	mtCameraNotFound, /*!< The given camera object was not found in the cameras array*/
	mtFeatureDataUnavailable, /*!< Feature Data unavailable for the current frame*/
	mtFeatureDataCorrupt, /*!< Feature Data is corrupt or incompatible with the current version*/
	mtXYZOutOfFOV, /*!< XYZ position is outside of calibrated field of view*/
	mtGrabFrameError,	/*!< Grab frame error*/
	mtGrabTimeOut,	/*!< Grab frame time out*/
	mtCannotCreateThread,	/*!< failed to create the thread*/
	mtHdrIsNotEnabled, /*!< HDR mode is not enabled yet*/
	mtFeatureNotSupported, /*!<Frame Embedded Info is not supported by the camera*/
	mtHDRFrameCycleNotSupported, /*!<Frame Cycle Number is not supported by the camera*/
	mtFeatureNotSupportedForX64, /*!<Feature not supported in the 64 bit version of the library */
	mtNonUniformLightOnCoolCard,  /*!<Non Uniform Light Distribution over the CoolCard */
	mtBackgroundProcessMutexError, /*!<Couldn't get the mutex */
	mtTooManyVectors, /*!<for disabling the unitracking if too many vectors detected */
	mtMethodIsObsolete, /*!<for disabling the unitracking if too many vectors detected */
	utInvalidVectors, /*!<Vectors either do not intersect or share a base/head XPoint with each other */
	utMismatchedVectors, /*!<The provided vectors didn't match to the provided template */
	mtInsufficientNumberOfXPoints, /*!<The API requires more xpoints that what provided */
	mtXPointsProcessingIsDisabled /*!<The XPoints Processing is not performed */
};

/*! Measurement hazards.
	\sa Xform3D_HazardCodeGet()
*/
enum mtMeasurementHazardCode { //In increasing level of "danger"
	mtNone=0,	/*!< No hazard - safe measurement */
	mtCustomCoolnessLoaded, /*!< Custom Coolness coefficients loaded, make sure the coefficients are correct */
	mtShadowOverXP, /*!< Part of an XPoint is shadowed, possibly compromising accuracy */
	mtCameraBelowMinimumOperatingTemperature, /*!< The internal camera temperature is below operating range, compromising accuracy */
	mtCameraAboveMaximumOperatingTemperature, /*!< The internal camera temperature is above operating range, compromising accuracy */
	mtRapidInternalTemperatureChange, /*!< The camera is undergoing a rapid temperature change, compromising accuracy */
	mtOutsideCalibratedVolume,  /*!< The marker is outside calibrated volume, ie, too far from the camera */
	mtOutsideExtendedCalibratedVolume, /*!< The marker is outside extended calibrated volume */
	mtCameraWarmingUp, /*!< The camera is not yet thermally stable enough to provide accurate measurements */
	mtDeficientMarkerTemplateForWarmupCorrection,
};

/*! LightCoolness constants
	\sa Camera_LightCoolnessSet(), Cameras_LightCoolnessSet(), Camera_HDRLightCoolnessSet(), Cameras_HDRLightCoolnessSet()
*/
const double mtDaylightCFL_7000K = 1.0; /*!< Daylight type compact fluorescent bulb (bluish-colored light) */
const double mtSunlight = 0.82;  /*!< Most indirect sunlight */
const double mtDaylightFluorescentTube_6500K = 0.8;	 /*!< Daylight type tube */
const double mtCoolWhiteFluorescentTube_4100K = 0.56;	/*!< Cool-white type tube */
const double mtSoftWhiteFluorescentTube_3000K = 0.27;	 /*!< Soft-white type tube */
const double mtSoftWhiteCFL_2700K = 0.16; /*!< Soft-white (common) compact fluorescent bulb */
const double mtHalogen = 0.02;	/*!< unfiltered halogen bulb */
const double mtIncandescent = 0; /*!< All types of filmaent-generated light */

/*! Class index for live object counters accessed via MTObjectsAlive()
	\sa MTObjectsAlive()
*/
enum mtObjType {
	mtCollection=0,		/*!< ;*/
	mtFacet,			/*!< ;*/
	mtPersistence,		/*!< ;*/
	mtStopWatch,		/*!< ;*/
	mtVector,			/*!< ;*/
	mtXform3D,			/*!< ;*/
	mtMarker,			/*!< ;*/
	mtXPoint,			/*!< ;*/
	mtCamera,			/*!< ;*/
	mtCloud,			/*!< ;*/
	ObjTypesCount
};

struct calRay{
  float X0, Y0;		//The ray intersection at Z=0
  double Dx2Dz, Dy2Dz; //The slope of the coordinates with respect to z
};

struct XPointType {
  double X, Y; //in image coordinates
  double TCX, TCY; //Thermally/light coolness corrected X/Y
  //Viewed from the XPoint (center):
  //- Line 0: dark side on the left
  //- Line 1: dark side on the right
  //The LineUnitV refers to the direction of end 0 of each line
  double LineUnitV[2][2]; //2 Lines x 2 coordinates (cosine, sine)
  double Lines[2][2][2];
  short BlackVal, WhiteValMin, WhiteValMax;
  short Valid; //if not 0, invalidates this XPoint (initialized to -1 when XPoint created)
};

struct Boundary {
    double Max_X;
    double Max_Y;
    double Max_Z;
    double Min_X;
    double Min_Y;
    double Min_Z;
    double Max_Dist;
    double Min_Dist;
    double Avg_Dist;
    double XPs_count_Left;
    double XPs_count_Right;
    double XPs_count_XYZ;
    double XPs_count_Max;
    double XPs_count_Min;
	double Deviation;
};


/*\}*/

/***********************************************************/
/** \name General */
/***********************************************************/
/*\{*/

EXT double CCONV MTTimeSecs();
EXT	void CCONV ResetMTTime();
EXT int CCONV MTObjectsAlive(mtObjType MTClass);
EXT void CCONV MTVersionGet(int *p_outMajor, int *p_outMinor, int *p_outBuild);
EXT void CCONV MTVersionGetEx(int *p_outMajor, int *p_outMinor, int *p_outBuild, int *p_outRevision);
EXT	mtCompletionCode CCONV MTLastError();
EXT char* CCONV MTLastErrorString();
EXT	char* CCONV MTHazardCodeString(mtMeasurementHazardCode Code);
EXT	int CCONV MTCopyString2Client(char *p_string, char *p_ClientMessageBuffer, int MessageBufferBytes);
EXT mtCompletionCode CCONV MicronTrackerLogFileSet(bool p_out);
EXT mtCompletionCode CCONV MicronTrackerLogFileGet(bool *p_out);

/*\}*/

/***********************************************************/
/** \name Collection */
/***********************************************************/
EXT mtHandle CCONV Collection_New();
EXT mtCompletionCode CCONV Collection_Free(mtHandle handle);
EXT mtCompletionCode CCONV Collection_Add(mtHandle handle, mtHandle intVal, double doubleVal);
EXT mtCompletionCode CCONV Collection_Remove(mtHandle handle, int index);
EXT mtCompletionCode CCONV Collection_Clear(mtHandle handle);
EXT int CCONV Collection_Count(mtHandle handle);
EXT mtCompletionCode CCONV Collection_Item(mtHandle handle, int index, 
							 mtHandle *p_outIntVal, double *p_outDoubleVal);
EXT mtCompletionCode CCONV Collection_LastItem(mtHandle handle, 
							 mtHandle *p_outIntVal, double *p_outDoubleVal);
EXT mtHandle CCONV Collection_Int(mtHandle handle, int index);
EXT double CCONV Collection_Double(mtHandle handle, int index);
/*\}*/

/***********************************************************/
/** \name Persistence */
/***********************************************************/
EXT mtHandle CCONV Persistence_New();
EXT mtCompletionCode CCONV Persistence_Free(mtHandle handle);
EXT mtCompletionCode CCONV Persistence_PathSet(mtHandle handle, const char *PathString);
EXT	mtCompletionCode CCONV Persistence_PathGet(mtHandle handle, 
						char *p_outBuffer, UINT BytesAllocated, int *p_outStrLen);
EXT mtCompletionCode CCONV Persistence_SectionSet(mtHandle handle, const char *SectionString);
EXT mtCompletionCode CCONV Persistence_SectionGet(mtHandle handle,
						char *p_outBuffer, UINT BytesAllocated, int *p_outStrLen);
EXT mtCompletionCode CCONV Persistence_SaveString(mtHandle handle, char *Name, char *Value);
EXT mtCompletionCode CCONV Persistence_RetrieveString(mtHandle handle, char *Name, char *p_DefaultValue, 
						char *p_outBuffer, int BytesAllocated, int *p_outStrLen);
EXT mtCompletionCode CCONV Persistence_SaveDouble(mtHandle handle, const char *Name, double Value);
EXT mtCompletionCode CCONV Persistence_RetrieveDouble(mtHandle handle, const char *Name, 
						double DefaultValue, double *p_outValue);
EXT mtCompletionCode CCONV Persistence_SaveInt(mtHandle handle, const char *Name, int Value);
mtCompletionCode CCONV Persistence_RetrieveInt(mtHandle handle, const char* Name,
						int DefaultValue, int *p_outValue);

/*\}*/

/***********************************************************/
/** \name StopWatch */
/***********************************************************/
EXT mtHandle CCONV StopWatch_New();
EXT mtCompletionCode CCONV StopWatch_Free(mtHandle handle);
EXT mtCompletionCode CCONV StopWatch_ResetAndGo(mtHandle handle);
EXT mtCompletionCode CCONV StopWatch_ResetAndPause(mtHandle handle);
EXT mtCompletionCode CCONV StopWatch_Pause(mtHandle handle);
EXT mtCompletionCode CCONV StopWatch_Go(mtHandle handle);
EXT mtCompletionCode CCONV StopWatch_ElapsedMicroSecsGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV StopWatch_ElapsedMilliSecsGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV StopWatch_ElapsedSecsGet(mtHandle handle, double *p_out);
/*\}*/

/***********************************************************/
/** \name Xform3D */
/***********************************************************/
EXT mtHandle CCONV Xform3D_New();
EXT mtHandle CCONV Xform3D_NewAs(mtHandle orgHandle);
EXT mtCompletionCode CCONV Xform3D_SetStateAs(mtHandle handle, mtHandle SourceHandle);
EXT mtCompletionCode CCONV Xform3D_Free(mtHandle handle);
EXT mtCompletionCode CCONV Xform3D_RotMatGet(mtHandle handle, double *p_out3x3);
EXT mtCompletionCode CCONV Xform3D_RotMatSet(mtHandle handle, double *p_newval3x3);
EXT mtCompletionCode CCONV Xform3D_RotQuaternionsSet(mtHandle handle, double *p_4Quaternions);
EXT mtCompletionCode CCONV Xform3D_RotQuaternionsGet(mtHandle handle, double *p_out4Quaternions);
EXT mtCompletionCode CCONV Xform3D_RotAnglesRadsSet(mtHandle handle, double RadsAroundX, double RadsAroundY, double RadsAroundZ);
EXT mtCompletionCode CCONV Xform3D_RotAnglesDegsSet(mtHandle handle, double DegsAroundX, double DegsAroundY, double DegsAroundZ);
EXT mtCompletionCode CCONV Xform3D_RotAnglesRadsGet(mtHandle handle, double *p_outRadsAroundX, double *p_outRadsAroundY, double *p_outRadsAroundZ);
EXT mtCompletionCode CCONV Xform3D_RotAnglesDegsGet(mtHandle handle, double *p_outDegsAroundX, double *p_outDegsAroundY, double *p_outDegsAroundZ);
EXT mtCompletionCode CCONV Xform3D_NormalizeRotationMatrix(mtHandle handle);
EXT mtCompletionCode CCONV Xform3D_ShiftGet(mtHandle handle, double *p_out3);
EXT mtCompletionCode CCONV Xform3D_ShiftSet(mtHandle handle, double *newval3);
EXT mtCompletionCode CCONV Xform3D_UnitScaleGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Xform3D_UnitScaleSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Xform3D_MTTimeStampSecsGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Xform3D_MTTimeStampSecsSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Xform3D_HazardCodeGet(mtHandle handle, mtMeasurementHazardCode *p_out);
EXT	mtCompletionCode CCONV Xform3D_IsIdentityGet(mtHandle handle, bool *p_out);
EXT mtCompletionCode CCONV Xform3D_XformLocation(mtHandle handle, double *p_SourceV3, double *p_outTargetV3);
EXT mtCompletionCode CCONV Xform3D_RotateLocation(mtHandle handle, double *p_SourceV3, double *p_outTargetV3, bool ScaleIt);
EXT mtCompletionCode CCONV Xform3D_Translate(mtHandle handle, double *p_TranslationV3, bool Subtract);
EXT mtCompletionCode CCONV Xform3D_Concatenate(mtHandle handle, mtHandle SubsequentXform3DHandle, mtHandle ResultHandle);
EXT mtCompletionCode CCONV Xform3D_Inverse(mtHandle handle, mtHandle ResultHandle);
EXT mtCompletionCode CCONV Xform3D_InBetween(mtHandle handle, mtHandle OtherHandle, double OtherFract0to1, mtHandle ResultHandle);
EXT mtCompletionCode CCONV Xform3D_AngularDifference(mtHandle handle, mtHandle OtherHandle, 
							double *pout_AngleRadians, double *pout_AxisUnitVector3);
EXT mtCompletionCode CCONV Xform3D_Store(mtHandle handle, mtHandle PersistenceHandle, char *SectionName);
EXT mtCompletionCode CCONV Xform3D_Restore(mtHandle handle, mtHandle PersistenceHandle, char *SectionName);
/*\}*/

/***********************************************************/
/** \name Vector */
/***********************************************************/
EXT mtHandle CCONV Vector_New();
EXT mtHandle CCONV Vector_NewAs(mtHandle SourceHandle);
EXT mtCompletionCode CCONV Vector_SetStateAs(mtHandle handle, mtHandle SourceHandle);
EXT mtCompletionCode CCONV Vector_Free(mtHandle handle);
EXT mtCompletionCode CCONV Vector_EndPosGet(mtHandle handle, double *p_out2x3);
EXT mtCompletionCode CCONV Vector_EndPosSet(mtHandle handle, double *p_out2x3);
EXT mtCompletionCode CCONV Vector_EndXPointsGet(mtHandle handle, double *p_out3x2x2);
EXT mtCompletionCode CCONV Vector_IdentifyingCameraHandleGet(mtHandle VectorHandle, mtHandle *p_outIdentifyingCameraHandle);
EXT mtCompletionCode CCONV Vector_PixelValueRangeGet(mtHandle handle, short *p_outMinLevel, short *p_outMaxLevel);
EXT mtCompletionCode CCONV Vector_LengthGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Vector_UnitVGet(mtHandle handle, double *p_out3);
EXT mtCompletionCode CCONV Vector_Store(mtHandle handle, mtHandle PersistenceHandle, char *SectionName);
EXT mtCompletionCode CCONV Vector_Restore(mtHandle handle, mtHandle PersistenceHandle, char *SectionName);
EXT mtCompletionCode CCONV Vector_MaxPosDist(mtHandle handle, mtHandle OtherVectorHandle, double *p_out);
EXT	mtCompletionCode CCONV Vector_Xform(mtHandle handle, mtHandle Xform3DHandle);
/*\}*/

/***********************************************************/
/** \name Facet */
/***********************************************************/
EXT mtHandle CCONV Facet_New();
EXT mtHandle CCONV Facet_NewAs(mtHandle orgHandle);
EXT mtCompletionCode CCONV Facet_Free(mtHandle handle);
EXT mtCompletionCode CCONV Facet_TemplateVectorsGet(mtHandle handle, mtHandle LongVectorHandle, mtHandle ShortVectorHandle);
EXT mtCompletionCode CCONV Facet_TemplateVectorsSet(mtHandle handle, mtHandle LongVectorHandle, mtHandle ShortVectorHandle);
EXT	mtCompletionCode CCONV Facet_TemplatePositionsGet(mtHandle handle, double *p_outPositions4x3);
EXT mtCompletionCode CCONV Facet_IdentifiedVectorsGet(mtHandle handle, mtHandle LongVectorHandle, mtHandle ShortVectorHandle);
EXT	mtCompletionCode CCONV Facet_MeasuredPositionsGet(mtHandle handle, double *p_outPositions4x3);
EXT mtCompletionCode CCONV Facet_IdentifiedMatchRMSEGet(mtHandle handle, mtHandle CameraHandle, double *p_out);
EXT mtCompletionCode CCONV Facet_WasIdentifiedGet(mtHandle handle, mtHandle CameraHandle, bool *p_out);
EXT mtCompletionCode CCONV Facet_IdentifiedXPointsGet(mtHandle handle, mtHandle CameraHandle, double *p_outXPCoords2x3x2x2);
EXT mtCompletionCode CCONV Facet_Facet2CameraXfGet(mtHandle handle, mtHandle CameraHandle, mtHandle ResultXfHandle);
EXT mtCompletionCode CCONV Facet_Identify(mtHandle handle, mtHandle CameraHandle, mtHandle VectorsHandles[], int VectorsNum, bool *p_out);
EXT mtCompletionCode CCONV Facet_ValidateTemplateVectors(mtHandle handle);
EXT mtCompletionCode CCONV Facet_SetTemplateVectorsFromSamples(mtHandle handle, mtHandle SampledVectorPairsCollection, double MaxSampleErrorAllowedMM);
EXT mtCompletionCode CCONV Facet_SetTemplateVectorsFromSamplesUT(mtHandle handle, 
															 mtHandle SampledVectorPairsCollectionHandle1,
															 mtHandle SampledVectorPairsCollectionHandle2,
															 mtHandle CalibratorXfCollectionHandle1,
															 mtHandle CalibratorXfCollectionHandle2,
															 double MaxSampleErrorAllowedMM);
EXT mtCompletionCode CCONV Facet_StoreTemplate(mtHandle handle, mtHandle PersistenceHandle, const char *SectionName);
EXT mtCompletionCode CCONV Facet_RestoreTemplate(mtHandle handle, mtHandle PersistenceHandle, const char *SectionName);

/*\}*/

/***********************************************************/
/** \name Marker */
/***********************************************************/
EXT mtHandle CCONV Marker_New();
EXT mtHandle CCONV Marker_NewAs( mtHandle orgHandle);
EXT mtCompletionCode CCONV Marker_Free(mtHandle handle);

EXT mtCompletionCode CCONV Marker_WasIdentifiedGet(mtHandle handle, mtHandle CameraHandle, bool *p_out);
EXT mtCompletionCode CCONV Marker_MarkerWasIdentifiedInRefSpaceGet(mtHandle handle, mtHandle CameraHandle, bool *p_out);
EXT	mtCompletionCode CCONV Marker_Marker2CameraXfGet(mtHandle handle, mtHandle CameraHandle, mtHandle outXfHandle, mtHandle *p_outIdentifyingCameraHandle);
EXT	mtCompletionCode CCONV Marker_Tooltip2MarkerXfGet(mtHandle handle, mtHandle XfHandle);
EXT	mtCompletionCode CCONV Marker_Tooltip2MarkerXfSet(mtHandle handle, mtHandle XfHandle);
EXT mtCompletionCode CCONV Marker_ReferenceMarkerHandleGet(mtHandle handle, mtHandle *RefHandle);
EXT mtCompletionCode CCONV Marker_ReferenceMarkerHandleSet(mtHandle handle, mtHandle RefHandle);
EXT mtCompletionCode CCONV Marker_Tooltip2ReferenceXfGet(mtHandle handle, mtHandle CameraHandle, mtHandle XfHandle, mtHandle *p_outIdentifyingCameraHandle);
EXT mtCompletionCode CCONV Marker_Marker2ReferenceXfGet(mtHandle handle, mtHandle CameraHandle, mtHandle XfHandle, mtHandle *p_outIdentifyingCameraHandle);
EXT mtCompletionCode CCONV Marker_IdentifiedFacetsGet(mtHandle handle, mtHandle CameraHandle, bool copy, mtHandle outCollectionHandle);
EXT mtCompletionCode CCONV Marker_NameSet(mtHandle handle, char *pName);
EXT mtCompletionCode CCONV Marker_NameGet(mtHandle handle, char *p_outNameBuffer, int NameBufferChars, int *p_outActualChars);
EXT mtCompletionCode CCONV Marker_TemplateFacetsGet(mtHandle handle, mtHandle *FacetsCollHandle);
EXT	mtCompletionCode CCONV Marker_FacetIndexGet(mtHandle handle, mtHandle FacetHandle, int *p_out1based);
EXT mtCompletionCode CCONV Marker_IdentifiedFacetIndexGet(mtHandle handle, mtHandle CameraHandle,  mtHandle IdentifiedFacetHandle, int *TemplateFacetIndex);

EXT mtCompletionCode CCONV Marker_AddTemplateFacet(mtHandle handle, mtHandle NewFacetHandle, mtHandle Facet1ToNewFacetXfHandle );
EXT mtCompletionCode CCONV Marker_RemoveTemplateFacet(mtHandle handle, mtHandle FacetToRemoveHandle);
EXT mtCompletionCode CCONV Marker_ResetIdentifiedFacets(mtHandle handle, mtHandle CamHandle);
EXT	mtCompletionCode CCONV Marker_Identify(mtHandle handle, mtHandle CameraHandle, mtHandle VectorsHandles[], int VectorsNum, mtHandle outCollectionHandle);
EXT mtCompletionCode CCONV Marker_ValidateTemplate(mtHandle handle);
EXT mtCompletionCode CCONV Marker_RestoreTemplate(mtHandle handle,  mtHandle PersistenceHandle, const char *NameInP);
EXT mtCompletionCode CCONV Marker_StoreTemplate(mtHandle handle, mtHandle PersistenceHandle, const char *NameInP);
EXT mtCompletionCode CCONV Marker_JitterFilterCoefficientSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Marker_JitterFilterCoefficientGet(mtHandle handle, double *newval);
EXT mtCompletionCode CCONV Marker_AngularJitterFilterCoefficientSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Marker_AngularJitterFilterCoefficientGet(mtHandle handle, double *newval);
mtCompletionCode CCONV Marker_ExtrapolatedFramesSet(mtHandle handle, int p_out);
mtCompletionCode CCONV Marker_ExtrapolatedFramesGet(mtHandle handle, int *p_out);

EXT mtCompletionCode CCONV Marker_SliderControlledXpointsCountSet(mtHandle handle, int p_out);
EXT mtCompletionCode CCONV Marker_SliderControlledXpointsCountGet(mtHandle handle, int *p_out);
EXT mtCompletionCode CCONV Marker_SliderControlledXpointControlPointsCountGet(mtHandle handle, int SliderControlledXpointIndex, int *p_out);
EXT mtCompletionCode CCONV Marker_MarkerSliderControlledGet(mtHandle handle, bool *p_out);
EXT mtCompletionCode CCONV Marker_MarkerSliderControlledSet(mtHandle handle, bool p_out);
EXT mtCompletionCode CCONV Marker_IsSliderControlledXpointDetected(mtHandle handle, int SliderControlledXpointIndex, bool *p_out);
EXT mtCompletionCode CCONV Marker_SliderControlledXpointFractionGet(mtHandle handle, int SliderControlledXpointIndex, double *Fraction);
EXT mtCompletionCode CCONV Marker_DetectedSliderControlledXpointsGet(mtHandle handle, mtHandle p_outDetectedSliderControlledXpointsHandlesCollection);
EXT mtCompletionCode CCONV Marker_SliderControlledTooltip2MarkerXfGet(mtHandle handle, int SliderControlledXpointId, mtHandle XfHandle);
EXT mtCompletionCode CCONV Marker_SliderControlledTooltip2ReferenceXfGet(mtHandle handle, mtHandle CameraHandle, mtHandle XfHandle, mtHandle *p_outIdentifyingCameraHandle);
EXT mtCompletionCode CCONV Marker_AddTemplateSliderControlledTooltip2markerXf(mtHandle handle, int SliderControlledXpointId, mtHandle NewSliderTooltip2MarkerHandle);
EXT mtCompletionCode CCONV Marker_RemoveTemplateSliderControlledTooltip2markerXf(mtHandle handle, int SliderControlledXpointId, mtHandle SliderTooltip2MarkerToRemoveHandle);
EXT mtCompletionCode CCONV Marker_TemplateSliderControlledTooltip2MarkerGet(mtHandle handle, mtHandle *p_outTemplateSliderControlledTooltipHandlesCollection);
EXT mtCompletionCode CCONV Marker_AddTemplateSliderControlledXpoint(mtHandle handle, int SliderControlledXpointId, mtHandle NewSliderControlledXpointHandle);
EXT mtCompletionCode CCONV Marker_RemoveTemplateSliderControlledXpoint(mtHandle handle, int SliderControlledXpointId, mtHandle SliderControlledXpointToRemoveHandle);
EXT mtCompletionCode CCONV Marker_TemplateSliderControlledXpointsGet(mtHandle handle, int SliderControlledXpointIndex, mtHandle *p_outTemplateSliderControlledXpointsHandlesCollection);
EXT mtCompletionCode CCONV Marker_TemplateFacet1ToFacetXfsGet(mtHandle handle, int FacetIndex, mtHandle *Facet1ToFacetXf);

/*\}*/

/***********************************************************/
/** \name XPoint */
/***********************************************************/
EXT mtHandle CCONV XPoint_New();
EXT mtHandle CCONV XPoint_NewAs(mtHandle SourceHandle);
EXT mtCompletionCode CCONV XPoint_Free(mtHandle handle);
EXT mtCompletionCode CCONV XPoint_IndexGet(mtHandle handle, int *p_index);
EXT mtCompletionCode CCONV XPoint_IndexSet(mtHandle handle, int  index);
EXT mtCompletionCode CCONV XPoint_3DPositionGet(mtHandle handle, double *p_outX, double *p_out_Y, double *p_outZ);
EXT mtCompletionCode CCONV XPoint_3DPositionSet(mtHandle handle, double x, double y, double z);

EXT mtCompletionCode CCONV XPoint_2DPositionGet(mtHandle handle, double *p_outXL, double *p_out_YL, double *p_out_XR, double *p_outYR, double *p_outXM, double *p_outYM);
EXT mtCompletionCode CCONV XPoint_DistGet(mtHandle handle, double *Dist);
EXT mtCompletionCode CCONV XPoint_XPStructGet(mtHandle handle, XPointType *LXP, XPointType *RXP, XPointType *MXP);
EXT mtCompletionCode CCONV XPoint_Store(mtHandle handle, mtHandle PersistenceHandle, char *SectionName);
EXT mtCompletionCode CCONV XPoint_Restore(mtHandle handle, mtHandle PersistenceHandle, char *SectionName);
EXT mtCompletionCode CCONV XPoint_SetStateAs(mtHandle handle, mtHandle SourceHandle);


/*\}*/

/***********************************************************/
/** \name XPoints */
/***********************************************************/
int CCONV XPoints_Counter(mtHandle CameraHandle);
EXT mtCompletionCode CCONV XPoints_ClearXPoints();
EXT mtCompletionCode CCONV XPoints_DetectedXPointsGet(mtHandle CameraHandle, mtHandle outIdentifiedCollectionHandle );
EXT mtCompletionCode CCONV XPoints_OverExposureControlInterleaveGet(int *p_out);
EXT mtCompletionCode CCONV XPoints_OverExposureControlInterleaveSet(int newval);
EXT mtCompletionCode CCONV XPoints_SensitivityGet(int *p_out);
EXT mtCompletionCode CCONV XPoints_SensitivitySet(int newval);
EXT mtCompletionCode CCONV XPoints_MisalignmentSensitivitySet(int newval);
EXT mtCompletionCode CCONV XPoints_MisalignmentSensitivityGet(int *p_out);
EXT mtCompletionCode CCONV XPoints_AutoAdjustCameraExposureGet(bool *p_out);
EXT mtCompletionCode CCONV XPoints_AutoAdjustCameraExposureSet(bool newval);
EXT mtCompletionCode CCONV XPoints_InvalidateUnidentifiedFacetsXPointsGet(bool *p_out);
EXT mtCompletionCode CCONV XPoints_InvalidateUnidentifiedFacetsXPointsSet(bool p_out);
EXT void CCONV XPoints_JitterFilterEnabledSet(bool newval);
EXT bool CCONV XPoints_JitterFilterEnabled();
EXT mtCompletionCode CCONV XPoints_ProcessFrame(mtHandle CamHandle);
EXT mtCompletionCode CCONV XPoints_XPointsBoundariesGet(mtHandle CameraHandle, Boundary *boundaries);
EXT mtCompletionCode CCONV XPoints_BackGroundProcessSet(bool newval);
EXT mtCompletionCode CCONV XPoints_BackGroundProcessGet(bool *p_out);
EXT mtCompletionCode CCONV XPoints_GetIdentifiedXPointsFromBackgroundThread(mtHandle CameraHandle);
EXT mtCompletionCode CCONV XPoints_BackgroundProcessingMutexSet(bool newval);
EXT mtCompletionCode CCONV XPoints_BackgroundProcessingMutexGet(bool *newval);


/*\}*/


/***********************************************************/
/** \name Markers */
/***********************************************************/
EXT int CCONV Markers_TemplatesCount();
EXT mtCompletionCode CCONV Markers_TemplatesMarkersGet(mtHandle outTemplatesCollectionHandle);
EXT mtCompletionCode CCONV Markers_TemplateItemGet(int index, mtHandle *p_outHandle );
EXT mtCompletionCode CCONV Markers_IdentifiedMarkersGet(mtHandle CameraHandle, mtHandle outIdentifiedCollectionHandle );
EXT mtCompletionCode CCONV Markers_UnidentifiedVectorsGet(mtHandle CameraHandle, mtHandle outUnidentifiedCollectionHandle );
EXT mtCompletionCode CCONV Markers_TemplateMatchToleranceMMGet(double *p_out);
EXT mtCompletionCode CCONV Markers_TemplateMatchToleranceMMDefaultGet(double *p_out);
EXT mtCompletionCode CCONV Markers_SliderControlledTemplateMatchToleranceMMGet(double *p_out);
EXT mtCompletionCode CCONV Markers_SliderControlledTemplateMatchToleranceMMSet(double newval);
EXT mtCompletionCode CCONV Markers_TemplateMatchToleranceMMSet(double newval);
EXT mtCompletionCode CCONV Markers_AngularDotProductToleranceDegGet(double *p_out);
EXT mtCompletionCode CCONV Markers_AngularDotProductToleranceDegDefaultGet(double *p_out);
EXT mtCompletionCode CCONV Markers_AngularDotProductToleranceDegSet(double newval);
EXT mtCompletionCode CCONV Markers_PredictiveFramesInterleaveGet(int *p_out);
EXT mtCompletionCode CCONV Markers_PredictiveFramesInterleaveSet(int newval);
EXT mtCompletionCode CCONV Markers_OverExposureControlInterleaveGet(int *p_out);
EXT mtCompletionCode CCONV Markers_OverExposureControlInterleaveSet(int newval);
EXT mtCompletionCode CCONV Markers_AutoAdjustCameraExposureGet(bool *p_out);
EXT mtCompletionCode CCONV Markers_AutoAdjustCameraExposureSet(bool newval);
EXT mtCompletionCode CCONV Markers_AutoAdjustHDRminLuxGet(bool *p_out);
EXT mtCompletionCode CCONV Markers_AutoAdjustHDRminLuxSet(bool newval);
EXT mtCompletionCode CCONV Markers_TemplateBasedWarmupCorrectionGet(bool *p_out);
EXT mtCompletionCode CCONV Markers_TemplateBasedWarmupCorrectionSet(bool newval);
EXT mtCompletionCode CCONV Markers_JitterFilterCoefficientGet(double *p_out);
EXT mtCompletionCode CCONV Markers_JitterFilterCoefficientSet(double newval);
EXT mtCompletionCode CCONV Markers_AngularJitterFilterCoefficientGet(double *p_out);
EXT mtCompletionCode CCONV Markers_AngularJitterFilterCoefficientSet(double newval);
EXT mtCompletionCode CCONV Markers_JitterFilterHistoryLengthGet(int *p_out);
EXT mtCompletionCode CCONV Markers_JitterFilterHistoryLengthSet(int p_out);

EXT void CCONV Markers_JitterFilterEnabledSet(bool newval);
EXT bool CCONV Markers_JitterFilterEnabled();
EXT void CCONV Markers_KalmanFilterEnabledSet(bool newval);
EXT bool CCONV Markers_KalmanFilterEnabled();
EXT mtCompletionCode CCONV Markers_AutoAdjustCam2CamRegistrationGet( bool *p_out);
EXT mtCompletionCode CCONV Markers_AutoAdjustCam2CamRegistrationSet( bool newval);
EXT mtCompletionCode CCONV Markers_LoadTemplates(char *DirPath);
EXT mtCompletionCode CCONV Cameras_RestoreLightCoolnessCoefficients(char *DirPath);
EXT mtCompletionCode CCONV Markers_ClearTemplates();
EXT mtCompletionCode CCONV Markers_AddTemplate(mtHandle MarkerHandle);
EXT mtCompletionCode CCONV Markers_Validate(char outTextLines[][120], int LinesCount, int *p_outLinesFilled);
EXT mtCompletionCode CCONV Markers_ProcessFrame(mtHandle CameraHandle);
EXT mtCompletionCode CCONV Markers_ExtrapolatedFramesGet(int *p_out);
EXT mtCompletionCode CCONV Markers_ExtrapolatedFramesSet(int newval);
EXT mtCompletionCode CCONV Markers_BackGroundProcessSet(bool newval);
EXT mtCompletionCode CCONV Markers_BackGroundProcessGet(bool *p_out);
EXT mtCompletionCode CCONV Markers_BackgroundProcessingMutexSet(bool newval);
EXT mtCompletionCode CCONV Markers_BackgroundProcessingMutexGet(bool *newval);
EXT mtCompletionCode CCONV Markers_IsBackgroundFrameProcessedGet(bool *p_out);
EXT void CCONV Markers_SmallerXPFootprintSet(bool newval);
EXT bool CCONV Markers_SmallerXPFootprint();
/*\}*/

/***********************************************************/
/** \name Camera */
/***********************************************************/
EXT mtCompletionCode CCONV Camera_ThermalWarningSensitivitySet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Camera_ThermalWarningSensitivityGet(mtHandle handle, double *p_out);
EXT int CCONV Camera_DriverNameGet(mtHandle handle,  char *p_out, int max_size);
EXT int CCONV Camera_ModelNameGet(mtHandle handle,  char *p_out, int max_size);
EXT mtCompletionCode CCONV Camera_BitsPerPixelGet(mtHandle handle, int *p_out);
EXT mtCompletionCode CCONV Camera_FramesGrabbedGet(mtHandle handle, int *p_out);
EXT mtCompletionCode CCONV Camera_ResolutionGet(mtHandle handle, int *p_outXres, int *p_outYres);
EXT mtCompletionCode CCONV Camera_NumofSensorsGet(mtHandle handle, int *p_out);
EXT mtCompletionCode CCONV Camera_SerialNumberGet(mtHandle handle, int *p_out);
EXT mtCompletionCode CCONV Camera_OpticalCenterGet(mtHandle handle, double *pLeftX, double *pLeftY, 
												double *pRightX, double *pRightY);
EXT mtCompletionCode CCONV Camera_OpticalCenterGet3(mtHandle handle, double *pLeftX, double *pLeftY, 
												double *pRightX, double *pRightY, double *pMiddleX, double *pMiddleY);
EXT mtCompletionCode CCONV Camera_FirmwareVersionGet(mtHandle handle, int *p_out_Major, int *p_outMinor, int *p_outType, int *p_outTev);
EXT mtCompletionCode CCONV Camera_MainBoardInfoGet(mtHandle handle, int *p_outVersion);
EXT mtCompletionCode CCONV Camera_SensorBoardInfoGet(mtHandle handle, int *p_outSensor1, int *p_outSensor2);
EXT mtCompletionCode CCONV Camera_LastFrameTemperatureGet(mtHandle handle, double *p_outCelsius);
EXT mtCompletionCode CCONV Camera_LastFrameTemperatureSet(mtHandle handle, double Celsius);
EXT mtCompletionCode CCONV Camera_LastFrameTemperatureRateGet(mtHandle handle, double *p_outDph);
EXT mtCompletionCode CCONV Camera_LastFrameTemperatureRateSet(mtHandle handle, double Dph);
EXT mtCompletionCode CCONV Camera_SecondsFromPowerupGet(mtHandle handle, unsigned long *p_out);
EXT mtCompletionCode CCONV Camera_GainFGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_GainFSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Camera_GainFMinGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_GainFMaxGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_GainDBGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_ShutterMsecsGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_ShutterMsecsSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Camera_ShutterMsecsMinGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_ShutterMsecsMaxGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_ExposureMinGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_ExposureMaxGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_ExposureGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_ExposureSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Camera_LuxPerPixelUnitGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_LuxAtXPointGet(mtHandle handle, double XPoint_X, double XPoint_Y, int SideI, double *p_outLuxAtXPoint);
EXT mtCompletionCode CCONV Camera_ShutterMsecsLimiterSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Camera_ShutterMsecsLimiterGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_GainFLimiterGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_GainFLimiterSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Camera_AutoExposureGet(mtHandle handle, int *p_out);
EXT mtCompletionCode CCONV Camera_AutoExposureSet(mtHandle handle, int newval);
EXT mtCompletionCode CCONV Camera_LightCoolnessGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_HDRLightCoolnessGet(mtHandle handle, double *p_out, int light_Index);
EXT mtCompletionCode CCONV Camera_HDRLightCoolnessSet(mtHandle handle, double newval, int light_Index);
EXT mtCompletionCode CCONV Camera_LightCoolnessSet(mtHandle handle, double newval);
EXT	mtCompletionCode CCONV Camera_LightCoolnessAdjustFromColorVector(mtHandle CameraHandle, mtHandle VectorHandle, int ColorProfile);
EXT	mtCompletionCode CCONV Camera_LightCoolnessAdjustFromCustomColorVector(mtHandle CameraHandle, mtHandle VectorHandle, int ColorProfile, double VectorPositionFract[]);
EXT	mtCompletionCode CCONV Camera_HDRLightCoolnessAdjustFromColorVector(mtHandle CameraHandle, mtHandle VectorHandle, int ColorProfile, int LightIndex);
EXT	mtCompletionCode CCONV Camera_HDRLightCoolnessAdjustFromCustomColorVector(mtHandle CameraHandle, mtHandle VectorHandle, int ColorProfile, int LightIndex, double VectorPositionFract[]);
EXT	mtCompletionCode CCONV Camera_HistogramEqualizeImagesGet(mtHandle handle, bool *p_out);
EXT	mtCompletionCode CCONV Camera_HistogramEqualizeImagesSet(mtHandle handle, bool newval);
EXT	mtCompletionCode CCONV Camera_IsImageGammaCorrectionEnabledGet(mtHandle handle, bool *p_out);
EXT	mtCompletionCode CCONV Camera_IsImageGammaCorrectionEnabledSet(mtHandle handle, bool newval);
EXT	mtCompletionCode CCONV Camera_ImageGammaValueGet(mtHandle handle, double *p_out);
EXT	mtCompletionCode CCONV Camera_ImageGammaValueSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Camera_ImagesGet(mtHandle handle, unsigned char *p_LeftImageBuffer, unsigned char *p_RightImageImageBuffer);
EXT mtCompletionCode CCONV Camera_ImagesGet3(mtHandle handle, unsigned char *p_LeftImageBuffer, unsigned char *p_RightImageImageBuffer, unsigned char *p_MiddleImageImageBuffer);
EXT mtCompletionCode CCONV Camera_HalfSizeImagesGet(mtHandle handle, unsigned char *LeftImage, unsigned char *RightImage);
EXT mtCompletionCode CCONV Camera_HalfSizeImagesGet3(mtHandle handle, unsigned char *LeftImage, unsigned char *MiddleImage, unsigned char *RightImage);
EXT mtCompletionCode CCONV Camera_24BitImagesGet3(mtHandle handle, unsigned char *p_LeftImageBuffer, unsigned char *p_RightImageBuffer, unsigned char *p_MiddleImageBuffer);
EXT mtCompletionCode CCONV Camera_24BitImagesGet(mtHandle handle, unsigned char *p_LeftImageBuffer, unsigned char *p_RightImageImageBuffer);
EXT mtCompletionCode CCONV Camera_24BitHalfSizeImagesGet(mtHandle handle, unsigned char *p_LeftHalfsizeImageBuffer, unsigned char *p_RightHalfsizeImageBuffer);
EXT mtCompletionCode CCONV Camera_24BitHalfSizeImagesGet3(mtHandle handle, unsigned char *p_LeftHalfsizeImageBuffer, unsigned char *p_RightHalfsizeImageBuffer, unsigned char *p_MiddleHalfsizeImageBuffer);

EXT mtCompletionCode CCONV Camera_PixelsGet(mtHandle handle, int SideI, int left_x, int top_y, 
						int width, int height, unsigned char *p_outValues_WxH);
EXT mtCompletionCode CCONV Camera_PixelSet(mtHandle handle, int SideI, int x, int y, short NewValue);
EXT mtCompletionCode CCONV Camera_ImagesSet(mtHandle handle, unsigned char *LeftImage,  unsigned char *RightImage);
EXT mtCompletionCode CCONV Camera_ImagesSet3(mtHandle handle, unsigned char *LeftImage,  unsigned char *RightImage,  unsigned char *MiddleImage);
EXT mtCompletionCode CCONV Camera_FeatureDataCaptureEnabledSet(mtHandle handle, bool newval);
EXT	mtCompletionCode CCONV Camera_FeatureDataCaptureEnabledGet(mtHandle handle, bool *p_out);
EXT	mtCompletionCode CCONV Camera_FeatureDataAccessGet(mtHandle handle, void **p_outFeatureDataAddr, int *p_outSize);
EXT	mtCompletionCode CCONV Camera_FeatureDataSet(mtHandle handle, void *p_FeatureData, int Size);
EXT mtCompletionCode CCONV Camera_LoadCalibration(mtHandle handle,  char *DirectoryPath); //For the current camera, given the directory
EXT mtCompletionCode CCONV Camera_LoadCalibrationFile(mtHandle handle,  char *FilePath, bool IgnoreSerialNumMismatch);  // or specific path
EXT mtCompletionCode CCONV Camera_CalibrationFileInfo(char *CalibrationFilePath, 
						char TextLines[][80], int LinesCount, int *pLinesFilled);
EXT int CCONV Camera_CalibrationFilePathGet(mtHandle handle, char *p_out, int max_size);
EXT mtCompletionCode CCONV Camera_UpdateCalibrationFile(mtHandle handle,
														double LX, double LY, double RX, double RY,
														double STD, double Gap, char *Date, bool Reset);
EXT mtCompletionCode CCONV Camera_UpdateCalibrationFile3(mtHandle handle,
														 double LX, double LY, double RX, double RY, double MX, double MY,
														 double STD, double Gap, char *Date, bool Reset);

EXT mtCompletionCode CCONV Camera_CalibrationInfo(mtHandle handle, 
						char TextLines[][80], int LinesCount, int *pLinesFilled);
EXT mtCompletionCode CCONV Camera_CalibrationLoadedGet(mtHandle handle, bool *p_out);
EXT mtCompletionCode CCONV Camera_InterpolatedRayGet( mtHandle CurrCam, double X, double Y, short SideI, calRay *pOutRay);
EXT	mtCompletionCode CCONV Camera_Triangulate(mtHandle handle, double LeftX, double LeftY, double RightX, double RightY,
									   double *p_outXYZ, double *p_outIntersectionGapMM);
mtCompletionCode CCONV Camera_Triangulate3(mtHandle handle, 
									   double Side0_X, double Side0_Y, double Side1_X, double Side1_Y,
									   int Side0_Index, int Side1_Index,
									   double *p_outXYZ, double *p_outIntersectionGapMM);
EXT	mtCompletionCode CCONV Camera_ProjectionOnImage(mtHandle handle, int SideI, double XYZ[],
									   double *p_outX, double *p_outY);
EXT mtCompletionCode CCONV Camera_GrabFrame(mtHandle handle);
EXT	mtMeasurementHazardCode CCONV Camera_LastFrameThermalHazard(mtHandle handle);
EXT mtCompletionCode CCONV Camera_FrameMTTimeSecsGet(mtHandle handle, double *p_out);
EXT mtHandle CCONV Camera_New(char *DriverName, int SerialNumber);
EXT mtCompletionCode CCONV Camera_Free(mtHandle handle);

//New Functions for BB2
EXT mtCompletionCode CCONV Camera_FrameTimeStamp(mtHandle handle, double *val);
EXT mtCompletionCode CCONV Camera_CameraRegisterSet(mtHandle handle, unsigned long reg, unsigned long val);
EXT mtCompletionCode CCONV Camera_CameraRegisterGet(mtHandle handle, unsigned long reg, unsigned long *val);
EXT mtCompletionCode CCONV Camera_TransmitSignal(mtHandle handle, bool OnOff, double Delay, double Duration);
EXT mtCompletionCode CCONV Camera_TriggerModeSet(mtHandle handle, bool OnOff);
EXT mtCompletionCode CCONV Camera_TriggerModeGet(mtHandle handle, bool *OnOff);
EXT mtCompletionCode CCONV Camera_SetCaptureSource (mtHandle handle);

//High Dynamic Range (HDR) functions
EXT	mtCompletionCode CCONV Camera_HdrEnabledGet(mtHandle handle, bool *p_out);
EXT	mtCompletionCode CCONV Camera_HdrEnabledSet(mtHandle handle, bool newval);
EXT	mtCompletionCode CCONV Camera_HdrMinLuxSet(mtHandle handle, double newval);
EXT	mtCompletionCode CCONV Camera_HdrMinLuxGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Camera_HdrShortCycleSet(mtHandle handle, bool newval);
EXT mtCompletionCode CCONV Camera_HdrShortCycleGet(mtHandle handle, bool *p_out);
EXT	mtCompletionCode CCONV Camera_HdrCycleLengthSet(mtHandle handle, int newval);
EXT	mtCompletionCode CCONV Camera_HdrCycleLengthGet(mtHandle handle, int *p_out);
EXT	mtCompletionCode CCONV Camera_HdrImageCompositingEnabledSet(mtHandle handle, bool newval);
EXT	mtCompletionCode CCONV Camera_HdrImageCompositingEnabledGet(mtHandle handle, bool *p_out);
EXT mtCompletionCode CCONV Camera_HdrFrameIndexInCycleGet(mtHandle handle, int *p_out);
EXT mtCompletionCode CCONV Camera_HdrFrameIndexInCycleSet(mtHandle handle, int newval);
EXT mtCompletionCode CCONV Camera_HdrCycledShutterSet(mtHandle handle, double Shutter1, int HDR_Cycle_Index);
EXT mtCompletionCode CCONV Camera_HdrCycledShutterGet(mtHandle handle, double *Shutter1, int HDR_Cycle_Index);
EXT mtCompletionCode CCONV Camera_HdrCycledGainSet(mtHandle handle, double Gain1, int HDR_Cycle_Index);
EXT mtCompletionCode CCONV Camera_HdrCycledGainGet(mtHandle handle, double *Gain1, int HDR_Cycle_Index);
EXT mtCompletionCode CCONV Camera_HdrCycledExposureSet(mtHandle handle, double newval, int HDR_Cycle_Index);
EXT mtCompletionCode CCONV Camera_HdrCycledExposureGet(mtHandle handle, double *Exposure1, int HDR_Cycle_Index);

EXT mtCompletionCode CCONV Camera_FrameEmbeddedShutterMsecsGet(mtHandle handle, double *val);
EXT mtCompletionCode CCONV Camera_FrameEmbeddedGainFGet(mtHandle handle, double *val);
EXT mtCompletionCode CCONV Camera_FrameEmbeddedExposureGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Markers_GetIdentifiedMarkersFromBackgroundThread(mtHandle CameraHandle);
EXT mtCompletionCode CCONV Camera_VideoLibraryVersionGet(mtHandle handle, int *p_outMajor, int *p_outMinor, int *p_outBuild, int *p_outRevision);
EXT mtCompletionCode CCONV Camera_ValidateCameraFirmwareVersion(mtHandle handle);

/*\}*/

/***********************************************************/
/** \name Cameras */
/***********************************************************/
EXT mtCompletionCode CCONV Cameras_AttachAvailableCameras (char *CalibrationFilesDirectory );
EXT void CCONV Cameras_Detach();
EXT mtCompletionCode CCONV Cameras_SerialNumbersGet(char *DriverName, mtHandle outCollection);
EXT mtCompletionCode CCONV Cameras_GrabFrame (mtHandle Cam );
EXT	mtHandle CCONV Cameras_FailedCamera();
EXT mtCompletionCode CCONV Cameras_Camera2Camera0XfGet (mtHandle CameraHandle, mtHandle outXform3DHandle);
EXT	mtCompletionCode CCONV Cameras_Camera2Camera0XfSet(mtHandle CameraHandle, mtHandle Xform3DHandle);
EXT mtCompletionCode CCONV Cameras_Camera2CameraXfGet (mtHandle FromCamHandle, mtHandle ToCamHandle, mtHandle ResultXform3DHandle);
EXT mtCompletionCode CCONV Cameras_CustomCoolnessLoaded(bool *p_out);
EXT int CCONV Cameras_CameraIndex(mtHandle CamHandle);
EXT int CCONV Cameras_Count();
EXT int CCONV Cameras_LastI();
EXT mtCompletionCode CCONV Cameras_ItemGet(int index, mtHandle *p_outCameraHandle);
EXT mtCompletionCode CCONV Cameras_ShutterMsecsLimiterGet(double *p_out);
EXT mtCompletionCode CCONV Cameras_ShutterMsecsLimiterSet(double newval);
EXT mtCompletionCode CCONV Cameras_GainFLimiterGet(double *p_out);
EXT mtCompletionCode CCONV Cameras_GainFLimiterSet(double newval);
EXT double CCONV Cameras_ShutterMsecsLimiterDefault();
EXT double CCONV Cameras_GainFLimiterDefaultdouble();
EXT double CCONV Cameras_LightCoolness();
EXT mtCompletionCode CCONV Cameras_LightCoolnessSet(double newval);
EXT double CCONV Cameras_HDRLightCoolness(int LightIndex);
EXT	mtCompletionCode CCONV Cameras_HDRLightCoolnessSet(double newval, int LightIndex);
EXT	mtCompletionCode CCONV Cameras_HistogramEqualizeImagesGet(bool *p_out);
EXT	mtCompletionCode CCONV Cameras_HistogramEqualizeImagesSet(bool newval);
EXT	mtCompletionCode CCONV Cameras_HdrMinLuxSet(int newval);
EXT mtCompletionCode CCONV Cameras_HdrMinLuxGet(int *p_out);
EXT mtCompletionCode CCONV Cameras_NumberOfProcessorsGet(int *p_out);
EXT mtCompletionCode CCONV Cameras_NumberOfProcessorsSet(int newval);
EXT	mtCompletionCode CCONV Cameras_IsImageGammaCorrectionEnabledGet(bool *p_out);
EXT	mtCompletionCode CCONV Cameras_IsImageGammaCorrectionEnabledSet(bool newval);
EXT mtCompletionCode CCONV Cameras_ImageGammaValueGet(double *p_out);
EXT mtCompletionCode CCONV Cameras_ImageGammaValueSet(double newval);


/***********************************************************/
/** \name Clouds */
/***********************************************************/
EXT int CCONV Clouds_TemplatesCount();
EXT mtCompletionCode CCONV Clouds_TemplatesCloudsGet(mtHandle outTemplatesCollectionHandle );
EXT mtCompletionCode CCONV Clouds_TemplateItemGet(int Index1based, mtHandle *p_outTemplateHandle );
EXT mtCompletionCode CCONV Clouds_IdentifiedCloudsGet(mtHandle CameraHandle, mtHandle outIdentifiedCollectionHandle );
EXT mtCompletionCode CCONV Clouds_TemplateMatchToleranceMMGet(double *p_out);
EXT mtCompletionCode CCONV Clouds_TemplateMatchToleranceMMDefaultGet(double *p_out);
EXT mtCompletionCode CCONV Clouds_TemplateMatchToleranceMMSet(double newval);
EXT void CCONV Clouds_JitterFilterEnabledSet(bool newval);
EXT bool CCONV Clouds_JitterFilterEnabled();
EXT mtCompletionCode CCONV Clouds_JitterFilterCoefficientGet(double *p_out);
EXT mtCompletionCode CCONV Clouds_JitterFilterCoefficientSet(double newval);
EXT mtCompletionCode CCONV Clouds_JitterFilterHistoryLengthGet(int *p_out);
EXT mtCompletionCode CCONV Clouds_JitterFilterHistoryLengthSet(int newval);
EXT mtCompletionCode CCONV Clouds_AutoAdjustCam2CamRegistrationGet(bool *p_out);
EXT mtCompletionCode CCONV Clouds_AutoAdjustCam2CamRegistrationSet(bool newval);
EXT mtCompletionCode CCONV Clouds_ProcessFrame(mtHandle CameraHandle);
EXT mtCompletionCode CCONV Clouds_ClearTemplates();
EXT mtCompletionCode CCONV Clouds_AddTemplate(mtHandle MarkerHandle);
EXT mtCompletionCode CCONV Clouds_Validate(char outTextLines[][120], int LinesCount, int *p_outLinesFilled);
EXT mtCompletionCode CCONV Clouds_LoadTemplates(char *DirPath);


/***********************************************************/
/** \name Cloud */
/***********************************************************/
EXT mtHandle CCONV Cloud_New();
EXT mtHandle CCONV Cloud_NewAs(mtHandle SourceHandle);
EXT mtCompletionCode CCONV Cloud_Free(mtHandle handle);
EXT mtCompletionCode CCONV Cloud_WasIdentifiedGet(mtHandle handle, mtHandle CameraHandle, bool *p_out);
EXT mtCompletionCode CCONV Cloud_CloudWasIdentifiedInRefSpaceGet(mtHandle handle, mtHandle CameraHandle, bool *p_out);
EXT mtCompletionCode CCONV Cloud_MeasurementHazardCodeGet(mtHandle CameraHandle, mtHandle CloudHandle, mtMeasurementHazardCode *p_out);
EXT mtCompletionCode CCONV Cloud_Cloud2CameraXfGet(mtHandle handle, 
											   mtHandle CameraHandle, mtHandle outXfHandle, 
											   mtHandle *p_outIdentifyingCameraHandle);
EXT mtCompletionCode CCONV Cloud_Tooltip2CloudXfGet(mtHandle handle, mtHandle XfHandle);
EXT mtCompletionCode CCONV Cloud_Tooltip2CloudXfSet(mtHandle handle, mtHandle XfHandle);
EXT mtCompletionCode CCONV Cloud_ReferenceCloudHandleGet(mtHandle CloudHandle, mtHandle *RefHandle);
EXT mtCompletionCode CCONV Cloud_ReferenceCloudHandleSet(mtHandle CloudHandle, mtHandle RefHandle);
EXT mtCompletionCode CCONV Cloud_Tooltip2ReferenceXfGet(mtHandle handle, mtHandle CameraHandle, mtHandle XfHandle, mtHandle *p_outIdentifyingCameraHandle);
EXT mtCompletionCode CCONV Cloud_Cloud2ReferenceXfGet(mtHandle handle, mtHandle CameraHandle, mtHandle outXfHandle, mtHandle *p_outIdentifyingCameraHandle);
EXT mtCompletionCode CCONV Cloud_IdentifiedXPointsGet(mtHandle handle, mtHandle CameraHandle, 
												  bool copy, mtHandle outCollectionHandle);
EXT mtCompletionCode CCONV Cloud_NameGet(mtHandle handle, char *p_outNameBuffer, int NameBufferChars, int *p_outActualChars);
EXT mtCompletionCode CCONV Cloud_NameSet(mtHandle handle, char *p_Name);
EXT mtCompletionCode CCONV Cloud_JitterFilterCoefficientGet(mtHandle handle, double *p_out);
EXT mtCompletionCode CCONV Cloud_JitterFilterCoefficientSet(mtHandle handle, double newval);
EXT mtCompletionCode CCONV Cloud_TemplateXPointsGet(mtHandle handle, mtHandle *p_outFacetsHandlesCollection);
EXT mtCompletionCode CCONV Cloud_AddTemplateXPoint(mtHandle handle, mtHandle NewXPointHandle);
EXT mtCompletionCode CCONV Cloud_RemoveTemplateXPoint(mtHandle handle, mtHandle XPointToRemoveHandle);
EXT mtCompletionCode CCONV Cloud_ResetIdentifiedXPoints(mtHandle handle, mtHandle CameraHandle);
EXT mtCompletionCode CCONV Cloud_ValidateTemplate(mtHandle handle);
EXT mtCompletionCode CCONV Cloud_StoreTemplate(mtHandle handle, mtHandle PersistenceHandle, const char *p_SectionPrefix);
EXT mtCompletionCode CCONV Cloud_RestoreTemplate(mtHandle CloudHandle, mtHandle PersistenceHandle, const char *p_SectionPrefix);
EXT mtCompletionCode CCONV Cloud_IsCloudRegisteringSet(mtHandle handle, bool NewVal);
EXT mtCompletionCode CCONV Cloud_IsCloudRegisteringGet(mtHandle handle, bool *p_out);


/*\}*/

#  ifdef __cplusplus
}
#endif

#endif // MTC_H
