/*
 * CS106L Assignment 5: TreeBook
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */

#include <iostream>
#include <string>

class User
{
public:
  User(const std::string& name);
  void add_friend(const std::string& name);
  std::string get_name() const;
  size_t size() const;
  void set_friend(size_t index, const std::string& name);

  /** 
   * STUDENT TODO:
   * Your custom operators and special member functions will go here!
   */
  //Part One:
  //std::ostream表示输出流，平时所见的std::cout是一个全局std::ostream对象，因此os作为一个引用本质就是“caller传入的输出流对象的alias”，可以写为<<的左侧对象
  //<<运算符需要的参数是其左右两侧的类型
  //<<作为输出流运算符，返回值应该设计成返回自身的引用以满足std::cout << user << '\n'从左到右的运算
  friend std::ostream& operator<<(std::ostream& os, const User& u); //函数的const声明，只适用于非静态成员函数，因为他仅限制this指针
  
  //Part Two:
  ~User();
  User(const User& user);
  User& operator=(const User& user);
  User(User&& user) = delete;
  User& operator=(User&& user) = delete;

  //Part Three:
  User& operator+=(User& rhs);
  bool operator<(const User& rhs) const;
private:
  std::string _name;
  //参考std::string *newFriends = new std::string[_capacity];
  std::string* _friends;  //*实际表示数组语义，而不要指针的语义带偏；他实际是一个lists of references to std::string
  size_t _size;
  size_t _capacity;
};