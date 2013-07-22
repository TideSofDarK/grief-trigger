cmake_minimum_required(VERSION 2.8)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET(CMAKE_CXX_FLAGS "/EHsc")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(CMAKE_CXX_FLAGS "-std=c++11")
endif()

project(grief-trigger)

file(GLOB SOURCE
    "*.cpp"
)

file(GLOB HEADER
    "*.h"
    "*.hpp"
)

add_executable(grief-trigger ${SOURCE})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

#Aurora
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(AURORA_INCLUDE_DIR "" CACHE FILEPATH "sets aurora include path")
	include_directories(${AURORA_INCLUDE_DIR})
	endif()

#zlib
find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( grief-trigger ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

#add MapLoader
set(MAPLOADER_INCLUDE_DIR "" CACHE FILEPATH "sets maploader include path")
include_directories(${MAPLOADER_INCLUDE_DIR})

#Find SFML
find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(grief-trigger ${SFML_LIBRARIES})
endif()

#Find THOR
find_package(Thor 2 REQUIRED)
include_directories(${THOR_INCLUDE_DIR})
target_link_libraries(grief-trigger ${THOR_LIBRARY})