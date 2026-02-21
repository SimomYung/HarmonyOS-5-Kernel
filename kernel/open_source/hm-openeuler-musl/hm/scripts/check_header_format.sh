#!/bin/bash
# Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Fri Apr 10 10:34:11 2020

set -e

whitelist=scripts/whitelist.txt

red='\e[1;31m' #Red
green='\e[1;32m' #Green
yello='\e[1;32m' #Yello
NC='\e[0m' #No colour here must next to color]]]]'

var_ifndef="^#ifndef"
var_define="^#define"
var_error="^# error"
#var_if_define="^#if !defined"

var_exclude_path_array=$(cat $whitelist)

#
#  | cut -d ":" -f 1 | head -n 1
#
function check_format()
{
	var_file=$1
	var_query=$2

	line=$(sed -e '/\/\*/{
		:start
		N;
		/\*\//!b start
		s@/\*.*\*/@@g;
	} ' $var_file | grep -Ev "(^$)" | grep -n "$var_query" | head -n 1)

	#echo $line
}

function read_dir()
{

	var_result=1
	#echo $var_exclude_path_array
	num_of_header_file=$(find $1 -name "*.h" | wc -l)
	echo "num of header files is :"$num_of_header_file

	i=0
	for path in ${var_exclude_path_array[@]}
	do
		if [ "$i" -eq 0 ]
		then
			i=1
			exclude_path="-path "$1/$path
		else
			exclude_path=$exclude_path" -o -path "$1/$path
		fi
	done

	#echo $(find $1  \( $exclude_path \) -a -prune -o -name "*.h" -print | wc -l)
	for file in $(find $1  \( $exclude_path \) -a -prune -o -name "*.h" -print)
	do
		if [ -d "$1""/""$file" ]
		then
			read_dir $1"/"$file
		else
			#echo "check file :$file"
			check_format $file $var_ifndef
			line_ifndef=$line
			line_ifndef_number=$(echo $line_ifndef | cut -d ":" -f 1 )
			line_ifndef_header_name=$(echo $line_ifndef | cut -d " " -f 2 )
			#echo $line_ifndef_number
			#echo $line_ifndef_header_name

			check_format $file $var_define
			line_define=$line
			line_define_number=$(echo $line_define | cut -d ":" -f 1  )
			line_define_number=$[$line_define_number - 1]
			line_define_header_name=$(echo $line_define | cut -d " " -f 2 )
			#echo $line_define_number
			#echo $line_define_header_name

			check_format $file "$var_error"
			line_error=$line
			line_error_number=$(echo $line_error | cut -d ":" -f 1 )
			line_error_number=$[$line_error_number - 1]
			#echo $line_error_number

			if [[ "$line_ifndef_number" != "$line_define_number" ]] && [[ "$line_ifndef_number" != "$line_error_number" ]]
			then
				var_result=0
				echo "invalid format of header file:$file"
			elif [[ "$line_ifndef_number" == "$line_define_number" ]] && [[ "$line_ifndef_header_name" != "$line_define_header_name" ]]
			then
				#echo "$line_ifndef  $line_define $line_error"
				var_result=0
				echo "invalid format of header file:$file"
			fi
		fi
	done
	return 0
}

function main()
{
	read_dir $1
	if [ "$var_result" == 0 ]
	then
		echo -e "${red} Are you forget this code in your header file?
				#ifndef _H
				#define _H
				...
				#endif ${NC}"

		exit 1	#indicates invalid header file
	else
		echo -e "${green} All header files format are correct! ${NC}"
	fi

	exit 0	#indicates valid header file
}

main .
