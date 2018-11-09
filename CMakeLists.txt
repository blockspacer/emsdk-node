cmake_minimum_required(VERSION 3.5)
project(hello-func-cpp-wasm CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
#add_subdirectory(tora/math)

set(CMAKE_CXX_FLAGS "--bind -O3 -s EXPORT_ES6=1 --std=c++17 -o out/hello-func-cpp-wasm.js -s WASM=1 -s EXPORT_NAME=\"'Module'\" -s NO_FILESYSTEM=1 -s ABORTING_MALLOC=1 -s ELIMINATE_DUPLICATE_FUNCTIONS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 --memory-init-file 0")

set(PROJECT_SRC src/hello-func-wasm.cpp)

if(EMSCRIPTEN)
    set(EM_TARGET_JS ${PROJECT_NAME}.js)
    #configure_file(shell.html index.html @ONLY)
endif()

add_executable(${PROJECT_NAME} 
	${PROJECT_SRC}
)

set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/out")

#target_compile_options(${PROJECT_NAME}
#    PRIVATE 
#    ${BUILD_FLAGS}
#)

set_target_properties( ${PROJECT_NAME} 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    #COMPILE_FLAGS "${BUILD_FLAGS}"
)

#Command Line:
#mkdir build
#cd build
#cmake ..

#
#mkdir build-emscripten
#cd build-emscripten
# emcmake cmake ..
#REM For windows make command use http://www.mingw.org/wiki/mingw !!!You must add C:\MinGW\bin; to your user PATH environment variable manually!!!
# emmake make
#REM index.html generates from configure_file - see shell.html from CMakeLists.txt
# emrun index.html