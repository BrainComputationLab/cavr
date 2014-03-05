#pragma once
#include <cavr/math/vector/data.h>
#include <cavr/math/vector/utils.h>

namespace cavr {

namespace math {

template<typename T, int N>
struct vec
  : public vector::data<T, N> {
  typedef T type;

  inline vec() {
  }

  template<typename U,
           typename =
             typename std::enable_if<std::is_convertible<U, T>::value>::type>
  inline vec(const U& u) {
    for (int i = 0; i < N; ++i) {
      this->v[i] = u;
    }
  }

  template<typename... U,
           typename =
             typename std::enable_if<vector::dims<U...>::value == N>::type>
  inline vec(U&&... u) {
    vector::assign(this->v, std::forward<U>(u)...);
  }

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == N>::type>
  inline vec& operator=(const U& rhs) {
    vec& self = *this;
    // check for self assigment through potential swizzling
    if (reinterpret_cast<const vec*>(&rhs) == this) {
      vec temp(rhs);
      for (int i = 0; i < N; ++i) {
        self[i] = temp[i];
      }
    } else {
      for (int i = 0; i < N; ++i) {
        self[i] = rhs[i];
      }
    }
    return *this;
  }

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == N>::type>
  struct vector_op {
    typedef typename U::type U_type;
    typedef vec<typename std::common_type<U_type, T>::type, N> return_type;
  };

  template<typename U,
           typename V = typename std::common_type<U, T>::type,
           typename =
             typename std::enable_if<vector::dims<U>::value == 1>::type>
  struct vector_scalar_op {
    typedef vec<V, N> return_type;
  };

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == N>::type>
  struct vector_dot_op {
    typedef typename U::type U_type;
    typedef typename std::common_type<U_type, T>::type return_type;
  };

  template<typename U>
  inline typename vector_op<U>::return_type operator+(const U& rhs) const {
    typename vector_op<U>::return_type result(*this);
    result += rhs;
    return result;
  }

  template<typename U>
  inline vec& operator+=(const U& rhs) {
    vec& self = *this;
    // check for self assignment through potential swizzling
    if (reinterpret_cast<const vec*>(&rhs) == this) {
      vec temp(rhs);
      for (int i = 0; i < N; ++i) {
        self[i] += temp[i];
      }
    } else {
      for (int i = 0; i < N; ++i) {
        self[i] += rhs[i];
      }
    }
    return *this;
  }

  template<typename U>
  inline typename vector_op<U>::return_type operator-(const U& rhs) const {
    typename vector_op<U>::return_type result(*this);
    result -= rhs;
    return result;
  }

  template<typename U>
  inline vec& operator-=(const U& rhs) {
    vec& self = *this;
    // check for self assignment through potential swizzling
    if (reinterpret_cast<const vec*>(&rhs) == this) {
      vec temp(rhs);
      for (int i = 0; i < N; ++i) {
        self[i] -= temp[i];
      }
    } else {
      for (int i = 0; i < N; ++i) {
        self[i] -= rhs[i];
      }
    }
    return *this;
  }

  inline vec operator-() const {
    vec result(*this);
    for (int i = 0; i < N; ++i) {
      result[i] = -result[i];
    }
    return result;
  }

  template<typename U>
  inline typename vector_scalar_op<U>::return_type operator*(const U& u) const {
    typename vector_scalar_op<U>::return_type result(*this);
    result *= u;
    return result;
  }

  template<typename U>
  vec& operator*=(const U& u) {
    vec& self = *this;
    for (int i = 0; i < N; ++i) {
      self[i] *= u;
    }
    return *this;
  }

  template<typename U>
  friend inline typename vector_scalar_op<U>::return_type
  operator*(const U& u, const vec& r) {
    return r * u;
  }

  template<typename U>
  inline typename vector_scalar_op<U>::return_type operator/(const U& u) const {
    typename vector_scalar_op<U>::return_type result(*this);
    result /= u;
    return result;
  }

  template<typename U>
  inline vec& operator/=(const U& u) {
    vec& self = *this;
    for (int i = 0; i < N; ++i) {
      self[i] /= u;
    }
    return *this;
  }

  template<typename U>
  inline typename vector_dot_op<U>::return_type dot(const U& u) const {
    typename vector_dot_op<U>::return_type result = 0;
    const vec& self = *this;
    for (int i = 0; i < N; ++i) {
      result += self[i] * u[i];
    }
    return result;
  }

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == 3>::type>
  inline typename vector_op<U>::return_type cross(const U& u) const {
    const vec& s = *this;
    return typename vector_op<U>::return_type(s[1] * u[2] - u[1] * s[2],
                                              s[2] * u[0] - u[2] * s[0],
                                              s[0] * u[1] - u[0] * s[1]);
  }

  inline const T& operator[](int i) const {
    return this->v[i];
  }

  inline T& operator[](int i) {
    return this->v[i];
  }
};

} // namespace math

} // namespace cavr
