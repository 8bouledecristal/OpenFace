# LandmarkDetector/CMakeLists.txt

# Dependencies
include_directories(${TBB_ROOT_DIR}/include)
include_directories(${BOOST_INCLUDE_DIR})
include_directories(./include)

# Source files
set(SOURCE
    src/CCNF_patch_expert.cpp
    src/FaceDetectorMTCNN.cpp
    src/CEN_patch_expert.cpp
    src/LandmarkDetectionValidator.cpp
    src/LandmarkDetectorFunc.cpp
    src/LandmarkDetectorModel.cpp
    src/LandmarkDetectorUtils.cpp
    src/LandmarkDetectorParameters.cpp
    src/Patch_experts.cpp
    src/PAW.cpp
    src/PDM.cpp
    src/SVR_patch_expert.cpp
    src/stdafx.cpp
)

set(HEADERS
    include/CCNF_patch_expert.h    
    include/LandmarkCoreIncludes.h
    include/LandmarkDetectionValidator.h
    include/LandmarkDetectorFunc.h
    include/LandmarkDetectorModel.h
    include/LandmarkDetectorParameters.h
    include/LandmarkDetectorUtils.h
    include/Patch_experts.h    
    include/PAW.h
    include/PDM.h
    include/SVR_patch_expert.h        
    include/stdafx.h
)

# Build a single shared library
add_library(LandmarkDetectorShared SHARED ${SOURCE} ${HEADERS})

# Include directories for consumers
target_include_directories(LandmarkDetectorShared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${TBB_ROOT_DIR}/include
)

# Link dependencies
target_link_libraries(LandmarkDetectorShared
    PUBLIC
        Utilities
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}
        ${TBB_LIBRARIES}
)

# Position independent code
set_target_properties(LandmarkDetectorShared PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Install
install(TARGETS LandmarkDetectorShared LIBRARY DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/OpenFace)
