#pragma once

#include "DxLib.h"
#include "TetrisData.h"


/// <summary>
/// テトリスに使用するリソースをすべて初期化します。
/// </summary>
/// <returns></returns>
extern BOOL TetrisResourceInit(void);

/// <summary>
/// テトリスに使用するステータスをすべて初期化します。
/// </summary>
/// <returns></returns>
extern void TetrisStatusInit(void);

/// <summary>
/// テトリスを動かします。
/// </summary>
/// <returns></returns>
extern BOOL TetrisPlay(void);

/// <summary>
/// テトリスを描画します。
/// </summary>
/// <returns></returns>
extern BOOL TetrisDraw(void);

/// <summary>
/// テトリスリソースデータをすべて解放します。
/// </summary>
/// <returns></returns>
extern BOOL TetrisRelease(void);


/// <summary>
/// BAGからミノを取得する
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
extern MINO_TYPE getBagMino(int index);
