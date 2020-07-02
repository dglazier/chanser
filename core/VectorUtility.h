#pragma once

#include <algorithm>
#include <numeric>

namespace chanser{
  namespace ranges
  {
    ////////////////////////////////////////////////////
    template<typename Range, typename T>
      bool contains(Range& range, T t)
    {
      return std::find(begin(range), end(range), t) != end(range);
    }


    ///////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    template<typename Range, typename Function>
      Function for_each(Range& range, Function f)
    {
      return std::for_each(begin(range), end(range), f);
    }
    ////////////////////////////////////////////////////////////////
    //filter vectors via a lambda function
    template <typename Cont, typename Pred>
      Cont filter(const Cont &container, Pred predicate){
      Cont result;
      std::copy_if(container.begin(),container.end(),std::back_inserter(result), predicate);
      return result;
    }
    ///////////////////////////////////////////////////////////////
  }
}
