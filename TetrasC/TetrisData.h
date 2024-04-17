#pragma once

#include "DxLib.h"



#define BOARD_W     10
#define BOARD_H     44
#define NEXT_MAX    5

/// これは全体のミノ順なので7の倍数で指定してください。
#define BAG_MAX     1400

typedef enum
{
    N,
    I,
    J,
    L,
    O,
    S,
    T,
    Z,
    G,
    C
} MINO_TYPE;


typedef struct
{
    MINO_TYPE current;
    POINT pos;
    int r;
    int prevSrs;
    BOOL prevSpin;
    BOOL btb;
    int ren;
    int delay;
} CONTROL;



typedef struct
{
    int deleteLine;
    int sendLine;
    int Tsm;
    int Tsmd;
    int Tss;
    int Tsd;
    int Tst;
    int Tetris;
    int Pc;
    int Delay;
} RESULT;

/// <summary>
/// テトリスデータ構造体です。
/// 一人用のすべてのデータを持ちます。
/// </summary>
typedef struct
{
    MINO_TYPE board[BOARD_W * BOARD_H];
    MINO_TYPE next[NEXT_MAX];
    BOOL holdEnable;
    MINO_TYPE hold;

    CONTROL control;
    CONTROL prevControl;

    RESULT prevResult;
    RESULT result;
    
    int bagTarget;

} TETRIS_DATA;








/// <summary>
/// テトリスデータを初期化します。
/// カレントも取り出してゲーム可能な状態にします。
/// </summary>
/// <param name="tet"></param>
/// <returns></returns>
extern void newTetrisData(TETRIS_DATA* tet);

/// <summary>
/// 新しいミノを取り出します。
/// </summary>
/// <param name="tet"></param>
/// <returns></returns>
extern BOOL newMino(TETRIS_DATA* tet);

/// <summary>
/// 設置後の処理です。
/// </summary>
/// <param name="tet"></param>
/// <returns></returns>
extern void finishDrop(TETRIS_DATA* tet);

/// <summary>
/// ホールドを取り出す動きです。
/// </summary>
extern BOOL Hold(TETRIS_DATA* tet);

extern BOOL MoveLeft(TETRIS_DATA* tet);
extern BOOL MoveRight(TETRIS_DATA* tet);
extern BOOL MoveDown(TETRIS_DATA* tet);
extern int MoveDD(TETRIS_DATA* tet);
extern BOOL MoveA(TETRIS_DATA* tet);
extern BOOL MoveB(TETRIS_DATA* tet);
extern BOOL MovePut(TETRIS_DATA* tet);

/// <summary>
/// 回転させたミノの位置を返します。
/// </summary>
/// <param name="current"></param>
/// <param name="currentPoint"></param>
/// <param name="r"></param>
/// <param name="cells"></param>
extern void GetCellPoints(MINO_TYPE current, POINT* currentPoint, int r, POINT* cells);

/// <summary>
/// ゴーストのセル位置を取得します。
/// </summary>
extern void getGhostCell(TETRIS_DATA* tet, POINT* cells);


/// <summary>
/// フィールドを正規化し、ライン消去を行います。
/// 0-19ライン目までしか消去しません。
/// </summary>
extern int CorrectBoard(MINO_TYPE* board, BOOL clear = TRUE);


/// <summary>
/// 埋まっているラインに消去用セルを設定します。
/// </summary>
extern int setFillBoardDeleteCell(MINO_TYPE* board);



/// ミノの形状を考慮した開始座標を取得します
extern BOOL GetMinoPisitionStart(MINO_TYPE* board, MINO_TYPE current, POINT* pos);



/// ミノとフィールドが衝突しているかどうかを返します。
extern BOOL CheckCollision(MINO_TYPE* board, POINT* cellPoints);


/// <summary>
/// ハードドロップした場所を取得します。
/// </summary>
extern int getHarddropPos(TETRIS_DATA* tet, POINT* pos);
extern int getHarddropPos(MINO_TYPE* board, MINO_TYPE current, int r, POINT* pos);

/// <summary>
/// ボードに置きます。
/// </summary>
/// <param name="board"></param>
/// <param name="current"></param>
/// <param name="r"></param>
/// <param name="pos"></param>
extern void putBoard(MINO_TYPE* board, MINO_TYPE current, int r, POINT* pos);



/// <summary>
/// TSpinかどうかの判定を行います。
/// 1はTSpin、2はTSpinMiniです
/// </summary>
/// <param name="board"></param>
/// <param name="current"></param>
/// <param name="currentPoint"></param>
/// <param name="r"></param>
/// <param name="spin"></param>
/// <param name="srs"></param>
/// <returns></returns>
extern int CheckTSpin(MINO_TYPE* board, MINO_TYPE current, POINT* currentPoint, int r, bool spin, int srs);


/// スーパーローテーションシステムを実装します。返り値はSRSパターンです
extern int GetSuperRotateMinoPoint(MINO_TYPE* board, MINO_TYPE current, POINT* currentPoint, int r, int nextR);

