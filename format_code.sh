#!/bin/bash
# Format all .c and .h files in the project using clang-format and your .clang-format file

find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;

echo "All C and header files have been formatted."