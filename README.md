# librm

[![build](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/build.yml/badge.svg)](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/build.yml)
[![docs](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/doxygen-gh-pages.yml/badge.svg)](https://github.com/IRobot-EC-2024/irobotEC/actions/workflows/doxygen-gh-pages.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/irobot-ec-2024/irobotec/badge/develop)](https://www.codefactor.io/repository/github/irobot-ec-2024/irobotec/overview/develop)

跨平台Robomaster嵌入式软件开发框架，为多种硬件平台提供统一的外设API、设备驱动和算法库。

- [x] STM32F/H
- [x] Linux PC
- [x] Raspberry Pi/Orange Pi
- [x] Jetson

使用类似Arduino的语法，快速操作手中的硬件和编写业务逻辑，无需关心偏底层的驱动编写。编译所需最低C++标准为C++17。

## 使用方法

### Linux

1. 下载仓库。注意为了连同第三方库一起下载，clone时需要使用`--recursive`参数：

    ```shell
    git clone --recursive https://github.com/IRobot-EC-2024/irobotEC.git
    ```

2. 在CMakeLists.txt里把整个仓库添加为子目录，并把`irobotEC`静态库链接到需要使用本库的目标上；

    ```cmake
    add_subdirectory(<仓库路径>)
    target_link_libraries(<目标> PUBLIC irobotEC)
    ```
3. 开始使用。使用方法请参考[examples](examples/)文件夹下的例程。

### STM32

方法同上，但需要注意librm的STM32部分基于HAL库开发，且依赖STM32CubeMX生成的CMake工程。请确保工程中存在CubeMX自动生成的`stm32cubemx` CMake
target。

## API文档

API文档可以使用Doxygen构建，也可以在[这里](https://irobot-ec-2024.github.io/irobotEC/)查阅。

```shell
doxygen ./Doxyfile
```

正确构建后，文档会被放在`docs/`文件夹下。