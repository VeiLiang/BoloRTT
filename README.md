# RT-Thread For Bolopi #
[中文页](README_zh.md) 
## Overview ##

Bolopi-F1 is a all io extracted for f1c100s development board.Such like 40pin rgb lcd,dvp(camera),audio,tv in (cvbs),tv out,SDIO ...
Any question about f1c100s ,welcome click https://whycan.com WhyCan Forum  have a look...

RT-Thread is an open source IoT operating system from China, which has strong scalability: from a tiny kernel running on a tiny core, for example ARM Cortex-M0, or Cortex-M3/4/7, to a rich feature system running on MIPS32, ARM Cortex-A8, ARM Cortex-A9 DualCore etc.

## License ##

RT-Thread RTOS is free software; you can redistribute it and/or modify it under terms of the GNU General Public License version 2 as published by the Free Software Foundation. RT-Thread RTOS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with RT-Thread; see file COPYING. If not, write to the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

As a special exception, including RT-Thread RTOS header files in a file, instantiating RT-Thread RTOS generics or templates, or linking other files with RT-Thread RTOS objects to produce an executable application, does not by itself cause the resulting executable application to be covered by the GNU General Public License. This exception does not however invalidate any other reasons why the executable file might be covered by the GNU Public License.

## IDE ##

For Windows:

This project is cross build in windows system.
So we suggest you chooes visual studio code as ide for development.
In that case,you can build project through press the hot key "Ctrl+Shift+B",and downlode firmware through click Terminal-RunTask "sunxi-fel download bin file"

For Linux:

Now that you've chosen Linux, sure you can handle all things

## Usage ##

RT-Thread RTOS uses [scons](http://www.scons.org) as building system. Therefore, please install scons and Python 2.7 firstly. 
So far, the RT-Thread scons building system support the command line compile or generate some IDE's project. There are some option varaibles in the scons building script (rtconfig.py):

* ```CROSS_TOOL``` the compiler need to ```use arm-eabi-gcc``` 
* ```EXEC_PATH``` the path of compiler. 

In SConstruct file:

```RTT_ROOT``` This variable is the root directory of RT-Thread RTOS. If you build the porting in the bsp directory, you can use the default setting. Also, you can set the root directory in ```RTT_ROOT``` environment variable and not modify SConstruct files.

When you set these variables correctly, you can use command:

    scons 

under BSP directory to simplely compile RT-Thread RTOS.

like build command:

    cd ./bsp/f1c/
    scons -j8


clean command:

    cd ./bsp/f1c/
    scons -c

 
NOTE: RT-Thread scons building system will tailor the system according to your rtconfig.h configuration header file. For example, if you disable the lwIP in the rtconfig.h by commenting the ```#define RT_USING_LWIP```, the generated project file should have no lwIP related files. 

## Contribution ##

Please refer the contributors in the rtthread github. Thank all of RT-Thread Developers. 
