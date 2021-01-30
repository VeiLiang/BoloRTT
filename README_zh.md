# RT-Thread For Bolopi#

## 简介 ##

菠萝派是一个全部io引出的F1C100S的开发板，包括了LCD接口，摄像头接口，音频输入输出，TF卡，TV 输入和输出等......

RT-Thread是一个来自中国的开源物联网操作系统，它提供了非常强的可伸缩能力：从一个可以运行在ARM Cortex-M0芯片上的极小内核，到中等的ARM Cortex-M3/4/7系统，甚至是运行于MIPS32、ARM Cortex-A系列处理器上功能丰富系统。

## 许可证 ##

RT-Thread is free software; you can redistribute it and/or modify it under terms of the GNU General Public License version 2 as published by the Free Software Foundation. RT-Thread RTOS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with RT-Thread; see file COPYING. If not, write to the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

As a special exception, including RT-Thread RTOS header files in a file, instantiating RT-Thread RTOS generics or templates, or linking other files with RT-Thread RTOS objects to produce an executable application, does not by itself cause the resulting executable application to be covered by the GNU General Public License. This exception does not however invalidate any other reasons why the executable file might be covered by the GNU Public License.

RT-Thread始终以开源的方式发展，所有发布的代码都遵循GPLv2+许可证。注意，GPLv2+的意思是，它是GPLv2的一个修改版本，添加了一定的例外。简单的来说，当使用RT-Thread时，除了RT-Thread发布的源代码以外的代码，可以不用遵循GPLv2的方式开源代码。

## 开发环境 ##

Windows下开发:
本项目是在Window下开发的，所以建议安装visual studio code (简称vs code)。在配置好编译器和安装完scons之后，编译非常方便，直接在vs code下面打开工程，然后按下快捷键Ctrl+Shift+B就可以进行编译了；下载的时候，板子按着下载按键上电，插入usb，直接在vs code中点击Terminal-RunTask ，然后选中"sunxi-fel download bin file"任务，就会进行下载

Linux 下开发:
都用linux开发了，所有的东西相信你都能搞定的

## 编译 ##

RT-Thread使用了[scons](http://www.scons.org)做为自身的编译构建系统，并进行一定的定制以满足自身的需求（可以通过scons --help查看RT-Thread中额外添加的命令）。在编译RT-Thread前，请先安装Python 2.7.x及scons。

截至目前，RT-Thread scons构建系统可以使用命令行方式编译代码，或者使用scons来生成不同IDE的工程文件。在使用scons时，需要对构建配置文件（rtconfig.py）中如下的变量进行配置：

* ```CROSS_TOOL``` 本工程需要使用的gcc交叉编译工具链```arm-eabi-gcc```  
* ```EXEC_PATH``` 工具链的路径. 

注：在SConstruct文件中：

```RTT_ROOT``` 这个变量指向了RT-Thread的发布源代码根目录。如果你仅计划编译bsp目录下的target，这个`RTT_ROOT`可以使用默认配置。另外，你也可以设置同名的环境变量来指向不同的RT-Thread源代码根目录。

当你把相关的配置都配置正确后，你可以在具有目标目录下（这个目录应包括rtconfig.py、SContruct文件）执行以下命令：

    scons 

从而简单地就编译好RT-Thread。

通常编译命令：

    cd ./bsp/f1c/
    scons -j8

编译清除命令

    cd ./bsp/f1c/
    scons -c

注意：RT-Thread的scons构建系统会根据配置头文件rtconfig.h来裁剪系统。例如，如果你关闭了rtconfig.h中的lwIP定义（通过注释掉```#define RT_USING_LWIP```的方式），则scons生成的IDE工程文件中将自动不包括lwIP相关的文件。而在RT-Thread 3.0版本中，可以通过menuconfig的方式来配置整个系统，而不需要再手工更改rtconfig.h配置头文件。

## 贡献者 ##

请访问github上RT-Thread项目上的contributors了解已经为RT-Thread提交过代码，PR的贡献者。感谢所有为RT-Thread付出的开发者们！
