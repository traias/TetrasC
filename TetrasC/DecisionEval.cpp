#include <math.h>
#include "DxLib.h"
#include "TetrisData.h"
#include "DecisionEval.h"


/// <summary>
/// 評価値の説明名称です。
/// </summary>
const char* EvalName[20] =
{
    "高さ",
    "高さH",
    "高さT",
    "屋根",
    "屋根SQ",
    "井戸",
    "凸凹",
    "凸凹SQ",
    "洞窟",
    "洞窟SQ",
    "閉塞",
    "閉塞SQ",
    "地形",
    "HoldT",
    "HoldI",
    "HoldO",
    "PC",
    "Delay",
    "Btb",
    "火力",
};



/// 関数プロトタイプ宣言です

/// 高さを取得します
void getHeight(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1, double* out2, double* out3);

/// 屋根を取得します
void getRoof(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1, double* out2);

/// 井戸を取得します
/// 返り値は井戸の列です。井戸は完成品を調べます
int getWell(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, int* depth, double* out1);

/// 表面の凸凹を取得します
/// 一番深い井戸は無視します
void getSurface(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, int wellColumn, int depth, double* out1, double* out2);

// 洞窟数を取得
void getCave(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1, double* out2);

// 閉塞数を取得
void getClosedArea(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1, double* out2);

/// 閉塞エリアを求めます。
int setClosedArea(MINO_TYPE* board, int* closedBoard, int x, int y, int kind, int closeCell);

/// 火力地形を取得します。
void getAttackPattern(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1);

// ホールド評価を取得
void getHold(EVAL_TABLE* EvalT, TETRIS_DATA* Tetris, double* out1, double* out2, double* out3);

// パフェ評価を取得
void getPerfectClear(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1);

// フィールド的にパフェ可能かどうかを返します。
// 可能な場合は手数を返します。-1はパフェ不可です。
int checkPerfectClear(MINO_TYPE* board);
bool checkPerfectClearSplit(MINO_TYPE* board, int height);

// ライン消去評価を取得
void GetLineClear(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1);




/// <summary>
/// 評価テーブルのデフォルト値を設定します。
/// </summary>
/// <param name="table"></param>
void setEvalTableDefault(EVAL_TABLE* table)
{
#if false
    table->heightWait[0] = 0.9;
    table->heightWait[1] = 0.95;
    table->heightWait[2] = 0.98;
    table->heightWait[3] = 1;
    table->heightWait[4] = 1;
    table->height = -0.40;
    table->heightMid = -0.10;
    table->heightTop = -0.10;
    table->roofWait[0] = 1;
    table->roofWait[1] = 2;
    table->roofWait[2] = 1.5;
    table->roofWait[3] = 1;
    table->roofWait[4] = 1;
    table->roof = -0.15;
    table->roofSq = -0.03;
    table->wellWait[0] = -0.5;
    table->wellWait[1] = -0.7;
    table->wellWait[2] = -0.2;
    table->wellWait[3] = 0.7;
    table->wellWait[4] = 0.7;
    table->well = 0.05;
    table->surfaceWait[0] = 0.7;
    table->surfaceWait[1] = 0.8;
    table->surfaceWait[2] = 0.9;
    table->surface = -0.2;
    table->surfaceSq = -0.2;
    table->cave = -0.08;
    table->caveSq = -0.05;
    table->close = -3.6;
    table->closeSq = -0.3;
    table->tsdForm[0] = 0.0;
    table->tsdForm[1] = 0.2;
    table->tsdForm[2] = 0.1;
    table->tsdForm[3] = 0.7;
    table->tsdForm[4] = 1.8;
    table->tstForm[0] = 0.0;
    table->tstForm[1] = 0.0;
    table->tstForm[2] = 0.0;
    table->tstForm[3] = 0.0;
    table->tstForm[4] = 5.0;
    table->holdT = 0.05;
    table->holdI = 0.10;
    table->holdO = -0.02;
    table->pcForm = 0.5;
    table->useT = -3.00;
    table->useI = -0.50;
    table->pc = 99;
    table->tss = 2.8;
    table->tsm = -1.5;
    table->line1 = -8.8;
    table->tsd = 7.20;
    table->tsmd = -1.0;
    table->line2 = -7.6;
    table->tst = 9;
    table->line3 = -7.4;
    table->line4 = 4.5;
    table->ren = 0.6;
    table->delay = -0.01;
    table->btb = 0.5;
#endif

    // STANDARD
#if true
    table->heightWait[0] = 0.9;
    table->heightWait[1] = 0.95;
    table->heightWait[2] = 0.98;
    table->heightWait[3] = 1;
    table->heightWait[4] = 1;
    table->height = -0.4;
    table->heightMid = -1.5;
    table->heightTop = -5.0;
    table->roofWait[0] = 1;
    table->roofWait[1] = 1;
    table->roofWait[2] = 1;
    table->roofWait[3] = 1;
    table->roofWait[4] = 1;
    table->roof = -0.34;
    table->roofSq = -0.01;
    table->wellWait[0] = -0.2;
    table->wellWait[1] = -0.2;
    table->wellWait[2] = 0.2;
    table->wellWait[3] = 0.5;
    table->wellWait[4] = 0.6;
    table->well = 0.5;
    table->surfaceWait[0] = 0.7;
    table->surfaceWait[1] = 0.8;
    table->surfaceWait[2] = 0.9;
    table->surface = -0.25;
    table->surfaceSq = -0.08;
    table->cave = -0.2;
    table->caveSq = -0.01;
    table->close = -2;
    table->closeSq = -0.03;
    table->tsdForm[0] = 0.2;
    table->tsdForm[1] = 0.5;
    table->tsdForm[2] = 0.2;
    table->tsdForm[3] = 0.5;
    table->tsdForm[4] = 1.8;
    table->tstForm[0] = 0.0;
    table->tstForm[1] = 0.0;
    table->tstForm[2] = 0.0;
    table->tstForm[3] = 0.0;
    table->tstForm[4] = 5.0;
    table->holdT = 0.05;
    table->holdI = 0.10;
    table->holdO = -0.02;
    table->pcForm = 0.5;
    table->useT = -1.50;
    table->useI = -0.50;
    table->pc = 999;
    table->tss = 1.2;
    table->tsm = -1.5;
    table->line1 = -1.4;
    table->tsd = 4.0;
    table->tsmd = -0.8;
    table->line2 = -1.0;
    table->tst = 6;
    table->line3 = -0.5;
    table->line4 = 4.0;
    table->ren = 0.2;
    table->delay = -0.03;
    table->btb = 0.5;

// back_to_back: 52,
// bumpiness : -24,
// bumpiness_sq : -7,
// row_transitions : -5,
// height : -39,
// top_half : -150,
// top_quarter : -511,
// jeopardy : -11,
// cavity_cells : -173,
// cavity_cells_sq : -3,
// overhang_cells : -34,
// overhang_cells_sq : -1,
// covered_cells : -17,
// covered_cells_sq : -1,
// tslot : [8, 148, 192, 407] ,
// well_depth : 57,
// max_well_depth : 17,
// well_column : [20, 23, 20, 50, 59, 21, 59, 10, -10, 24] ,
// move_time : -3,
// wasted_t : -152,
// b2b_clear : 104,
// clear1 : -143,
// clear2 : -100,
// clear3 : -58,
// clear4 : 390,
// tspin1 : 121,
// tspin2 : 410,
// tspin3 : 602,
// mini_tspin1 : -158,
// mini_tspin2 : -93,
// perfect_clear : 999,
// combo_garbage : 150,
// use_bag : true,
// timed_jeopardy : true,
// stack_pc_damage : false,
// sub_name : None,

#endif

    // FAST
#if false 
    table->heightWait[0] = 0.9;
    table->heightWait[1] = 0.95;
    table->heightWait[2] = 0.98;
    table->heightWait[3] = 1;
    table->heightWait[4] = 1;
    table->height = -0.40;
    table->heightMid = -0.10;
    table->heightTop = -0.10;
    table->roofWait[0] = 1;
    table->roofWait[1] = 2;
    table->roofWait[2] = 1.5;
    table->roofWait[3] = 1;
    table->roofWait[4] = 1;
    table->roof = -0.15;
    table->roofSq = -0.03;
    table->wellWait[0] = -0.5;
    table->wellWait[1] = -0.7;
    table->wellWait[2] = -0.2;
    table->wellWait[3] = 0.7;
    table->wellWait[4] = 0.7;
    table->well = 0.05;
    table->surfaceWait[0] = 0.7;
    table->surfaceWait[1] = 0.8;
    table->surfaceWait[2] = 0.9;
    table->surface = -0.2;
    table->surfaceSq = -0.2;
    table->cave = -0.08;
    table->caveSq = -0.05;
    table->close = -3.6;
    table->closeSq = -0.3;
    table->tsdForm[0] = 0.0;
    table->tsdForm[1] = 0.2;
    table->tsdForm[2] = 0.1;
    table->tsdForm[3] = 0.7;
    table->tsdForm[4] = 1.8;
    table->tstForm[0] = 0.0;
    table->tstForm[1] = 0.0;
    table->tstForm[2] = 0.0;
    table->tstForm[3] = 0.0;
    table->tstForm[4] = 5.0;
    table->holdT = 0.05;
    table->holdI = 0.10;
    table->holdO = -0.02;
    table->pcForm = 0.5;
    table->useT = -3.00;
    table->useI = -0.50;
    table->pc = 99;
    table->tss = 2.8;
    table->tsm = -1.5;
    table->line1 = -8.8;
    table->tsd = 7.20;
    table->tsmd = -1.0;
    table->line2 = -7.6;
    table->tst = 9;
    table->line3 = -7.4;
    table->line4 = 4.5;
    table->ren = 0.6;
    table->delay = -0.01;
    table->btb = 0.5;



    // back_to_back: 10,
    // bumpiness: -7,
    // bumpiness_sq : -28,
    // row_transitions : -5,
    // height : -46,
    // top_half : -126,
    // top_quarter : -493,
    // jeopardy : -11,
    // cavity_cells : -176,
    // cavity_cells_sq : -6,
    // overhang_cells : -47,
    // overhang_cells_sq : -9,
    // covered_cells : -25,
    // covered_cells_sq : 1,
    // tslot : [0, 150, 296, 207] ,
    // well_depth : 158,
    // max_well_depth : -2,
    // well_column : [31, 16, -41, 37, 49, 30, 56, 48, -27, 22] ,
    // b2b_clear : 74,
    // clear1 : -122,
    // clear2 : -174,
    // clear3 : 11,
    // clear4 : 424,
    // tspin1 : 131,
    // tspin2 : 392,
    // tspin3 : 628,
    // mini_tspin1 : -188,
    // mini_tspin2 : -682,
    // perfect_clear : 991,
    // combo_garbage : 272,
    // move_time : -1,
    // wasted_t : -147,
    // use_bag : true,
    // timed_jeopardy : false,
    // stack_pc_damage : false,
    // sub_name : None,

#endif
}




/// 評価関数群の実装です


// 評価関数
void GetEvaluation(TETRIS_DATA* tetris, double *eval, EVAL_TABLE* evalT)
{
    int BoardHeight[BOARD_W] = { 0 };

    /// 各列の高さを事前に取得しておきます
    for (int x = 0; x < BOARD_W; x++)
    {
        for (int i = (BOARD_H - 1); i >= 0; i--)
        {
            if ((tetris->board[x + (i * BOARD_W)]) != N)
            {
                BoardHeight[x] = (i + 1);
                break;
            }
        }
    }

    int eval_index = 0;

    /// 高さ評価です0 1 2
    {
        double out1 = 0;
        double out2 = 0;
        double out3 = 0;
        getHeight(evalT, tetris, &out1, &out2, &out3);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
        eval[eval_index++] = out3;
    }

    /// 屋根評価です3 4
    {
        double out1 = 0;
        double out2 = 0;
        getRoof(evalT, tetris, BoardHeight, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// 井戸評価です5
    int wellColumn = -1;
    int Depth = 0;
    {
        double out1 = 0;
        wellColumn = getWell(evalT, tetris, BoardHeight, &Depth, &out1);
        eval[eval_index++] = out1;
    }

    /// 凸凹評価です6 7
    {
        double out1 = 0;
        double out2 = 0;
        getSurface(evalT, tetris, BoardHeight, wellColumn, Depth, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// 洞窟評価です8 9
    {
        double out1 = 0;
        double out2 = 0;
        getCave(evalT, tetris, BoardHeight, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// 閉塞評価です10 11
    {
        double out1 = 0;
        double out2 = 0;
        getClosedArea(evalT, tetris, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// 地形評価です12
    {
        double out1 = 0;
        getAttackPattern(evalT, tetris, BoardHeight, &out1);
        eval[eval_index++] = out1;
    }

    /// ホールド評価です13 14 15
    {
        double out1 = 0;
        double out2 = 0;
        double out3 = 0;
        getHold(evalT, tetris, &out1, &out2, &out3);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
        eval[eval_index++] = out3;
    }

    /// パフェ評価です16
    {
        double out1 = 0;
        getPerfectClear(evalT, tetris, &out1);
        eval[eval_index++] = out1;
    }

    /// ディレイ評価です17
    {
        eval[eval_index++] = tetris->prevResult.Delay * evalT->delay;
    }

    /// Btb評価です18
    {
        if (tetris->control.btb == TRUE) eval[eval_index++] = evalT->btb;
        else eval[eval_index++] = 0;
    }

    /// ライン消去火力評価です19
    {
        double out1 = 0;
        GetLineClear(evalT, tetris, &out1);
        eval[eval_index++] = out1;
    }
}


/// 高さを取得します
void getHeight(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1, double* out2, double* out3)
{
    double height = 0;

    double t[BOARD_W] =
    {
        evalT->heightWait[0],
        evalT->heightWait[1],
        evalT->heightWait[2],
        evalT->heightWait[3],
        evalT->heightWait[4],
        evalT->heightWait[4],
        evalT->heightWait[3],
        evalT->heightWait[2],
        evalT->heightWait[1],
        evalT->heightWait[0],
    };

    for (int x = 0; x < BOARD_W; x++)
    {
        for (int y = (BOARD_H - 1); y >= 0; y--)
        {
            if ((tetris->board[x + (y * BOARD_W)]) != 0)
            {
                if (height < (y * t[x])) { height = (y * t[x]); }
                break;
            }
        }
    }

    *out1 = (height * evalT->height);

    /// 約半分の高さ評価です
    if (height >= 8)
    {
        *out2 = ((height - 8) * evalT->heightMid);
    }
    else
    {
        *out2 = 0;
    }

    /// 上部の高さ評価です
    if (height >= 15)
    {
        *out3 = ((height - 15) * evalT->heightTop);
    }
    else
    {
        *out3 = 0;
    }
}


/// 屋根を取得します
void getRoof(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1, double* out2)
{
    double roof[BOARD_W] = { 0 };
    double roof2[BOARD_W] = { 0 };
    double t[BOARD_W] = 
    {
        evalT->roofWait[0],
        evalT->roofWait[1],
        evalT->roofWait[2],
        evalT->roofWait[3],
        evalT->roofWait[4],
        evalT->roofWait[4],
        evalT->roofWait[3],
        evalT->roofWait[2],
        evalT->roofWait[1],
        evalT->roofWait[0],
    };
    for (int i = 0; i < BOARD_W; i++)
    {

        for (int y = (boardHeight[i] - 1); y >= 0; y--)
        {
            if ((tetris->board[i + (y * BOARD_W)]) == N)
            {
                int cell = boardHeight[i] - y;
                roof[i] += (cell * t[i]);
                roof2[i] += ((double)cell * cell * t[i]);
            }
        }
    }

    double roofs = 0;
    double roofs2 = 0;
    for (int i = 0; i < BOARD_W; i++)
    {
        roofs += roof[i];
        roofs2 += roof2[i];
    }

    *out1 = (roofs * evalT->roof);
    *out2 = (roofs2 * evalT->roofSq);
}


/// 井戸を取得します
/// 返り値は井戸の列です。井戸は完成品を調べます
int getWell(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, int* depth, double* out1)
{
    double column_score[BOARD_W] =
    {
        evalT->wellWait[0],
        evalT->wellWait[1],
        evalT->wellWait[2],
        evalT->wellWait[3],
        evalT->wellWait[4],
        evalT->wellWait[4],
        evalT->wellWait[3],
        evalT->wellWait[2],
        evalT->wellWait[1],
        evalT->wellWait[0],
    };

    /// 取りあえず一番深い列を井戸認定します
    int wellColumn = 0;
    int wellColumnOther = 0;
    *out1 = 0;
    for (int i = 1; i < BOARD_W; i++)
    {
        if (boardHeight[i] < boardHeight[wellColumn])
        {
            wellColumn = i;
            wellColumnOther = 0;
            *out1 = 0;
        }
        else if (boardHeight[i] == boardHeight[wellColumn])
        {
            *out1 += column_score[wellColumn];
            wellColumnOther++;
        }
    }

    /// 複数の井戸が見つかったらそれは井戸ではない
    if (wellColumnOther > 0)
    {
        return -1;
    }

    /// 一番深い井戸の一番下から数えて4段を最大として井戸扱いにします
    (*depth) = 0;
    bool finish = false;
    for (int i = boardHeight[wellColumn]; i < (boardHeight[wellColumn] + 4); i++)
    {
        BOOL fill = true;
        for (int x = 0; x < BOARD_W; x++)
        {
            if (x == wellColumn) { continue; }
            if (tetris->board[x + (i * BOARD_W)] == N)
            {
                fill = FALSE;
                break;
            }
        }

        if (fill == TRUE)
        {
            if (finish == true)
            {
                (*depth) = 0;
                break;
            }
            (*depth)++;
        }
        else
        {
            if ((*depth) > 0)
            {
                finish = true;
            }
            break;
        }
    }

    *out1 += (((double)(*depth)) * evalT->well);
    *out1 += column_score[wellColumn];

    return wellColumn;
}


/// 表面の凸凹を取得します
/// 一番深い井戸は無視します
void getSurface(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, int wellColumn, int depth, double* out1, double* out2)
{
    double Surface = 0;
    double Surface2 = 0;

    int prevHeight = boardHeight[0];

    for (int i = 0; i < BOARD_W; i++)
    {
        int Height = boardHeight[i] - 1;
        if (i == wellColumn) { Height += depth; }

        double s = abs(prevHeight - Height);
        if (s < 0) { s = 0; }

        if ((i == 0) && (prevHeight > Height))
        {
            s *= evalT->surfaceWait[0];
        }
        if ((i == 1) && (prevHeight > Height))
        {
            s *= evalT->surfaceWait[1];
        }
        if ((i == 2) && (prevHeight > Height))
        {
            s *= evalT->surfaceWait[2];
        }
        if ((i == 7) && (prevHeight < Height))
        {
            s *= evalT->surfaceWait[2];
        }
        if ((i == 8) && (prevHeight < Height))
        {
            s *= evalT->surfaceWait[1];
        }
        if ((i == 9) && (prevHeight < Height))
        {
            s *= evalT->surfaceWait[0];
        }

        Surface += s;
        Surface2 += (s * s);
        prevHeight = Height;
    }

    *out1 = (Surface * evalT->surface);
    *out2 = (Surface2 * evalT->surfaceSq);
}


// 洞窟数を取得
void getCave(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1, double* out2)
{
    double cave = 0;
    for (int x = 0; x < BOARD_W; x++)
    {
        for (int y = 0; y < boardHeight[x]; y++)
        {
            if ((tetris->board[x + (y * BOARD_W)]) == 0)
            {
                cave++;
            }
        }
    }
    *out1 = cave * evalT->cave;
    *out2 = (cave * cave) * evalT->caveSq;
}

// 閉塞数を取得
void getClosedArea(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1, double* out2)
{
    /*
    ■■■■■■■■■
    ■□■■□□■■■
    ■■■■■□■■■
    ■■■□□□■■■
    ■■■■■■■□□
    こんな感じの場合は閉塞数3になります。
    */

    int ClosedBoard[21 * BOARD_W] = { 0 };

    int AreaKind = 0;
    int ClosedCount = 0;
    for (int y = 0; y < 21; y++)
    {
        for (int x = 0; x < BOARD_W; x++)
        {
            if ((ClosedBoard[(y * BOARD_W) + x] == 0) && ((tetris->board[x + (y * BOARD_W)]) == 0))
            {
                AreaKind++;
                if (setClosedArea(tetris->board, ClosedBoard, x, y, AreaKind, 1) != 0)
                {
                    ClosedCount++;
                }
            }
        }
    }

    *out1 = (ClosedCount * evalT->close);
    *out2 = (((double)ClosedCount * ClosedCount) * evalT->closeSq);
}

/// 閉塞エリアを求めます。
int setClosedArea(MINO_TYPE* board, int* closedBoard, int x, int y, int kind, int closeCell)
{
    if (y < 0) return closeCell;
    if (y >= 21) return 0;
    if (x < 0) return closeCell;
    if (x >= BOARD_W) return closeCell;

    // 今参照しているINDEXを持っておきます。
    int index = (y * BOARD_W) + x;

    if ((closedBoard[index] == 0) && ((board[x + (y * BOARD_W)]) == 0))
    {
        closedBoard[index] = kind;
        closeCell = setClosedArea(board, closedBoard, x + 1, y, kind, closeCell);
        closeCell = setClosedArea(board, closedBoard, x, y - 1, kind, closeCell);
        closeCell = setClosedArea(board, closedBoard, x - 1, y, kind, closeCell);
        closeCell = setClosedArea(board, closedBoard, x, y + 1, kind, closeCell);
    }

    return closeCell;
}

void getAttackPattern(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1)
{
    double hyoka = 0.0;
    double hyoka0 = 0.0;
    double hyoka1 = 0.0;
    int Board[BOARD_H] = { 0 };

    for (int y = 0; y < BOARD_H; y++)
    {
        for (int x = 0; x < BOARD_W; x++)
        {
            if (tetris->board[x + (y * BOARD_W)] != N)
            {
                Board[y] |= (1 << x);
            }
        }
    }

    for (int x = 0; x < (BOARD_W - 3); x++)
    {
        for (int y = boardHeight[x] + 2; y >= 2; y--)
        {
            // yがBOARD_H以上または0未満の場合、次の繰り返しに進む
            if (y >= BOARD_H || y < 0) { continue; }

            // 事前にビット操作を行い、結果を変数に保存
            int boardY_2 = Board[y - 2];
            int boardY_1 = Board[y - 1];
            int boardY_0 = Board[y - 0];

            // 埋まっていたら次のX軸に移る
            if ((boardY_2 & (7 << x)) == 0x7)
            {
                break;
            }

            // 一番下の段の評価
            double hyouka1 = 0.0;
            if (
                (boardY_2 & (1 << x)) != 0 &&
                (boardY_2 & (1 << (x + 1))) == 0 &&
                (boardY_2 & (1 << (x + 2))) != 0)
            {
                hyouka1 += evalT->tsdForm[0];
                if ((boardY_2 | (7 << x)) == 0x3FF)
                {
                    hyouka1 += evalT->tsdForm[1];
                }
            }
            else
            {
                continue;
            }

            // ２番下の段の評価
            double hyouka2 = 0.0;
            if (
                (boardY_1 & (1 << x)) == 0 &&
                (boardY_1 & (1 << (x + 1))) == 0 &&
                (boardY_1 & (1 << (x + 2))) == 0)
            {
                hyouka2 += evalT->tsdForm[2];
                if ((boardY_1 | (7 << x)) == 0x3FF)
                {
                    hyouka2 += evalT->tsdForm[3];
                }
            }
            else
            {
                continue;
            }

            // ３番下の段の評価
            double hyouka3 = 0.0;
            if (
                (boardY_0 & (1 << x)) != 0 &&
                (boardY_0 & (1 << (x + 1))) == 0 &&
                (boardY_0 & (1 << (x + 2))) == 0)
            {
                hyouka3 += evalT->tsdForm[4];
            }
            else if (
                (boardY_0 & (1 << x)) == 0 &&
                (boardY_0 & (1 << (x + 1))) == 0 &&
                (boardY_0 & (1 << (x + 2))) != 0)
            {
                hyouka3 += evalT->tsdForm[4];
            }
            else if ((boardY_0 & (1 << (x + 1))) != 0)
            {
                continue;
            }

            // 最終評価の計算
            double totalHyoka = hyouka1 + hyouka2 + hyouka3;
            if (hyoka0 < totalHyoka)
            {
                hyoka0 = totalHyoka;
            }
            else if (hyoka1 < totalHyoka)
            {
                hyoka1 = totalHyoka;
            }
        }
    }

    hyoka = hyoka0 + hyoka1;

#if true

    double hyoka2 = 0.0;
    double hyoka3 = 0.0;

    for (int x = 0; x < (BOARD_W - 4); x++)
    {
        for (int y = boardHeight[x] + 5; y >= 5; y--)
        {
            // yがBOARD_H以上または0未満の場合、次の繰り返しに進む
            if (y >= BOARD_H || y < 0) { continue; }

            int boardY_4 = Board[y - 4];
            int boardY_3 = Board[y - 3];
            int boardY_2 = Board[y - 2];
            int boardY_1 = Board[y - 1];
            int boardY_0 = Board[y - 0];

            // TSTのB回転の評価計算
            double bHyoka1 = 0.0, bHyoka2 = 0.0, bHyoka3 = 0.0;
            if (!(boardY_4 & (1 << (x + 2))) && (boardY_4 & (1 << (x + 3))))
            {
                bHyoka1 += evalT->tstForm[0];
                if ((boardY_4 | (0x4 << x)) == 0x3FF)
                {
                    bHyoka1 += evalT->tstForm[1];
                }
            }

            if (bHyoka1 != 0.0 &&
                !(boardY_3 & ((1 << (x + 1)) | (1 << (x + 2)))) &&
                boardY_2 & (1 << (x + 1)) &&
                !(boardY_2 & (1 << (x + 2))) &&
                boardY_2 & (1 << (x + 3)) &&
                !(boardY_1 & ((1 << (x + 0)) | (1 << (x + 1)) | (1 << (x + 2)))) &&
                !(boardY_0 & ((1 << (x + 0)) | (1 << (x + 1)))) &&
                boardY_0 & (1 << (x + 2))
                )
            {
                bHyoka3 += evalT->tstForm[4];
            }
            else if (
                (boardY_3 & (1 << (x + 1))) ||
                (boardY_3 & (1 << (x + 2))) ||
                (boardY_2 & (1 << (x + 2))) ||
                (boardY_1 & (1 << (x + 0))) ||
                (boardY_1 & (1 << (x + 1))) ||
                (boardY_1 & (1 << (x + 2))) ||
                (boardY_0 & (1 << (x + 0))) ||
                (boardY_0 & (1 << (x + 1)))
                )
            {
                bHyoka2 = 0;
                bHyoka1 = 0;
            }

            double totalBHyoka = bHyoka1 + bHyoka2 + bHyoka3;
            if (hyoka2 < totalBHyoka)
            {
                hyoka2 = totalBHyoka;
            }

            // TSTのA回転の評価計算
            double aHyoka1 = 0.0, aHyoka2 = 0.0, aHyoka3 = 0.0;
            if (!(boardY_4 & (1 << (x + 1))) && (boardY_4 & (1 << (x + 2))))
            {
                aHyoka1 += evalT->tstForm[0];
                if ((boardY_4 | (0x2 << x)) == 0x3FF)
                {
                    aHyoka1 += evalT->tstForm[1];
                }
            }

            if (aHyoka1 != 0.0 &&
                !(boardY_3 & ((1 << (x + 1)) | (1 << (x + 2)))) &&
                boardY_2 & (1 << (x + 0)) &&
                !(boardY_2 & (1 << (x + 1))) &&
                boardY_2 & (1 << (x + 2)) &&
                !(boardY_1 & ((1 << (x + 1)) | (1 << (x + 2)) | (1 << (x + 3)))) &&
                !(boardY_0 & ((1 << (x + 2)) | (1 << (x + 3)))) &&
                boardY_0 & (1 << (x + 1))
                )
            {
                aHyoka3 += evalT->tstForm[4];
            }
            else if (
                (boardY_3 & (1 << (x + 1))) ||
                (boardY_3 & (1 << (x + 2))) ||
                (boardY_2 & (1 << (x + 1))) ||
                (boardY_1 & (1 << (x + 1))) ||
                (boardY_1 & (1 << (x + 2))) ||
                (boardY_1 & (1 << (x + 3))) ||
                (boardY_0 & (1 << (x + 2))) ||
                (boardY_0 & (1 << (x + 3)))
                )
            {
                aHyoka2 = 0;
                aHyoka1 = 0;
            }

            double totalAHyoka = aHyoka1 + aHyoka2 + aHyoka3;
            if (hyoka3 < totalAHyoka)
            {
                hyoka3 = totalAHyoka;
            }
        }
    }

    *out1 = hyoka + hyoka2 + hyoka3;
#endif

}

// ホールド評価を取得
void getHold(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1, double* out2, double* out3)
{
    /// ホールドTミノ評価です
    if (tetris->hold == T)*out1 = evalT->holdT;
    else *out1 = 0;

    /// ホールドIミノ評価です
    if (tetris->hold == I)*out2 = evalT->holdI;
    else *out2 = 0;

    /// ホールドOミノ評価です
    if (tetris->hold == O)*out3 = evalT->holdO;
    else *out3 = 0;
}


// パフェ評価を取得
void getPerfectClear(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1)
{
    int result = checkPerfectClear(tetris->board);

    if (result < 0) *out1 = 0.00;
    else *out1 = evalT->pcForm;
}

// フィールド的にパフェ可能かどうかを返します。
// 可能な場合は手数を返します。-1はパフェ不可です。
int checkPerfectClear(MINO_TYPE* board)
{
    int space_count = 0;
    bool check_split = false;

    /// パフェが取れそうな高さです。
    int height = 0;

    /// とりあえず高さを調べます。
    for (int y = 0; y < 7; y++)
    {
        int noFillCount = 0;
        for (int x = 0; x < BOARD_W; x++)
        {
            /// 空白セル数をカウントします。
            if ((board[x + (y * BOARD_W)]) == N)
            {
                space_count++;
                noFillCount++;
            }
        }

        /// 何もない段の場合はそこまででパフェの計算を行います。
        if (noFillCount == 10)
        {
            /// いずれにしても2の倍数でないとだめです。
            if ((space_count % 2) == 0)
            {
                // 空行含めて4の倍数の場合です。
                if ((space_count % 4) == 0)
                {
                    height = y + 1;

                    // 偶数なら6段、奇数なら5段にしておきます。
                    if ((height % 2) == 0) { height = 6; }
                    if ((height % 2) == 1) { height = 5; }
                }
                else
                {
                    height = y;

                    // 偶数なら6段、奇数なら5段にしておきます。
                    if ((height % 2) == 0) { height = 6; }
                    if ((height % 2) == 1) { height = 5; }
                }
            }

            break;
        }
    }

    /// 結局高さ0の場合はパフェが取れません。
    if (height == 0 || height > 6) return -1;

    /// 分割空間(縦埋まりの計算)のセル数も4の倍数でないとパフェが取れません。
    if (check_split == true)
    {
        if (checkPerfectClearSplit(board, height) == false)
        {
            if (height >= 4)
            {
                return -1;
            }

            if (checkPerfectClearSplit(board, height + 2) == false)
            {
                return -1;
            }
            space_count += 20;
        }
    }

    return space_count;
}


bool checkPerfectClearSplit(MINO_TYPE* board, int height)
{
    // 空きセル数をカウントします。
    int space_count = 0;
    for (int x = 0; x < BOARD_W; x++)
    {
        // 埋まってるセル数をカウントします。
        int fill_count = 0;

        // 縦閉塞を調べます。
        bool side_space = false;
        int line_space = 0;

        bool side_space_left = x > 0;
        bool side_space_right = x < (BOARD_W - 1);

        for (int y = 0; y < height; y++)
        {
            // 空きセルのカウントを行います。
            if ((board[x + (y * BOARD_W)]) == 0)
            {
                space_count++;
                line_space++;

                // 両サイドのいずれかが空きセルかどうかを調べます。
                if (side_space_left && (board[(x - 1) + (y * BOARD_W)]) == 0) side_space = true;
                if (side_space_right && (board[(x + 1) + (y * BOARD_W)]) == 0) side_space = true;
            }

            // ミノセルのカウントを行います。
            else fill_count++;
        }

        // スペースがありサイドが全部埋まってたらパフェはできません。
        if ((line_space > 0) && (side_space == false)) { return false; }

        // 縦埋めがあった場合、以前のスペースが4の倍数でなければパフェはできません。
        if (fill_count == height)
        {
            if ((space_count % 4) != 0) return false;
            space_count = 0;
        }
    }

    // 最後ももちろん空きセルが4の倍数でないとパフェできません。
    if ((space_count % 4) != 0) return false;

    // ここまでくればパフェ可能です。
    return true;
}



// ライン消去評価を取得
void GetLineClear(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1)
{
    double attack = 0;
    *out1 = 0;

    /// 何も消していなければスコア0です。
    if (tetris->prevResult.deleteLine == 0)
    {
        /// Tミノは火力の損があるのでマイナス評価です。
        if (tetris->prevControl.current == T) { *out1 = evalT->useT; }
        if (tetris->prevControl.current == I) { *out1 = evalT->useI; }

        return;
    }

    /// パフェ状態、パフェは最大
    if (tetris->prevResult.Pc != 0)
    {
        *out1 = evalT->pc;
        return;
    }

    if (tetris->prevResult.deleteLine == 1)
    {
        if (tetris->prevResult.Tss != 0)
        {
            attack = evalT->tss;
        }
        else if (tetris->prevResult.Tsm != 0)
        {
            attack = evalT->tsm;
        }
        else
        {
            attack = evalT->line1;
        }
    }
    if (tetris->prevResult.deleteLine == 2)
    {
        if (tetris->prevResult.Tsd != 0)
        {
            attack = evalT->tsd;
        }
        else if (tetris->prevResult.Tsmd != 0)
        {
            attack = evalT->tsmd;
        }
        else
        {
            attack = evalT->line2;
        }
    }
    if (tetris->prevResult.deleteLine == 3)
    {
        if (tetris->prevResult.Tst != 0)
        {
            attack = evalT->tst;
        }
        else
        {
            attack = evalT->line3;
        }
    }
    if (tetris->prevResult.deleteLine == 4)
    {
        attack = evalT->line4;
    }

    /// REN火力を加算します。
    if (tetris->control.ren >= 1)
    {
        double ren = ((double)tetris->control.ren - 1);
        attack += ren * evalT->ren;
    }

    (*out1) = attack;
}

