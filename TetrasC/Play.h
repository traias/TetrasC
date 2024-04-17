#pragma once

#include "DxLib.h"
#include "Game.h"


/// <summary>
/// ゲームプレイの初期化
/// </summary>
/// <returns>成功</returns>
extern BOOL PlayInit(void);

/// <summary>
/// ゲームプレイの本体
/// </summary>
/// <returns>継続</returns>
extern BOOL Play(void);

/// <summary>
/// ゲームプレイの解放
/// </summary>
/// <returns>次のシーケンス</returns>
extern GAME_SCENE PlayRelease(void);


