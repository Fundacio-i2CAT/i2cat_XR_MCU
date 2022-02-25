#include <cmath>
#include "PlayerTransformation.h"
#include "PlayerUtils.h"

namespace mcu
{

transformation::transformation()
{
	for (size_t dimIndex = 0; dimIndex < 4; ++dimIndex)
	{
		m_mat[dimIndex][dimIndex] = 1.f;
	}
}
bool transformation::set_position(const position_3d& i_pos)
{
	bool somethingChanged = false;

	for (size_t dimIndex = 0; dimIndex < i_pos.size(); ++dimIndex)
	{
		if(eps_different(m_mat[dimIndex][s_dim],i_pos[dimIndex]))
		{
			m_mat[dimIndex][s_dim] = i_pos[dimIndex];
			somethingChanged = true;
		}
	}	

	return somethingChanged;
}


 float transformation::get(size_t i_indexI, size_t i_indexJ) const 
 {
	 return  m_mat[i_indexI][i_indexJ];
 }

position_3d transformation::get_position() const
{
	return { m_mat[0][s_dim], m_mat[1][s_dim], m_mat[2][s_dim]};
}
bool transformation::set_rotation(const rotation_2d& i_rot)
{
	//since we deal with floats, consider an eps for handling comparisons
	static const double s_epsilon = 1e-5;
	bool somethingChanged = false;

	float rot00 = static_cast<float>(cos(i_rot + M_PI));
	float rot01 = static_cast<float>(sin(i_rot + M_PI));
	float rot10 = static_cast<float>(-sin(i_rot + M_PI));
	float rot11 = static_cast<float>(cos(i_rot + M_PI));

	// Y Axis Rotation
	if(eps_different(m_mat[0][0],rot00))
	{
		m_mat[0][0] = rot00;
		somethingChanged = true;
	}
	if(eps_different(m_mat[0][2],rot01))
	{
		m_mat[0][2] = rot01;
		somethingChanged = true;
	}
	if(eps_different(m_mat[2][0],rot10))
	{
		m_mat[2][0] = rot10;
		somethingChanged = true;
	}
	if(eps_different(m_mat[2][2],rot11))
	{
		m_mat[2][2] = rot11;
		somethingChanged = true;
	}

	return somethingChanged;
}
bool transformation::set_rotation(const quaternion& i_rot)
{
	return set_rotation(to_euler_angle(Coord::y,i_rot));
}
position_3d transformation::transform(const position_3d& i_pos) const
{
	position_3d res = { 0 };

	for (size_t rowIndex = 0; rowIndex < s_dim; ++rowIndex)
	{
		for (size_t columnIndex = 0; columnIndex < s_dim; ++columnIndex)
		{
			 res[rowIndex] += m_mat[rowIndex][columnIndex] * i_pos[columnIndex];
		}
	}	

	return res;
}

void transformation::in_place_transform(position_3d& i_pos) const
{
	position_3d res = { 0 };

	for (size_t rowIndex = 0; rowIndex < s_dim; ++rowIndex)
	{
		for (size_t columnIndex = 0; columnIndex < s_dim; ++columnIndex)
		{
			 res[rowIndex] += m_mat[rowIndex][columnIndex] * i_pos[columnIndex];
		}
	}	

	i_pos = std::move(res);
}
void transformation::in_place_transform(float& io_posX, float& io_posY, float& io_posZ) const
{
	position_3d res = { 0 };

	 res[0] = m_mat[0][0] * io_posX + m_mat[0][1] * io_posY + m_mat[0][2] * io_posZ;
	 res[1] = m_mat[1][0] * io_posX + m_mat[1][1] * io_posY + m_mat[1][2] * io_posZ;
	 res[2] = m_mat[2][0] * io_posX + m_mat[2][1] * io_posY + m_mat[2][2] * io_posZ;

	io_posX = res[0];
	io_posY = res[1];
	io_posZ = res[2];
}

float distance(const position_3d& i_lhs,const position_3d& i_rhs)
{
	const position_3d dist = i_lhs - i_rhs;

	return std::sqrt(dist[0] * dist[0] + dist[1] * dist[1] + dist[2] * dist[2]);
}
float square_distance(const position_3d& i_lhs, const position_3d& i_rhs)
{
	const position_3d dist = i_lhs - i_rhs;

	return dist[0] * dist[0] + dist[1] * dist[1] + dist[2] * dist[2];
}

}