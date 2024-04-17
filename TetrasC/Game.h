#pragma once

#include "DxLib.h"

/// <summary>
/// ゲームのシーケンスです。
/// </summary>
typedef enum
{
    GAME_INIT = 0,
    GAME_MENU,
    GAME_CONFIG,
    GAME_PLAY,
    GAME_RESULT,
    GAME_CONTINUE,
    GAME_END,
}GAME_SCENE;

/// <summary>
/// ゲーム内の更新データです。
/// 場面にかかわらず更新したいデータを設定できます。
/// </summary>
extern void updateGame(void);

/// <summary>
/// ゲームの本体です。
/// </summary>
/// <returns>ゲームの継続</returns>
extern BOOL Game(void);

