#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Object.h"
#include "Material.h"
//-----------------------------------------------------------------------------
//TestPlaneクラス
//-----------------------------------------------------------------------------
class Ground : public Object {
private:
	//// SRT情報（姿勢情報）
	//DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	//DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	//DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// 描画の為の情報（メッシュに関わる情報）
	IndexBuffer	 m_IndexBuffer; // インデックスバッファ
	VertexBuffer<VERTEX_3D>	m_VertexBuffer; // 頂点バッファ

	Texture m_Texture;//テクスチャ
	std::unique_ptr<Material>m_Material;

	int m_SizeX;
	int m_SizeZ;
	std::vector<VERTEX_3D>m_Vertices;//頂点情報

public:

	Ground(Camera* cam);//コンストラクタ
	~Ground();//デストラクタ


	void Init();
	void Draw();
	void Update();
	void Uninit();

	std::vector<VERTEX_3D>GetVertices();
};