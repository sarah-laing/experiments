#include <iostream>
 
template< typename... Ts >
struct tupleN
{
  const static size_t N = sizeof...( Ts );
  typedef tupleN< Ts... > type;
};
 
// http://stackoverflow.com/a/4693493/2060026
template< typename... Args > struct variadic_typedef
{
  typedef variadic_typedef< Args... > type;
};
 
template< typename... Largs, typename...Rargs >
struct variadic_typedef< variadic_typedef< Largs... >, variadic_typedef< Rargs... > >
{
  typedef typename variadic_typedef< Largs..., Rargs... >::type type;
};
 
template< typename... Args > struct as_tuple
{
  typedef tupleN< Args... > type;
};
 
template< typename... Args > struct as_tuple< variadic_typedef<Args...> >
{
  typedef typename as_tuple< Args... >::type type;
};
 
template< typename... Largs, typename...Rargs >
struct as_tuple< variadic_typedef< Largs... >, variadic_typedef< Rargs... > >
{
  typedef tupleN< Largs..., Rargs... > type;
};
 
template< template<typename...> class wrapper, typename... Args >
struct wrap_variadic_typedef
{
  typedef wrapper< Args... > type;
};
 
template< template<typename...> class wrapper, typename... Args >
struct wrap_variadic_typedef< wrapper, variadic_typedef<Args...> >
{
  typedef wrapper< Args... > type;
};
 
template< class > struct rewrap_variadic_template
{
  typedef variadic_typedef<> type;
};
 
template< template<typename...> class unwrap
        , typename... Args
        >
struct rewrap_variadic_template< unwrap<Args...> >
{
  typedef variadic_typedef< Args... > type;
};
 
template< size_t K, typename T >
struct generateN
{
  using head = variadic_typedef<T>;
  typedef typename generateN< K-1, T >::type tail;
  using vt_tail = rewrap_variadic_template< tail >;
  typedef typename variadic_typedef< head, tail >::type type;
  //struct type { static const size_t N = 1000; };
};
 
template< typename T >
struct generateN<0,T>
{
  using type = variadic_typedef<>;
  //struct type { static const size_t N = 5000; };
};
 
template< size_t N >
void test_tupleN()
{
  std::cout << std::endl << "test_tupleN" << std::endl;
 
  using vt1 = variadic_typedef<int>;
 
  using vt3 = variadic_typedef< variadic_typedef< vt1, vt1 >::type, variadic_typedef< vt1 >::type >::type;
  using tuple3 = typename as_tuple< vt3 >::type;
  std::cout << "length(tuple3) = " << tuple3::N << std::endl; // = 3
 
  using wrap_t3_in_tupleN = wrap_variadic_typedef< tupleN, vt3 >::type;
  std::cout << "length(wrap_t3_in_tupleN) = " << wrap_t3_in_tupleN::N << std::endl; // = 3
 
  using rewrapped_t3 = rewrap_variadic_template< wrap_t3_in_tupleN >::type;
  using rewrapped_t3_in_tupleN = wrap_variadic_typedef< tupleN, rewrapped_t3 >::type;
  std::cout << "length(rewrapped_t3_in_tupleN) = " << rewrapped_t3_in_tupleN::N << std::endl; // = 3
 
  typedef typename generateN<N,int>::type generatedN;
  using generated_tupleN = typename wrap_variadic_typedef< tupleN, typename generatedN::type >::type;
 
  std::cout << "length(generated_tupleN<" << N << ">) = "
            << generated_tupleN::type::N
            << std::endl; // = N
 
  std::cout << "length(generateN<0,int>) = "
  			<< wrap_variadic_typedef< tupleN, typename generateN<0,int>::type >::type::N  // = 0
            << std::endl;
}
 
int main(int, char*[])
{
  test_tupleN<800>();
  return 0;
}
