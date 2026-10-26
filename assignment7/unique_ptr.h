#pragma once

#include <cstddef>
#include <utility>

namespace cs106l {

/**
 * @brief A smart pointer that owns an object and deletes it when it goes out of scope.
 * @tparam T The type of the object to manage.
 * @note This class is a simpler version of `std::unique_ptr`.
 */
template <typename T> class unique_ptr {
private:
  /* STUDENT TODO: What data must a unique_ptr keep track of? */
  T* ptr;

public:
  /**
   * @brief Constructs a new `unique_ptr` from the given pointer.
   * @param ptr The pointer to manage.
   * @note You should avoid using this constructor directly and instead use `make_unique()`.
   */
  unique_ptr(T* ptr) {
    /* STUDENT TODO: Implement the constructor */
    this->ptr = ptr;  //注意阅读理解！！上面只是说不应该直接调用这个函数，而不是说用make_unique来构造这个底层函数；再读一遍明显发现逻辑理解问题了
  }

  /**
   * @brief Constructs a new `unique_ptr` from `nullptr`.
   */
  unique_ptr(std::nullptr_t) {
    /* STUDENT TODO: Implement the nullptr constructor */
    this->ptr = nullptr;  //unique_ptr(std::nullptr_t) 采用了“只有参数类型、没有参数名”的写法，因此那是一个type不是一个object
  }

  /**
   * @brief Constructs an empty `unique_ptr`.
   * @note By default, a `unique_ptr` points to `nullptr`.
   */
  unique_ptr() : unique_ptr(nullptr) {}

  /**
   * @brief Dereferences a `unique_ptr` and returns a reference to the object.
   * @return A reference to the object.
   */
  T& operator*() {
    /* STUDENT TODO: Implement the dereference operator */
    return *(this->ptr);
  }

  /**
   * @brief Dereferences a `unique_ptr` and returns a const reference to the object.
   * @return A const reference to the object.
   */
  const T& operator*() const {
    /* STUDENT TODO: Implement the dereference operator (const) */
    return *(this->ptr);  //由返回值的const保证
  }

  /**
   * @brief Returns a pointer to the object managed by the `unique_ptr`.
   * @note This allows for accessing the members of the managed object through the `->` operator.
   * @return A pointer to the object.
   */
  T* operator->() {
    /* STUDENT TODO: Implement the arrow operator */
    return this->ptr;  //不要被绕晕了，this是指向当前对象unique_ptr的指针，其存储字段this->ptr才是指向实际内存data的指针！
  }

  /**
   * @brief Returns a const pointer to the object managed by the `unique_ptr`.
   * @note This allows for accessing the members of the managed object through the `->` operator.
   * @return A const pointer to the object.
   */
  const T* operator->() const {
    /* STUDENT TODO: Implement the arrow operator */
    return this->ptr;
  }

  /**
   * @brief Returns whether or not the `unique_ptr` is non-null.
   * @note This allows us to use a `unique_ptr` inside an if-statement.
   * @return `true` if the `unique_ptr` is non-null, `false` otherwise.
   */
  operator bool() const {
    /* STUDENT TODO: Implement the boolean conversion operator */
    return this->ptr != nullptr;  //跟上面的operator->一样，ptr才是要访问的指针
  }

  /** STUDENT TODO: In the space below, do the following:
   * - Implement a destructor
   * - Delete the copy constructor
   * - Delete the copy assignment operator
   * - Implement the move constructor
   * - Implement the move assignment operator
   */
  ~unique_ptr()
  {
    delete this->ptr;
  }
  unique_ptr(const unique_ptr& u) = delete;
  unique_ptr& operator=(const unique_ptr& u) = delete;
  unique_ptr(unique_ptr&& u)  //据此理解move semantics
  {
    this->ptr = u.ptr;  //不再需要嵌套的std::move(u.ptr)，因为u.ptr本身就是一个地址值而非对象
    u.ptr = nullptr;  //手动设为空指针，u被析构的时候被处理的是nullptr，否则会访问指向正被当前unique_ptr管理的对象的指针
  }
  unique_ptr& operator=(unique_ptr&& u) //move assignment的返回值是正常的引用即可
  {
    if (this == &u) // copy/move assignment逻辑都要记得检查是否为相同对象
    {
      return *this;
    }
    delete this->ptr; //相当于先析构原有资源，再移动构造新资源

    this->ptr = u.ptr;
    u.ptr = nullptr;

    return *this;
  }
};

/**
 * @brief Creates a new unique_ptr for a type with the given arguments.
 * @example auto ptr = make_unique<int>(5);
 * @tparam T The type to create a unique_ptr for.
 * @tparam Args The types of the arguments to pass to the constructor of T.
 * @param args The arguments to pass to the constructor of T.
 */
template <typename T, typename... Args> 
unique_ptr<T> make_unique(Args&&... args) {
  return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}