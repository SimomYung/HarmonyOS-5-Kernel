#!/bin/bash
  
trap "" 13

connection=$1

work_dir=$PWD
mkdir -p output/result
mkdir -p output/coverage

for ((i=0;i<=7;i++))
do
    if [ -e /dev/loop$i ]
    then
        continue
    fi
    mknod /dev/loop$i b 7 $i
done

cp -rf /data/qemu/scripts/* /usr/bin/scripts/
cd /usr/bin/scripts/
chmod 777 *.sh


echo "挂载磁盘"
# 在docker上挂在磁盘（挂在目录为./disk-0_dir），注意脚本添加可执行权限
./mount_disk.sh disk-0.img

echo "在磁盘中设置断点文件"
# 在磁盘中设置断点文件，防止qemu虚机内的业务组件全部自动拉起。后续可以根据测试场景，按需拉起组件
touch disk-0_dir/no_start_initrd_service
touch disk-0_dir/no_start_rpg_service

echo "往磁盘中复制qemu内load和exec脚本，以及用例"
cp $work_dir/dtest-ysx/script/load_in_qemu.sh disk-0_dir/
cp $work_dir/dtest-ysx/script/exec_in_qemu.sh disk-0_dir/
cp $work_dir/dtest-ysx/script/load_st_in_qemu.sh disk-0_dir/
cp $work_dir/dtest-ysx/script/exec_st_in_qemu.sh disk-0_dir/
cp -rf $work_dir/* disk-0_dir/


echo "卸载磁盘"
# 卸载磁盘
sh umount_disk.sh disk-0.img

echo "启动qemu"
rm -rf /tmp/vm-aarch64-memback0-0.bin
./qemu.run -q

sleep 60
cd $work_dir

echo "调用qemu内部署脚本"
if [[ "$connection" == "ssh"  ]]
then
  chmod +x $PWD/dtest-ysx/script/call_load_in_qemu.sh
  $PWD/dtest-ysx/script/call_load_in_qemu.sh
else
  python3 $PWD/dtest-ysx/script/call_load_in_qemu.py $qemu_telnet_port
fi
