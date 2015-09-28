// �޽� �⺻ Ŭ����
//
// cMesh Ȯ��
// cMesh -> cRigidMesh
//			  -> cSkinnedMesh
// 
#pragma once


#include "node.h"

namespace graphic
{
	class cRenderer;

	class cMesh : public cNode
	{
	public:
		cMesh(cRenderer &renderer, const int id, const sRawMesh &rawMesh);
		cMesh(cRenderer &renderer, const int id, const sRawBone &rawBone);
		virtual ~cMesh();

		virtual bool Move(const float elapseTime) override;
		virtual void Render(cRenderer &renderer, const Matrix44 &parentTm) override;
		virtual void RenderShadow(cRenderer &renderer, const Matrix44 &viewProj,
			const Vector3 &lightPos, const Vector3 &lightDir, const Matrix44 &parentTm) override;

		const cBoundingBox* GetBoundingBox();
		cMeshBuffer* GetMeshBuffer();
		void SetMeshBuffer(cMeshBuffer *buffer);
		

	protected:
		void CreateMaterials(cRenderer &renderer, const sRawMesh &rawMesh);

		virtual void RenderShader(cRenderer &renderer, cShader &shader, const Matrix44 &parentTm);
		virtual void RenderShadow(cRenderer &renderer, cShader &shader, const Matrix44 &parentTm);


	protected:
		bool m_isSkinned;
		vector<cMaterial> m_mtrls;
		vector<cTexture*>m_colorMap;  // reference
		vector<cTexture*>m_normalMap;  // reference
		vector<cTexture*>m_specularMap;  // reference
		vector<cTexture*>m_selfIllumMap;  // reference
		cMeshBuffer *m_buffers; // reference (���� �޽��� ��� ����)

		// debug ��.
		bool m_isBoneMesh;
	};


	inline cMeshBuffer* cMesh::GetMeshBuffer() { return m_buffers; }
	inline void cMesh::SetMeshBuffer(cMeshBuffer *buffer) { m_buffers = buffer; }
}
