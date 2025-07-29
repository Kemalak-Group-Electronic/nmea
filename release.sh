#!/bin/bash

# Minimal build script for NMEA0183 project
set -e  # Exit on error

# Configure build directory
BUILD_DIR="./release"

# Create build directory if missing
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating release directory..."
    mkdir -p "$BUILD_DIR/include"
fi

cp ./build/libNMEA0183Lib.a ./release/libNMEA0183Lib.a
cp ./include/nmea0183.h ./release/include/nmea0183.h
cp ./include/gpgga.h ./release/include/gpgga.h
cp ./include/gprmc.h ./release/include/gprmc.h
cp ./include/nmea0183Type.h ./release/include/nmea0183Type.h

echo "Release files copied successfully."

# Run CMake and Make
cd "$BUILD_DIR"
cmake ..
make

echo "Release project successfully built."
