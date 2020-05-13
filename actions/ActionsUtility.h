#pragma once

#include <algorithm>
 
namespace ranges
{
  template<typename Range, typename Function>
    Function for_each(Range& range, Function f)
  {
    return std::for_each(begin(range), end(range), f);
  }
}
