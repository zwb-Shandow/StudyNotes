# 库打包及查找

CMake 提供两种方式用于查找库，分别为 find_package 和 pkg-config。

## 1. find_package

> 通过 CMAKE_PREFIX_PATH 指定 find_package 查找目录

提供两种完全不同的方式来搜索，分别为 Module 模式和 Config 模式。

- **Module mode**

查找 Find<PackageName>.cmake 文件。

仅支持 find_package 基本命令签名。

- **Config mode**

查找 `<lowercasePackageName>-config.cmake` 或 `<PackageName>Config.cmake` 文件；如对版本有要求，则查找 `<lowercasePackageName>-config-version.cmake` 或 `<PackageName>ConfigVersion.cmake`。

支持 find_package 全部命令签名

### 1.1 命令

```
find_package(<PackageName> [version] [EXACT] [QUIET] [MODULE]
             [REQUIRED] [[COMPONENTS] [components...]]
             [OPTIONAL_COMPONENTS components...]
             [NO_POLICY_SCOPE])
```

### 1.2 配置文件创建

#### 1.2.1 准备 xx-config.cmake.in

```cmake
@PACKAGE_INIT@  # 在 configure_file 时自动替换为对路径及内容进行检查的预设 cmake 宏

# Our library dependencies (contains definitions for IMPORTED targets)
include(${CMAKE_CURRENT_LIST_DIR}/@CMAKE_PROJECT_NAME@-targets.cmake)

# 由 configure_package_config_file 的 PATH_VARS 替换，如果引用的文件或目录不存在，则宏将失败
set_and_check(@CMAKE_PROJECT_NAME@_INCLUDE_DIRS "@PACKAGE_INCLUDE_INSTALL_DIR@")
set_and_check(@CMAKE_PROJECT_NAME@_LIB_DIR "@PACKAGE_LIB_INSTALL_DIR@")
check_required_components(@CMAKE_PROJECT_NAME@)

# 包的所有必须依赖项必须在配置文件中找到， 若依赖缺失，则 cmake 阶段终止；若不指定依赖，则 make 阶段终止
include(CMakeFindDependencyMacro)
# 在 CONFIG 配置中查找依赖的唯一方法，所有包含在 find_package 中的内容应当无修改的写入此函数中
# 若依赖仅提供了 PkgConfig 配置，则可自定义 Module 配置
find_dependency(absl CONFIG REQUIRED)  

# Set found status
set(@CMAKE_PROJECT_NAME@_FOUND TRUE) 
set(@CMAKE_PROJECT_NAME@_INCLUDE_DIRS ${@CMAKE_PROJECT_NAME@_INCLUDE_DIRS} ${INCLUDE_DIRS})
set(@CMAKE_PROJECT_NAME@_LIBRARY_DIRS ${@CMAKE_PROJECT_NAME@_LIB_DIR} ${LIBRARY_DIRS})
set(@CMAKE_PROJECT_NAME@_LIBRARIES ${@CMAKE_PROJECT_NAME@_LIBS_LIST})
```

 #### 1.2.1 库导出为 target

将库文件，可执行文件，头文件安装到指定目录

```cmake
install(TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME}-targets
		LIBRARY DESTINATION lib  # 动态库安装路径
		ARCHIVE DESTINATION lib  # 静态库安装路径
		RUNTIME DESTINATION bin  # 可执行文件安装路径
		PUBLIC_HEADER DESTINATION include)  # 头文件安装路径
```

将 `EXPORT ${PROJECT_NAME}` 的信息写入到 **xx-targets.cmake** 文件中

```cmake
install(EXPORT ${PROJECT_NAME}
		FILE ${PROJECT_NAME}-targets.cmake
		NAMESPACE ${PROJECT_NAME}::  # 可选项
		DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME})
```

#### 1.2.2 获取配置变量，寻找项目依赖包

生成 **xx-config.cmake** 文件，命令如下

```cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
	${PROJECT_NAME}-config.cmake.in ${PROJECT_NAME}-config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}
	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)
install(FILES ${PROJECT_NAME}-config.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME})
```

#### 1.2.3 写入库的版本信息

生成 **xx-config-version.cmake** 文件，命令如下

```cmake
write_basic_package_version_file(
	${PROJECT_NAME}-config-version.cmake
	VERSION ${PROJECT_VERSION}
	COMPATILITY AnyNewerVersion)  # 表示该库向下兼容
install(FILES ${PROJECT_NAME}-config-version.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME})
```

## 2. pkg-config

> 通过 PKG_CONFIG_PATH 指定 pkg-config 搜索路径

### 2.1 命令行工具

```shell
# 显示库的编译器标志及链接标志
pkg-config --cflags --libs <package>
```

### 2.2 命令

提供 `pkg_check_modules()` 和 `pkg_search_module()` 命令。区别在于， `pkg_search_module()` 除了检查所有指定的模块之外，它**只搜索第一个成功的匹配项**。

```cmake
find_package(PkgConfig REQUIRED)
pkg_search_module(<prefix>
                  [REQUIRED] [QUIET]
                  [NO_CMAKE_PATH]
                  [NO_CMAKE_ENVIRONMENT_PATH]
                  [IMPORTED_TARGET [GLOBAL]]
                  <moduleSpec> [<moduleSpec>...])
pkg_check_modules(<prefix>
                  [REQUIRED] [QUIET]
                  [NO_CMAKE_PATH]
                  [NO_CMAKE_ENVIRONMENT_PATH]
                  [IMPORTED_TARGET [GLOBAL]]
                  <moduleSpec> [<moduleSpec>...])
```

### 2.2 配置文件创建

pkg-config 通过在指定路径中搜索 xx.pc 文件来定位库的相关路径，pc 文件的安装路径通常为 `${CMAKE_INSTALL_PREFIX}/lib/pkgconfig` 或 `${CMAKE_INSTALL_PREFIX}/share/pkgconfig`，以下给出 xx.pc.in 文件的模板示例:

```
prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
includedir=${prefix}/@CMAKE_INSTALL_INCLUDEDIR@
libdir=${exec_prefix}/@CMAKE_INSTALL_LIBDIR@

Name: lib@CMAKE_PROJECT_NAME@
Version: @PROJECT_VERSION@
Description: @PACKAGE_DESCRIPTION@
URL: @PACKAGE_URL@
Requires:
Libs: -L${libdir} -l@CMAKE_PROJECT_NAME@
Cflags: -I${includedir}
```

通过 `configure_file()` 命令，将 xx.pc.in 生成 xx.pc 文件。

```cmake
configure_file(xx.pc.in xx.pc @ONLY)
```



以下是关键字的简短说明。

- **Name**: 库或软件包的可读名称。不影响 pkg-config 工具的使用，该工具查找 .pc 文件的名称。
- **URL**: 软件包信息 url
- **Description**: 软件包的简要概述
- **Version**: 软件包的版本
- **Requires**: 软件包所需的依赖项列表。可以使用比较运算符(=、<、>、<=、>=)指定依赖版本
- **Requires.private**: 软件包所需但未向应用程序公开的依赖列表。**推荐(避免 [overlinking](https://wiki.mageia.org/en/Overlinking_issues_in_packaging) 问题)**
- **Conflicits**: 可选字段，描述与此软件包冲突的包
- **Cflags**: 特定与此软件包的编译器标志以及不支持 pkg-config 的任何必须库。如果所需库支持 pkg-config，则应将它们添加到 Requires 或 Requires.private。
- **Libs**: 特定于此软件包的链接标志以及不支持 pkg-config 的任何必须库。
- **Libs.private**: 软件包所需但未公开给应用程序的私有库的链接标志。

## 3. 参考示例

[cmake_package](https://github.com/zwb-Shandow/cmake-package.git)

## 4. 参考链接

[1] [Guide to pkg-config](https://people.freedesktop.org/~dbn/pkg-config-guide.html)