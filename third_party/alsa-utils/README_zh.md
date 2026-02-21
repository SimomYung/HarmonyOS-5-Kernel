- [三方开源软件alsa-utils](#三方开源软件alsa-utils)
  - [1. alsa-utils简介](#1-alsa-utils简介)
  - [2. 组件引入背景简述](#2-组件引入背景简述)
  - [3. 使用场景](#3-使用场景)
  - [4. 为OpenHarmony带来的价值](#4-为openharmony带来的价值)
  - [5. 如何使用](#5-如何使用)
    - [5.1 添加编译](#51-添加编译)
  - [5.2 常用命令的使用](#52-常用命令的使用)

## 三方开源软件alsa-utils
### 1. alsa-utils简介
alsa-utils作为一种HDI之外的定位工具，便于问题定界、方便对HDI不熟悉的驱动移植生态伙伴使用，包含用于ALSA的命令行实用程序。
您可以通过[ALSA官网](http://www.alsa-project.org)了解更多关于ALSA项目的信息。

### 2. 组件引入背景简述
为满足OEM生态伙伴已经支持ALSA的产品采用"ALSA兼容方案"接入OpenHarmony，而提供的命令行定位调试工具。

### 3. 使用场景
只能用于已安装ALSA驱动程序和ALSA C库(ALSA -lib)的系统中使用，且只能作为独立工具使用。

### 4. 为OpenHarmony带来的价值
1. 满足OEM生态伙伴中已支持的ALSA的产品快速接入OpenHarmony诉求。
2. 降低开发者学习成本加速南向生态发展。

### 5. 如何使用
#### 5.1 添加编译
由于alsa-utils作为调试工具默认不编译打包，您可以采用以下两种方式进行编译：
1. 如果采用Openharmony主线源码
您可采用以下命令进行编译：
```bash
./build.sh --product-name [PRODUCT_NAME] --ccache --build-target third_party/alsa-utils:alsa-utils
```
2. 如已拉出分支需要编译打包
在现有的OpenHarmony的文档或者子系统中有如下添加方法
方法：在您的 bundle.json 文件 添加
```json
"build": {
  "sub_component": [
  	"//third_party/alsa-utils:alsa-utils"
  ]
}
```
注意：此三方件编译需要先编译ALSA-lib

### 5.2 常用命令的使用

aplay放音XXX.wav音频文件，详细参数及使用请用“ -h” 查看

```bash
aplay  /data/XXX.wav
```

arecord录音test.wav，其中 -d 30表示录制30s时间， -r 44100表示rate 为44100Hz， -c 2表示为双声道立体声， -t wav表示录制的音频文件是wav格式

```bash
arecord -d 30 -f cd -r 44100 -c 2 -t wav test.wav
```

amixer命令打开录放音通路，contents用来查看各个元素（以dayu_200开发板举例），cset来设置录放音通路：

```bash
# amixer contents
numid=2,iface=MIXER,name='Capture MIC Path'
  ; type=ENUMERATED,access=rw------,values=1,items=4
  ; Item #0 'MIC OFF'
  ; Item #1 'Main Mic'
  ; Item #2 'Hands Free Mic'
  ; Item #3 'BT Sco Mic'
  : values=1
numid=1,iface=MIXER,name='Playback Path'
  ; type=ENUMERATED,access=rw------,values=1,items=11
  ; Item #0 'OFF'
  ; Item #1 'RCV'
  ; Item #2 'SPK'
  ; Item #3 'HP'
  ; Item #4 'HP_NO_MIC'
  ; Item #5 'BT'
  ; Item #6 'SPK_HP'
  ; Item #7 'RING_SPK'
  ; Item #8 'RING_HP'
  ; Item #9 'RING_HP_NO_MIC'
  ; Item #10 'RING_SPK_HP'
  : values=6
#amixer cset numid=1  6
#amixer cset numid=2  1
```