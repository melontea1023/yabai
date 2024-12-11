#include    "main.h"
#include    "Application.h"
#include <iostream>

//=======================================
//エントリーポイント
//=======================================
int main(void)
{
#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//defined(DEBUG) || defined(_DEBUG)

    std::cout << "こん";

    // アプリケーション実行
    Application app(SCREEN_WIDTH,SCREEN_HEIGHT);//SCREEN_WIDTH,SCREEN_HEIGHTを送っている
    app.Run();

    return 0;
}