cmake_minimum_required(VERSION 2.8.3)
project(ANDemo)
set(CMAKE_BUILD_TYPE release )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -fPIC")
else()
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -std=c++11 -fPIC -rdynamic")
endif()
add_definitions(-D_AQTDUMPLOG)
find_package(Threads REQUIRED)

find_package(OpenCV REQUIRED)

include_directories(
include
${OpenCV_INCLUDE_DIRS}
)
link_directories(./lib)

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(ANDemo ${SOURCE_FILES} ${HEADER_FILES} )
target_link_libraries(ANDemo
       ${OpenCV_LIBS}
       ${CMAKE_THREAD_LIBS_INIT}
       libcaffe.so
       libAquetiFace.so
       libssd_detector.so
       libGPUJPEG.so
       libVideoAnalyticsApi.so
)



