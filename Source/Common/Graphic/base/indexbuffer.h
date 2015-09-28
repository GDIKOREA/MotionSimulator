// �ε��� ���� Ŭ����
// LPDIRECT3DINDEXBUFFER9 �������̽��� ���� ���� ���� �������.
#pragma once


namespace graphic
{
	class cRenderer;

	class cIndexBuffer
	{
	public:
		cIndexBuffer();
		virtual ~cIndexBuffer();

		bool Create(cRenderer &renderer, int faceCount);
		void* Lock();
		void Unlock();
		void Bind(cRenderer &renderer) const;
		int GetFaceCount() const;
		void Clear();

		void Set(cRenderer &renderer, cIndexBuffer &rhs);
		//cIndexBuffer& operator=(cIndexBuffer &rhs);


	private:
		LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; // �ε��� ����
		int m_faceCount;
	};


	inline int cIndexBuffer::GetFaceCount() const { return m_faceCount; }
}
