## 1 Tests Running Environment Requirements

- cmake > 3.16
- kbuild-tools(kbuild-conf kbuild-fixdep kbuild-mconf kbuild-unifdef) are located in sdk
- python3
how to install sdk
```
$ wget https://kernel.archive.rnd.huawei.com/archive/hongmengsdk/hongmeng-x86_64-arm-euler-eabi-toolchain-latest.sh --no-check-certificate
$ ./hongmeng-x86_64-arm-euler-eabi-toolchain-latest.sh -d ~/sdk/arm -y
```

how to install kbuild
```
$ cp ~/sdk/arm/sysroots/x86_64-eulersdk-linux/usr/bin/kbuild-*   /usr/local/bin
or 
$ export PATH=~/sdk/arm/sysroots/x86_64-eulersdk-linux/usr/bin/:${PATH}
```
## 2 Quick Start
### 2.1 Run tests in local:
```
$ git clone ssh://git@codehub-dg-y.huawei.com:2222/HongMengOS/SRC/hm-cmake-modules.git
$ cd tests 
$ ./build.sh 
```

### 2.2 Run tests in docker(CI):
- [config docker](https://codehub-y.huawei.com/HongMengOS/CI/hm-CI/wiki/view/doc/689699)
- start docker
```
$ git clone ssh://git@codehub-dg-y.huawei.com:2222/HongMengOS/SRC/hm-cmake-modules.git
$ docker run --rm --name tests -it --privileged -v [abs_path]/hm-cmake-modules:/share_files kernel.dockerhub.rnd.huawei.com/hmci-docker-image:v1-11.0 /bin/bash
```
- run tests
```
$ cp /opt/buildtools/kbuild/*  /usr/local/bin
$ cd /share_files/tests
$ ./build.sh  -d / 
```

## 3 Directory
tools: This is implementation of the test framework.
test_tools: This is test cases for testing test framework.
cmake_modules/cmd: This directory contains functional tests of Hmbuild Grammer.


