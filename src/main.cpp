#include <iostream>
#include <types.h>
#include <Scalar.h>
#include "ScalarField.h"
#include <vector>
#include <array>
#include <cmath>
#include "zipper.h"

// TODO Convert to m_ for member variables 

constexpr int get_N(int n)
{
    return n < 1 ? 1 : n;
}

template <typename T>
constexpr T ipow(T num, unsigned int pow)
{
    return (pow >= sizeof(unsigned int)*8) ? 0 :
        pow == 0 ? 1 : num * ipow(num, pow-1);
}

template <int rank, int dim = 3, typename T = double>
struct Tensor
{
  static constexpr int N = get_N(ipow(dim, rank));
  std::array<T, N> m_data;

  template <int r, int d, typename U>
  friend std::ostream& operator<<(std::ostream& os, const Tensor<r, d, U>& t);

  size_t convert_index(const std::array<size_t, rank> indices)
  {
    if constexpr (rank == 0) { return 0; }
    else
    {
      size_t idx = 0;
      for (size_t i = rank; i > 0 ; i--)
      {
        idx += (indices[i-1] - 1) * std::pow(dim, rank - i);
      }
      return idx;
    }
  }

  T& i(const std::array<size_t, rank> indices)
  {
    return m_data[convert_index(indices)];
  };
  
  const T& c_i(const std::array<size_t, rank> indices)
  {
    return m_data[convert_index(indices)];
  };

  auto operator*(const Tensor<rank-1, dim, T>& other)
  {
    Tensor<rank-1, dim, T> result;
    // TODO Iterator that can give i, j, k like a zipper 
    //
    return result;
  }
  
};

template <int r, int d, typename U>
std::ostream& operator<<(std::ostream& os, const Tensor<r, d, U>& t)
{
  os << "[ ";
  for (size_t i = 0; i < t.N; i++)
  {
    os << t.m_data[i] << " ";
  }
  os << "]";
  return os;
}

int main(int argc, char * argv[])
{

  
  Tensor<1> vec{1.0, 1.0, 3.0};
  Tensor<0> scalar{1.0};
  Tensor<2> tensor{1.1, 1, 1, 2.1, 1, 1, 1, 1, 27};
  std::cout << "vector : " << vec << std::endl;
  std::cout << "scalar : " << scalar << std::endl;
  std::cout << "tensor (rank 2) : " << tensor << std::endl;

  std::cout << tensor.by_indices({3,3}) << std::endl;

  return 0;
}
