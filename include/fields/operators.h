#ifndef OPERATORS_H
#define OPERATORS_H

#include <ostream>

#include "types.h"
#include "zipper.h"


namespace yaff::op
{
  template<typename T>
  struct SumOp
  {
    using r_type = T;
    r_type operator()(const T& a, const T& b) { return a + b; };
  };

  template<typename T/*, bool is_newline = false*/>
  struct PrintOp
  {
    using r_type = void;
    r_type operator()(std::ostream& os, const T& a) 
    { 
      os << a;
      //if(is_newline) os << "\n";
    };
  };
  


  template <template <typename> class O, typename data_type, typename T1, typename T2>
  auto for_each(const arr_t<T1>& A, const arr_t<T2>& B) 
  {
    // This is the lowest level, so assuming the all the checks are ok
    O<data_type> op;
    arr_t<typename O<data_type>::r_type> result(A.size());
    
    for (auto && [r, a, b] : zip(result, A, B)) 
    {
      r = op(a, b);
    }
    return result;
  }
  
  template <typename lhs, template <typename> class O, typename data_type>
  void for_each(lhs A, const arr_t<data_type>& B) 
  {
    O<data_type> op;
    for (const auto& b :  B) 
    {
      op(A, b);
    }
  }
}


#endif /* OPERATORS_H */
