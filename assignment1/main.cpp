/*
 * CS106L Assignment 1: SimpleEnroll
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 *
 * Welcome to Assignment 1 of CS106L! Please complete each STUDENT TODO
 * in this file. You do not need to modify any other files.
 *
 * Students must implement: parse_csv, write_courses_offered,
 * write_courses_not_offered
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string COURSES_OFFERED_PATH = "student_output/courses_offered.csv";
const std::string COURSES_NOT_OFFERED_PATH = "student_output/courses_not_offered.csv";

/**
 * Represents a course a student can take in ExploreCourses.
 * You must fill in the types of the fields in this struct.
 * Hint: Remember what types C++ streams work with?!
 */
struct Course {
  /* STUDENT TODO */std::string title;
  /* STUDENT TODO */std::string number_of_units;
  /* STUDENT TODO */std::string quarter;
};

/**
 * (STUDENT TODO) Look at how the main function (at the bottom of this file)
 * calls `parse_csv`, `write_courses_offered`, and `write_courses_not_offered`.
 * Modify the signatures of these functions so that they work as intended, and then delete this
 * comment!
 */

/**
 * Note:
 * We need to #include utils.cpp _after_ we declare the Course struct above
 * so that the code inside utils.cpp knows what a Course is.
 * Recall that #include literally copies and pastes file contents.
 */
#include "utils.cpp"

/**
 * This function should populate the `courses` vector with structs of type
 * `Course`. We want to create these structs with the records in the courses.csv
 * file, where each line is a record!
 *
 * Hints:
 * 1) Take a look at the split function we provide in utils.cpp
 * 2) Each LINE is a record! *this is important, so we're saying it again :>)*
 * 3) The first line in the CSV defines the column names, so you can ignore it!
 *
 * @param filename The name of the file to parse.
 * @param courses  A vector of courses to populate.
 */
std::string parse_csv(std::string filename, std::vector<Course>& courses) {
  /* (STUDENT TODO) Your code goes here... */
  std::ifstream in(filename);
  std::string fhead;
  std::string line;

  std::getline(in, fhead);
  while(std::getline(in, line)) //getline函数返回值是input，也就是输入流引用。此处返回值经过static_cast，文件结束或出现错误时可以退出
  {
    const auto fields = split(line, ',');//getline读取的line中分隔符读取后会直接被丢弃，line作为std::string类型可以自行维护长度和终止符'\0'，且终止符这一细节在C++大部分情况中被隐藏，不需要担心
    
    Course course{fields[0], fields[1], fields[2]}; //在C语言的经验来看要么直接对数组进行赋值要么分配一个节点再存入数组，这里的写法似乎不是这样的
    
    courses.push_back(course);  //push_back实际接受对一个lvalue的引用，复制并构造一个Course对象；原Course对象其中字符串具有值语义
  }
  return fhead; //返回的是一个值，没问题；生命周期问题会出现在尝试返回一个局部变量的引用这种情况
}

/**
 * This function has TWO requirements.
 *
 * 1) Write the courses that are offered to the file
 * "student_output/courses_offered.csv"
 *
 * 2) Delete the courses that are offered from the `all_courses` vector.
 * IMPORTANT: do this after you write out to the file!
 *
 * HINTS:
 * 1) Keep track of the classes that you need to delete!
 * 2) Use the delete_elem_from_vector function we give you!
 * 3) Remember to write the CSV column headers at the beginning of the output!
 *    See courses.csv for reference.
 *
 * @param all_courses A vector of all courses gotten by calling `parse_csv`.
 *                    This vector will be modified by removing all offered courses.
 */
void write_courses_offered(std::vector<Course>& all_courses, std::string csv_header) {
  /* (STUDENT TODO) Your code goes here... */
  std::ofstream out(COURSES_OFFERED_PATH);
  std::vector<Course> selected_courses{};  //引用必须绑定到对应的已存在的对象，因此不能直接声明引用却不完成赋值；把他当成alias看待

  out << csv_header << "\n";
  for (const auto &course : all_courses)
  {
    if (course.quarter != "null")//字符串相等可有==相等直接判断，不再是C中的地址运算，这一点也跟python的类特有运算符方法相似，想到简单写法直接写就好了底层大概有实现
    {
      out << course.title << "," << course.number_of_units << "," << course.quarter << "\n";
      selected_courses.push_back(course);
    }
  }
  
  for (const auto &d : selected_courses)  //所有引用，都是直接指向了原元素
  {
    delete_elem_from_vector(all_courses, d);  //保存引用，然后对原courses数组进行修改
  }
}

/**
 * This function writes the courses NOT offered to the file
 * "student_output/courses_not_offered.csv".
 *
 * This function is ALWAYS called after the `write_courses_offered` function.
 * `unlisted_courses` will trivially contain classes that are not offered
 * since you delete offered classes from `all_courses` in the
 * `write_courses_offered` function.
 *
 * HINT: This should be VERY similar to `write_courses_offered`
 *
 * @param unlisted_courses A vector of courses that are not offered.
 */
void write_courses_not_offered(std::vector<Course> unlisted_courses, std::string csv_header) {
  /* (STUDENT TODO) Your code goes here... */
  std::ofstream out(COURSES_NOT_OFFERED_PATH);
  out << csv_header << "\n";
  for (const auto &course : unlisted_courses)
  {
    out << course.title << "," << course.number_of_units << "," << course.quarter << "\n";
  }
}

int main() {
  /* Makes sure you defined your Course struct correctly! */
  static_assert(is_valid_course<Course>, "Course struct is not correctly defined!");

  std::vector<Course> courses;
  const std::string csv_header(parse_csv("courses.csv", courses));//相对寻址方案使用cwd，这与运行命令所在目录有关，而非exe的位置；正常project可以写个assert确保路径正确

  /* Uncomment for debugging... */
  // print_courses(courses);

  write_courses_offered(courses, csv_header);
  write_courses_not_offered(courses, csv_header);

  return run_autograder();
}