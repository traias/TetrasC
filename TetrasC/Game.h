#pragma once

#include "DxLib.h"

/// <summary>
/// �Q�[���̃V�[�P���X�ł��B
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
/// �Q�[�����̍X�V�f�[�^�ł��B
/// ��ʂɂ�����炸�X�V�������f�[�^��ݒ�ł��܂��B
/// </summary>
extern void updateGame(void);

/// <summary>
/// �Q�[���̖{�̂ł��B
/// </summary>
/// <returns>�Q�[���̌p��</returns>
extern BOOL Game(void);

