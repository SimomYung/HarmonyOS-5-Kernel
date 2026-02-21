## About
This module support HMbuild and test case.

## Directories
Directory src is hm-cmake-modules implementation.
Directory tests is hm-cmake-modules test cases.

## CI
Triggered by "test yocto" and "test cmake"

## Install
If you want to install hm-cmake, create a separate build tree
You may use the ``DESTDIR=<install_path>`` option to specify a custom
installation directory for CMake 

```
  $ mkdir build & cd build
  $ cmake .. & make
  $ DESTDIR=./ouput cmake --install .
```

## Test
### Normal Test
if you plan to develop hm-cmake or otherwise run the test suite, create
a separate build tree:

```
  $ mkdir cmake-build && cd cmake-build
  $ cmake .. & make test
  $ make test_clean 
```
### Enalbe Test with HM sdk
When you enable hm-sdk,tests that are tagged with CROSS will enable
these tests try to use cmakeng and compiler which are installed in sdk
so need install cmakeng to sdk at first
This is a sample and it has one more installation step than Normal test
**You need to change the directory to your local sdk directory**
```
  $ mkdir cmake-build && cd cmake-build
  $ cmake -DENABLE_TEST_WITH_SDK=True \
          -DTEST_HM_SDK_SYSROOT="/home/xxx/sdk/arm/sysroots/armv7a-euler-eabi/" \
          -DTEST_HM_NATIVE_SDK_PATH="/home/xxx/sdk/arm/sysroots/x86_64-eulersdk-linux/" \
          -DTEST_HM_TC_FILE="/home/xxx/sdk/arm/sysroots/x86_64-eulersdk-linux/usr/share/cmake/Modules/arm-euler-eabi-toolchain.cmake" \
           ..
  $ DESTDIR=~/sdk/arm/sysroots/x86_64-eulersdk-linux/ cmake --install .
  $ make test
  $ make test_clean
```

