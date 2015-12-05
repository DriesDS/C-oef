#ifndef vector_hpp
#define vector_hpp

#include <vector>
#include <cassert>
#include <iostream>

namespace tws {

  class vector {
    private:
      typedef std::vector<double> data_type ;
     
    public:
      typedef double  value_type ;
      typedef int     size_type ;
      //typedef data_type::size_type size_type ;

    public:
      inline vector( size_type n )
      : data_( n )
      {}

      vector( vector const& v )
      : data_( v.data_ )
      {}

      template <typename Vector>
      vector( Vector const& v )
      : data_( v.size() )
      {
        for (size_type i=0; i<size(); ++i) { data_[i] = v[i] ; }
      }

      inline size_type size() const { return data_.size() ; }
      inline value_type operator[](size_type i) const { return data_[i] ; }
      inline value_type& operator[](size_type i) { return data_[i] ; }
      
    public:
      template <typename V>
      vector& operator+=( const V& v1 ) {
        for (size_type i=0; i<size(); ++i) { 
          data_[i] += v1[i] ; 
        }
        return *this;
      } 

      template <typename V>
      vector& operator-=( const V& v1 ) {
        for (size_type i=0; i<size(); ++i) { 
          data_[i] -= v1[i]; 
        }
        return *this;
      } 

      template <typename V>
      vector& operator=( const V& v1 ) {
        for (size_type i=0; i<size(); ++i) { 
          data_[i] = v1[i] ; 
        }
        return *this;
      } 

    public:
      friend std::ostream& operator<<( std::ostream& ostr, vector const& v ) {
        ostr << "(" << v.size() << ")[" ;
        for (int i=0; i<v.size(); ++i) {
          ostr << v[i] << "," ;
        }
        return ostr ;
      }

    private:
      data_type data_ ;
  } ;

  template <typename V1, typename V2>
  double inner_prod( const V1& v1, const V2& v2 ) {
    double inner_res;
    inner_res = 0;
    for (int i=0; i<v1.size(); ++i) { 
      inner_res += v1[i]*v2[i] ; 
    }
    return inner_res;
  } 

}

#endif
