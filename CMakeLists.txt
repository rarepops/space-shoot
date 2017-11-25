cmake_minimum_required(VERSION 3.8)
project(SpaceShoot CXX)

file (GLOB sourcefiles "*.hpp" ".h" "*.cpp")

add_executable(SpaceShoot ${sourcefiles})
target_link_libraries(SpaceShoot ${all_libs})


IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(SpaceShoot PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)