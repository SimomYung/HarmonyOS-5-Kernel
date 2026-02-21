#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
# Description: Shell scripts to generate interface lists
# Author: Huawei OS Kernel Lab
# Create: Fri Sep 18 10:12:11 2020
set -e

files=$(ls man)
TMP_FILE1=api1.list
TMP_FILE2=api2.list
API_LIST=api.list
POSIX_API_LIST=posix_api.list
POSIX_SUP_LIST=posix_api_supported.list

# Read first line of every md
for filename in $files
do
	file="man"/$filename
	sed -n '1,1p' $file >> ${TMP_FILE1}
done

# Delete '# ', '()', '\' and ', ', just retain name of interfaces
sed -i 's/# //g' ${TMP_FILE1}
sed -i 's/()//g' ${TMP_FILE1}
sed -i 's/\\//g' ${TMP_FILE1}
sed -i 's/, / /g' ${TMP_FILE1}

# Output one interface in a line
for LINE in $(cat ${TMP_FILE1})
do
	echo $LINE 2>&1 >> ${TMP_FILE2}
done

# Sort by alphabet sequence and remove repetitive interfaces
sort -u ${TMP_FILE2} >> ${API_LIST}

rm -f ${TMP_FILE1}
rm -f ${TMP_FILE2}

# Generate supported POSIX interface list
for LINE in $(cat ${API_LIST})
do
	if [ ! -z "$(grep -w "$LINE" $POSIX_API_LIST)" ]; then
		echo $LINE 2>&1 >> $POSIX_SUP_LIST
	fi
done
