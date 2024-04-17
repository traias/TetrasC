#include "DxLib.h"
#include "Common.h"
#include "Game.h"
#include "Key.h"

/// <summary>
/// ���j���[�I���̍���
/// </summary>
typedef enum
{
    MENU_INIT = 10,
    MENU_PLAY,
    MENU_CONFIG,
    MENU_END,
} MENU_SELECT;


/// <summary>
/// ���j���[�̏���
/// </summary>
/// <returns></returns>
BOOL MenuMain(void);

/// <summary>
/// ���j���[�̕`��
/// </summary>
void MenuDraw(void);

/// <summary>
/// ���j���[�I����Ԃ̕ύX�ł��B
/// </summary>
/// <param name="select">���݂̑I�����j���[</param>
/// <param name="increment">���啪�̈ړ��l</param>
/// <returns>�I�����j���[</returns>
MENU_SELECT changeMenuSelect(MENU_SELECT select, int increment);


/// <summary>
/// ���j���[�̏I���ł��B
/// </summary>
static int menuAbort = false;

/// <summary>
/// ���j���[�I������
/// </summary>
static MENU_SELECT Selector = MENU_INIT;


/// <summary>
/// ���j���[�̏�����
/// </summary>
/// <returns>����</returns>
BOOL MenuInit(void)
{
    Selector = MENU_PLAY;
    return TRUE;
}


/// <summary>
/// ���j���[�̖{��
/// </summary>
/// <returns>�p��</returns>
BOOL Menu(void)
{
    BOOL result = MenuMain();
    MenuDraw();

    return result;
}

/// <summary>
/// ���j���[�̉��
/// </summary>
/// <returns>���̃V�[�P���X</returns>
GAME_SCENE MenuRelease(void)
{
    switch (Selector)
    {
    case MENU_PLAY:
    case MENU_CONFIG:
        return GAME_PLAY;

    case MENU_END:
        return GAME_END;
    }

    return GAME_END;
}



/// <summary>
/// ���j���[�̏���
/// </summary>
/// <returns></returns>
BOOL MenuMain(void)
{
    int trigger = getKeyTrigger();

    if (trigger & (INPUT_U))
    {
        Selector = changeMenuSelect(Selector, -1);
    }
    if (trigger & (INPUT_D))
    {
        Selector = changeMenuSelect(Selector, 1);
    }

    if (trigger & (INPUT_ESC))
    {
        Selector = MENU_END;
        return FALSE;
    }

    if ((trigger & (INPUT_ENTER)) || (trigger & (INPUT_A)))
    {
        return FALSE;
    }

    return TRUE;
}

/// <summary>
/// ���j���[�I����Ԃ̕ύX�ł��B
/// </summary>
/// <param name="select">���݂̑I�����j���[</param>
/// <param name="increment">���啪�̈ړ��l</param>
/// <returns>�I�����j���[</returns>
MENU_SELECT changeMenuSelect(MENU_SELECT select, int increment)
{
    MENU_SELECT destSelect = select;

    switch (select)
    {
    case MENU_INIT:
        if (increment > 0) { destSelect = MENU_PLAY; }
        if (increment < 0) { destSelect = MENU_PLAY; }
        break;
    case MENU_PLAY:
        if (increment > 0) { destSelect = MENU_CONFIG; }
        if (increment < 0) { destSelect = MENU_END; }
        break;
    case MENU_CONFIG:
        if (increment > 0) { destSelect = MENU_END; }
        if (increment < 0) { destSelect = MENU_PLAY; }
        break;
    case MENU_END:
        if (increment > 0) { destSelect = MENU_PLAY; }
        if (increment < 0) { destSelect = MENU_CONFIG; }
        break;
    }

    return destSelect;
}

/// <summary>
/// ���j���[�̕`��
/// </summary>
void MenuDraw(void)
{
    unsigned int color = GetColor(0xFF, 0xFF, 0xFF);

    SetFontSize(40);
    DrawString(60, 40, "PLAY", color);
    DrawString(60, 80, "CONFIG", color);
    DrawString(60, 120, "EXIT", color);

    switch (Selector)
    {
    case MENU_PLAY:
        DrawString(20, 40, ">", color);
        break;
    case MENU_CONFIG:
        DrawString(20, 80, ">", color);
        break;
    case MENU_END:
        DrawString(20, 120, ">", color);
        break;
    }
}