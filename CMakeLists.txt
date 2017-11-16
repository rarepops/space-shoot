cmake_minimum_required(VERSION 3.8)
project(SpaceShoot CXX)

file (GLOB sourcefiles "*.hpp" "*.cpp")
add_executable(SpaceShoot ${sourcefiles})

target_link_libraries(SpaceShoot ${all_libs})

IF (WIN32)
	SET(SDL2_dll "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2-2.0.5/lib/x86/SDL2.dll" CACHE STRING "Where the SDL2.dll can be found")
    SET(SDL2_image_dll_path "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_image-2.0.1/lib/x86" CACHE PATH "Where the SDL2_image.dll can be found")
    SET(SDL2_mixer_dll_path "${CMAKE_SOURCE_DIR}/thirdparty/Win/SDL2_mixer-2.0.1/lib/x86" CACHE PATH "Where the SDL2_mixer.dll can be found")
    SET(DLLFileList "${SDL2_image_dll_path}/SDL2_image.dll" "${SDL2_image_dll_path}/libjpeg-9.dll" "${SDL2_image_dll_path}/libpng16-16.dll" "${SDL2_image_dll_path}/libtiff-5.dll" "${SDL2_image_dll_path}/libwebp-4.dll" "${SDL2_image_dll_path}/zlib1.dll" "${SDL2_mixer_dll_path}/SDL2_mixer.dll" "${SDL2_mixer_dll_path}/libvorbisfile-3.dll"  "${SDL2_mixer_dll_path}/libvorbis-0.dll"  "${SDL2_mixer_dll_path}/libogg-0.dll"  "${SDL2_mixer_dll_path}/libmodplug-1.dll"  "${SDL2_mixer_dll_path}/libFLAC-8.dll" ${SDL2_dll})
 # Copy dll's to output path
# file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
 #file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
   file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)
 # Set working folder to project folder (to find project data runtime)
 set_target_properties(SpaceShoot PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
ENDIF(WIN32)