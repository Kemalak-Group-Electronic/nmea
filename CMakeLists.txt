cmake_minimum_required (VERSION 3.5)
project(nmea LANGUAGES C)

# Set the C standard to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 -O0")

option(USE_PREBUILT_LIB "Use prebuilt static library instead of building from source" OFF)

if(USE_PREBUILT_LIB)
    include_directories(release/include)
    add_executable(nmea example/basic.c)
    target_link_libraries(nmea PRIVATE ${CMAKE_SOURCE_DIR}/release/libNMEA0183Lib.a)
    target_include_directories(nmea PUBLIC include)
else()
    # Set the library directories
    include_directories(Inc)

    # Create the static library
    add_library(NMEA0183Lib STATIC 
        src/nmea0183.c
        src/nmeaParserStatic.c
        src/utils.c)
    # Make sure consumers can find our headers
    target_include_directories(NMEA0183Lib PUBLIC include)
    add_executable(nmea example/basic.c)
    target_link_libraries(nmea PRIVATE NMEA0183Lib)
endif()
