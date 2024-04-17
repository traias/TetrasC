#include "DxLib.h"
#include "Key.h"

/// <summary>
///  �O��̃L�[��Ԃł��B
/// </summary>
static char prevKeyState[256] = { 0 };

/// <summary>
///  ����̃L�[��Ԃł��B
/// </summary>
static char keyState[256] = { 0 };

/// <summary>
/// �L�[�{�[�h���͂��Ǘ����܂��B
/// </summary>
void updateKey(void)
{
    memcpy(prevKeyState, keyState, sizeof(char) * 256);

    // �L�[�{�[�h���͎��s
    if (GetHitKeyStateAll(keyState) == -1) { return; }
}

/// <summary>
/// �L�[���͂��g���K�Ƃ��Ď擾���܂��B
/// </summary>
/// <returns>�g���K�L�[</returns>
int getKeyTrigger(void)
{
    int trigger = 0;

    if ((keyState[KEY_INPUT_UP] != 0) && (prevKeyState[KEY_INPUT_UP] == 0)) { trigger |= INPUT_U; }
    if ((keyState[KEY_INPUT_DOWN] != 0) && (prevKeyState[KEY_INPUT_DOWN] == 0)) { trigger |= INPUT_D; }
    if ((keyState[KEY_INPUT_LEFT] != 0) && (prevKeyState[KEY_INPUT_LEFT] == 0)) { trigger |= INPUT_L; }
    if ((keyState[KEY_INPUT_RIGHT] != 0) && (prevKeyState[KEY_INPUT_RIGHT] == 0)) { trigger |= INPUT_R; }
    if ((keyState[KEY_INPUT_Z] != 0) && (prevKeyState[KEY_INPUT_Z] == 0)) { trigger |= INPUT_B; }
    if ((keyState[KEY_INPUT_X] != 0) && (prevKeyState[KEY_INPUT_X] == 0)) { trigger |= INPUT_A; }
    if ((keyState[KEY_INPUT_C] != 0) && (prevKeyState[KEY_INPUT_C] == 0)) { trigger |= INPUT_H; }
    if ((keyState[KEY_INPUT_ESCAPE] != 0) && (prevKeyState[KEY_INPUT_ESCAPE] == 0)) { trigger |= INPUT_ESC; }
    if ((keyState[KEY_INPUT_RETURN] != 0) && (prevKeyState[KEY_INPUT_RETURN] == 0)) { trigger |= INPUT_ENTER; }

    return trigger;
}

/// <summary>
/// �L�[���͂���ԂƂ��Ď擾���܂��B
/// </summary>
/// <returns>�L�[���</returns>
int getKeyState(void)
{
    int state = 0;

    if (keyState[KEY_INPUT_UP] != 0) { state |= INPUT_U; }
    if (keyState[KEY_INPUT_DOWN] != 0) { state |= INPUT_D; }
    if (keyState[KEY_INPUT_LEFT] != 0) { state |= INPUT_L; }
    if (keyState[KEY_INPUT_RIGHT] != 0) { state |= INPUT_R; }
    if (keyState[KEY_INPUT_Z] != 0) { state |= INPUT_B; }
    if (keyState[KEY_INPUT_X] != 0) { state |= INPUT_A; }
    if (keyState[KEY_INPUT_C] != 0) { state |= INPUT_H; }
    if (keyState[KEY_INPUT_ESCAPE] != 0) { state |= INPUT_ESC; }
    if (keyState[KEY_INPUT_RETURN] != 0) { state |= INPUT_ENTER; }

    return state;
}

