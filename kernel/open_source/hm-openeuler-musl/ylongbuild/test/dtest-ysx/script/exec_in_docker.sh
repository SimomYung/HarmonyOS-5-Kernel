#!/bin/bash
  

exec_mode=$3
exec_list=$(echo $1 | tr ',' ' ')
echo ${exec_list[@]}

connection=$2

if [[ "$exec_mode" == "exec_dir" ]]
then
  # 过滤掉目录
  for exec in ${exec_list[@]}; do
    if [[ ! -d $PWD/testcase/${exec} ]]; then
      real_exec_list=(${real_exec_list[@]} "${exec}")
    fi
  done
  exec_list=("${real_exec_list[@]}")
  echo "real exec list:" "${exec_list[@]}"
fi

echo "调用qemu内执行脚本"
if [[ "$connection" == "ssh"  ]]
then
  echo "using SSH connection"
  chmod +x $PWD/dtest-ysx/script/call_exec_in_qemu.sh
  $PWD/dtest-ysx/script/call_exec_in_qemu.sh ${exec_list[@]}
else
  python3 $PWD/dtest-ysx/script/call_exec_in_qemu.py $qemu_telnet_port ${exec_list[@]}
  python3 $PWD/dtest-ysx/script/call_test_st_in_qemu.py $qemu_telnet_port ${exec_list[@]}
fi

sleep 60
kill -9 $(ps -ef | grep "qemu" | grep -v grep | awk '{print $2}' )

echo "拷贝用例结果"
chmod +x $PWD/dtest-ysx/script/scp_telnet_result.sh
$PWD/dtest-ysx/script/scp_telnet_result.sh


if [ -f "/bin/NCSClient.sh" ]
then
  echo "生成覆盖率报告"
  /bin/NCSClient.sh -r /usr1/workspace/lltcov -b /usr1/workspace/cov_data -o $PWD/output/coverage/
else
  echo "不存在NCSClient脚本，不生成覆盖率数据"
fi
