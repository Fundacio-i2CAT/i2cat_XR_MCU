#include "PlayerUtils.h"
#include "DDKLogger.h"
#include <limits>

namespace mcu
{

position_3d operator+(const position_3d& i_lhs,const position_3d& i_rhs)
{
	return { i_lhs[0] + i_rhs[0], i_lhs[1] + i_rhs[1], i_lhs[2] + i_rhs[2] };
}
position_3d operator-(const position_3d& i_lhs,const position_3d& i_rhs)
{
	return { i_lhs[0] - i_rhs[0], i_lhs[1] - i_rhs[1], i_lhs[2] - i_rhs[2] };
}
float operator*(const position_3d& i_lhs,const position_3d& i_rhs)
{
	return i_lhs[0] * i_rhs[0] + i_lhs[1] * i_rhs[1] + i_lhs[2] * i_rhs[2];
}
position_3d operator*(const float& i_lhs,const position_3d& i_rhs)
{
	return {i_rhs[0] * i_lhs, i_rhs[1] * i_lhs, i_rhs[2] * i_lhs};
}
position_3d operator*(const position_3d& i_lhs,const float& i_rhs)
{
	return { i_lhs[0] * i_rhs, i_lhs[1] * i_rhs, i_lhs[2] * i_rhs };
}
float norm(const position_3d& i_lhs)
{
	return std::sqrt(i_lhs * i_lhs);
}
float angle(const position_3d& i_lhs,const position_3d& i_rhs)
{
	return std::acos( (i_lhs * i_rhs) / (norm(i_lhs) * norm(i_rhs)) );
}

position_2d operator+(const position_2d& i_lhs,const position_2d& i_rhs)
{
	return { i_lhs[0] + i_rhs[0], i_lhs[1] + i_rhs[1] };
}
position_2d operator-(const position_2d& i_lhs,const position_2d& i_rhs)
{
	return { i_lhs[0] - i_rhs[0], i_lhs[1] - i_rhs[1] };
}
float operator*(const position_2d& i_lhs,const position_2d& i_rhs)
{
	return i_lhs[0] * i_rhs[0] + i_lhs[1] * i_rhs[1];
}
position_2d operator*(const float& i_lhs,const position_2d& i_rhs)
{
	return { i_rhs[0] * i_lhs, i_rhs[1] * i_lhs };
}
position_2d operator*(const position_2d& i_lhs,const float& i_rhs)
{
	return { i_lhs[0] * i_rhs, i_lhs[1] * i_rhs };
}
float norm(const position_2d& i_lhs)
{
	return std::sqrt(i_lhs * i_lhs);
}
float angle(const position_2d& i_lhs,const position_2d& i_rhs)
{
	return std::acos((i_lhs * i_rhs) / (norm(i_lhs) * norm(i_rhs)));
}

quaternion operator+(const quaternion& i_lhs,const quaternion& i_rhs)
{
	return { i_lhs[0] + i_rhs[0],i_lhs[1] + i_rhs[1], i_lhs[2] + i_rhs[2], i_lhs[3] + i_rhs[3] };
}
quaternion operator-(const quaternion& i_lhs,const quaternion& i_rhs)
{
	return { i_lhs[0] - i_rhs[0],i_lhs[1] - i_rhs[1], i_lhs[2] - i_rhs[2], i_lhs[3] - i_rhs[3] };
}
quaternion operator*(const quaternion& i_lhs,const quaternion& i_rhs)
{
	return { i_lhs[0] * i_rhs[0] - i_lhs[1] * i_rhs[1] - i_lhs[2] * i_rhs[2] - i_lhs[3] * i_rhs[3],
			i_lhs[0] * i_rhs[1] + i_lhs[1] * i_rhs[0] + i_lhs[2] * i_rhs[3] - i_lhs[3] * i_rhs[2],
			i_lhs[0] * i_rhs[2] - i_lhs[1] * i_rhs[3] + i_lhs[2] * i_rhs[0] + i_lhs[3] * i_rhs[1],
			i_lhs[0] * i_rhs[3] + i_lhs[1] * i_rhs[2] - i_lhs[2] * i_rhs[1] + i_lhs[3] * i_rhs[0] };
}
quaternion conjugate(const quaternion& other)
{
	return { other[0],-other[1],-other[2],-other[3] };
}
quaternion inv(const quaternion& other)
{
	const float squaredNorm = other[0] * other[0] + other[1] * other[1] + other[2] * other[2] + other[3] * other[3];

	return { other[0] / squaredNorm,-other[1] / squaredNorm,-other[2] / squaredNorm,-other[3] / squaredNorm };
}
position_3d as_vector(const quaternion& other)
{
	return { other[1],other[2],other[3] };
}
position_3d transform(const quaternion& i_lhs, const position_3d& i_rhs)
{
	return as_vector( i_lhs * quaternion{ 0.f,i_rhs[0],i_rhs[1],i_rhs[2] } * conjugate(i_lhs) );
}

const double k_quaternionPrecision = 0.001f;
const double k_epsilon = 1e-5;

bool eps_equal(const int32_t& i_lhs,const int32_t& i_rhs,double i_eps)
{
	if(i_eps == 0.f)
	{
		return i_lhs == i_rhs;
	}
	else
	{
		return std::abs(i_lhs - i_rhs) < i_eps;
	}
}
bool eps_equal(const double& i_lhs,const double& i_rhs,double i_eps)
{
	return std::abs(i_lhs - i_rhs) < i_eps;
}
bool eps_equal(const float& i_lhs,const float& i_rhs,double i_eps)
{
	return std::abs(i_lhs - i_rhs) < i_eps;
}
double to_euler_angle(Coord i_index,const quaternion& i_quat)
{
	switch(i_index)
	{
	case Coord::x:
	{
		const double sinr_cosp = 2 * (i_quat[0] * i_quat[1] + i_quat[2] * i_quat[3]);
		const double cosr_cosp = 1 - 2 * (i_quat[1] * i_quat[1] + i_quat[3] * i_quat[3]);

		return std::atan2(sinr_cosp,cosr_cosp);
	}
	case Coord::y:
	{
		const double sinr_cosp = 2 * (i_quat[0] * i_quat[2] + i_quat[1] * i_quat[3]);
		const double cosr_cosp = 1 - 2 * (i_quat[2] * i_quat[2] + i_quat[3] * i_quat[3]);

		return std::atan2(sinr_cosp,cosr_cosp);
	}
	case Coord::z:
	{
		const double sinp = 2 * (i_quat[0] * i_quat[3] - i_quat[2] * i_quat[1]);
		if(std::abs(sinp) >= 1)
			return std::copysign(M_PI / 2,sinp); // use 90 degrees if out of range
		else
			return std::asin(sinp);
	}
	}
}

bounding_box::bounding_box()
: m_vertices({ position_3d{0.f,0.f,0.f},position_3d{0.f,0.f,0.f} })
{
}
bounding_box::bounding_box(const position_3d& i_topLeft,const position_3d& i_bottomRight)
: m_vertices({ i_topLeft,i_bottomRight })
{
}
void bounding_box::update(const position_3d& i_vertex)
{
	//Top left
	if(m_vertices[Coords::LowerLeft][0] < i_vertex[0])
	{
		m_vertices[Coords::LowerLeft][0] = i_vertex[0];
	}
	if(m_vertices[Coords::LowerLeft][1] < i_vertex[1])
	{
		m_vertices[Coords::LowerLeft][1] = i_vertex[1];
	}
	if(m_vertices[Coords::LowerLeft][2] < i_vertex[2])
	{
		m_vertices[Coords::LowerLeft][2] = i_vertex[2];
	}

	//Bottom right
	if(m_vertices[Coords::UpperRight][0] > i_vertex[0])
	{
		m_vertices[Coords::UpperRight][0] = i_vertex[0];
	}
	if(m_vertices[Coords::UpperRight][1] > i_vertex[1])
	{
		m_vertices[Coords::UpperRight][1] = i_vertex[1];
	}
	if(m_vertices[Coords::UpperRight][2] > i_vertex[2])
	{
		m_vertices[Coords::UpperRight][2] = i_vertex[2];
	}
}
position_3d bounding_box::get_vertex(Coords i_coord) const
{
	return m_vertices[i_coord];
}
position_3d bounding_box::center() const
{
	return m_vertices[Coords::LowerLeft] + (m_vertices[Coords::UpperRight] - m_vertices[Coords::LowerLeft]) * 0.5f;
}
bool bounding_box::operator==(const bounding_box& other) const
{
	if(eps_different(m_vertices[Coords::LowerLeft],other.m_vertices[Coords::LowerLeft]))
	{
		return false;
	}
	if(eps_different(m_vertices[Coords::UpperRight],other.m_vertices[Coords::UpperRight]))
	{
		return false;
	}

	return true;
}
bool bounding_box::operator!=(const bounding_box& other) const
{
	if(eps_different(m_vertices[Coords::LowerLeft],other.m_vertices[Coords::LowerLeft]))
	{
		return true;
	}
	if(eps_different(m_vertices[Coords::UpperRight],other.m_vertices[Coords::UpperRight]))
	{
		return true;
	}

	return false;
}

Frustrum::Frustrum(float i_primaryDistance, double i_mainOperture,double i_totalOperture)
: m_primaryDistance(i_primaryDistance)
, m_mainOperture(i_mainOperture* (M_PI / 360.f))
, m_totalOperture(i_totalOperture* (M_PI / 360.f))
{
}
bool Frustrum::update(const position_3d& i_center,const quaternion& i_rotation)
{
	bool somethingChanged = false;

	if(eps_different(m_center,i_center))
	{
		m_center = i_center;
		somethingChanged = true;
	}

	if(eps_different(m_rotation,i_rotation,k_quaternionPrecision))
	{
		m_rotation = i_rotation;
		somethingChanged = true;
	}

	return somethingChanged;
}
const position_3d& Frustrum::get_position() const
{
	return m_center;
}
auto Frustrum::get_rotation() const
{
	return m_rotation;
}

}