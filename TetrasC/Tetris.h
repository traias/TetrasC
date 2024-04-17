#pragma once

#include "DxLib.h"
#include "TetrisData.h"


/// <summary>
/// �e�g���X�Ɏg�p���郊�\�[�X�����ׂď��������܂��B
/// </summary>
/// <returns></returns>
extern BOOL TetrisResourceInit(void);

/// <summary>
/// �e�g���X�Ɏg�p����X�e�[�^�X�����ׂď��������܂��B
/// </summary>
/// <returns></returns>
extern void TetrisStatusInit(void);

/// <summary>
/// �e�g���X�𓮂����܂��B
/// </summary>
/// <returns></returns>
extern BOOL TetrisPlay(void);

/// <summary>
/// �e�g���X��`�悵�܂��B
/// </summary>
/// <returns></returns>
extern BOOL TetrisDraw(void);

/// <summary>
/// �e�g���X���\�[�X�f�[�^�����ׂĉ�����܂��B
/// </summary>
/// <returns></returns>
extern BOOL TetrisRelease(void);


/// <summary>
/// BAG����~�m���擾����
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
extern MINO_TYPE getBagMino(int index);
