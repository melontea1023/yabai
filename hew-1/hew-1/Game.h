#pragma once
#include <iostream>
//#include "TitleScene.h"
//#include "ResultScene.h"
#include "Input.h"

enum SceneName {
	TITLE,
	RESULT
};

class Game
{
public:



private:
public:
	Game(); // コンストラクタ
	~Game(); // デストラクタ

	static void Init(); // 初期化
	static void Update(); // 更新
	static void Draw(); // 描画
	static void Uninit(); // 終了処理

	static Game* GetInstance();

	void ChangeScene(SceneName sName); // シーンを変更
};
