#include "DxLib.h"
#include "Common.h"
#include "Game.h"
#include "Init.h"
#include "Menu.h"
#include "Play.h"
#include "Key.h"


/// <summary>
/// ゲーム内の更新データです。
/// 場面にかかわらず更新したいデータを設定できます。
/// </summary>
void updateGame(void)
{
    updateKey();
}

/// <summary>
/// ゲームの本体です。
/// </summary>
/// <returns>ゲームの継続</returns>
BOOL Game(void)
{
    static GAME_SCENE gameScene = GAME_INIT;
    static BOOL changeScene = TRUE;

    switch (gameScene)
    {
    case GAME_INIT:
        if (Init() == FALSE) { return FALSE; }
        gameScene = GAME_MENU;
        break;

    case GAME_MENU:
        if (changeScene == TRUE)
        {
            if (MenuInit() == FALSE) { return FALSE; }
            changeScene = FALSE;
        }
        if (Menu() == FALSE)
        {
            gameScene = MenuRelease();
            changeScene = TRUE;
        }
        break;

    case GAME_CONFIG:
        break;

    case GAME_PLAY:
        if (changeScene == TRUE)
        {
            if (PlayInit() == FALSE) { return FALSE; }
            changeScene = FALSE;
        }
        if (Play() == FALSE)
        {
            gameScene = PlayRelease();
            changeScene = TRUE;
        }
        break;

    case GAME_RESULT:
        break;

    case GAME_CONTINUE:
        break;

    case GAME_END:
        return FALSE;
    }

    return TRUE;
}

