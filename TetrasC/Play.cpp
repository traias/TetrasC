#include "DxLib.h"
#include "Common.h"
#include "Game.h"
#include "Key.h"
#include "Tetris.h"


/// <summary>
/// �Q�[���v���C����
/// </summary>
/// <returns></returns>
BOOL PlayMain(void);

/// <summary>
/// �Q�[���v���C�`��
/// </summary>
void PlayDraw(void);


/// <summary>
/// �Q�[���v���C�̏�����
/// </summary>
/// <returns>����</returns>
BOOL PlayInit(void)
{
    TetrisResourceInit();
    TetrisStatusInit();
    return TRUE;
}

/// <summary>
/// �Q�[���v���C�̖{��
/// </summary>
/// <returns>�p��</returns>
BOOL Play(void)
{
    BOOL result = PlayMain();
    PlayDraw();

    return result;
}

/// <summary>
/// �Q�[���v���C�̉��
/// </summary>
/// <returns>���̃V�[�P���X</returns>
GAME_SCENE PlayRelease(void)
{
    TetrisRelease();
    return GAME_END;
}





/// <summary>
/// �Q�[���v���C����
/// </summary>
/// <returns></returns>
BOOL PlayMain(void)
{
    return TetrisPlay();
}

/// <summary>
/// �Q�[���v���C�`��
/// </summary>
void PlayDraw(void)
{
    TetrisDraw();
}




