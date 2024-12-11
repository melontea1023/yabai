#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Object.h"
#include "Material.h"
//-----------------------------------------------------------------------------
//TestPlane�N���X
//-----------------------------------------------------------------------------
class Ground : public Object {
private:
	//// SRT���i�p�����j
	//DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	//DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	//DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// �`��ׂ̈̏��i���b�V���Ɋւ����j
	IndexBuffer	 m_IndexBuffer; // �C���f�b�N�X�o�b�t�@
	VertexBuffer<VERTEX_3D>	m_VertexBuffer; // ���_�o�b�t�@

	Texture m_Texture;//�e�N�X�`��
	std::unique_ptr<Material>m_Material;

	int m_SizeX;
	int m_SizeZ;
	std::vector<VERTEX_3D>m_Vertices;//���_���

public:

	Ground(Camera* cam);//�R���X�g���N�^
	~Ground();//�f�X�g���N�^


	void Init();
	void Draw();
	void Update();
	void Uninit();

	std::vector<VERTEX_3D>GetVertices();
};