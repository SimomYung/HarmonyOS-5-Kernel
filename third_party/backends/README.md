# SANE
## Introduction
SANE is an application programming interface (API) that provides standardized access to any raster image scanner hardware (flatbed scanner, hand-held scanner, video- and still-cameras, frame-grabbers, etc.). 

You can also learn more about the SANE project through [the official website](http://sane-project.org/)

## Background Brief
In the process of OpenHarmony's southward ecological development, it is necessary to be compatible with printers in the stock market. The use of CUPS printing system can directly connect with most printers in the market, which also reduces the difficulty for printer manufacturers to adapt to OpenHarmony.

## Directory structure
```
- LICENSE                           Copyright File
- OAT.xml                           OAT.XML filtering configuration file
- README.OpenSource                 Project README OpenSource files
- README.md                         English Description
- README_zh.md                      Chinese Description
- backend                           scanning device backend source code
- include                           SANE API interface
- lib                               SANE library source code
- sanei                             SANE internal utility functions and tools
- doc                               documents and instruction files
```

## How to use
### 1、Header file import
```c
#include <sane/sane.h>
```
### 2、Add Compilation Dependency
Add in the bundle. json file
```json
"deps": {
  "third_party": [
    "backends"
  ]
}
```
Add dependencies where needed in BUILD.gn

```json
deps += [ "//third_party/backends:third_sane" ]
```
### 3、Example of interface usage
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
[How to involve](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/contribute/参与贡献.md)

[Commit message spec](https://gitee.com/openharmony/device_qemu/wikis/Commit%20message%E8%A7%84%E8%8C%83)