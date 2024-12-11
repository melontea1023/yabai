#include "Renderer.h"
#include "Camera.h"
#include "Application.h"
#include "input.h"
#include "Game.h"
#include "GolfBall.h"

using namespace std;
using namespace DirectX::SimpleMath;

//=======================================
//初期化処理
//=======================================
void Camera::Init()
{
	m_Position = Vector3(0.0f, 20.0f, -100.0f);
	m_Target = Vector3(0.0f, 0.0f, 0.0f);//向いている方向
	m_CameraDirection = 3.14f;
}


//=======================================
//更新処理
//=======================================
void Camera::Update()
{
	//左右キーで入力
	if (Input::GetKeyPress(VK_LEFT)) {
		m_CameraDirection += 0.02f;
	}
	if (Input::GetKeyPress(VK_RIGHT)) {
		m_CameraDirection -= 0.02f;
	}
	//ゴルフボールの位置取得
	vector<GolfBall*>ballpt = Game::GetInstance()->GetObjects<GolfBall>();
	if (ballpt.size() > 0) {
		Vector3 ballPos = ballpt[0]->GetPosition();
		//ballPos = ballpt[1]->GetRotation();

		//カメラの位置を更新
		m_Position.x = ballPos.x + sin(m_CameraDirection) * 50;
		m_Position.y = ballPos.y + 20;
		m_Position.z = ballPos.z + cos(m_CameraDirection) * 50;
		//カメラの注視点を更新
		m_Target = ballPos;

		// ボールを打つ処理
		//if (Input::GetKeyPress(VK_G)) {
		//	Vector3 forward(sin(m_CameraDirection), 0.0f, cos(m_CameraDirection)); // カメラの向き
		//	m_GolfBall.ShootInDirection(forward, -1.0f); // ボールを速度10.0で打ち出す
	}
}


//=======================================
//描画処理
//=======================================
void Camera::Draw()
{

}


//=======================================
//終了処理
//=======================================
void Camera::Uninit()
{

}

void Camera::SetCamera(int mode) {
	if (mode == 0) {//3D
		// ビュー変換後列作成
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up); // 左手系にした　20230511 by suzuki.tomoki
		// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
		// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
		// このコードは確認テストのために残す
		// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

		Renderer::SetViewMatrix(&m_ViewMatrix);

		//プロジェクション行列の生成
		constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角
		float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
		
		float nearPlane = 1.0f;       // ニアクリップ　   一番近く    n~
		float farPlane = 1000.0f;      // ファークリップ　一番遠く　  ~n

		//プロジェクション行列の生成
		Matrix projectionMatrix;
		projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki
		// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
		// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
		// このコードは確認テストのために残す
		// projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

		Renderer::SetProjectionMatrix(&projectionMatrix);
	}
	else if (mode == 1) {//2D
		Vector3 pos = { 0.0f,0.0f, -10.0f };
		Vector3 tgt = { 0.0f,0.0f, 1.0f };
		Vector3 up = Vector3{ 0.0f,1.0f, 0.0f };
		m_ViewMatrix = DirectX::XMMatrixLookAtLH(pos, tgt, up);
		Renderer::SetViewMatrix(&m_ViewMatrix);

		//プロジェクション行列の生成
		float nearPlane = 1.0f;
		float farPlane = 1000.0f;
		Matrix projectionMatrix = DirectX::XMMatrixOrthographicLH(
			static_cast<float>(Application::GetWidth()),
			static_cast<float>(Application::GetHeight()),
			nearPlane,farPlane);
		
		projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
		Renderer::SetProjectionMatrix(&projectionMatrix);

	}
}

