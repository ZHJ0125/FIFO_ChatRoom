# FIFO_ChatRoom
Linux 使用命名管道实现聊天室功能

## 根目录文件说明

|文件(夹)名称|用途|
|---|---|
|.vscode|我的VSCode配置信息|
|Data|用于存放项目资料，包括图片和日志文件|
|Project|用于存放项目源代码|
|LICENSE|项目开源许可证|
|README.md|项目自述文件|

## 程序功能说明

- [x] 实现了多客户端的群聊功能
- [x] 实现了客户端双方的私聊功能
- [x] 私聊对象通过客户端的用户名进行标识
- [x] 完善了客户端和服务器的退出方式
- [ ] 正在尝试使用QT编写程序的UI界面

## 编译和测试说明

* 使用 make 实现客户端和服务器的编译
* server 是服务器程序
* client 是客户端程序
* 客户端默认为群发模式
* 客户端的私聊格式为 `to: [Target_Client_Name] [Message_To_Be_Sent]`

> 程序编译示例
```bash
zhj@thinkpad-vmware:~/FIFO_ChatRoom/Project$ ls
client.c  fifo.c  fifo.h  Makefile  server.c
zhj@thinkpad-vmware:~/FIFO_ChatRoom/Project$ make
cc    -c -o client.o client.c
cc    -c -o fifo.o fifo.c
cc    -c -o server.o server.c
rm ./*.o
zhj@thinkpad-vmware:~/FIFO_ChatRoom/Project$ ls
client  client.c  fifo.c  fifo.h  Makefile  server  server.c
```

## 效果展示

> 目前已录制了两个视频，分别是[Linux_命名管道_聊天室_测试视频](https://www.bilibili.com/video/BV1Pi4y187co/)和[Linux_命名管道_聊天室_测试视频_V2.0](https://www.bilibili.com/video/BV1254y197AU/)

[![V2.0效果展示截图](/Data/Image/Video_Show_V2.png)](https://www.bilibili.com/video/BV1Pi4y187co/)

* 点击观看 **[BiliBili - Linux_命名管道_聊天室_测试视频_V2.0版本](https://www.bilibili.com/video/BV1254y197AU/)**
   * 修复了V1.0版本退出程序的bug
   * 实现了在某个客户端退出后，服务器向其他用户发送通知的功能
   * 优化了客户端界面

* 点击观看 **[BiliBili - Linux_命名管道_聊天室_测试视频_V1.0版本](https://www.bilibili.com/video/BV1Pi4y187co/)**
   * 实现了群聊功能
   * 实现了以用户名为标识的私聊功能

## 项目日志

> 详细内容请看：[LOG](/Data/Log/README.md)

### Day0

* 具体内容请看： [Day0.md](/Data/Log/Day0.md)
* 编写项目需求、可行性分析和技术分析
* 上传命名管道的基础代码 -> 基础代码来自 [Embedded_Linux仓库](https://github.com/ZHJ0125/Embedded_Linux/tree/master/Homework/lesson11)

### Day1

* 具体内容请看： [Day1.md](/Data/Log/Day1.md)
* 客户端之间的**群聊功能**已经实现
* 客户端之间的**私聊功能**已经实现

### Day2

* 具体内容请看： [Day2.md](/Data/Log/Day2.md)
* 实现了使用用户名作为标识的私聊功能
* 优化了客户端显示的内容
* 录制了测试视频并投稿至B站

### Day3

* 具体内容请看： [Day3.md](/Data/Log/Day3.md)
* 测试视频已在 [B站审核已通过](https://www.bilibili.com/video/BV1Pi4y187co/)
* 编写阶段性成果文章并 [发布在CSDN](https://blog.csdn.net/ZHJ123CSDN/article/details/105449547)
* 尝试使用QT编写界面

### Day4

* 具体内容请看： [Day4.md](/Data/Log/Day4.md)
* NULL

### Day5

* 具体内容请看： [Day5.md](/Data/Log/Day5.md)
* 安装 Qt for Linux
* 发现关于QT的严重问题

### Day6

* 具体内容请看： [Day6.md](/Data/Log/Day6.md)
* 修复退出程序的bug
* 实现了在某个客户端退出后，服务器向其他用户发送通知的功能
* 将新版本内容录制为测试视频并投稿至B站

## 程序代码

> 源代码遵循 [GNU General Public License v3.0](https://gitee.com/zhj0125/FIFO_ChatRoom/blob/master/LICENSE) 开源许可协议

* Gitee 仓库地址 **（推荐）** ： [https://gitee.com/zhj0125/FIFO_ChatRoom](https://gitee.com/zhj0125/FIFO_ChatRoom/)
* GitHub 仓库地址： [https://github.com/ZHJ0125/FIFO_ChatRoom](https://github.com/ZHJ0125/FIFO_ChatRoom/)
* 如果您对代码有任何疑问或修改意见，欢迎 [提出issue](https://gitee.com/zhj0125/FIFO_ChatRoom/issues) 或者直接私信我，谢谢。
