# FIFO_ChatRoom
Linux 使用命名管道实现聊天室功能

## 功能说明

* 多客户端的群聊功能
* 客户端双方的私聊功能
* 私聊对象通过客户端的用户名进行标识
* 完善了客户端和服务器的退出方式

## 编译和测试说明

* 使用 make 实现客户端和服务器的编译
* server 是服务器程序
* client 是客户端程序

> example
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

[![效果展示截图](/Data/Image/Video_Show.png)](https://www.bilibili.com/video/BV1Pi4y187co/)

* 点击前往B站，观看 **[BiliBili - Linux_命名管道_聊天室_测试视频](https://www.bilibili.com/video/BV1Pi4y187co/)**

## 四、程序代码

> 源代码遵循 [GNU General Public License v3.0](https://gitee.com/zhj0125/FIFO_ChatRoom/blob/master/LICENSE) 开源许可协议

* Gitee 仓库地址 ***（推荐）*** ： [https://gitee.com/zhj0125/FIFO_ChatRoom/tree/master/Project](https://gitee.com/zhj0125/FIFO_ChatRoom/tree/master/Project)
* GitHub 仓库地址： [https://github.com/ZHJ0125/FIFO_ChatRoom/tree/master/Project](https://github.com/ZHJ0125/FIFO_ChatRoom/tree/master/Project)
* 如果您对代码有任何疑问或修改意见，欢迎 [提出issue](https://gitee.com/zhj0125/Embedded_Linux/issues) 或者直接私信我，谢谢。