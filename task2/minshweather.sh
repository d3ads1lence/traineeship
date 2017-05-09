#!/bin/bash

function get_temp(){
temp=&(wget --quiet -O - pogoda.by | iconv -f cp1251 | grep "Температура воздуха" | awk '{ print $6 }' | sed -r 's/<.+//')
echo -n $temp
}

read line < "config.ini"
if [ ! -z "$line" ];
then 
echo "Interval: $line"
else 
echo "Config file is empty"
exit
fi
while true; do get_temp; sleep $line; done;
