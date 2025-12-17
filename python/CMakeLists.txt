# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
cmake_minimum_required(VERSION 3.16)

find_package( Boost 1.5.9 REQUIRED COMPONENTS filesystem system python)

# Python stuff.
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
message(-----------------)
message(${Python3_INCLUDE_DIRS})
execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE NumPy_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
include_directories(${Python3_INCLUDE_DIRS} ${NumPy_INCLUDE_DIR})
message(STATUS "NumPy include dir: ${NumPy_INCLUDE_DIR}")
message(-----------------)
# TBB library.
include_directories(${TBB_ROOT_DIR}/include)

# Local libraries.
include_directories(${LandmarkDetector_SOURCE_DIR}/include)
  
include_directories(../lib/local/LandmarkDetector/include)
include_directories(../lib/local/FaceAnalyser/include)  
include_directories(../lib/local/Utilities/include)  
include_directories(../lib/3rdParty/OpenCV/include/opencv2)  
      
add_library(pyopenface SHARED pyopenface.cpp detector.cpp tracker.cpp)
# target_link_libraries(pyopenface LandmarkDetectorShared)
target_link_libraries(pyopenface LandmarkDetectorShared ${OpenCV_LIBS} ${Boost_LIBRARIES} ${Python3_LIBRARIES} ${TBB_LIBRARIES} dlib_shared)
# target_link_libraries(pyopenface dlib_shared)

set_property(TARGET pyopenface PROPERTY CXX_STANDARD 11)
set_property(TARGET pyopenface PROPERTY SUFFIX .so)
set_property(TARGET pyopenface PROPERTY PREFIX "")

install(TARGETS pyopenface LIBRARY DESTINATION /usr/lib/python3.8/dist-packages PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ GROUP_WRITE WORLD_WRITE WORLD_EXECUTE WORLD_READ)

