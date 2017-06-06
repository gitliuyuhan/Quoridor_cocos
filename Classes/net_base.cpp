/*======================================================
    > File Name: net_base.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年11月14日 星期一 15时54分34秒
 =======================================================*/

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "net_base.h"
#include "string.h"
#include "buffer.h"

using namespace lnet;

int lnet::setNonBlockFd(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

std::string lnet::nowTimeToString()
{
    time_t     t = time(NULL);
    struct tm  tmp;
    char       t_str[20];
    localtime_r(&t,&tmp);
    snprintf(t_str,20,"%04d/%02d/%02d %02d:%02d:%02d",
            tmp.tm_year+1900,tmp.tm_mon+1,tmp.tm_mday,
            tmp.tm_hour,tmp.tm_min,tmp.tm_sec);
    return std::string(t_str);
}

int lnet::readTcpMessage(int fd,char* buf)
{
    lnet::Buffer  recv_buf(1024);
    //buf = (char*)malloc(1024);
    int data_len = 0;
    while(1)
    {
        memset(buf,'\0',1024);
        int ret = recv(fd,buf,1024,0);
        if(recv < 0)
        {
            close(fd);
            return -1;
        }
        else if(ret == 0)
        {
            close(fd);
            return 0;
        }
        else
        {
            recv_buf.writeBuffer(buf,ret);
        }
        if(recv_buf.readableSize() > sizeof(int))
        {
            recv_buf.copySomeData<int>(&data_len,sizeof(int));
            if(recv_buf.readableSize() >= (data_len + sizeof(int)))
              break;
        }
    }
    recv_buf.readBuffer<int>(&data_len,sizeof(int));
    memset(buf,'\0',1024);
    recv_buf.readBuffer<char>(buf,data_len);
    return data_len;
}

int lnet::writeTcpMessage(int fd,const char* buf,int len)
{
    lnet::Buffer send_buf(1024);
    send_buf.writeBuffer((const char*)&len,sizeof(int));
    send_buf.writeBuffer((const char*)buf,len);
    int ret = send(fd,send_buf.begin(),len+sizeof(int),0);
    if(ret < 0)
    {
        close(fd);
        return -1;
    }
    else if(ret == 0)
    {
        close(fd);
        return 0;
    }
    else
    {
        return ret;
    }
}
