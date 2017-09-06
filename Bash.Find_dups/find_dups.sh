#!/bin/sh

temp_file=$(mktemp)
rm_dups=$1/rm_dups.sh

echo "#!/bin/sh" > "$rm_dups"
find $1 -type f | while read fname; do
if ! grep -q -x "$(basename "$fname")" ${temp_file}
	then
		if [ $(find $1 -name "$(basename "$fname")" | wc -l) -gt 1 ]
  		then
  		echo "#----------$(basename "$fname")----------" >> "$rm_dups"
		find $1 -name "$(basename "$fname")" | while read dup_name; do
			echo -n "#rm " >> "$rm_dups"
			echo "$dup_name" >> "$rm_dups"
		done
		echo "$(basename "$fname")" >> ${temp_file}
		fi
fi
done
rm ${temp_file}