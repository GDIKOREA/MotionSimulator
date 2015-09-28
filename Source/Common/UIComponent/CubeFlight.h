//
// ť�� ����ü ������ �����Ѵ�.
//
//
#pragma once


class cCubeFlight
{
public:
	cCubeFlight();
	virtual ~cCubeFlight();

	bool Init(graphic::cRenderer &renderer);
	void Render(graphic::cRenderer &renderer);
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
