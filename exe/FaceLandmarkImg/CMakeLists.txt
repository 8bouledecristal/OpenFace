# Local libraries
# add_subdirectory(exe/FaceLandmarkImg)
include_directories(${LandmarkDetector_SOURCE_DIR}/include)
include_directories(/root/openface/lib/local/LandmarkDetector/include)
	
add_executable(FaceLandmarkImg FaceLandmarkImg.cpp)
target_link_libraries(FaceLandmarkImg
    PRIVATE
        LandmarkDetectorShared
        FaceAnalyser
        GazeAnalyser
        Utilities
        ${OpenCV_LIBRARIES}
        ${Boost_LIBRARIES}
)

install (TARGETS FaceLandmarkImg DESTINATION bin)