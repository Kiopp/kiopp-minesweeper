#!/bin/bash

# Input PNG file
input_png="image.png"

# Output C file
output_c="image_data.c"

# Check if input file exists
if [ ! -f "$input_png" ]; then
  echo "Error: Input file '$input_png' not found."
  exit 1
fi

# Get image dimensions
dimensions=$(identify -format "%w,%h" "$input_png")
width=$(echo "$dimensions" | cut -d ',' -f 1)
height=$(echo "$dimensions" | cut -d ',' -f 2)

# Convert to RGBA raw data and format into C array
echo "unsigned int image_width = $width;" > "$output_c"
echo "unsigned int image_height = $height;" >> "$output_c"
echo "unsigned char image_data[] = {" >> "$output_c"

convert "$input_png" rgba:- | hexdump -v -e '4/1 "0x%02x, "' >> "$output_c"

# Finish the C array
echo "};" >> "$output_c"

echo "Conversion complete. Output written to '$output_c'."