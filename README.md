
# 北京交通大学操作系统实验（dxy）

北京交通大学计算机专业操作系统实验（dxy老师视频所授，与mooc不同），总共分为四次实验，此为最终版本。

> 四次实验：
> 1.实现操作系统的引导启动和时钟中断
> 2.实现键盘和鼠标的中断驱动
> 3.实现多任务切换
> 4.实现系统调用和简单的Shell

## 开发环境

1. Vscode编辑器。
2. C++，X86汇编语言。
3. Linux系统的服务器。
4. Windows远程连接。

## 运行和测试环境

Linux服务器，Virtual Box，运行代码运行启动虚拟机并进行测试

## 源码清单

|名称|说明|
|---|---|
|loader.s	|实现引导程序的汇编文件|
|kernel.cpp	|实现内核程序的C++文件|
|link.ld	|链接文件|
|Makefile	|Makefile文件|
|gdt.h	|对gdt的类的定义|
|gdt.cpp	|实现gdt的函数操作|
|types.h	|对不同字节类型数据的命名定义|
|port.h	|对不同字节的端口的类的定义|
|port.cpp	|实现不同端口的类的读写操作|
|interrupts.h	|对中断管理器和控制器的类的定义|
|interrupts.cpp	|对中断的类的函数的实现|
|driver.h	|对驱动和驱动管理器的类的实现|
|driver.cpp	|实现上述类的操作|
|keyboard.h	|对键盘驱动和键盘控制器的类的定义|
|keyboard.cpp	|实现键盘类的相关操作|
|mouse.h	|对鼠标驱动和鼠标控制器的类的定义|
|mouse.cpp	|实现鼠标类的相关操作|
|multitasking.h	|对任务和任务管理器类的定义|
|multitasking.cpp	|实现任务和任务管理器的相关操作|
|syscalls.h	|对实现系统调用类的定义|
|syscalls.cpp	|实现系统调用的相关操作|

## 参考资料

1. 《自己动手写操作系统》 于渊
2. Youtube视频https://www.youtube.com/playlist?list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M
3. B站视频https://www.bilibili.com/video/BV1Ng411x7As/?p=22&spm_id_from=333.1007.top_right_bar_window_history.content.click&vd_source=09687b709481a7b54f76a44617097552

> 本实验最初参考即上述视频的教程，结合了老师的一些自己的修改
> 
> 教程编写的操作系统内核代码
> 作者的个人网站（可以下载视频中的源代码）：http://www.algorithman.de/Algorithmen/kayos/index.php
> 作者的个人介绍：http://www.algorithman.de/Autor/index.php
> 参考网站：
> http://www.lowlevel.eu/
> https://wiki.osdev.org
>
> B站简介：原作者是 Viktor Engelmann ，2016年于柏林工业大学取得博士学位。曾在Qt公司工作，有超过20年的C++编程经验。
> 在该系列视频中，作者从系统引导开始，用汇编语言和C++逐步开发出可以运行于VirtualBox虚拟机上的操作系统。其中实现了包括中断、键盘鼠标控制，PCI、桌面、并发、动态内存管理、网络、文件系统等一系列功能。这个系列非常具有参考学习价值，适合有一定汇编语言和C++编程基础的同学动手从头写操作系统的入门。
