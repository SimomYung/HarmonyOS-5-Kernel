# libfuse

仓库包含第三方开源软件libfuse，libfuse是FUSE（用户态文件系统）框架的用户态程序接口。libfuse提供挂载、卸载文件系统、解析和传递内核的请求接口等功能。在OpenHarmony中，开发者可以通过注册回调函数的形式，实现特定的用户态文件系统功能，例如cloudfiledaemon（云端文件系统服务）等。

## 目录结构

```
doc/        文档
example/    样例代码
include/    libfuse库头文件
lib/        libfuse库源代码
test/       测试脚本代码
util/       fusermount等二进制工具
README      README说明
```

## OpenHarmony如何使用libfuse

OpenHarmony只编译使用libfuse仓库中lib/和include/目录下的源代码和头文件。libfuse作为cloudfiledaemon的依赖模块，来实现具体的云端文件系统功能。

## OpenHarmony如何集成libfuse

libfuse编译为动态链接库，打包到system分区镜像中。

### 1. libfuse的编译

libfuse的编译入口在其根目录下的BUILD.gn中。简单示意如下：

```
libfuse_source = [
    "//third_party/libfuse/lib/fuse.c",
    "//third_party/libfuse/lib/fuse_loop.c",
    "//third_party/libfuse/lib/fuse_loop_mt.c",
    "//third_party/libfuse/lib/fuse_lowlevel.c",
    "//third_party/libfuse/lib/fuse_opt.c",
    "//third_party/libfuse/lib/fuse_signals.c",
    "//third_party/libfuse/lib/buffer.c",
    "//third_party/libfuse/lib/compat.c",
    "//third_party/libfuse/lib/cuse_lowlevel.c",
    "//third_party/libfuse/lib/helper.c",
    "//third_party/libfuse/lib/modules/subdir.c",
    "//third_party/libfuse/lib/mount_util.c",
    "//third_party/libfuse/lib/fuse_log.c",
    "//third_party/libfuse/lib/mount.c",
    "//third_party/libfuse/lib/modules/iconv.c",
]

ohos_shared_library("libfuse") {
    configs = [ ":libfuse_config" ]
    sources = libfuse_source
    ldflags = [
        "-ldl",
        "-Wl,--version-script",
        rebase_path("//third_party/libfuse/lib/fuse_versionscript", root_build_dir),
        "-Wl,-soname,libfuse3.so.3",
        "-Wl,--no-undefined",
        "-Wl,--as-needed",
        "-shared",
        "-fPIC",
    ]
    external_deps = [ "c_utils:utils" ]
    subsystem_name = "thirdparty"
    part_name = "libfuse"
    output_name = "libfuse"
    install_enable = true
    install_images = [ "system" ]
}

```

### 2. 使用libfuse

在需要使用libfuse的模块构建配置中，增加对应依赖，示例如下：

```
ohos_shared_library("cloudfiledaemon") {

  sources = [
    ......
  ]

  include_dirs = [
    ......
    "//third_party/libfuse/include",
    "//third_party/libfuse/lib",
  ]

  external_deps = [
    "//third_party/libfuse:libfuse",
  ]

...
}
```

## libfuse相关内容

[libfuse主页](https://github.com/libfuse/libfuse)
[libfuse文档](https://libfuse.github.io/doxygen/index.html)

## License

- include/ 和 lib/ 目录下的源文件，使用LGPL2.1协议；
- 其他源文件，使用GPL2协议。

## 风险提示

**libfuse是LGPLV2和GPLV2协议类型的三方开源软件，OpenHarmony当前仅使用LGPLV2部分。使用时需履行相应的开源义务。**
