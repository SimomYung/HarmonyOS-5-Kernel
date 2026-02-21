#!/bin/sh

cp /opt/vrpv8/home/testcase/hlibc/ut_hlibc /hpe
cat > /hpe/conf/app.cfg << EOF
1
/hpe/ut_hlibc --gtest_output=xml:/opt/vrpv8/home/hpetest-gtest-result.xml
EOF

cat >> /hpe/conf/config.hpe << EOF
[ut_hlibc]
hpe_blkmem_size=8
hpe_pool_gpa=8388608
hpe_pool_size=9216
hpe_pool_hpa=0
hpe_pktmbuf_threshold=32768
hpe_board_type=21
hpe_skip_mods=none
hpe_dmod_path=/var/run/dmod
hpe_blockpool_num=128
[/ut_hlibc]
EOF

cd /bin
./start_hpe.sh qemu pre_run

/bin/hctl >> /var/run/hpe/log/start_hctl.log 2>&1 &

# /var/run/test_finish.log exists, means all testcase had run
finish_file="/var/run/test_finish.log"

while true
do
    if [ -f "$finish_file" ]; then
        # collect sysm cov data
        hpecli sysm_cov
        sleep 10s
        echo "start collect cov data...................."
        cp -rf /var/log/lltcov /opt/vrpv8/home
        echo "end collect cov data......................"
        sync
        sync
        echo "FINISHED"
        echo "FINISHED"
        echo "FINISHED"
    fi
done
