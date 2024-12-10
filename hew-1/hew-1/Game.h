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
	Game(); // �R���X�g���N�^
	~Game(); // �f�X�g���N�^

	static void Init(); // ������
	static void Update(); // �X�V
	static void Draw(); // �`��
	static void Uninit(); // �I������

	static Game* GetInstance();

	void ChangeScene(SceneName sName); // �V�[����ύX
};
