
#include "stdafx.h"
#include "CubeFlight.h"


cCubeFlight::cCubeFlight()
{
}

cCubeFlight::~cCubeFlight()
{

}


// 클래스 초기화.
bool cCubeFlight::Init()
{
	m_mesh[0].SetCube(Vector3(-1, -1, -1), Vector3(1, 1, 1));
	m_mesh[1].SetCube(Vector3(1, -1, -1), Vector3(3, 1, 1));
	m_mesh[2].SetCube(Vector3(-1, 1, -1), Vector3(1, 2, 1));

	m_mesh[0].GetMaterial().InitBlue();
	m_mesh[1].GetMaterial().InitBlue();
	m_mesh[2].GetMaterial().InitBlue();

	return true;
}


// 화면에 출력한다.
void cCubeFlight::Render()
{
	m_mesh[0].Render(m_tm);
	m_mesh[1].Render(m_tm);
	m_mesh[2].Render(m_tm);
}


// 매 프레임마다 연산할 것을 추가한다.
void cCubeFlight::Update(const float deltaSeconds)
{
}


// 큐브 각도를 오일러 각으로 설정한다.
void cCubeFlight::SetEulerAngle(const float roll, const float pitch, const float yaw)
{
	m_rotate.Euler2(Vector3(roll, yaw, pitch));
	m_tm = m_rotate.GetMatrix();
}
