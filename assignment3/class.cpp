#include <string>
#include "class.h"

teacher::teacher()  //default constructor
{
    // 进入constructor前 会按声明顺序默认初始化，int等基本数据类型会变成undefined；this->name{"X1zZy"};本身也是初始化则无法作用于已存在的对象
    this->name = "X1zZy";
    this->rank = 0;
    this->bigOface = 'o';
}

teacher::teacher(std::string name, int rank, int bigOface): /*list intialization constructor*/
name{name}, rank{rank}, bigOface{bigOface} 
{
    /*可以完成验证和其他辅助操作*/
}

void teacher::setFace()
{
    this->bigOface = 0;
}

void teacher::fuckFace(int fuckType)
{
    this->bigOface = fuckType;
}

int teacher::getRank() const
{
    return this->rank;
}

void teacher::setRank(int rank)
{
    this->rank = rank;
}