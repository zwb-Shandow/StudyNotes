# CMake

## 1. CMake 示例

```cmake
#======================================================================
# 指定cmake最低版本
cmake_minimum_required (VERSION 3.15)
# 给项目命名
project (demo) # 会隐式定义<project_name>_BINARY_DIR和<project_name>_SOURCE_DIR

# 允许gdb调试
set (CMAKE_BUILD_TYPE "Debug") # 可选Debug或Release
set (CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set (CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 添加C++11支持及其他选项
set (CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-11")
#===========================================================================

# 添加所有头文件，使得系统可以找到对应头文件，相当于gcc的-I参数
include_directories (
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/src")


add_executable (可执行文件名 生成该可执行文件的源文件 )

add_library (libname [SHARED | STATIC | MODULE] source1 source2...) # 生成动态库，静态库

# 向当前工程添加存放源文件的子目录，并可以指定中间二进制和目标二进制的存放位置
# EXCLUDE_FROM_ALL含义：将这个目录从编译过程中排除
# 添加一个文件夹进行编译，该文件夹下的CMakeLists.txt负责编译该文件夹下的源码
add_subdirectory (src_dir [binary_dir] [EXCLUDE_FROM_ALL])

# 将dir目录下的所有源代码列表存储到VARIABLE中
aux_source_directory (dir VARIABLE)

find_package (<name> [major.minor] [QUIET] [NO_MODULE] [ [ REQUIRED | COMPONENTS ] [ componets... ] ] )

# 添加链接库，相当于指定gcc的-I参数
target_link_libraries (name) 
# 指定目标包含的头文件路径
target_include_directories(name)
# 指定目标的编译选项
target_compile_options(name)

# 动态链接库或静态链接库的搜索路径，相当于gcc的-L参数
# link_directories (dir)

# 自定义搜索规则
file (GLOB VARIABLE "*.cpp")

# set直接设置变量的值
set (VARIABLE source1 source2...)
```

## 2. CMake 常用命令

### 2.1 CMake Install命令

```cmake
# install用于指定在安装时的运行规则。可以用来安装目标二进制、动态库、静态库以及文件、目录、脚本等
# 可能会用到变量`CMAKE_INSTALL_PREFIX`,用于指定 cmake install 时的相对地址前缀
install(TARGETS <target>... [...])
install({FILES | PROGRAMS} <file>... [...])
install(DIRECTORY <dir>... [...])
install(SCRIPT <file> [...])
install(CODE <code> [...])
install(EXPORT <export-name> [...])

# 1. 目标文件的安装
install(TARGETS targets...
	EXPORT <export-name>
	ARCHIVE DESTINATION <dir>  # 静态库  安装目录变量${CMAKE_INSTALL_LIBDIR}，默认安装文件夹为 lib
	LIBRARY DESTINATION <dir>  # 动态库  安装目录变量${CMAKE_INSTALL_LIBDIR}，默认安装文件夹为 lib
	RUNTIME DESTINATION <dir>  # 可执行二进制文件  安装目录变量${CMAKE_INSTALL_BINDIR}，默认安装文件夹为 bin
	PUBLIC_HEADER DESTINATION <dir>  # 与库关联的PUBLIC头文件  安装目录变量${CMAKE_INSTALL_INCLUDEDIR}，默认安装文件夹为 include
	PRIVATE_HEADER DESTINATION <dir>) # 与库关联的PRIVATE头文件  安装目录变量${CMAKE_INSTALL_INCLUDEDIR}，默认安装文件夹为 include
	
# 示例	
# 为了符合一般的默认安装路径，如果设置了DESTINATION参数，推荐配置在安装目录变量下的文件夹。
# 如下，会将可执行二进制 myrun 安装到 ${CMAKE_INSTALL_BINDIR} 目录，动态库 libmylib.so 安装到 ${CMAKE_INSTALL_LIBDIR} 目录，静态库 libmystaticlib.a 安装到 ${CMAKE_INSTALL_LIBDIR} 目录。
INSTALL(TARGETS myrun mylib mystaticlib
       RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
       LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
       ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 2. 目录的安装
# 如果目录名不以/结尾，那么这个目录将被安装为目标路径下的abc，如果目录名以/结尾，代表将这个目录中的内容安装到目标路径，但不包括这个目录本身。
# 可以使用PATTERN或REGEX选项以精细的粒度控制目录的安装，可以指定一个通配模式或正则表达式以匹配输入目录中遇到的目录或文件。PATTERN仅匹配完整的文件名，而REGEX将匹配文件名的任何部分，但它可以使用/和$模拟PATTERN行为。
install(DIRECTORY dirs...
		TYPE <type> | DESTINATION <dir>
		FILES_MATCHING
		PATTERN <pattern> | REGEX <regex>)
		
# 示例
# 如下，会将 icons 目录安装到 share/myproj，将 scripts/ 中的内容安装到 share/myproj，两个目录均不包含目录名为 CVS 的子目录，对于scripts/*的文件指定权限为OWNER_EXECUTE，OWNER_WRITE，OWNER_READ，GROUP_EXECUTE，GROUP_READ。
install(DIRECTORY icons scripts/
		DESTINATION share/myproj
		PATTERN "CVS" EXCLUDE
		PATTERN "scripts/*"
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)

# 3. 普通文件的安装
# FILES 为普通的文本文件，PROGRAMS 为非目标文件的可执行程序（如脚本文件）
# FILES|PROGRAMS若为相对路径给出的文件名，将相对于当前源目录进行解释
install(<FILES|PROGRAMS> files...
		TYPE <type> | DESTINATION <dir>)
```

**CMake install 默认路径**

|      **TYPE类型**       |                    **安装目录变量**                     | **默认安装文件夹** |
| :---------------------: | :-----------------------------------------------------: | :----------------: |
|           BIN           | ${CMAKE_INSTALL_BINDIR} / ${CMAKE_INSTALL_PREFIX}==/usr |        bin         |
|          SBIN           |                ${CMAKE_INSTALL_SBINDIR}                 |        sbin        |
| LIB(包含动态库和静态库) |                 ${CMAKE_INSTALL_LIBDIR}                 |        lib         |
|         INCLUDE         |               ${CMAKE_INSTALL_INCLUDEDIR}               |      include       |

### 2.2 CMake常用变量

```cmake
# 通过project指令定义的项目名称
PROJECT_NAME

# 三个变量指代内容一致，如果是in-source编译，指向工程顶层目录，如果是out-of-source编译，指向工程编译发生的目录
CMAKE_BINARY_DIR / PROJECT_BINARY_DIR / <project_name>_BINARY_DIR

# 三个变量指代内容一致，始终是工程顶层目录
CMAKE_SOURCE_DIR / PROJECT_SOURCE_DIR / <project_name>_SOURCE_DIR

# 指当前处理的CMakeLists.txt所在的路径
CMAKE_CURRENT_SOURCE_DIR

# 如果是in-source编译，跟CMAKE_CURRENT_SOURCE_DIR一致，如果是out-of-source编译,指向target编译目录
# 使用 ADD_SUBDIRECTORY(src bin)可以更改这个变量的值
CMAKE_CURRENT_BINARY_DIR

# 用来重新定义可执行文件和库文件的存放目录
EXECUTABLE_OUTPUT_PATH / LIBRARY_OUTPUT_PATH
```

### 2.3 CMake 编译选项

```cmake
# 使用 ADD_LIBRARY 时生成动态库
BUILD_SHARED_LIBS

# 使用 ADD_LIBRARY 时生成静态库
BUILD_STATIC_LIBS

# 设置 C 编译选项，也可以通过指令 ADD_DEFINITIONS() 添加
CMAKE_C_FLAGS

# 设置 C++ 编译选项，也可以通过指令 ADD_DEFINITIONS() 添加
CMAKE_Cxx_FLAGS
```

### 2.4 CMake Library

#### 2.4.1 动态库

- 动态库生成

```cmake
add_library (libname SHARED source1)
target_include_libraries(libname
	item1
	item2
)
```

- 动态库调用

```cmake
include_directories(lib_include_path)
add_executable(binname source)
target_link_libraries(binname libname)
```

#### 2.4.2 静态库

- 静态库生成

```cmake
add_library (libname STATIC source1)
target_include_libraries(libname
	item1
	item2
)
```

- 静态库调用

```cmake
include_directories(lib_include_path)
add_executable(binname source)
target_link_libraries(binname libname)
```

#### 2.4.3 接口库

- 接口库不生成任何文件

```cmake
add_library(libname INTERFACE)
target_link_libraries(libname 
	INTERFACE
		item1
		item2
)
target_include_directories(libname
	INTERFACE
		include)
```

- 接口库调用

```cmake
target_link_libraries(binname libname)
```

### 2.5 CMakePackageConfigHelpers

```cmake
configure_package_config_file(<input>   # 输入文件，后缀为-config.cmake.in 或 config.cmake.in
							  <output>  # 输出文件，后缀为-config.cmake 或 config.cmake
							  INSTALL_DESTINATION <path>  # .cmake 文件的安装目录
							  PATH_VARS <var1> <var2>)  # PATH_VARS后接的每一个变量都有形参、实参两种意义，形参是指在cmake.in 中可以用 @PACKAGE_CMAKE_DIR@ @PACKAGE_INCLUDE_INSTALL_DIR@ @PACKAGE_SYSCONFIG_INSTALL_DIR@来引用这些变量值；实参是指这些变量会将他们的值传递到函数内部，给形参。如果是相对路径也会自动转换为安装到的全局路径
# 示例

```

### 2.6 find_package

```cmake
<package-name>_FOUND # 是否查找到package的标志
<package-name>_CONFIG # 读取的配置文件路径, 可用的变量设置在跑配置文件中
```

### 2.7 set

- https://blog.csdn.net/Calvin_zhou/article/details/104060927

## 3. 杂项

### 3.1 .cmake文件

> 为什么要自定义 xx.cmake文件？
>
> 	在使用cmake进行代码构建的时候，当需要通过find_package(xx)来配置相关库的路径（头文件路径、链接库路径）时，如果没有在系统环境变量中设置相关的 xx_DIR等，cmake会提示出错。
> 	
> 	所以所有网上介绍某个库的安装时，都会要求设置相关的系统环境变量。但是当项目中需要某个库的多个版本时，由于系统环境变量对于某个特定的库只能设置一个相关的环境变量，因此可以自定义 xx.cmake，只需要把 xx.cmake放在库目录的顶层文件夹中。

### 3.2 cmake options与if设置开关量

```cmake
options(switch "this is a option for switch" ON)
if (switch)
	message("define address")
else ()
	message("not define address")
endif
```

## 参考链接:

[1] [https://blog.csdn.net/bbszhenshuai/article/details/106200228](https://blog.csdn.net/bbszhenshuai/article/details/106200228)
