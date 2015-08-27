
#include "stdafx.h"
#include "CubeFlight.h"


cCubeFlight::cCubeFlight()
{
}

cCubeFlight::~cCubeFlight()
{

}


// Ŭ���� �ʱ�ȭ.
bool cCubeFlight::Init()
{
	m_mesh[0].SetCube(Vector3(-1, -1, -1), Vector3(1, 1, 1));
	m_mesh[1].SetCube(Vector3(1, -1, -1), Vector3(3, 1, 1));
	m_mesh[2].SetCube(Vector3(-1, 1, -1), Vector3(1, 2, 1));
	const float weight = 0.15f;
	m_mesh[3].SetCube(Vector3(-4, -weight, -weight), Vector3(6, weight, weight));
	m_mesh[4].SetCube(Vector3(-weight, -6, -weight), Vector3(weight, 6, weight));
	m_mesh[5].SetCube(Vector3(-weight, -weight, -6), Vector3(weight, weight, 6));

	m_mesh[0].GetMaterial().InitBlue();
	m_mesh[1].GetMaterial().InitBlue();
	m_mesh[2].GetMaterial().InitBlue();
	m_mesh[3].GetMaterial().InitRed();
	m_mesh[4].GetMaterial().InitGreen();
	m_mesh[5].GetMaterial().InitWhite();

	return true;
}


// ȭ�鿡 ����Ѵ�.
void cCubeFlight::Render()
{
	m_mesh[0].Render(m_tm);
	m_mesh[1].Render(m_tm);
	m_mesh[2].Render(m_tm);
	m_mesh[3].Render(m_tm);
	m_mesh[4].Render(m_tm);
	m_mesh[5].Render(m_tm);
}


// �� �����Ӹ��� ������ ���� �߰��Ѵ�.
void cCubeFlight::Update(const float deltaSeconds)
{
}


// ť�� ������ ���Ϸ� ������ �����Ѵ�.
void cCubeFlight::SetEulerAngle(const float roll, const float pitch, const float yaw)
{
	m_rotate.Euler2(Vector3(roll, yaw, pitch));
	m_tm = m_rotate.GetMatrix();
}
