#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

#include "net_socket.h"
#include "net_base.h"
#include "singleton.h"

#define USER (lnet::Singleton<User>::getInstance())

class User
{
public:
    User():_is_conn(false)
    {}
    ~User(){}
    //connect server
    int connectServer();
    //login
    int login(std::string name,std::string passwd);
    //匹配玩家
    int matchOpponent();
    //走棋
    int walkChess(int type,int x,int y);
    int opponentWalkChess(int& type,int& x,int& y);
    //获取对手
    std::string getOpponent();
    int _play_flag;
private:
    bool _is_conn;
    int _connfd;
    std::string _username;
    std::string _passwd;
    int _roomid;
    //对手
    std::string _opponent;
};

#endif
