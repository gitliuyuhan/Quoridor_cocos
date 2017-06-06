#include "user.h"

typedef struct
{
    int msgtype;
    char name[20];
    char passwd[20];
    char opponent[20];
    int type;
    int x;
    int y;
    int play_flag;
}Message;

void copyStr(char* s1,std::string& s2)
{
    std::copy(s2.begin(),s2.end(),s1);
    s1[s2.size()] = '\0';
}

int User::connectServer()
{
    lnet::Socket clientSocket(10000,"127.0.0.1");
    clientSocket.connect();
    _connfd = clientSocket.getSockfd();
    if(_connfd < 0)
        return 0;
    else
    {
        _is_conn = true;
        return 1;
    }
}

int User::login(std::string name,std::string passwd)
{
    Message  msg;
    msg.msgtype = 0;
    copyStr(msg.name,name);
    copyStr(msg.passwd,passwd);
    
    std::cout << msg.msgtype << " " 
        << msg.name << " " 
        << msg.passwd << std::endl;
    std::cout << sizeof(msg) << std::endl;
    int ret = lnet::writeTcpMessage(_connfd,
                (const char*)&msg,
                sizeof(msg));
    if(ret <= 0)
      return 0;
    char buf[1024];
    ret = lnet::readTcpMessage(_connfd,buf);
    if(ret <= 0)
      return 0;
    _username = name;
    _passwd = passwd;

    std::cout << "read end free" << std::endl;
    return 1;
}

int User::matchOpponent()
{ 
    Message msg;
    msg.msgtype = 1;
    copyStr(msg.name,_username);

    int ret = lnet::writeTcpMessage(_connfd,
                (const char*)&msg,
                sizeof(msg));
    if(ret <= 0)
      return 0;
    char buf[1024];
    ret = lnet::readTcpMessage(_connfd,buf);
    if(ret <= 0)
      return 0;
    
    Message* m = (Message*)buf;
    _opponent = m->opponent;
    _play_flag = m->play_flag;
    std::cout << _opponent << std::endl;

    return 1;
}

int User::walkChess(int type,int x,int y)
{
    Message msg;
    msg.msgtype = 2;
    copyStr(msg.opponent,_opponent);
    msg.type = type;
    msg.x = x;
    msg.y = y;

    int ret = lnet::writeTcpMessage(_connfd,
                (const char*)&msg,
                sizeof(msg));
    if(ret <= 0)
      return 0;
    return 1;
}

int User::opponentWalkChess(int& type,int& x,int& y)
{
    Message* msg;
    char buf[1024];
    int ret = lnet::readTcpMessage(_connfd,buf);
    if(ret <= 0)
      return 0;
    
    msg = (Message*)buf;
    type = msg->type;
    x = msg->x;
    y = msg->y;

    return 1;
}

std::string User::getOpponent()
{
    return _opponent;
}
