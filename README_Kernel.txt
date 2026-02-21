################################################################################

1. How to Build
	- get Toolchain
		- Command Line Tools for HarmonyOS 5.0.0 Release
		  https://developer.huawei.com/consumer/cn/download/
		  Command Line Tools for Linux 5.0.3.906
		

	- compilation process
		To compile this project, perform the following steps:
		- bash build_tools.sh 
		- CONFIG=build.conf bash build.sh
		This script will build the whole project.

2. Output files
	If the compilation is successful, binary files such as .so and .ko files related to the Hongmeng kernel are archived in output/.

3. How to Clean
	To clean the whole project, perform the following steps:
		- rm -rf output

################################################################################
