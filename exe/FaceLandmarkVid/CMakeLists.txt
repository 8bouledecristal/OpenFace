# Local libraries
include_directories(${LandmarkDetector_SOURCE_DIR}/include)
include_directories(/root/openface/lib/local/LandmarkDetector/include)
	
add_executable(FaceLandmarkVid FaceLandmarkVid.cpp)
target_link_libraries(FaceLandmarkVid LandmarkDetectorShared)
target_link_libraries(FaceLandmarkVid FaceAnalyser)
target_link_libraries(FaceLandmarkVid GazeAnalyser)
target_link_libraries(FaceLandmarkVid Utilities)

install (TARGETS FaceLandmarkVid DESTINATION bin)