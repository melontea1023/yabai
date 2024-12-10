#pragma once
#pragma once
#include <SimpleMath.h>
#include"direct3d.h"

class Object {
protected:
	DirectX::SimpleMath::Vector2 m_Position = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
	DirectX::SimpleMath::Vector2 m_Rotation = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
	DirectX::SimpleMath::Vector2 m_Scale = DirectX::SimpleMath::Vector2(1.0f, 1.0f);

public:


	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;
};