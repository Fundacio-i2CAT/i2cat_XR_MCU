#pragma once

#include "DDKLogger.h"
#include <array>
#include <cmath>
#include <stdint.h>

namespace mcu
{

extern const double k_epsilon;
enum class Coord
{
	x,
	y,
	z
};

typedef std::array<float,3> position_3d;

position_3d operator+(const position_3d& i_lhs,const position_3d& i_rhs);
position_3d operator-(const position_3d& i_lhs,const position_3d& i_rhs);
float operator*(const position_3d& i_lhs,const position_3d& i_rhs);
position_3d operator*(const float& i_lhs,const position_3d& i_rhs);
position_3d operator*(const position_3d& i_lhs,const float& i_rhs);
float norm(const position_3d& i_lhs);
float angle(const position_3d& i_lhs, const position_3d& i_rhs);

typedef std::array<float,2> position_2d;

position_2d operator+(const position_2d& i_lhs,const position_2d& i_rhs);
position_2d operator-(const position_2d& i_lhs,const position_2d& i_rhs);
float operator*(const position_2d& i_lhs,const position_2d& i_rhs);
position_2d operator*(const float& i_lhs,const position_2d& i_rhs);
position_2d operator*(const position_2d& i_lhs,const float& i_rhs);
float norm(const position_2d& i_lhs);
float angle(const position_2d& i_lhs,const position_2d& i_rhs);

typedef std::array<float,4> quaternion;

quaternion operator+(const quaternion& i_lhs,const quaternion& i_rhs);
quaternion operator-(const quaternion& i_lhs,const quaternion& i_rhs);
quaternion operator*(const quaternion& i_lhs,const quaternion& i_rhs);
quaternion conjugate(const quaternion& other);
quaternion inv(const quaternion& other);
position_3d as_vector(const quaternion& other);
position_3d transform(const quaternion& i_lhs, const position_3d& i_rhs);

typedef double rotation_2d;

bool eps_equal(const int32_t& i_lhs,const int32_t& i_rhs,double i_eps = 0.f);
bool eps_equal(const double& i_lhs,const double& i_rhs,double i_eps = k_epsilon);
bool eps_equal(const float& i_lhs,const float& i_rhs,double i_eps = k_epsilon);
template<typename T,size_t Dim>
inline bool eps_equal(const std::array<T,Dim>& i_lhs,const std::array<T,Dim>& i_rhs,double i_eps = k_epsilon);
template<typename T>
inline bool eps_different(const T& i_lhs,const T& i_rhs,double i_eps = k_epsilon);
double to_euler_angle(Coord i_index,const quaternion& i_quat);

struct bounding_box
{
public:
	enum Coords
	{
		LowerLeft = 0,
		UpperRight
	};

	bounding_box();
	bounding_box(const position_3d& i_topLeft,const position_3d& i_bottomRight);
	void update(const position_3d& i_vertex);
	position_3d get_vertex(Coords i_coord) const;
	position_3d center() const;

	bool operator==(const bounding_box& other) const;
	bool operator!=(const bounding_box& other) const;

private:
	std::array<position_3d,2> m_vertices;
};

struct Frustrum
{
	enum Area
	{
		Main,
		Periphereal,
		None
	};
	enum RotationAxis
	{
		x,
		y,
		Count
	};

	friend inline Area operator&(const Frustrum& i_lhs,const position_3d& i_rhs)
	{
		//free vector between pov and player
		const position_3d relPos = i_rhs - i_lhs.m_center;

		//transform relPos by current quaternion
		const quaternion antiRotation = inv(i_lhs.m_rotation);
		const position_3d transRelPos = transform(antiRotation,relPos);
		const position_2d transRelPosPlaneXZ{ transRelPos[0],transRelPos[2] };

		//by now we only take into account xz plane
		const double rhsAngle[RotationAxis::Count] = { M_PI_2 - std::acos(transRelPos[1] / norm(transRelPos)),
													std::acos(transRelPosPlaneXZ[1] / norm(transRelPosPlaneXZ)) };

		const auto areaResolver = [&i_lhs](double i_angle)
		{
			const double deltaAngle = std::abs(i_angle);
			if(deltaAngle < i_lhs.m_totalOperture)
			{
				if(deltaAngle < i_lhs.m_mainOperture)
				{
					return Main;
				}
				else
				{
					return Periphereal;
				}
			}
			else
			{
				return None;
			}
		};

		const Area hArea = areaResolver(rhsAngle[RotationAxis::y]);
		const Area vArea = areaResolver(rhsAngle[RotationAxis::x]);

		if(hArea == Main && vArea == Main)
		{
			return Main;
		}
		else if(hArea == None || vArea == None)
		{
			return None;
		}
		else
		{
			return Periphereal;
		}
	}
	friend inline Area operator&(const Frustrum& i_lhs,const bounding_box& i_rhs)
	{
		const position_3d boundingBoxCenter = i_rhs.center();
		const position_3d center2center = boundingBoxCenter - i_lhs.m_center;

		//depending on how far is the center of the bounding box we will consider a certain fov strategy
		if(norm(center2center) < i_lhs.m_primaryDistance)
		{
			//in this strategy we just set as Main Area any bounding box whose center is on the positive half hyper-plane defined by the transformed z by rotation
			const position_3d transformedZ = transform(i_lhs.m_rotation,position_3d{ 0.f,0.f,1.f });

			return (angle(center2center,transformedZ) < M_PI_2) ? Main : None;
		}
		else
		{
			const Area res[3] = { i_lhs & i_rhs.get_vertex(bounding_box::LowerLeft),i_lhs & boundingBoxCenter,i_lhs & i_rhs.get_vertex(bounding_box::UpperRight) };

			if(res[0] == Main || res[1] == Main || res[2] == Main)
			{
				return Main;
			}
			else if(res[0] == Periphereal || res[1] == Periphereal || res[2] == Periphereal)
			{
				return Periphereal;
			}
			else
			{
				return None;
			}
		}
	}
public:
	Frustrum(float i_primaryDistance, double i_mainOperture,double i_totalOperture);

	bool update(const position_3d& i_center,const quaternion& i_rotation);
	const position_3d& get_position() const;
	auto get_rotation() const;

private:
	position_3d m_center = { 0 };
	quaternion m_rotation = { 0.f };
	float m_primaryDistance = 0.f;
	double m_mainOperture = 0.f;
	double m_totalOperture = 0.f;
};

}

#include "PlayerUtils.inl"