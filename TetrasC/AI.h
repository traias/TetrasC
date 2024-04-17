#pragma once
#include "DxLib.h"
#include "TetrisData.h"



/// <summary>
/// AI�ӎv����̏��������s���܂��B
/// </summary>
extern void AiInit(void);

/// <summary>
/// AI�ӎv����̉�����s���܂��B
/// </summary>
extern void AiRelease(void);

/// <summary>
/// Ai�̈ӎv������J�n���܂��B
/// </summary>
extern void AiDecision(TETRIS_DATA* tet);

/// <summary>
/// ���݂̈ӎv������~���܂��B
/// </summary>
extern void AiAbort(void);

/// <summary>
/// ���݂̌�����擾���܂��B
/// </summary>
/// <returns></returns>
extern char* AiGetSolution(void);

/// <summary>
/// ���݂̌��肳�ꂽ�f�[�^���擾���܂��B
/// </summary>
/// <returns></returns>
extern void AiGetSolutionTetris(TETRIS_DATA* solution);

