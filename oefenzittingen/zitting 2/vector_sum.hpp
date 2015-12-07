#ifndef vector_sum_hpp
#define vector_sum_hpp

#include <cassert>

namespace tws {
  template <typename V1, typename V2>
  class vector_sum {
    public:
      typedef double                         value_type ;
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

      value_type operator[]( size_type const& i ) const { return v1_[i]+v2_[i]; }

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

  template <typename V1>
  class vector_scal_prod {
    public:
      typedef double    value_type ;
      typedef typename V1::size_type size_type ;

    public:
      vector_scal_prod( V1 const& v1, double const& scal )
      : v1_( v1 ), scal_( scal )
      {}

      size_type size() const {
        return v1_.size();
      }

      value_type operator[]( size_type const& i ) const { return v1_[i]*scal_; }

    private:
      V1 const v1_ ;
      double const scal_ ;
  };

  template <typename V1, typename V2>
    vector_sum<V1,V2> operator+( V1 const& v1, V2 const& v2 ) {return vector_sum<V1,V2>(v1,v2);}

  template <typename V1, typename V2>
    vector_diff<V1,V2> operator-( V1 const& v1, V2 const& v2 ) {return vector_diff<V1,V2>(v1,v2);}

  template <typename V1>
    vector_scal_prod<V1> operator*( double const& scal, V1 const& v1) {return vector_scal_prod<V1>(v1, scal) ; }

}

#endif
