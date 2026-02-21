# ylong_c

ylong_c开发仓

# how to build hlibc 

## login docker
sudo docker run --hostname hongmeng -v /home/z00509929/:/home/z00509929/ -v /dev:/dev -v /lib/modules/:/lib/modules/ -v /run:/run --add-host='osc:127.0.0.1' -e UID=1000 -e UPDATE_PNFTOOL=true --privileged -i --rm -t szvecr02.his.huawei.com:80/ecr-build/yunshan_iot:ecos4.13.3.allcode_hongmeng_v1.3.6 /bin/bash

## build hlibc rpm

./build.sh -a aarch64 -r http://100.95.111.143/repo_hpp_DevelopBranch/app_r21/mid2021.repo

###
![image](/uploads/79a9fcc6-61bf-46c6-92f1-847c06a0d7e9/1626837037701.png '1626837037701.png')