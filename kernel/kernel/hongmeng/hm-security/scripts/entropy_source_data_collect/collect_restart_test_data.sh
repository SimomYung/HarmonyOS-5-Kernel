#!/bin/bash

# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
# Description: Collect entropy source restart test data
# Author: Huawei OS Kernel Lab
# Create: Thu Dec 17 20:16:29 2020

set -e

readonly ENTROPY_SOURCE_LIST=("devhost" "sysmgr" "jitter")

board_ip=$1
local_ip=$2
user=$3
passwd=$4
entropy_count=$5
entropy_size=(0 0 0)
entropy_source=""

function wait_reboot(){
	local _ret=0
	echo TINFO "waiting for system reboot..."
	ping ${board_ip} -i 1 -c 10 | grep "[1-9][0-9]*% packet loss"
	if [ $? -ne 0 ]; then
		echo "ping sucess, system does not reboot."
		_ret=1
	else
		echo "system begin to reboot successfully."
	fi
	sleep 90
	ping ${board_ip} -i 1 -c 10 | grep " 0% packet loss"
	if [ $? -ne 0 ]; then
		echo "ping failed, system reboot failed."
		_ret=1
	else
		echo "system reboot successfully."
	fi
	return ${_ret}
}

# copy first 1000 byte entropy data from board (every reboot operation)
# see NIST SP800-90B 3
function copy_data_from_board(){
	# copy entropy data from board
	local cmd_line_put="ftpput -u ${user} -p ${passwd} ${local_ip} ${entropy_source}_entropy_data /tmp/entropy_${entropy_source}_entropy_read"
	python3 hm-test/lib/run_on_host/run_command.py "$board_ip" "$cmd_line_put" > /dev/null
	sudo chmod 666 /home/${user}/${entropy_source}_entropy_data > /dev/null
	mv /home/${user}/${entropy_source}_entropy_data ./

	# copy first 1000 byte entropy data to restart_data file
	dd if=./${entropy_source}_entropy_data of=./temp_1000 bs=1 count=1000 > /dev/null
	cat temp_1000 >> ${entropy_source}_restart_data

	# rm temp file
	rm ./${entropy_source}_entropy_data ./temp_1000
}

function generate_entropy(){
	local cmd_line_wc="wc -c /tmp/entropy_${entropy_source}_entropy_read"

	# check if we have enough data
	local entropy_size_collect=0
	local size_info=""
	while [ $entropy_size_collect -lt 1024 ]
	do
		size_info=$(python3 hm-test/lib/run_on_host/run_command.py "$board_ip" "$cmd_line_wc")
		entropy_size_collect=$(echo $size_info | cut -d' ' -f1)
	done
}

function get_one_entropy_source_data(){
	touch ${entropy_source}_restart_data

	local restart_data_size=$(wc -c ${entropy_source}_restart_data  |cut -d' ' -f1)
	if [ $restart_data_size -ge 1000000 ]; then
		echo $restart_data_size
	else
		generate_entropy
		# copy entropy data file from board
		copy_data_from_board
		restart_data_size=$(wc -c ${entropy_source}_restart_data  |cut -d' ' -f1)
		echo $restart_data_size
	fi
}

# main process
for i in $(seq 1 10000)
do
	j=0
	while [ $j -le $entropy_count ]
	do
		if [ ${entropy_size[$j]} -ge 1000000 ]; then
			let j=j+1
			continue
		fi

		entropy_source=${ENTROPY_SOURCE_LIST[$j]}
		entropy_size[$j]=$(get_one_entropy_source_data)

		let j=j+1
	done

	j=0
	sum_size=0
	while [ $j -le $entropy_count ]
	do
		tmp_size=${entropy_size[$j]}
		let sum_size=sum_size+tmp_size
		echo "size is ${tmp_size}"
		echo "sum_size is ${sum_size}"

		let j=j+1
	done

	let enough_size=1000000*entropy_count
	if [ $sum_size -ge $enough_size ]; then
		break
	fi
	
	# reboot board
	restart_cmd="reboot"
	python3 hm-test/lib/run_on_host/run_command.py "$board_ip" "$restart_cmd"

	# wait reboot ok
	wait_reboot
done
