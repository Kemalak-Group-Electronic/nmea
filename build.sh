#!/bin/bash

# Minimal build script for NMEA0183 project
set -e  # Exit on error

# Configure build directory
BUILD_DIR="build"

# Create build directory if missing
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

# Run CMake and Make
cd "$BUILD_DIR"
cmake ..
make