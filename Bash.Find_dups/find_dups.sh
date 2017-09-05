#!/bin/sh

temp_file=$(mktemp)

find $1 -type f | while read fname; do
if ! grep -q -x "$(basename "$fname")" ${temp_file}
	then
		if [ $(find $1 -name "$(basename "$fname")" | wc -l) -gt 1 ]
  		then
  		echo "----------$(basename "$fname")----------"
  		find $1 -name "$(basename "$fname")"
		echo "$(basename "$fname")" >> ${temp_file}
		fi
fi
done
rm ${temp_file}