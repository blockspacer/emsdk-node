cmake_minimum_required(VERSION 3.5)
project(hello-func-cpp-asmjs CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
#add_subdirectory(tora/math)

#set(CMAKE_CXX_FLAGS "--bind -O3 -s EXPORT_ES6=1 --std=c++17 -o out/hello-func-cpp-wasm.js -s WASM=1 -s EXPORT_NAME=\"'Module'\" -s NO_FILESYSTEM=1 -s ABORTING_MALLOC=1 -s ELIMINATE_DUPLICATE_FUNCTIONS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 --memory-init-file 0")
#set(CMAKE_CXX_FLAGS "--std=c++17")
set(CMAKE_CXX_FLAGS "--bind -O3 -s EXPORT_ES6=1 --std=c++17 -o out/hello-func-cpp-asmjs.js -s WASM=0 -s EXPORT_NAME=\"'Module'\" -s NO_FILESYSTEM=1 -s ABORTING_MALLOC=1 -s ELIMINATE_DUPLICATE_FUNCTIONS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 --memory-init-file 0")

set(PROJECT_SRC ../src/hello-func-wasm.cpp)

#if(EMSCRIPTEN)
    #set(EM_TARGET_JS ${PROJECT_NAME}.js)
    #configure_file(shell.html index.html @ONLY)
#endif()

add_executable(${PROJECT_NAME} 
	${PROJECT_SRC}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)

set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../out")

#set(WASM_CXX_FLAGS  "${CMAKE_CXX_FLAGS} --bind -O3 -s EXPORT_ES6=1 --std=c++17 -o out/hello-func-cpp-wasm.js -s WASM=1 -s EXPORT_NAME=\"'Module'\" -s NO_FILESYSTEM=1 -s ABORTING_MALLOC=1 -s ELIMINATE_DUPLICATE_FUNCTIONS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 --memory-init-file 0")
#string(REPLACE " " ";" WASM_FLAGS ${WASM_CXX_FLAGS})

#set_target_properties(${PROJECT_NAME} PROPERTIES
#    COMPILE_FLAGS ${WASM_FLAGS}
#)

set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
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