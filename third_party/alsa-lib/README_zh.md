- [三方开源软件alsa-lib](#三方开源软件alsa-lib)
  - [1. Alsa-lib简介](#1-alsa-lib简介)
  - [2. 引入背景简述](#2-引入背景简述)
  - [3. 使用场景](#3-使用场景)
  - [4. 为OpenHarmony带来的价值](#4-为openharmony带来的价值)
  - [5. 如何使用](#5-如何使用)
    - [5.1 内核编译开关配置](#51-内核编译开关配置)
    - [5.2 产品化编译开关配置](#52-产品化编译开关配置)
    - [5.3 alsa-lib组件编译依赖添加](#53-alsa-lib组件编译依赖添加)
## 三方开源软件alsa-lib
### 1. Alsa-lib简介
ALSA-lib是和Linux内核虚拟设备ALSA进行接口的库，虽然Linux下ALSA给用户空间提供了系统调用，但alsa-lib对系统调用进行进一步封装，并实现了很多插件，例如多音频源进行混音播放时，调用的就是dmix插件。ALSA-lib是ALSA框架应用库(必需基础库)。

源代码最新参考资料可以访问：[alsa-lib官网主页](http://www.alsa-project.org/alsa-doc/alsa-lib/)。
您也可以通过[ALSA官网主页](http://www.alsa-project.org)了解更多关于ALSA项目的信息。

### 2. 引入背景简述
Openharmony南向生态发展过程中，已经支持过ALSA的OEM产商在接入OpenHarmony过程中存在使用内核ALSA框架的诉求。鉴于此OpenHarmony Audio为支持南向生态伙伴快速接入鸿蒙、快速实现产品化而提供的“ALSA兼容方案”，本方案采用插件化的适配器模式通过alsa-lib实现ALSA的兼容。

### 3. 使用场景
本方案仅供OEM产商中已支持ALSA框架的设备在移植OpenHarmony系统过程中作为备选方案使用，建议优先选择ADM(Audio Driver Model, HDF 音频驱动模型)。
本仓接口能力的直接调用对象为驱动子系统的Audio模块。

### 4. 为OpenHarmony带来的价值
1. 满足OEM生态伙伴中已支持的ALSA的产品快速接入OpenHarmony诉求。
2. 提供产品化过程中ADM(Audio Driver Model, HDF 音频驱动模型)暂不具备的能力。

### 5. 如何使用
ADM与ALSA在一款特定产品上的使用方式为二选一，考虑到设备内存，不建议两者同时存在。
使用过程中需要完成内核编译选项的配置、产品化编译开关的配置，如需产商需要自己实现设备抽象层(DAL)，则需要配置对的编译依赖。

#### 5.1 内核编译开关配置

需要在各自产品的Linux kernel配置文件中打开对应开关，路径如下：其中${product_name}表示您的产品名称

kernel/linux/config/linux-5.10/arch/arm64/configs/${product_name}_standard_defconfig

以rk3568_standard_defconfig为例配置如下：

```sh
CONFIG_SOUND=y
CONFIG_SND=y

# CONFIG_DRIVERS_HDF_AUDIO is not set
# CONFIG_DRIVERS_HDF_AUDIO_RK3568  is not set
```
#### 5.2 产品化编译开关配置

需要在各自产品配置文件中打开对应开关，路径如下：其中\\${product_company}表示您的企业名称，${product_name}表示您的产品名称
```bash

/vendor/${product_company}/${product_name}/config.json
```
开关如下：在drivers_peripheral_audio部件节点下，找到特性宏drivers_peripheral_audio_alsa_lib，开启为true，关闭为false，默认为false.
```json
drivers_peripheral_audio_alsa_lib = true
```

#### 5.3 alsa-lib组件编译依赖添加
如需要重新实现Audio DAL，则需要添加alsa-lib组件的编译依赖，可以采用以下两种方法进行添加。

**方法一（可选）：**
在您的 bundle.json 文件 添加
```json
"deps": {
  "third_part": [
    "alsa-lib"
  ]
}
```
**方法二（可选）：**
在您的BUILD.gn需要的地方添加依赖
```json
deps += [ "//third_party/alsa-lib:libasound" ]
```
