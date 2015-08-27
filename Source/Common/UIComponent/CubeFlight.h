//
// 큐브 비행체 정보를 관리한다.
//
//
#pragma once


class cCubeFlight
{
public:
	cCubeFlight();
	virtual ~cCubeFlight();

	bool Init();
	void Render();
	void Update(const float deltaSeconds);

	void SetEulerAngle(const float roll, const float pitch, const float yaw);
	const Quaternion& GetRotation() const;


protected:
	graphic::cCube2 m_mesh[6];
	Vector3 m_locate;
	Quaternion m_rotate;
	Matrix44 m_tm;
};


inline const Quaternion& cCubeFlight::GetRotation() const { return m_rotate; }
