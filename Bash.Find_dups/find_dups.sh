#!/bin/sh

temp_file=$(mktemp)

find $1 -type f | while read fname; do
if grep -q -x "$(basename "$fname")" ${temp_file}
	then
	echo "file in the list" >> /dev/null
	else
		if find $1 -name "$(basename "$fname")"
  		then
		echo "$(basename "$fname")" >> ${temp_file}
		fi
fi
done
rm ${temp_file}