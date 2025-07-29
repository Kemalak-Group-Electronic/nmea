#!/bin/bash

# Clean script for NMEA0183 project
echo "Cleaning build artifacts..."

# Remove the build directory and its contents
rm -rf release/

# Optional: Remove any other generated files
# rm -f *.o *.a *.so

echo "Clean complete."