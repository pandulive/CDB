
cmake_minimum_required(VERSION 3.16)
project(dbtest VERSION 1.0) #Project name

# set(CMAKE_CXX_STANDARD 17) #setting C++ 14 standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

find_package ( PkgConfig REQUIRED )
# find_package ( Threads REQUIRED )
pkg_check_modules(DBTEST REQUIRED gstreamer-1.0  gtkmm-3.0 gtk+-3.0 sqlite3 )


#including GStreamer header files directory
include_directories(
        ${DBTEST_INCLUDE_DIRS}
)

#linking GStreamer library directory
link_directories(
        ${DBTEST_LIBRARY_DIRS}
)

# Add other flags to the compiler
add_definitions(${DBTEST_CFLAGS_OTHER})

#building target executable
include_directories(includes)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

#linking library with target executable

target_link_libraries( ${PROJECT_NAME}  ${DBTEST_LIBRARIES} )


