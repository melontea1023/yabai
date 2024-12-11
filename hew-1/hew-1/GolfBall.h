#pragma once
#include "Object.h"
#include "MeshRenderer.h"
#include "Texture.h"
#include "Material.h"

class GolfBall :public Object
{
private:
	//速度
	DirectX::SimpleMath::Vector3 m_Velocity = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	//加速度
	DirectX::SimpleMath::Vector3 m_Acceralation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	//角速度
	DirectX::SimpleMath::Vector3 m_AngularVelocity = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

	// 描画の為の情報（メッシュに関わる情報）
	MeshRenderer m_MeshRenderer; // 頂点バッファ・インデックスバッファ・インデックス数

	// 描画の為の情報（見た目に関わる部分）
	std::vector<std::unique_ptr<Material>> m_Materiales;
	std::vector<SUBSET> m_subsets;
	std::vector<std::unique_ptr<Texture>> m_Textures; // テクスチャ
	
	int m_State = 0;
	int m_StopCount = 0;
public:
	GolfBall(Camera* cam);//コンストラクタ
	~GolfBall();//デストラクタ

	void Init();
	void Update();
	void Draw();
	void Uninit();

	void SetState(int s);
	int GetState();

	void Shot(DirectX::SimpleMath::Vector3 v);


	//void ShootInDirection(const DirectX::SimpleMath::Vector3& direction, float speed); // 打ち出しメソッド
};

