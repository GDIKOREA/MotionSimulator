#include "stdafx.h"
#include "skybox.h"


using namespace graphic;


cSkyBox::cSkyBox()
{
}

cSkyBox::~cSkyBox()
{
}


//------------------------------------------------------------------------
// textureFilePath : �� ���� ��ο� skybox_top, skybox_front, skybox_back, 
//				skybox_left, skybox_right, skybox_bottom.jpg ������ �־�� �Ѵ�.
//------------------------------------------------------------------------
bool cSkyBox::Create(cRenderer &renderer, const string &textureFilePath)
{
	string textureFileName[] = 
	{
		"skybox_front.jpg", "skybox_back.jpg", "skybox_left.jpg", 
		"skybox_right.jpg", "skybox_top.jpg", "skybox_bottom.jpg"
	};

	for (int i=0; i < MAX_FACE; ++i)
	{
		const string fileName = textureFilePath + "/" + textureFileName[ i];
		m_textures[ i].Create( renderer, fileName );
	}

	if (!CreateVertexBuffer(renderer))
		return false;

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool  cSkyBox::CreateVertexBuffer(cRenderer &renderer)
{
	// Example diagram of "front" quad
	// The numbers are vertices
	// 
	// 2  __________ 4
	//   |\         |
	//	 |  \       |
	//   |    \     |
	//   |      \   |
	// 1 |        \ | 3
	//	  ----------	 
	const float size = 300;
	sVertexTex SkyboxMesh[24] =
	{
		// Front quad, NOTE: All quads face inward
		sVertexTex(-size, -size,  size,  0.0f, 1.0f ),
		sVertexTex(-size,  size,  size,  0.0f, 0.0f ),
		sVertexTex( size, -size,  size,  1.0f, 1.0f ),
		sVertexTex( size,  size,  size,  1.0f, 0.0f ),

		// Back quad
		sVertexTex( size, -size, -size,  0.0f, 1.0f ),
		sVertexTex( size,  size, -size,  0.0f, 0.0f ),
		sVertexTex(-size, -size, -size,  1.0f, 1.0f ),
		sVertexTex(-size,  size, -size,  1.0f, 0.0f ),

		// Left quad
		sVertexTex(-size, -size, -size,  0.0f, 1.0f ),
		sVertexTex(-size,  size, -size,  0.0f, 0.0f ),
		sVertexTex(-size, -size,  size,  1.0f, 1.0f ),
		sVertexTex(-size,  size,  size,  1.0f, 0.0f ),

		// Right quad
		sVertexTex( size, -size,  size,  0.0f, 1.0f ),
		sVertexTex( size,  size,  size,  0.0f, 0.0f ),
		sVertexTex( size, -size, -size,  1.0f, 1.0f ),
		sVertexTex( size,  size, -size,  1.0f, 0.0f ),

		// Top quad
		sVertexTex(-size,  size,  size,  0.0f, 1.0f ),
		sVertexTex(-size,  size, -size,  0.0f, 0.0f ),
		sVertexTex( size,  size,  size,  1.0f, 1.0f ),
		sVertexTex( size,  size, -size,  1.0f, 0.0f ),

		// Bottom quad
		sVertexTex(-size, -size, -size,  0.0f, 1.0f ),
		sVertexTex(-size, -size,  size,  0.0f, 0.0f ),
		sVertexTex( size, -size, -size,  1.0f, 1.0f ),
		sVertexTex( size, -size,  size,  1.0f, 0.0f ),
	};

	const int vtxSize = 24;
	m_vtxBuff.Create(renderer, vtxSize, sizeof(sVertexTex), sVertexTex::FVF);

	sVertexTex *pv = (sVertexTex*)m_vtxBuff.Lock();
	memcpy( pv, SkyboxMesh, sizeof(sVertexTex) * 24 );
	m_vtxBuff.Unlock();

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void cSkyBox::Render(cRenderer &renderer, const Matrix44 &tm)
{
	renderer.GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	renderer.GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	DWORD lighting, fogEnable;
	renderer.GetDevice()->GetRenderState(D3DRS_LIGHTING, &lighting);
	renderer.GetDevice()->GetRenderState(D3DRS_FOGENABLE, &fogEnable);
	renderer.GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	renderer.GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
	renderer.GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	renderer.GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


	//mat matView, matViewSave, matWorld;
	Matrix44 matView, matViewSave, matWorld;
	renderer.GetDevice()->GetTransform(D3DTS_VIEW, (D3DXMATRIX*)&matViewSave);
	matView = matViewSave;
	matView._41 = 0.0f; matView._42 = -0.4f; matView._43 = 0.0f;
	renderer.GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&matView);
	// Set a default world matrix
	renderer.GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&tm);

	// render
	m_vtxBuff.Bind(renderer);
	for (int i = 0 ; i < MAX_FACE; i++)
	{
		//GetDevice()->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP);
		//GetDevice()->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP);

		m_textures[ i].Bind(renderer, 0);
		renderer.GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}

	renderer.GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&matViewSave);


	renderer.GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	renderer.GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	renderer.GetDevice()->SetRenderState(D3DRS_LIGHTING, lighting);
	renderer.GetDevice()->SetRenderState(D3DRS_FOGENABLE, fogEnable);
}

