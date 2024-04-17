#include "DxLib.h"
#include "Common.h"
#include "Game.h"
#include "Key.h"
#include "Tetris.h"


/// <summary>
/// ゲームプレイ処理
/// </summary>
/// <returns></returns>
BOOL PlayMain(void);

/// <summary>
/// ゲームプレイ描画
/// </summary>
void PlayDraw(void);


/// <summary>
/// ゲームプレイの初期化
/// </summary>
/// <returns>成功</returns>
BOOL PlayInit(void)
{
    TetrisResourceInit();
    TetrisStatusInit();
    return TRUE;
}

/// <summary>
/// ゲームプレイの本体
/// </summary>
/// <returns>継続</returns>
BOOL Play(void)
{
    BOOL result = PlayMain();
    PlayDraw();

    return result;
}

/// <summary>
/// ゲームプレイの解放
/// </summary>
/// <returns>次のシーケンス</returns>
GAME_SCENE PlayRelease(void)
{
    TetrisRelease();
    return GAME_END;
}





/// <summary>
/// ゲームプレイ処理
/// </summary>
/// <returns></returns>
BOOL PlayMain(void)
{
    return TetrisPlay();
}

/// <summary>
/// ゲームプレイ描画
/// </summary>
void PlayDraw(void)
{
    TetrisDraw();
}




