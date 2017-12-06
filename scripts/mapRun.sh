#!/bin/sh

#sleep 3600;

for file in dataConvertion/euDaq/*140.bin_new.root; do
    echo $file;
    num=`echo $file | awk -F"run000" '{print $2}' | awk -F"." '{print $1}'`
    #python kpix_mapping_new.py $file -o kpixMap/map1017New/eudaq_run$num -k 26
    #python kpix_mapping_new.py $file -o kpixMap/map1017New/eudaq_run$num -k 28
    python kpix_mapping_new.py $file -o kpixMap/map1017New/eudaq_run$num -k 30
done


for file in dataConvertion/kpixDaq/*2017_10_18_17_*.bin_new.root; do
    echo $file;
    num=`echo $file | awk -F"kpixDaq/" '{print $2}' | awk -F"." '{print $1}'`
    #python kpix_mapping_new.py $file -o kpixMap/map1017New/kpix_$num -k 26
    #python kpix_mapping_new.py $file -o kpixMap/map1017New/kpix_$num -k 28
    python kpix_mapping_new.py $file -o kpixMap/map1017New/kpix_$num -k 30
done
