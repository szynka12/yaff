#ifndef SCALAR_H
#define SCALAR_H

#include <iostream>
#include "types.h"
#include "operators.h"



namespace yaff
{
  template <typename type = flt_t>
  class Scalar
  {
  public:
    using data_type = type;
  
  private:
    
    data_type   v_ = 0.0;

  public:
    
    Scalar() {};
    Scalar(data_type value) : v_(value) {};
    virtual ~Scalar() {};

    data_type v() const { return v_; }

    Scalar operator+(const Scalar& other) 
    {
      return Scalar( op::SumOp<data_type>()(v_, other.v_));
    };
    
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const Scalar<T>& s);
  };

  template<typename data_type>
  std::ostream& operator<<(std::ostream& os, const Scalar<data_type>& s)
  {
    op::PrintOp<data_type>()(os, s.v_);
    return os;
  }


}



#endif /* SCALAR_H */
