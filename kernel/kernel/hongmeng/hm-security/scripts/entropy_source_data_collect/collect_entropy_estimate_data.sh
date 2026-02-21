#!/bin/bash

# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
# Description: Collect entropy source test data
# Author: Huawei OS Kernel Lab
# Create: Thu Dec 17 20:16:29 2020

set -e

board_ip=$1
local_ip=$2
user=$3
passwd=$4

function copy_data_from_board(){
	# copy entropy data from board
	local cmd_line_put="ftpput -u ${user} -p ${passwd} ${local_ip} ${entropy_source}_entropy_data /tmp/entropy_${entropy_source}_entropy_read"
	python3 hm-test/lib/run_on_host/run_command.py "$board_ip" "$cmd_line_put"
	sudo chmod 666 /home/${user}/${entropy_source}_entropy_data
	mv /home/${user}/${entropy_source}_entropy_data ./
}

function generate_entropy(){
	local cmd_line_wc="wc -c /tmp/entropy_${entropy_source}_entropy_read"
	local size_info=$(python3 hm-test/lib/run_on_host/run_command.py "$board_ip" "$cmd_line_wc")
	echo $size_info | cut -d' ' -f1
}

# collect 1024 * 1024 bytes entropy data
function collect_entropy_data(){
	local size=0
	while [ $size -lt "1048576" ]
	do
		size=$(generate_entropy)
		echo $size
	done

	copy_data_from_board

}

entropy_source="sysmgr"
collect_entropy_data
entropy_source="devhost"
collect_entropy_data
