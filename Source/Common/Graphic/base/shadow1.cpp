
#include "stdafx.h"
#include "shadow1.h"

using namespace graphic;


cShadow1::cShadow1()
{
}

cShadow1::~cShadow1()
{
}


// �׸��� Ŭ���� ����.
bool cShadow1::Create(cRenderer &renderer, const int textureWidth, const int textureHeight)
{
	D3DVIEWPORT9 viewport = {0, 0, textureWidth, textureHeight, 0.0f, 1.0f};
	return m_surface.Create(renderer, textureWidth, textureHeight, 1, 
		D3DFMT_X8R8G8B8, true, D3DFMT_D24X8, viewport, true);
}


// �׸��� �ؽ��ĸ� ������Ʈ �Ѵ�.
void cShadow1::UpdateShadow(cRenderer &renderer, cNode &node)
{
	RET(!m_surface.IsLoaded());

	const Vector3 pos = node.GetTransform().GetPosition();

	// ���� �������� ���� �׸��� ������ �ʿ��� ������ ���´�.
	Vector3 lightPos;
	Matrix44 view, proj, tt;
	cLightManager::Get()->GetMainLight().GetShadowMatrix(
		pos, lightPos, view, proj, tt );

	m_surface.Begin();

	renderer.GetDevice()->Clear(0, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, 0x00000000, 1.0f, 0L);

	node.RenderShadow(renderer, view*proj, lightPos, Vector3(0,-1,0), Matrix44::Identity);
	m_surface.End();
}


// �׸��� �� ���. (������)
void cShadow1::RenderShadowMap(cRenderer &renderer)
{
	m_surface.Render(renderer);
}
