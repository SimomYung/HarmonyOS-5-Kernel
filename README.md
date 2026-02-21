# HarmonyOS-5-Kernel
fork from https://consumer.huawei.com/en/opensource/detail/?siteCode=worldwide&amp;productCode=Smartphones&amp;fileType=openSourceSoftware&amp;pageSize=10&amp;curPage=1

## 查看源码请切换分支，原始代码包请查看releases。

## 本文档提供 Hongmeng 1.9.3 开源项目的编译指导。

### 1. 如何构建
	获取工具链：
		下载地址 https://developer.huawei.com/consumer/cn/download/command-line-tools-for-hmos
		获取Command Line Tools for HarmonyOS 5.0.0 Release
	编译流程：
		编译完整项目请执行以下命令：
		- bash build_tools.sh 
		- CONFIG=build.conf bash build.sh
		
### 2.输出文件及清理
	编译成功后，与 Hongmeng kernel 相关的二进制文件例如 .so 和 .ko 将在 ./output。
	如果要清理编译文件，只需执行 rm -rf output 删除 ./output。
	
### 3.如何构建模块
	仅当需要使用 OpenHarmony 构建系统时才需进行模块构建。
	首先你需要获取  OpenHarmony  源码：https://gitcode.com/openharmony/manifest.git
	复制你想构建的模块源码到  OpenHarmony  源码。如果  OpenHarmony  源码中有相关源码，则先删除相关源码，再将本项目的代码复制进去。注意，所有模块可以一次全部构建。
	构建命令请参考：https://gitee.com/openharmony/build/blob/master/docs/生成开源软件包.md
