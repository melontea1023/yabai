#pragma once
#include "Object.h"
#include "MeshRenderer.h"
#include "Texture.h"
#include "Material.h"

class GolfBall :public Object
{
private:
	//���x
	DirectX::SimpleMath::Vector3 m_Velocity = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	//�����x
	DirectX::SimpleMath::Vector3 m_Acceralation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	//�p���x
	DirectX::SimpleMath::Vector3 m_AngularVelocity = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

	// �`��ׂ̈̏��i���b�V���Ɋւ����j
	MeshRenderer m_MeshRenderer; // ���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�E�C���f�b�N�X��

	// �`��ׂ̈̏��i�����ڂɊւ�镔���j
	std::vector<std::unique_ptr<Material>> m_Materiales;
	std::vector<SUBSET> m_subsets;
	std::vector<std::unique_ptr<Texture>> m_Textures; // �e�N�X�`��
	
	int m_State = 0;
	int m_StopCount = 0;
public:
	GolfBall(Camera* cam);//�R���X�g���N�^
	~GolfBall();//�f�X�g���N�^

	void Init();
	void Update();
	void Draw();
	void Uninit();

	void SetState(int s);
	int GetState();

	void Shot(DirectX::SimpleMath::Vector3 v);


	//void ShootInDirection(const DirectX::SimpleMath::Vector3& direction, float speed); // �ł��o�����\�b�h
};

