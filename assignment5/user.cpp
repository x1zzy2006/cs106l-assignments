#include "user.h"

/**
 * Creates a new User with the given name and no friends.
 */
User::User(const std::string &name)
    : _name(name), _friends(nullptr), _size(0), _capacity(0)
{
}

/**
 * Adds a friend to this User's list of friends.
 * @param name The name of the friend to add.
 */
void User::add_friend(const std::string &name)
{
  if (_size == _capacity)
  {
    _capacity = 2 * _capacity + 1;
    std::string *newFriends = new std::string[_capacity];
    for (size_t i = 0; i < _size; ++i)
    {
      newFriends[i] = _friends[i];
    }
    delete[] _friends;
    _friends = newFriends;
  }

  _friends[_size++] = name;
}

/**
 * Returns the name of this User.
 */
std::string
User::get_name() const
{
  return _name;
}

/**
 * Returns the number of friends this User has.
 */
size_t
User::size() const
{
  return _size;
}

/**
 * Sets the friend at the given index to the given name.
 * @param index The index of the friend to set.
 * @param name The name to set the friend to.
 */
void User::set_friend(size_t index, const std::string &name)
{
  _friends[index] = name;
}

/**
 * STUDENT TODO:
 * The definitions for your custom operators and special member functions will go here!
 */
// Part One:
std::ostream &operator<<(std::ostream &os, const User &u) // non-member functions不需要指定查找空间User::
{
  os << "User(name=" << u._name << ", friends=["; // this是指针，所以->访问字段；此处u作为引用或者原变量，需要用.访问字段
  for (int i = 0; i < u._size; i++)
  {
    os << u._friends[i];
    if (i != u._size - 1)
    {
      os << ", ";
    }
  }
  os << "])";
  return os;
}

// Part Two:
User::~User()
{
  delete[] _friends;
}
// 忽视了新数组的分配，注意复制成员元素并不能分配空间，只有完成了对copy constructor的定义才能够实现分配空间的功能
User::User(const User &user) : _name(user._name), _friends(new std::string[user._capacity]), _size(user._size), _capacity(user._capacity)
{
  for (int i = 0; i < user._size; i++)
  {
    this->_friends[i] = user._friends[i];
  }
}
User &User::operator=(const User &user)
{
  if (this == &user) // if (*this == user) 一是没有定义operator==，不要下意识默认以为可以执行这个运算；二是这是检查值相等，不符合检查是否是同一个对象的根本要求
  {
    return *this; // return user;不能同时满足返回一个User&和参数是一个const User&的要求
  }
  delete[] this->_friends;
  this->_name = user._name; // std::string自身的copy assigment保证了可以获取独立的字符串的内容，要学会思考底层实现
  this->_friends = new std::string[user._capacity];
  this->_size = user._size;
  this->_capacity = user._capacity;
  for (int i = 0; i < user._size; i++)
  {
    this->_friends[i] = user._friends[i];
  }
  return *this;
}

// Part Three:
User &User::operator+=(User &rhs)
{
  this->add_friend(rhs._name);
  rhs.add_friend(this->_name);
  return *this;
}
bool User::operator<(const User& rhs) const //完成operator<的定义，能够满足std::set存储对调用std::less<T>进行比较的要求
{
  return this->_name < rhs._name;
}

//逆天又不给匹配build里的main
