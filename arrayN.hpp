#include <iostream>
#include <iterator>
 
template< typename T >
struct value
{
  T t_;
  value() : t_( T() ) {}
  value( T t ) : t_( t ) {}
  void t( T t ) { t_ = t; }
  T& t() { return t_; }
  typedef T value_t;
};
 
template< size_t N, typename T >
struct arrayN : arrayN < N-1, T >
{
  value<T> v;
  arrayN<N,T>() : v() {}
  const size_t I = N;
  typedef T value_t;
};
 
template< typename T >
struct arrayN< 0, T >
{
  value<T> v;
  arrayN<0,T>() : v() {}
  typedef struct arrayN type;
  const size_t I = 0;
  typedef T value_t;
};
 
template< size_t I, typename list_t, typename value_t = typename list_t::value_t >
value< value_t > & index( list_t & l )
{
  typedef arrayN< I, value_t > which_base;
  list_t * pl = &l;
  auto pitem = static_cast< which_base* >( pl );
  return pitem->v;
}
 
template< size_t N >
struct applyN
{
  template< typename F >
  static auto apply( F& f ) -> decltype( f.template operator()<0>() )
  {
    applyN<N-1>::apply(f);
    return f.template operator()<N>();
  }
};
 
template<>
struct applyN<0>
{
  template< typename F >
  static auto apply( F& f ) -> decltype( f.template operator()<0>() )
  {
    return f.template operator()<0>();
  }
};
 
template< size_t N, typename F >
void forN( F & f )
{
  applyN<N>::apply(f);
}
 
template< typename T >
struct set_ith
{
  T& t_;
  set_ith( T & t ) : t_( t ) {}
 
  template< size_t N >
  void operator()()
  {
    return index<N>( t_ ).t( N );
  }
};
 
template< typename T, typename out_iterator_t >
struct get_ith
{
  T& t_;
  out_iterator_t & output;
 
  get_ith( T & t, out_iterator_t & o ) : t_( t ), output( o ) {}
 
  template< size_t N >
  void operator()()
  {
	if( 0 == N%50 ) 
	  *output = index<N>( t_ ).t();
  }
};
 
template< size_t N, size_t K, typename T >
struct testN : arrayN< K, T >
{
  testN() : arrayN<K,T>()
  {
    arrayN<K,T> &a( *this );
    set_ith< arrayN<K,T> > s( a );
    forN<K>(s);
  }
};
 
template< typename T, size_t W >
struct test_forNxN
{
  template< size_t N >
  void operator()()
  {
    std::cout << std::endl << "test_forNxN< " << N << " >: ";
    testN<N,W,int> t0;
    std::ostream_iterator< int > out( std::cout, " " );
    get_ith< testN<N,W,int>, std::ostream_iterator< int > > g( t0, out );
    forN<W>(g);
    std::cout << std::endl;
  }
};
 
int main(int, char*[])
{
  test_forNxN<void,300> t10;
  forN<10>(t10); // compiler fails somewhere between 300 and 400
  return 0;
}
