cmake_minimum_required(VERSION 3.16)
project(pyopenface LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PREFIX_PATH "/root/diff/usr/local" ${CMAKE_PREFIX_PATH})

# Dependencies
find_package(Boost REQUIRED COMPONENTS filesystem system python)
find_package(dlib REQUIRED)

# Python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE NumPy_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# pybind11
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

# Module
pybind11_add_module(pyopenface
    pyopenface.cpp
    detector.cpp
)

# Includes
target_include_directories(pyopenface PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${NumPy_INCLUDE_DIR}
    ../lib/local/LandmarkDetector/include
    ../lib/local/GazeAnalyser/include
    ../lib/local/FaceAnalyser/include
    ../lib/local/Utilities/include
    ../lib/3rdParty/OpenCV/include
)

# Link libraries (KEYWORD SIGNATURE ONLY)
target_link_libraries(pyopenface PRIVATE
    LandmarkDetectorShared
    FaceAnalyser
    GazeAnalyser
    Utilities
    dlib::dlib
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
)

set_target_properties(pyopenface PROPERTIES
    CXX_STANDARD 11
)

install(TARGETS pyopenface
    LIBRARY DESTINATION ${Python3_SITEARCH}
)
