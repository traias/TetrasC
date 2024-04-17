#pragma once

///�@�L�[�̓��͒萔�ł��B
#define INPUT_U         (1 << 0)
#define INPUT_D         (1 << 1)
#define INPUT_L         (1 << 2)
#define INPUT_R         (1 << 3)
#define INPUT_A         (1 << 4)
#define INPUT_B         (1 << 5)
#define INPUT_H         (1 << 6)
#define INPUT_ESC       (1 << 7)
#define INPUT_ENTER     (1 << 8)


/// <summary>
/// �L�[�{�[�h���͂��Ǘ����܂��B
/// </summary>
extern void updateKey(void);

/// <summary>
/// �L�[���͂��g���K�Ƃ��Ď擾���܂��B
/// </summary>
/// <returns>�g���K�L�[</returns>
extern int getKeyTrigger(void);

/// <summary>
/// �L�[���͂���ԂƂ��Ď擾���܂��B
/// </summary>
/// <returns>�L�[���</returns>
extern int getKeyState(void);
