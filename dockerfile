# syntax=docker/dockerfile:1
FROM osrf/ros:kinetic-desktop-full-xenial

# install app dependencies
RUN apt-get update && apt-get install libdc1394-22-dev \
					  				  libraw1394-dev \
					  				  libxft-dev \
					  				  libfltk1.3-dev \
					  				  libvxl1-dev -y

COPY MTHome /MTHome
COPY MTRosDemo /MTRosDemo

ENV MTHome=/MTHome
ENV ROS_MASTER_URI=http://biomed:11311

RUN g++ -D_LINUX64 /MTRosDemo/SimpleDemoC.cpp -w\
        -L/MTRosDemo -lMTC -ldc1394 -lraw1394 -lpthread -lm -lvnl -lvnl_algo \
        -I/opt/ros/kinetic/include -L/opt/ros/kinetic/lib -Wl,-rpath,/opt/ros/kinetic/lib \
        -lroscpp -lrosconsole -lrostime -lroscpp_serialization -ltf2 -ltf2_ros \
        -lboost_system -lboost_thread -pthread -lactionlib

CMD ["./a.out"]

