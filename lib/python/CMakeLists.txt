# cmake_minimum_required(VERSION 3.10)
# project(pylib LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # --- Python ---
# find_package(PythonInterp REQUIRED)
# find_package(PythonLibs REQUIRED)

# # --- Boost ---
# # Use library name that matches the installed package
# # Python 3.x usually installs libboost_python38.so or libboost_python3.so
# set(Boost_PYTHON_LIB python38)  # adjust if your library is python38
# find_package(Boost REQUIRED COMPONENTS ${Boost_PYTHON_LIB})

# if(NOT Boost_FOUND)
#     message(FATAL_ERROR "Boost.Python not found. Install libboost-python3-dev")
# endif()

# # --- Module ---
# add_library(pylib MODULE pylib.cpp)

# target_include_directories(pylib PRIVATE
#     ${Boost_INCLUDE_DIRS}
#     ${PYTHON_INCLUDE_DIRS}
# )

# target_link_libraries(pylib PRIVATE
#     ${Boost_LIBRARIES}
#     ${PYTHON_LIBRARIES}
# )

# # --- Python extension naming ---
# set_target_properties(pylib PROPERTIES
#     PREFIX ""
#     SUFFIX ".so"
#     OUTPUT_NAME "pylib"
# )
