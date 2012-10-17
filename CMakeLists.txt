cmake_minimum_required (VERSION 2.6) 
project (SimpleHeadPoseEstimator) 

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB SHPE_SOURCES	
	*.cpp *.h *.c)

include_directories(${OpenGL_INCLUDE_DIRS})

IF(APPLE)
	# since we're using FreeGLUT, we need the X11 OpenGL and not the OSX internal
	include_directories(/usr/X11/include/)
	link_directories(/usr/X11/lib)
ENDIF(APPLE)

add_executable(SHPE ${SHPE_SOURCES})

target_link_libraries(SHPE ${OpenCV_LIBS} ${OpenGL_LIBS})

IF(APPLE)
	target_link_libraries(SHPE GL X11 GLU freeglut)
ENDIF(APPLE)
