#!/bin/bash

# Path to the compiled executable
EXECUTABLE="./release/nmea"

# Check if the executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Executable not found at $EXECUTABLE"
    echo "Please build the project first using build.sh"
    exit 1
fi

# Run the program
echo "Running Release NMEA0183..."
$EXECUTABLE