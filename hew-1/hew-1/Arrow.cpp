#include <memory>
#include "Arrow.h"
#include "StaticMesh.h"
#include "utility.h"
#include "Game.h"
#include "GolfBall.h"

using namespace std;
using namespace DirectX::SimpleMath;

// �R���X�g���N�^
Arrow::Arrow(Camera* cam) :Object(cam)
{

}

// �f�X�g���N�^
Arrow::~Arrow()
{

}

//=======================================
// ����������
//=======================================
void Arrow::Init()
{
	// ���b�V���ǂݍ���
	StaticMesh staticmesh;

	// 3D���f���f�[�^
	std::u8string modelFile = u8"assets/model/arrow/arrow.fbx";

	// �e�N�X�`���f�B���N�g��
	std::string texDirectory = "assets/model/arrow";

	// Mesh��ǂݍ���
	std::string tmpStr1(reinterpret_cast<const char*>(modelFile.c_str()), modelFile.size());
	staticmesh.Load(tmpStr1, texDirectory);

	m_MeshRenderer.Init(staticmesh);

	// �V�F�[�_�I�u�W�F�N�g����
	m_Shader.Create("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");

	// �T�u�Z�b�g���擾
	m_subsets = staticmesh.GetSubsets();

	// �e�N�X�`�����擾
	m_Textures = staticmesh.GetTextures();

	// �}�e���A�����擾
	vector<MATERIAL> materials = staticmesh.GetMaterials();

	// �}�e���A���������[�v
	for (int i = 0; i < materials.size(); i++)
	{
		// �}�e���A���I�u�W�F�N�g����
		std::unique_ptr<Material> m = std::make_unique<Material>();

		// �}�e���A�������Z�b�g
		m->Create(materials[i]);

		// �}�e���A���I�u�W�F�N�g��z��ɒǉ�
		m_Materiales.push_back(std::move(m));
	}

	// ���f���ɂ���ăX�P�[���𒲐�
	m_Scale.x = 3;
	m_Scale.y = 3;
	m_Scale.z = 3;

	m_State = 1;
}

//=======================================
// �X�V����
//=======================================
void Arrow::Update()
{
	if (m_State == 0)return; // ��\���Ȃ�return

	// �S���t�{�[���̈ʒu���擾
	vector<GolfBall*> ballpt = Game::GetInstance()->GetObjects<GolfBall>();
	if (ballpt.size() > 0)
	{
		// ���̈ʒu���X�V
		m_Position = ballpt[0]->GetPosition();
	}

	// �����I���Ȃ�
	if (m_State == 1)
	{
		m_Scale.z = 3; // �������Œ�

		// ��������]������
		m_Rotation.y += 0.03f;

		if (m_Rotation.y > 6.28)m_Rotation.y = 0;
	}
	// �p���[�I���Ȃ�
	else if (m_State == 2)
	{
		// �傫����ύX������
		m_Scale.z += 0.04f;
		if (m_Scale.z > 4)m_Scale.z = 1;
	}
}

//=======================================
// �`�揈��
//=======================================
void Arrow::Draw()
{
	if (m_State == 0)return; // ��\���Ȃ�return

	// SRT���쐬
	Matrix r = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
	Matrix t = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	Matrix s = Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);

	Matrix worldmtx;
	worldmtx = s * r * t;
	Renderer::SetWorldMatrix(&worldmtx); // GPU�ɃZ�b�g

	m_Shader.SetGPU();

	// �C���f�b�N�X�o�b�t�@�E���_�o�b�t�@���Z�b�g
	m_MeshRenderer.BeforeDraw();

	// �J�����̐ݒ���w��
	m_Camera->SetCamera(0);

	//�}�e���A���������[�v 
	for (int i = 0; i < m_subsets.size(); i++)
	{
		// �}�e���A�����Z�b�g(�T�u�Z�b�g���̒��ɂ���}�e���A���C���f�b�N�X���g�p)
		m_Materiales[m_subsets[i].MaterialIdx]->SetGPU();

		if (m_Materiales[m_subsets[i].MaterialIdx]->isTextureEnable())
		{
			m_Textures[m_subsets[i].MaterialIdx]->SetGPU();
		}

		m_MeshRenderer.DrawSubset(
			m_subsets[i].IndexNum, // �`�悷��C���f�b�N�X��
			m_subsets[i].IndexBase, // �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu	
			m_subsets[i].VertexBase); // ���_�o�b�t�@�̍ŏ�����g�p
	}
}

//=======================================
// �I������
//=======================================
void Arrow::Uninit()
{

}


//��Ԃ̐ݒ�
void Arrow::SetState(int s)
{
	m_State = s;
}

// ���̃x�N�g�����擾
Vector3 Arrow::GetVector()
{
	//���̏�����Ԃ̌���
	Vector3 res = { 0, 0, -1 };

	// �x�N�g������]
	Matrix r = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
	res = Vector3::Transform(res, r);

	//���̒���(�p���[)���|����
	res *= m_Scale.z;

	return res;
}
