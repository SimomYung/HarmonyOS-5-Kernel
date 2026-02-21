#!/bin/sh
trap "" 13

echo "1" > /hpe/conf/app.cfg 
echo $1 >> /hpe/conf/app.cfg 

echo "FINISHED LOAD ST QEMU"
