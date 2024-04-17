#pragma once
#include "DxLib.h"
#include "TetrisData.h"



/// <summary>
/// AI意思決定の初期化を行います。
/// </summary>
extern void AiInit(void);

/// <summary>
/// AI意思決定の解放を行います。
/// </summary>
extern void AiRelease(void);

/// <summary>
/// Aiの意思決定を開始します。
/// </summary>
extern void AiDecision(TETRIS_DATA* tet);

/// <summary>
/// 現在の意思決定を停止します。
/// </summary>
extern void AiAbort(void);

/// <summary>
/// 現在の決定を取得します。
/// </summary>
/// <returns></returns>
extern char* AiGetSolution(void);

/// <summary>
/// 現在の決定されたデータを取得します。
/// </summary>
/// <returns></returns>
extern void AiGetSolutionTetris(TETRIS_DATA* solution);

