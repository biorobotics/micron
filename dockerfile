# syntax=docker/dockerfile:1
FROM osrf/ros:kinetic-desktop-full-xenial

# install app dependencies
RUN apt-get update && apt-get install libdc1394-22-dev \
					  				  libraw1394-dev \
					  				  libxft-dev \
					  				  libfltk1.3-dev \
					  				  libvxl1-dev -y

# RUN apt-get install -y unzip wget && \
# 	wget https://github.com/opencv/opencv/archive/3.3.1.zip && \
# 	unzip 3.3.1.zip && \
# 	rm 3.3.1.zip && \
# 	mv opencv-3.3.1 OpenCV && \
# 	cd OpenCV && \
# 	mkdir build && \
# 	cd build && \
# 	cmake -DWITH_QT=ON -DWITH_OPENGL=ON -DFORCE_VTK=ON -DWITH_TBB=ON -DWITH_GDAL=ON -DWITH_XINE=ON -DBUILD_EXAMPLES=ON -DENABLE_PRECOMPILED_HEADERS=OFF .. && \
# 	make -j4 && \
# 	make install && \
# 	ldconfig

COPY cv_bridge_test_ws /cv_bridge_test_ws
COPY MTHome /MTHome
COPY MTRosDemo /MTRosDemo

ENV MTHome=/MTHome
ENV ROS_MASTER_URI=http://biomed:11311

RUN g++ -D_LINUX64 /MTRosDemo/SimpleDemoC.cpp -w\
        -L/MTRosDemo -lMTC -ldc1394 -lraw1394 -lpthread -lm -lvnl -lvnl_algo \
        -I/opt/ros/kinetic/include -L/opt/ros/kinetic/lib -Wl,-rpath,/opt/ros/kinetic/lib \
        -L/opt/ros/kinetic/lib/x86_64-linux-gnu \
        -I/opt/ros/kinetic/include/opencv-3.3.1-dev/opencv \
        -I/opt/ros/kinetic/include/opencv-3.3.1-dev \
        -lroscpp -lrosconsole -lrostime -lroscpp_serialization -ltf2 -ltf2_ros \
        -lboost_system -lboost_thread -pthread -lactionlib \
        -lcv_bridge -lopencv_imgcodecs3 -lopencv_imgproc3 -lopencv_core3 -limage_transport

CMD ["./a.out"]