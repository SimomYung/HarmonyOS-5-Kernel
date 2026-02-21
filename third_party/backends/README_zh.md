# 三方开源软件sane-backends
## SANE简介
SANE是一个应用程序编程接口（API），它提供对任何光栅图像扫描仪硬件（平板扫描仪、手持式扫描仪、视频和静态摄像机、帧处理器等）的标准化访问。

您也可以通过[SANE官网主页](http://sane-project.org/)了解更多关于SANE项目的信息。

## 引入背景简述
OpenHarmony南向生态发展过程中，需要对存量市场的扫描仪进行兼容。使用SANE扫描系统能直接对接市场上大部分的扫描仪，也减少了扫描仪驱动适配OpenHarmony系统的难度。

## 目录结构
```
- LICENSE                           版权文件
- OAT.xml                           OAT.xml过滤配置文件
- README.OpenSource                 项目README.OpenSource文件
- README.md                         英文说明
- README_zh.md                      中文说明
- backend                           扫描设备后端源码
- include                           SANE API接口
- lib                               SANE 库源码
- sanei                             SANE 内部实用函数和工具
- doc                               文档和说明文件
```

## 如何使用
### 1、头文件引入
```c
#include <sane/sane.h>
```
### 2、添加编译依赖
在您的 bundle.json 文件 添加
```json
"deps": {
  "third_party": [
    "backends"
  ]
}
```
在您的BUILD.gn需要的地方添加依赖
```json
deps += [ "//third_party/backends:third_sane" ]
include_dirs += [ "//third_party/backends/include" ]
```
```c
SANE_Status status;
SANE_Handle handle;

// Initialize SANE
status = sane_init(NULL, NULL);
if (status != SANE_STATUS_GOOD) {
    fprintf(stderr, "Failed to initialize SANE: %s\n", sane_strstatus(status));
    return 1;
}

// Open the first scanner device
status = sane_open("your_scanner_device_name", &handle);
if (status != SANE_STATUS_GOOD) {
    fprintf(stderr, "Failed to open scanner: %s\n", sane_strstatus(status));
    return 1;
}

// Get scanner device information
const SANE_Device *device_info;
status = sane_get_devices(&device_info, SANE_FALSE);
if (status != SANE_STATUS_GOOD) {
    fprintf(stderr, "Failed to get scanner device information: %s\n", sane_strstatus(status));
    return 1;
}

// Set scan parameters
SANE_Parameters parameters;
status = sane_get_parameters(handle, &parameters);
if (status != SANE_STATUS_GOOD) {
    fprintf(stderr, "Failed to get scan parameters: %s\n", sane_strstatus(status));
    return 1;
}

// Start scanning
SANE_Image image;
status = sane_start(handle);
if (status != SANE_STATUS_GOOD) {
    fprintf(stderr, "Failed to start scanning: %s\n", sane_strstatus(status));
    return 1;
}

// Read scan data
do {
    status = sane_read(handle, &image);
    if (status != SANE_STATUS_GOOD) {
        fprintf(stderr, "Failed to read scan data: %s\n", sane_strstatus(status));
        break;
    }

} while (status == SANE_STATUS_GOOD);

// Finish scanning
status = sane_cancel(handle);
if (status != SANE_STATUS_GOOD) {
    fprintf(stderr, "Failed to cancel scanning: %s\n", sane_strstatus(status));
    return 1;
}

// Close the scanner device
status = sane_close(handle);
if (status != SANE_STATUS_GOOD) {
    fprintf(stderr, "Failed to close scanner: %s\n", sane_strstatus(status));
    return 1;
}

// Exit SANE
sane_exit();
```

### 相关仓
[print_print_fwk](https://gitee.com/openharmony/print_print_fwk)

### 参与贡献
[如何贡献](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/contribute/参与贡献.md)

[Commit message规范](https://gitee.com/openharmony/device_qemu/wikis/Commit%20message%E8%A7%84%E8%8C%83)

