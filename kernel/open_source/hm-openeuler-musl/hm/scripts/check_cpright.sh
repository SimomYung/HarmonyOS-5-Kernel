#!/bin/bash
# Copyright (C) Huawei Technologies Co., Ltd. 2018. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 14 10:18:17 2018

# Check is there a Huawei copyright statement in the target directories and
# files.

set -e

COPYRIGHTLINE=5
COPYRIGHT="Copyright (C) Huawei Technologies Co., Ltd."

red='\e[1;31m' #Red
green='\e[1;32m' #Green
yello='\e[1;32m' #Yello
NC='\e[0m' #No colour here must next to color]]]]'

whitelist=scripts/whitelist.txt
var_exclude_path_array=$(cat $whitelist)

# Check is there a copyright in the target files
function copyright_operation() {
	var=$1 # file path and name
	if [[ ! -h "$var" ]]
	then
		sed -n "1,$COPYRIGHTLINE p" $1 | grep -q "$COPYRIGHT" # check fisrt few line for copyright
		if [ $? -ne 0 ]
		 then
			echo -e "${red} $var is missing copyright statment! ${NC}"
		fi
	fi
}

# root_dir can be a file or path
function check_copyright() {
	root_dir=$1
	i=0
	for path in ${var_exclude_path_array[@]}
	do
		if [ "$i" -eq 0 ]
		then
			i=1
			exclude_path="-path "$root_dir/$path
		else
			exclude_path=$exclude_path" -o -path "$root_dir/$path
		fi
	done

	for element in $(find $root_dir \( $exclude_path \) -a -prune -o -name "*.[chS]" -print)
	do
		dir_or_file=$element			#$root_dir"/"$element
		if [ -d "$dir_or_file" ]
		then
			check_copyright $dir_or_file	# if is a dir, check recursively
		else
			copyright_operation $dir_or_file
		fi
	done
}

# main

function main() {
	dir=$1

	check_copyright $dir | tee check.log
	grep -q "missing" check.log
	if [ $? -eq 0 ]
	then
		echo -e "${red} Sorry, you have unsolved copyright! ${NC}"
		rm -f check.log
		return 1
	else
		echo -e "${green} Congratulations! All files's copyright check succeed. ${NC}"
		rm -f check.log
		return 0
	fi
}

main .
