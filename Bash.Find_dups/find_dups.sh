#!/bin/bash

dir=$1
rm_dups="$1"/rm_dups.sh

find_by_name()
{
temp_file=$(mktemp)
echo "#!/bin/bash" > "$rm_dups"
chmod +x "$rm_dups"
find "$1" -type f | while read fname; do
if ! grep -q -x "$(basename "$fname")" ${temp_file}
then
	if [ $(find "$1" -name "$(basename "$fname")" | wc -l) -gt 1 ]
  	then
  		echo "#----------$(basename "$fname")----------" >> "$rm_dups"
		find "$1" -type f -name "$(basename "$fname")" | while read dup_name; do
			printf "%s" "#rm " >> "$rm_dups"
			short_name=$(echo "$dup_name" | sed "s|$1|.|g")
			printf "%q\n" "$short_name" >> "$rm_dups"
		done
		echo "$(basename "$fname")" >> ${temp_file}
	fi
fi
done
rm ${temp_file}
}

find_by_cont()
{
echo "#!/bin/bash" > "$rm_dups"
chmod +x "$rm_dups"
find $1 -type f -print0 | xargs -0 md5sum | sort | uniq -w32 -D | while read md5 path; do
if [ "$previous" = "$md5" ]
then
	printf "%s" "#rm " >> "$rm_dups"
	short_name=$(echo "$path" | sed "s|$1|.|g")
	printf "%q\n" "$short_name" >> "$rm_dups"
else
	echo "#----------$md5----------" >> "$rm_dups"
fi
previous=$md5
done
}

if [ -n "$1" ] #check 1st parameter
then 
	if [ -d "$1" ] #check path to folder
	then
		if [ -n "$2" ] #check 2nd parameter
		then
			case $2 in
				"name") find_by_name $1;;
				"cont") find_by_cont $1;;
				*     ) echo "Parametr should be \"name\" or \"cont\"";;
			esac
		else
		find_by_name $1
		fi
	else
		echo "Path doesn\`t exist"
	fi
else
	echo "Enter the path"
fi 