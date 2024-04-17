#pragma once

#include "DxLib.h"
#include "Game.h"

/// <summary>
/// メニューの初期化
/// </summary>
/// <returns>成功</returns>
extern BOOL MenuInit(void);

/// <summary>
/// メニューの本体
/// </summary>
/// <returns>継続</returns>
extern BOOL Menu(void);

/// <summary>
/// メニューの解放
/// </summary>
/// <returns>次のシーケンス</returns>
extern GAME_SCENE MenuRelease(void);

