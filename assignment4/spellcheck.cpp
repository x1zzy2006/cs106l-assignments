#include "spellcheck.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>
#include <cctype>   // isspace
#include <iterator> // std::inserter

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred); //先声明函数，后引入函数的实现文件

Corpus tokenize(std::string &source)
{
  // Step One:
  //  find_all设计保证了begin和end被包含；此外还可以额外研究两种边界情况：begin出现两次和空字符串，可以注意他们当前处理逻辑下能不能顺利满足后续逻辑（这些重复位置会在下一步产生空区间，之后由 `erase_if` 清除对应的空 `Token`）
  //  isspace问题的解决：std::isspace会在 std 中形成重载集合，isspace（::isspace）则直接指向全局空间中的C版本函数
  //  模版参数中intellisense初步推导类型失败时，不会认为是当前填入参数的错误，而会认为当前模版不可用
  auto whitespaces = find_all(source.begin(), source.end(), isspace);

  // Step Two:
  // transform gets a pair of consecutive iterators by loop, token constructor take it and capture &source, insert happens during the loop
  // transform作为loop，同时提供了token constructor和inserter的接口
  // 重点在读懂transform的参数列表
  Corpus tokens{};
  auto first1 = whitespaces.begin(); // 注意这里可以理解为指针指针，first1当前实际是指向source.begin()这个iterator的iterator
  auto last1 = whitespaces.end() - 1;
  auto first2 = whitespaces.begin() + 1;
  auto make_token = [&source](auto it1, auto it2)
  { return Token{source, it1, it2}; }; // token构造函数保证取到两个iterator就可以构造一个token，token需要保证能够正确处理两个iterator相邻以及重合的情况
  std::transform(first1, last1, first2,
                 std::inserter(tokens, tokens.end()),
                 make_token);

  // Step Three:
  // 采用先写入所有候选，再检查候选的模式；而不是先检查候选写入
  // lambda中的capture用于外层作用域的局部变量
  // const Token &t类型简单，并且变量名信息量少时，就不要使用auto，同时避免了潜在的歧义，显式指定类型可以看作一种写好的接口约定
  // 类似erase_if这种函数都是接受container reference，传入给pred的参数都是container内的完成解引用的单个元素
  auto isempty = [](const Token &t)
  { return t.content.empty(); };
  std::erase_if(tokens, isempty); // 第一个参数是std::set<Key, Compare, Alloc>& c，tokens满足std::set，剩余的都是模板参数编译器可以自行推导

  return tokens;
}

// constant, batch-processing-like C++ statements just like linux commands
std::set<Misspelling> spellcheck(const Corpus &source, const Dictionary &dictionary)
{
  namespace rv = std::ranges::views;
  // Step One:
  // view可以理解为不修改source就将会取到的lazy evaluation的每个token
  auto check_incorrect = [&dictionary](const Token &t)
  { return !dictionary.contains(t.content); };
  auto view1 = source | rv::filter(check_incorrect); // pipeline的写法更简洁

  // Step Two:
  // consider what struct Misspelling needs, then you can focus on how to collect "suggestions" in the dictionary,
  // then you sort out the main process: looping the dictionary, filter to get "one-distance"s, then you can lock in the inner lambda: get_one_distance
  // convert是/* A lambda function taking a Token -> Misspelling */
  // get_one_distance是/* A lambda function predicate */
  // lambda本质就是一个函数，按照函数的逻辑来写，分析return需要的处理步骤和参数
  auto convert = [&dictionary](const Token &t)
  {
    auto get_one_distance = [&t](const std::string &word)
    { return levenshtein(t.content, word) == 1; };
    auto view_one_distance = dictionary | rv::filter(get_one_distance);

    std::set<std::string> suggestions(view_one_distance.begin(), view_one_distance.end()); // 内层view是局部变量，需要即时materialize取出其中的实质信息：set constructor能够完成对view的materialize，而后Misspelling调用{}的aggregate initiallization保证完成新对象的创建
    return Misspelling{t, suggestions};
  };
  auto view2 = view1 | rv::transform(convert); // source是viewable_range，可以被处理；view1本身就是view

  // Step Three:
  auto check_suggestions = [](const Misspelling &m) /*lambd表达式参数类型 应该对应 输入range的iterator解引用结果*/
  { return !m.suggestions.empty(); };
  auto view3 = view2 | rv::filter(check_suggestions);

  std::set<Misspelling> result(view3.begin(), view3.end());
  return result;
}

/* Helper methods */

#include "utils.cpp"


//评测机竟然不支持在build目录中的main