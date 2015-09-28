
#include "stdafx.h"
#include "skybox2.h"

using namespace graphic;


cSkyBox2::cSkyBox2() 
:	m_sphere(NULL)
,	m_envMap(NULL)
{
}

cSkyBox2::~cSkyBox2()
{
	SAFE_RELEASE(m_sphere);
	SAFE_RELEASE(m_envMap);
}


bool cSkyBox2::Create(cRenderer &renderer, const string &skyboxFileName, const float radius)
{
	RETV(m_sphere, true);// �̹� �����Ǿ��ٸ� ����.

	if (FAILED(D3DXCreateSphere(renderer.GetDevice(), radius, 30, 30, &m_sphere, 0)))
	{
		return false;
	}

	if (FAILED(D3DXCreateCubeTextureFromFileA(renderer.GetDevice(), 
		skyboxFileName.c_str(), &m_envMap)))
	{
		return false;
	}

	if (!m_shader.Create(renderer, cResourceManager::Get()->FindFile("sky.fx"), "SkyTech"))
	{
		return false;
	}

	m_radius = radius;
	m_hWVP = m_shader.GetValueHandle("gWVP");
	m_hEnvMap = m_shader.GetValueHandle("gEnvMap");

	m_shader.SetTexture("gEnvMap", m_envMap);
	return true;
}


void cSkyBox2::Render(cRenderer &renderer, const Matrix44 &tm)//tm = Matrix44::Identity
{
	RET(!m_sphere);

	Matrix44 world;
	world.SetTranslate( cMainCamera::Get()->GetEyePos() );
	world = world * tm * cMainCamera::Get()->GetViewProjectionMatrix();

	m_shader.SetMatrix(m_hWVP, world);

	m_shader.Begin();
	m_shader.BeginPass();
	m_sphere->DrawSubset(0);
	m_shader.EndPass();
	m_shader.End();
}

