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
//コンスト
GolfBall::GolfBall(Camera* cam) : Object(cam) {

}


GolfBall::~GolfBall() {

}


void GolfBall::Init()
{
	// メッシュ読み込み
	StaticMesh staticmesh;

	//3Dモデルデータ
	std::u8string modelFile = u8"assets/model/golfball2/golf_ball.obj";

	//テクスチャディレクトリ
	std::string texDirectory = "assets/model/golfball2";

	//Meshを読み込む
	std::string tmpStr1(reinterpret_cast<const char*>(modelFile.c_str()), modelFile.size());
	staticmesh.Load(tmpStr1, texDirectory);

	m_MeshRenderer.Init(staticmesh);

	// シェーダオブジェクト生成
	m_Shader.Create("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");

	// サブセット情報取得
	m_subsets = staticmesh.GetSubsets();

	// テクスチャ情報取得
	m_Textures = staticmesh.GetTextures();

	// マテリアル情報取得	
	std::vector<MATERIAL> materials = staticmesh.GetMaterials();

	// マテリアル数分ループ
	for (int i = 0; i < materials.size(); i++)
	{
		// マテリアルオブジェクト生成
		std::unique_ptr<Material> m = std::make_unique<Material>();

		// マテリアル情報をセット
		m->Create(materials[i]);

		// マテリアルオブジェクトを配列に追加
		m_Materiales.push_back(std::move(m));
	}

	//モデルによってスケールを調整

	//最初に速度を与える
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);
	m_Velocity = Vector3(0.0f, 0.0f, 0.0f);//速度
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f); // 回転初期化
	m_AngularVelocity = Vector3(0.0f, 0.0f, 0.0f); // 角速度初期化

}

void GolfBall::Update()
{

	if (m_State != 0)return;
	Vector3 oldPos = m_Position;//1フレーム前の記憶


	//速度が０に近づいたら停止
	if (m_Velocity.LengthSquared() < 0.03f)
	{
		m_StopCount++;
	}
	else {
		m_StopCount = 0;
		//減速度（1フレーム当たり
		float decelerationPower = 0.1f;
		Vector3 deceleration = -m_Velocity;//速度の逆ベクトルの計算
		deceleration.Normalize();//ベクトルの正規化
		m_Acceralation = deceleration * decelerationPower;
		//加速度を速度に加算
		m_Velocity += m_Acceralation;


	}
	//１０フレーム連続でほとんど動いていないなら静止状態
	if (m_StopCount > 10)
	{
		m_Velocity = Vector3(0.0f, 0.0f, 0.0f);
		m_State = 1;
	}
	//重力
	const float gravity = 0.24f;
	m_Velocity.y -= gravity;
	//速度を座標に計算
	m_Position += m_Velocity;

	float radius = 1.0f;//ボールモデルの直径

	//Groundの頂点データ取得
	vector<Ground*>grounds = Game::GetInstance()->GetObjects<Ground>();
	vector<VERTEX_3D> vertices;
	for (auto& g : grounds) {
		vector<VERTEX_3D> vecs = g->GetVertices();
		for (auto& v : vecs)
		{
			vertices.emplace_back(v);
		}
	}

	float moveDistance = 9999;//移動距離
	Vector3 contactPoint;//接触点
	Vector3 normal;

	//線分とポリゴンの当たり判定
	bool senbunFg = false;
	for(int i = 0; i < vertices.size(); i += 3) {
		//三角形ポリゴン
		Collision::Polygon collisionPolygon = {
			vertices[i + 0].position,
			vertices[i + 1].position,
			vertices[i + 2].position };
		Vector3 cp;//接触点
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

	//球体とポリゴンの当たり判定
	if (!senbunFg) {
		for (int i = 0; i < vertices.size(); i += 3) {
			//三角形ポリゴン
			Collision::Polygon collsionPolygon = {
				vertices[i + 0].position,vertices[i + 1].position,vertices[i + 2].position };

			Vector3 cp;
			Collision::Sphere collisionSpher = { m_Position,radius };
			if (Collision::CheckHit(collisionSpher, collsionPolygon, cp)) {
				float md = 0;//接触点
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
	if (moveDistance != 9999)//もし当たっていたら
	{
		m_Velocity.y = -gravity;
		//ボールの速度ベクトルの法線方向成分と接戦方向成分を分解
		float velocitynormal = Collision::Dot(m_Velocity, normal);
		Vector3 v1 = velocitynormal * normal;
		Vector3 v2 = m_Velocity - v1;
		//反射係数
		const float restitution = 1.0f;
		//反射ベクトルを計算
		Vector3 reflectedVelocity = v2 - restitution * v1;
		//ボールの速度を更新
		m_Velocity = reflectedVelocity;
	}
	if (m_Position.y < -100) {
		m_Position = Vector3(0.0f, 50.0f, 0.0f);
		m_Velocity = Vector3(0.0f, 0.0f, 0.0f);
	}
}

void GolfBall::Draw()
{
	// SRT情報作成
	Matrix r = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
	Matrix t = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	Matrix s = Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);

	Matrix worldmtx;
	worldmtx = s * r * t;
	Renderer::SetWorldMatrix(&worldmtx); // GPUにセット

	m_Shader.SetGPU();

	// インデックスバッファ・頂点バッファをセット
	m_MeshRenderer.BeforeDraw();

	m_Camera->SetCamera(0);

	//マテリアル数分ループ 
	for (int i = 0; i < m_subsets.size(); i++)
	{
		// マテリアルをセット(サブセット情報の中にあるマテリアルインデックスを使用)
		m_Materiales[m_subsets[i].MaterialIdx]->SetGPU();

		if (m_Materiales[m_subsets[i].MaterialIdx]->isTextureEnable())
		{
			m_Textures[m_subsets[i].MaterialIdx]->SetGPU();
		}

		m_MeshRenderer.DrawSubset(
			m_subsets[i].IndexNum,		// 描画するインデックス数
			m_subsets[i].IndexBase,		// 最初のインデックスバッファの位置	
			m_subsets[i].VertexBase);	// 頂点バッファの最初から使用
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
//	if (!GroundFg) return; // 地面についていない場合は打ち出さない
//
//	DirectX::SimpleMath::Vector3 normalizedDirection = direction;
//	normalizedDirection.Normalize(); // 方向ベクトルを正規化
//	m_Velocity = normalizedDirection * speed; // 速度を設定
//
//	GroundFg = false; // 空中状態にする
//}
