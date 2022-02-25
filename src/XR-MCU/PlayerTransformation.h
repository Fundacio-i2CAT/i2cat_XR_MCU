#pragma once

#include "PlayerDefs.h"
#include "PlayerUtils.h"
#include <array>
#include <iostream>
#include <vector>

namespace mcu
{

class transformation
{
	static const size_t s_dim = 3;

public:
	transformation();

	bool set_position(const position_3d& i_pos);
	bool set_rotation(const rotation_2d& i_rot);
	bool set_rotation(const quaternion& i_rot);

	position_3d get_position() const;

	position_3d transform(const position_3d& i_pos) const;

	float get(size_t i_indexI, size_t i_indexJ) const;

	void in_place_transform(position_3d& i_pos) const;
	void in_place_transform(float& io_posX, float& io_posY, float& io_posZ) const;
	template<typename Iterator>
	bounding_box in_place_transform(Iterator itInit, Iterator itEnd) const
	{

		bounding_box res;

		for (; itInit != itEnd; ++itInit)
		{
			position_3d tmp = { 0.f, 0.f, 0.f };

			tmp[0] = m_mat[0][0] * itInit->x + m_mat[0][1] * itInit->y + m_mat[0][2] * itInit->z;
			tmp[1] = m_mat[1][0] * itInit->x + m_mat[1][1] * itInit->y + m_mat[1][2] * itInit->z;
			tmp[2] = m_mat[2][0] * itInit->x + m_mat[2][1] * itInit->y + m_mat[2][2] * itInit->z;

			itInit->x = tmp[0] + m_mat[0][s_dim];
			itInit->y = tmp[1] + m_mat[1][s_dim];
			itInit->z = tmp[2] + m_mat[2][s_dim];

			res.update({ itInit->x,itInit->y,itInit->z });
		}

		return res;
	}

private:
	float m_mat[4][4] = { { 0.f } };

};


float distance(const position_3d& i_lhs,const position_3d& i_rhs);
float square_distance(const position_3d& i_lhs, const position_3d& i_rhs);

}