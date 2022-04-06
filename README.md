# Topuino

[English](doc/README_en.md)

[用户手册](doc/user_manual.md)

## 什么是 Topuino

Topuino 是一个桌面小摆件，可以实现通用服务器或计算机的数据监控，包括 CPU 占用、RAM 占用、两个硬盘的可用空间、硬盘读写速度、网络 IO 速率。

![](https://img.vvzero.com/ih/5d606a6c-926f-4690-8924-8931ec0f5f97.jpg)

## 为什么叫 Topuino

在 Linux 系列服务器上，通常使用 top 命令查看 CPU 内存占用，本项目最初的设想也是将 top 命令实物化，这就是 Topuino 中 Top 的由来。

在选型的时候，为了兼顾开发效率和成本，项目选用了大名鼎鼎的 ESP8266 单片机，配合了 Arduino 开发框架，Arduino 则是 Topuino 中 uino 的由来。

## Topuino 的工作原理

- 服务端使用了 Flask，维护一个内存数据库（实际上是 Python 的字典），保存着从站（被监控服务器）UUID 与运行参数的映射关系。主站接收从站的运行数据，并向 Topuino 回传数据；
- 客户端采用 python 的 psutil 库，获取所有的运行数据；
- 硬件端使用了 ESP-12F 作为 MCU，显示采用 LED 阵列和数码管，显示驱动是 TM1638 芯片。
