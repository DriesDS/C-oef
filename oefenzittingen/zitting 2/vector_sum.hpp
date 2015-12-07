#ifndef vector_sum_hpp
#define vector_sum_hpp

#include <cassert>
#include <type_traits>

namespace tws {
  template <typename V1, typename V2>
  class vector_sum {
    public:
      //typedef double                         value_type ;
      typedef typename V1::size_type size_type ;

    public:
      vector_sum( V1 const& v1, V2 const& v2 )
      : v1_( v1 )
      , v2_( v2 )
      {
        assert( v1_.size()==v2_.size() ) ;
      }

      size_type size() const {
        return v1_.size() ;
      }

      auto operator[]( size_type const& i ) const { return v1_[i]+v2_[i]; }

    private:
      V1 const v1_ ;
      V2 const v2_ ;
  } ;

  template <typename V1, typename V2>
  class vector_diff {
    public:
      typedef double                value_type ;
      typedef typename V1::size_type size_type ;

    public:
      vector_diff( V1 const& v1, V2 const& v2 )
      : v1_( v1 )
      , v2_( v2 )
      {
        assert( v1_.size()==v2_.size() ) ;
      }

      size_type size() const {
        return v1_.size() ;
      }

      value_type operator[]( size_type const& i ) const { return v1_[i]-v2_[i]; }

    private:
      V1 const v1_ ;
      V2 const v2_ ;
  } ;

  template <typename V1, typename scal>
  class vector_scal_prod {
    public:
      //typedef double    value_type ;
      typedef typename V1::size_type size_type ;

    public:
      vector_scal_prod( V1 const& v1, scal const& s )
      : v1_( v1 ), s_( s )
      {}

      size_type size() const {
        return v1_.size();
      }

      auto operator[]( size_type const& i ) const { return v1_[i]*s_; }

    private:
      V1 const v1_ ;
      double const s_ ;
  };

  template <typename V1, typename V2>
  class vector_prod {
    public:
      typedef double                         value_type ;
      typedef typename V1::size_type size_type ;

    public:
      vector_prod( V1 const& v1, V2 const& v2 )
      : v1_( v1 )
      , v2_( v2 )
      {
        assert( v1_.size()==v2_.size() ) ;
      }

      size_type size() const {
        return v1_.size() ;
      }

      value_type operator[]( size_type const& i ) const { return v1_[i]*v2_[i]; }

    private:
      V1 const v1_ ;
      V2 const v2_ ;
  } ;

  template <typename V1, typename V2>
    vector_sum<V1,V2> operator+( V1 const& v1, V2 const& v2 ) {return vector_sum<V1,V2>(v1,v2);}

  template <typename V1, typename V2>
    vector_diff<V1,V2> operator-( V1 const& v1, V2 const& v2 ) {return vector_diff<V1,V2>(v1,v2);}

  template <typename scal, typename V2>
    typename std::enable_if<std::is_scalar<scal>::value, vector_scal_prod<V2,scal> >::type operator*( scal const& s, V2 const& v2) {return vector_scal_prod<V2,scal>(v2, s) ; }

  template <typename V1, typename V2>
    typename std::enable_if<! std::is_scalar<V1>::value, vector_prod<V1,V2> >::type operator*( V1 const& v1, V2 const& v2) {return vector_prod<V1,V2>(v1, v2) ; }

}

#endif
