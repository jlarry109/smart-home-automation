#!/bin/bash

# List all directories and their contents
# Run this from the root of your project

echo "Listing all directories and their contents:"
find . -type d | while read dir; do
  echo
  echo "Directory: $dir"
  ls -la "$dir"
done
