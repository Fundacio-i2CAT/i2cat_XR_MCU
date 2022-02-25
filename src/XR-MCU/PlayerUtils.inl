
namespace mcu
{

template<typename T, size_t Dim>
bool eps_equal(const std::array<T,Dim>& i_lhs,const std::array<T,Dim>& i_rhs,double i_eps)
{
	for(size_t index=0;index<Dim;++index)
	{
		if(eps_different(i_lhs[index],i_rhs[index]))
		{
			return false;
		}
	}

	return true;
}
template<typename T>
bool eps_different(const T& i_lhs,const T& i_rhs,double i_eps)
{
	return (eps_equal(i_lhs,i_rhs,i_eps) == false);
}

}