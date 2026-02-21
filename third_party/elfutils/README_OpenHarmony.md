# elfutils

仓库包含第三方开源软件elfutils，elfutils是一个用于读取、创建和修改ELF二进制文件，查找和处理GNU/Linux上进程和核心文件的DWARF调试数据、符号、线程状态和堆栈跟踪的工具和库的集合。libabigail软件依赖了elfutils。

## 目录结构

```
backends/      不同平台的后端实现
doc/           文档
lib/           公共库源码
libasm/        asm相关库源码
libcpu/        cpu相关库源码
libdw/         dw相关库源码
libdwelf/      dwelf相关库源码
libdwfl/       dwfl相关库源码
libebl/        ebl相关库源码
libelf/        elf相关库源码
src/           源文件
tests/         测试目录
COPYING        证书文件
COPYING-GPLV2  证书文件
COPYING-LGPLV3 证书文件
README         README说明
```

## OpenHarmony如何使用elfutils

OpenHarmony上elfutils是用作libabigail的依赖软件，libabigail在生成ABI特征文件的时候，需要通过elfutils读取二进制库内的DWARF调试数据。

## OpenHarmony如何集成elfutils

libabigail的编译依赖了elfutils，elfutils被集成到libabigail工具中。

### 1.elfutils的编译

elfutils的编译入口在其根目录下的BUILD.gn中。简单示意如下：

```makefile
# Build libelf
ohos_static_library("libelf_static") {
  configs = [ ":elfutils_defaults" ]

  sources = sources_libelf
  include_dirs = [
    "//third_party/elfutils",
    "//third_party/elfutils/lib",
    "//third_party/elfutils/libelf",
  ]
  deps = [ "//third_party/zlib:libz" ]

  subsystem_name = "thirdparty"
  part_name = "elfutils"
}

# Build libdw
ohos_static_library("libdw_static") {
  configs = [ ":elfutils_defaults" ]

  sources = sources_backends
  sources += sources_libcpu
  sources += sources_libdw
  sources += sources_libdwelf
  sources += sources_libdwfl
  sources += sources_libebl

  include_dirs = [
    "//third_party/elfutils",
    "//third_party/elfutils/lib",
    "//third_party/elfutils/libasm",
    "//third_party/elfutils/libelf",
    "//third_party/elfutils/libcpu",
    "//third_party/elfutils/libdw",
    "//third_party/elfutils/libdwelf",
    "//third_party/elfutils/libdwfl",
    "//third_party/elfutils/libebl",
  ]
  deps = [ ":libelf_static" ]

  subsystem_name = "thirdparty"
  part_name = "elfutils"
}

```

### 2.使用elfutils

libabigail的编译依赖了elfutils。简单示意如下：

```makefile
ohos_static_library("libabigail_static") {
  configs = [ "//third_party/libabigail:libabigail_defaults" ]

  sources = [
    ......
  ]

  include_dirs = [
    ......
  ]

  external_deps = [
    "//third_party/elfutils:libdw_static",
  ]

  subsystem_name = "thirdparty"
  part_name = "libabigail"
}
```
## elfutils相关内容

[elfutils官网](https://sourceware.org/elfutils/)

## License

- `LGPL V3.0`


- `GPL V2.0`


- `GPL V3.0`

## 风险提示

**<u>‍elfutils是LGPLV3和GPLV3协议类型的三方开源软件，请谨慎使用，使用时须履行相应的开源义务。</u>**
