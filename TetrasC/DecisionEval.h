#pragma once

/// <summary>
/// 評価データです。
/// </summary>
typedef struct
{
    double eval[20];
} EVAL;


/// <summary>
/// 評価テーブル構造体です。
/// </summary>
typedef struct
{
    double heightWait[5];
    double height;
    double heightMid;
    double heightTop;
    double roofWait[5];
    double roof;
    double roofSq;
    double wellWait[5];
    double well;
    double surfaceWait[3];
    double surface;
    double surfaceSq;
    double cave;
    double caveSq;
    double close;
    double closeSq;
    double tsdForm[5];
    double tstForm[5];
    double holdT;
    double holdI;
    double holdO;
    double pcForm;

    double useT;
    double useI;
    double pc;
    double tss;
    double tsm;
    double line1;
    double tsd;
    double tsmd;
    double line2;
    double tst;
    double line3;
    double line4;
    double ren;

    double delay;
    double btb;
} EVAL_TABLE;


// デフォルトの評価テーブルを取得します。
extern void setEvalTableDefault(EVAL_TABLE* table);


// 評価関数
extern void GetEvaluation(TETRIS_DATA* tetris, double* eval, EVAL_TABLE* evalT);
