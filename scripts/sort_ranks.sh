#!/bin/sh

# Output rank files with <pub	rank	year> format

filename="$1"
directory="$2"
#echo $filename
filename_actual=${filename##*/}
#echo $filename_actual
field_1=$3
field_2=$4
field_3=$5


echo "IN: $filename";
echo "OUT: $directory/$filename_actual";


export LC_ALL=C;

cut -f $field_1,$field_2,$field_3 "$filename" | awk -F '	' '{OFS = FS}; {n = split($2,rank,"|")}; {print $1,rank[n],$3}' | cut -f 1,2,3 --output-delimiter='	' | sort -k2,2 -gr > "$directory/$filename_actual"
