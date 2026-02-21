# LAME

LAME是LGPL协议授权的高质量MP3编码器。LAME于1998年发布，此后进行了多次改进，引入了新的心理声学模型，被公认为一款出色的MP3编码器，在业界应用广泛。

[LAME官网地址](https://lame.sourceforge.io)为[https://lame.sourceforge.io](https://lame.sourceforge.io)，您可以通过访问它获取更多关于LAME项目的信息。

OpenHarmony引入第三方开源软件LAME主要用于对数字音频进行MP3编码。LAME最近更新发布的版本为3.100，这也是OpenHarmony引入的版本。

## 主要目录结构

```
doc              #文档
include          #包含头文件
libmp3lame       #MP3编码主要源代码
test             #测试代码
Dll              #Windows dll接口
vc_solution      #vs解决方案
Makefile         #编译描述文件
INSTALL          #安装（编译）指导
API              #接口说明
COPYING          #版权声明
LICENSE          #许可证说明
README.md        #软件说明
```

## OpenHarmony如何集成LAME

LAME位于OpenHarmony的thirdparty目录下，使用OpenHarmony中依赖部件的方式进行编译，得到单独的动态链接库。OpenHarmony通过链接到这个库，实现相关功能。

1. 主干代码下载
   ```
   repo init -u https://gitee.com/openharmony/manifest.git -b master --no-repo-verify
   repo sync -c
   repo forall -c 'git lfs pull'
   ```
2. 在使用的模块进行依赖
   ```
   deps = [ "//third_party/lame:lame" ]
   ```
3. 预处理
   ```
   ./build/prebuilts_download.sh
   ```
4. 编译
   ```
   ./build.sh --product-name rk3568 --ccache
   ```
编译得到的动态链接库所在路径：`out/rk3568/thirdparty/lame/liblame.z.so`。

## 许可证

本项目遵从[LICENSE](https://gitee.com/openharmony-sig/third_party_lame/blob/master/LICENSE)和[COPYING](https://gitee.com/openharmony-sig/third_party_lame/blob/master/COPYING)中所描述的许可证。

## 特别注意

根据LAME项目[LICENSE](https://gitee.com/openharmony-sig/third_party_lame/blob/master/LICENSE)的要求，如果后续对LAME有修改，**必须**将修改反馈至LAME项目。