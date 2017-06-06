/*======================================================
    > File Name: net_base.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年11月14日 星期一 15时48分58秒
 =======================================================*/

#ifndef NET_BASE_H
#define NET_BASE_H

#include <iostream>
#include <string>

namespace lnet
{
//设为非阻塞
int setNonBlockFd(int fd);
//当前时间转string
std::string nowTimeToString();
//tcp消息解包
int readTcpMessage(int fd,char* buf);
//tcp消息封包
int writeTcpMessage(int fd,const char* buf,int len);
}

#endif //NET_BASE_H

