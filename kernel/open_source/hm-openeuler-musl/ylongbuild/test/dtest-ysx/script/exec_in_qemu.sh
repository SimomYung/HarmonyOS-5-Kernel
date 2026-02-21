#!/bin/sh

export module="$@"
echo "module=$module"

dmesg -n 3

/etc/init.d/pre_init.sh &
/bin/dophi_server &
mv /bin/ysckd.elf /bin/hmnetd.elf
/usr/bin/sectool.elf -l /bin/hmnetd.elf lwipd_exec_t
/bin/hmnetd.elf &
touch /tmp/ysckd.path
sleep 5
/bin/install_ydrv.sh

if [ "$module" == "ylongc" ];then
    cp /opt/vrpv8/home/testcase/hlibc/ut_hlibc /hpe
cat > /hpe/conf/app.cfg << EOF
1
/hpe/ut_hlibc --gtest_output=xml:/opt/vrpv8/home/hpetest-gtest-result.xml
EOF
    cd /bin
    ./start_hpe.sh qemu

elif [ "$module" == "hm" ];then
    cd /opt/vrpv8/home/testcase/hm/
    tar -zxvf *.tar.gz
    rm *.tar.gz
    cd hm_hal_irq_t_tmp
    sh debug_runtest.sh test conf list

elif [ "$module" == "hpk" ];then
    cp /opt/vrpv8/home/testcase/hpk/hpk.bin /bin
cat > /hpe/conf/app.cfg << EOF
0
EOF
    cd /bin
    ./start_hpe.sh qemu

elif [ "$module" == "dap" ];then
    cp /opt/vrpv8/home/testcase/dap/daptest /hpe
    cd /hpe
    ./daptest --gtest_output=xml:/opt/vrpv8/home/daptest-gtest-result.xml

else
    echo "No such module"
    exit 1
fi