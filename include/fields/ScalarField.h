#ifndef SCALARFIELD_H
#define SCALARFIELD_H

#include "types.h"
#include "operators.h"

namespace yaff 
{

template <typename type = flt_t>
class ScalarField 
{
 public:
  using data_type = type;

 private:
  arr_t<data_type> v_;

 public:
  ScalarField(){};
  ScalarField(idx_t n) : v_(n, 0.0){};
  ScalarField(arr_t<data_type> data) : v_(data) {};
    
  ScalarField operator+(const ScalarField& other) 
  {
    return ScalarField( op::for_each<op::SumOp, data_type>(v_, other.v_) );
  };
  
  template <typename T>
  friend std::ostream& operator<<(std::ostream& os, const ScalarField<T>& s);

};


template<typename data_type>
std::ostream& operator<<(std::ostream& os, const ScalarField<data_type>& s)
{
  op::for_each<std::ostream&, op::PrintOp, data_type>(os, s.v_);
  return os;
}

}  // namespace yaff

#endif /* SCALARFIELD_H */
