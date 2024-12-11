#include <memory>
#include "GolfBall.h"
#include "StaticMesh.h"
#include "utility.h"
#include "input.h"
#include "Collision.h"
#include "Game.h"
#include "Ground.h"

using namespace std;
using namespace DirectX::SimpleMath;
//�R���X�g
GolfBall::GolfBall(Camera* cam) : Object(cam) {

}


GolfBall::~GolfBall() {

}


void GolfBall::Init()
{
	// ���b�V���ǂݍ���
	StaticMesh staticmesh;

	//3D���f���f�[�^
	std::u8string modelFile = u8"assets/model/golfball2/golf_ball.obj";

	//�e�N�X�`���f�B���N�g��
	std::string texDirectory = "assets/model/golfball2";

	//Mesh��ǂݍ���
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
	std::vector<MATERIAL> materials = staticmesh.GetMaterials();

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

	//���f���ɂ���ăX�P�[���𒲐�

	//�ŏ��ɑ��x��^����
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);
	m_Velocity = Vector3(0.0f, 0.0f, 0.0f);//���x
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f); // ��]������
	m_AngularVelocity = Vector3(0.0f, 0.0f, 0.0f); // �p���x������

}

void GolfBall::Update()
{

	if (m_State != 0)return;
	Vector3 oldPos = m_Position;//1�t���[���O�̋L��


	//���x���O�ɋ߂Â������~
	if (m_Velocity.LengthSquared() < 0.03f)
	{
		m_StopCount++;
	}
	else {
		m_StopCount = 0;
		//�����x�i1�t���[��������
		float decelerationPower = 0.1f;
		Vector3 deceleration = -m_Velocity;//���x�̋t�x�N�g���̌v�Z
		deceleration.Normalize();//�x�N�g���̐��K��
		m_Acceralation = deceleration * decelerationPower;
		//�����x�𑬓x�ɉ��Z
		m_Velocity += m_Acceralation;


	}
	//�P�O�t���[���A���łقƂ�Ǔ����Ă��Ȃ��Ȃ�Î~���
	if (m_StopCount > 10)
	{
		m_Velocity = Vector3(0.0f, 0.0f, 0.0f);
		m_State = 1;
	}
	//�d��
	const float gravity = 0.24f;
	m_Velocity.y -= gravity;
	//���x�����W�Ɍv�Z
	m_Position += m_Velocity;

	float radius = 1.0f;//�{�[�����f���̒��a

	//Ground�̒��_�f�[�^�擾
	vector<Ground*>grounds = Game::GetInstance()->GetObjects<Ground>();
	vector<VERTEX_3D> vertices;
	for (auto& g : grounds) {
		vector<VERTEX_3D> vecs = g->GetVertices();
		for (auto& v : vecs)
		{
			vertices.emplace_back(v);
		}
	}

	float moveDistance = 9999;//�ړ�����
	Vector3 contactPoint;//�ڐG�_
	Vector3 normal;

	//�����ƃ|���S���̓����蔻��
	bool senbunFg = false;
	for(int i = 0; i < vertices.size(); i += 3) {
		//�O�p�`�|���S��
		Collision::Polygon collisionPolygon = {
			vertices[i + 0].position,
			vertices[i + 1].position,
			vertices[i + 2].position };
		Vector3 cp;//�ڐG�_
		Collision::Segment collisionSegment = { oldPos,m_Position };
		if (Collision::CheckHit(collisionSegment, collisionPolygon, cp)) {
			float md = 0;
			Vector3 np = Collision::moveSphere(
				collisionSegment, radius, collisionPolygon, cp, md);
			if (moveDistance > md) {
				moveDistance = md;
				m_Position = np;
				contactPoint = cp;
				normal = Collision::GetNormal(collisionPolygon);
			}
			senbunFg = true;
		}
	}

	//���̂ƃ|���S���̓����蔻��
	if (!senbunFg) {
		for (int i = 0; i < vertices.size(); i += 3) {
			//�O�p�`�|���S��
			Collision::Polygon collsionPolygon = {
				vertices[i + 0].position,vertices[i + 1].position,vertices[i + 2].position };

			Vector3 cp;
			Collision::Sphere collisionSpher = { m_Position,radius };
			if (Collision::CheckHit(collisionSpher, collsionPolygon, cp)) {
				float md = 0;//�ڐG�_
				Vector3 np = Collision::moveSphere(collisionSpher, collsionPolygon, cp);
				md = (np - oldPos).Length();
				if (moveDistance > md) {
					moveDistance = md;
					m_Position = np;
					contactPoint = cp;
					normal = Collision::GetNormal(collsionPolygon);
				}
			}
		}
	}
	if (moveDistance != 9999)//�����������Ă�����
	{
		m_Velocity.y = -gravity;
		//�{�[���̑��x�x�N�g���̖@�����������Ɛڐ���������𕪉�
		float velocitynormal = Collision::Dot(m_Velocity, normal);
		Vector3 v1 = velocitynormal * normal;
		Vector3 v2 = m_Velocity - v1;
		//���ˌW��
		const float restitution = 1.0f;
		//���˃x�N�g�����v�Z
		Vector3 reflectedVelocity = v2 - restitution * v1;
		//�{�[���̑��x���X�V
		m_Velocity = reflectedVelocity;
	}
	if (m_Position.y < -100) {
		m_Position = Vector3(0.0f, 50.0f, 0.0f);
		m_Velocity = Vector3(0.0f, 0.0f, 0.0f);
	}
}

void GolfBall::Draw()
{
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
			m_subsets[i].IndexNum,		// �`�悷��C���f�b�N�X��
			m_subsets[i].IndexBase,		// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu	
			m_subsets[i].VertexBase);	// ���_�o�b�t�@�̍ŏ�����g�p
	}
}

void GolfBall::Uninit()
{

}

void GolfBall::SetState(int s) { m_State = s; }
int GolfBall::GetState() { return m_State; }

void GolfBall::Shot(Vector3 v) { m_Velocity = v; }
//void GolfBall::ShootInDirection(const DirectX::SimpleMath::Vector3& direction, float speed)
//{
//	if (!GroundFg) return; // �n�ʂɂ��Ă��Ȃ��ꍇ�͑ł��o���Ȃ�
//
//	DirectX::SimpleMath::Vector3 normalizedDirection = direction;
//	normalizedDirection.Normalize(); // �����x�N�g���𐳋K��
//	m_Velocity = normalizedDirection * speed; // ���x��ݒ�
//
//	GroundFg = false; // �󒆏�Ԃɂ���
//}
