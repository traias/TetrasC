#pragma once

#include "TetrisData.h"
#include "DecisionEval.h"





typedef struct tag_DECISION_TETRIS
{
    // 有効無効
    BOOL enable;

    // テトリスデータです。
    TETRIS_DATA tetris;

    // 評価値です。
    EVAL eval;

    // 総合評価値です。
    double totalEval;

    // 設置情報のハッシュコードです。
    // [ミノ種:8bit][回転:4bit][[セルの座標y:8bit,x:4bit]*4]
    __int64 hash;

    // ボードのハッシュコードです。すべてビットで表現しています。
    __int64 boardHash[4];

    // 入力コマンドです。
    char command[32];

    // ハードドロップしたセル数です。
    int harddropCell;

    // 親のポインタです。親がいなけれnullです。
    tag_DECISION_TETRIS* parent;

    // 一番の親データです。
    // このデータは現在盤面の次になります。
    tag_DECISION_TETRIS* top;

    // 子供のデータです。複数あります。
    tag_DECISION_TETRIS* child;
    int childCount;

} DECISION_TETRIS;



/// <summary>
/// 意思決定システムの初期化を行います。
/// これはメモリの動的確保を処理中に行わないようにするためです。
/// </summary>
extern void DecisionSystemInit(void);

/// <summary>
/// 意思決定システムの解放を行います。
/// これを呼び出すと意思決定システムは使用不能になります。
/// </summary>
extern void DecisionSystemRelease(void);

/// <summary>
/// 意思決定を行います。
/// 事前に初期化を呼ぶ必要があります。
/// </summary>
/// <param name="tet"></param>
extern void Decision(TETRIS_DATA* tet);


/// <summary>
/// 意思決定の中断を行います。
/// </summary>
extern void decisionAbort();


/// <summary>
/// 意思決定されたコマンドを取得します。
/// </summary>
/// <returns></returns>
extern char* getDecisionCommand(void);

/// <summary>
/// 意思決定されたデータを返します。
/// </summary>
/// <param name="tetris"></param>
extern void getDecisionTetrisData(TETRIS_DATA* tetris);
