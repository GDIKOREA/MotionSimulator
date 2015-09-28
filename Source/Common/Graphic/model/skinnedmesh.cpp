
#include "stdafx.h"
#include "skinnedmesh.h"

using namespace graphic;


cSkinnedMesh::cSkinnedMesh(cRenderer &renderer, const int id, vector<Matrix44> *palette, const sRawMesh &raw) :
	cMesh(renderer, id, raw)
,	m_rawMesh(raw)
,	m_palette(palette)
,	m_skinnMeshBuffer(NULL)
{

	// ���̴���Ű���� ���ϴ� ��Ȳ�� ���� ����Ʈ���� ��Ű���� 
	// �ϱ� ���ؼ� ���۸� �������� �����Ѵ�.
	if (m_palette && (64 < m_palette->size()))
	{
		m_skinnMeshBuffer = new cMeshBuffer(renderer, raw);
		SetMeshBuffer(m_skinnMeshBuffer);
	}

}

cSkinnedMesh::~cSkinnedMesh()
{
	SAFE_DELETE(m_skinnMeshBuffer);
}


void cSkinnedMesh::Render(cRenderer &renderer, const Matrix44 &parentTm)
{
	if (m_shader && m_palette && (m_palette->size() < 64))
	{
		ApplyPaletteShader(*m_shader);
		cMesh::RenderShader(renderer, *m_shader, parentTm);
	}
	else
	{
		ApplyPalette(renderer);
		cMesh::Render(renderer, parentTm);
	}
}

void cSkinnedMesh::RenderShader(cRenderer &renderer, cShader &shader, const Matrix44 &parentTm)
{
	ApplyPaletteShader(shader);
	cMesh::RenderShader(renderer, shader, parentTm);
}


void cSkinnedMesh::RenderShadow(cRenderer &renderer, cShader &shader, const Matrix44 &parentTm)
{
	ApplyPaletteShader(shader);
	cMesh::RenderShadow(renderer, shader, parentTm);
}


// �ȷ�Ʈ ����.
void cSkinnedMesh::ApplyPalette(cRenderer &renderer)
{
	RET(!m_palette);

	// ����Ʈ���� ��Ű���� �� �������� �Ҵ�� ���۰� ���ٸ� ���� �����Ѵ�.
	if (!m_skinnMeshBuffer)
	{
		m_skinnMeshBuffer = new cMeshBuffer(renderer, m_rawMesh);
		SetMeshBuffer(m_skinnMeshBuffer);
	}


	cVertexBuffer &vtxBuffer = m_skinnMeshBuffer->GetVertexBuffer();
	const int pos_offset = vtxBuffer.GetOffset(D3DDECLUSAGE_POSITION);
	const int normal_offset = vtxBuffer.GetOffset(D3DDECLUSAGE_NORMAL);

	if (BYTE *vertices = (BYTE*)vtxBuffer.Lock())
	{
		BOOST_FOREACH (const sVertexWeight &weight, m_rawMesh.weights)
		{
			const int vtxIdx = weight.vtxIdx;
			Vector3 p(0,0,0);
			Vector3 n(0,0,0);

			switch (weight.size)
			{
			case 5:
				p += (m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ weight.w[ 4].bone]) * weight.w[ 4].weight;
				n += m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ weight.w[ 4].bone]) * weight.w[ 4].weight;
			case 4:
				p += (m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ weight.w[ 3].bone]) * weight.w[ 3].weight;
				n += m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ weight.w[ 3].bone]) * weight.w[ 3].weight;
			case 3:
				p += (m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ weight.w[ 2].bone]) * weight.w[ 2].weight;
				n += m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ weight.w[ 2].bone]) * weight.w[ 2].weight;
			case 2: 
				p += (m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ weight.w[ 1].bone]) * weight.w[ 1].weight;
				n += m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ weight.w[ 1].bone]) * weight.w[ 1].weight;
			case 1: 
				p += (m_rawMesh.vertices[ vtxIdx] * (*m_palette)[ weight.w[ 0].bone]) * weight.w[ 0].weight;
				n += m_rawMesh.normals[ vtxIdx].MultiplyNormal( (*m_palette)[ weight.w[ 0].bone]) * weight.w[ 0].weight;
			}

			Vector3 *pv = (Vector3*)(vertices + (vtxBuffer.GetSizeOfVertex() * vtxIdx) + pos_offset);
			Vector3 *pn = (Vector3*)(vertices + (vtxBuffer.GetSizeOfVertex() * vtxIdx) + normal_offset);
			*pv = p;
			*pn = n;
		}
	}

	vtxBuffer.Unlock();
}


// �ȷ�Ʈ ����.
void cSkinnedMesh::ApplyPaletteShader(cShader &shader)
{
	RET(!m_palette);

	const int paletteSize = min(64, m_palette->size());
	shader.SetMatrixArray("mPalette", (Matrix44*)&(*m_palette)[0], paletteSize);
}
