#!/bin/bash 

for file in dataConvertion/euDaq/*.bin;do ./newana $file ;done

for file in dataConvertion/kpixDaq/*.bin;do ./newana $file ;done
