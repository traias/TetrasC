#include <math.h>
#include "DxLib.h"
#include "TetrisData.h"
#include "DecisionEval.h"


/// <summary>
/// �]���l�̐������̂ł��B
/// </summary>
const char* EvalName[20] =
{
    "����",
    "����H",
    "����T",
    "����",
    "����SQ",
    "���",
    "�ʉ�",
    "�ʉ�SQ",
    "���A",
    "���ASQ",
    "��",
    "��SQ",
    "�n�`",
    "HoldT",
    "HoldI",
    "HoldO",
    "PC",
    "Delay",
    "Btb",
    "�Η�",
};



/// �֐��v���g�^�C�v�錾�ł�

/// �������擾���܂�
void getHeight(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1, double* out2, double* out3);

/// �������擾���܂�
void getRoof(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1, double* out2);

/// ��˂��擾���܂�
/// �Ԃ�l�͈�˂̗�ł��B��˂͊����i�𒲂ׂ܂�
int getWell(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, int* depth, double* out1);

/// �\�ʂ̓ʉ����擾���܂�
/// ��Ԑ[����˂͖������܂�
void getSurface(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, int wellColumn, int depth, double* out1, double* out2);

// ���A�����擾
void getCave(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1, double* out2);

// �ǐ����擾
void getClosedArea(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1, double* out2);

/// �ǃG���A�����߂܂��B
int setClosedArea(MINO_TYPE* board, int* closedBoard, int x, int y, int kind, int closeCell);

/// �Η͒n�`���擾���܂��B
void getAttackPattern(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1);

int checkTsd(int* board, int x, int y);

int checkTstR(int* board, int x, int y);

int checkTstL(int* board, int x, int y);


// �z�[���h�]�����擾
void getHold(EVAL_TABLE* EvalT, TETRIS_DATA* Tetris, double* out1, double* out2, double* out3);

// �p�t�F�]�����擾
void getPerfectClear(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1);

// �t�B�[���h�I�Ƀp�t�F�\���ǂ�����Ԃ��܂��B
// �\�ȏꍇ�͎萔��Ԃ��܂��B-1�̓p�t�F�s�ł��B
int checkPerfectClear(MINO_TYPE* board);
BOOL checkPerfectClearSplit(MINO_TYPE* board, int height);

// ���C�������]�����擾
void GetLineClear(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1);




/// <summary>
/// �]���e�[�u���̃f�t�H���g�l��ݒ肵�܂��B
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
    table->heightWait[0] = 0.9;
    table->heightWait[1] = 0.95;
    table->heightWait[2] = 0.98;
    table->heightWait[3] = 1;
    table->heightWait[4] = 1;
    table->height = -0.2;
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
    table->tsdForm[1] = 1.0;
    table->tsdForm[2] = 1.2;
    table->tsdForm[3] = 2.5;
#if false
    table->tsdForm[0] = 0.1;
    table->tsdForm[1] = 0.4;
    table->tsdForm[2] = 0.1;
    table->tsdForm[3] = 0.4;
    table->tsdForm[4] = 1.5;
    table->tstForm[0] = 0.0;
    table->tstForm[1] = 0.0;
    table->tstForm[2] = 0.0;
    table->tstForm[3] = 0.0;
    table->tstForm[4] = 5.0;
#endif

    table->holdT = 0.05;
    table->holdI = 0.10;
    table->holdO = -0.02;
    table->pcForm = 0.5;
    table->useT = -1.80;
    table->useI = -0.50;
    table->pc = 999;
    table->tss = 1.8;
    table->tsm = -1.8;
    table->line1 = -3.5;
    table->tsd = 7.0;
    table->tsmd = -1.2;
    table->line2 = -2.5;
    table->tst = 9.0;
    table->line3 = -1.6;
    table->line4 = 5.8;
    table->ren = 0.3;
    table->delay = -0.04;
    table->btb = 0.8;

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




/// �]���֐��Q�̎����ł�


// �]���֐�
void GetEvaluation(TETRIS_DATA* tetris, double *eval, EVAL_TABLE* evalT)
{
    int BoardHeight[BOARD_W] = { 0 };

    /// �e��̍��������O�Ɏ擾���Ă����܂�
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

    /// �����]���ł�0 1 2
    {
        double out1 = 0;
        double out2 = 0;
        double out3 = 0;
        getHeight(evalT, tetris, &out1, &out2, &out3);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
        eval[eval_index++] = out3;
    }

    /// �����]���ł�3 4
    {
        double out1 = 0;
        double out2 = 0;
        getRoof(evalT, tetris, BoardHeight, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// ��˕]���ł�5
    int wellColumn = -1;
    int Depth = 0;
    {
        double out1 = 0;
        wellColumn = getWell(evalT, tetris, BoardHeight, &Depth, &out1);
        eval[eval_index++] = out1;
    }

    /// �ʉ��]���ł�6 7
    {
        double out1 = 0;
        double out2 = 0;
        getSurface(evalT, tetris, BoardHeight, wellColumn, Depth, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// ���A�]���ł�8 9
    {
        double out1 = 0;
        double out2 = 0;
        getCave(evalT, tetris, BoardHeight, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// �Ǖ]���ł�10 11
    {
        double out1 = 0;
        double out2 = 0;
        getClosedArea(evalT, tetris, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// �n�`�]���ł�12
    {
        double out1 = 0;
        getAttackPattern(evalT, tetris, BoardHeight, &out1);
        eval[eval_index++] = out1;
    }

    /// �z�[���h�]���ł�13 14 15
    {
        double out1 = 0;
        double out2 = 0;
        double out3 = 0;
        getHold(evalT, tetris, &out1, &out2, &out3);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
        eval[eval_index++] = out3;
    }

    /// �p�t�F�]���ł�16
    {
        double out1 = 0;
        getPerfectClear(evalT, tetris, &out1);
        eval[eval_index++] = out1;
    }

    /// �f�B���C�]���ł�17
    {
        eval[eval_index++] = tetris->prevResult.Delay * evalT->delay;
    }

    /// Btb�]���ł�18
    {
        if (tetris->control.btb == TRUE) eval[eval_index++] = evalT->btb;
        else eval[eval_index++] = 0;
    }

    /// ���C�������Η͕]���ł�19
    {
        double out1 = 0;
        GetLineClear(evalT, tetris, &out1);
        eval[eval_index++] = out1;
    }
}


/// �������擾���܂�
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
            if ((tetris->board[x + (y * BOARD_W)]) != N)
            {
                if (height < (y * t[x])) { height = (y * t[x]); }
                break;
            }
        }
    }

    *out1 = (height * evalT->height);

    /// �񔼕��̍����]���ł�
    if (height >= 8)
    {
        *out2 = ((height - 8) * evalT->heightMid);
    }
    else
    {
        *out2 = 0;
    }

    /// �㕔�̍����]���ł�
    if (height >= 15)
    {
        *out3 = ((height - 15) * evalT->heightTop);
    }
    else
    {
        *out3 = 0;
    }
}


/// �������擾���܂�
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


/// ��˂��擾���܂�
/// �Ԃ�l�͈�˂̗�ł��B��˂͊����i�𒲂ׂ܂�
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

    /// ��肠������Ԑ[�������˔F�肵�܂�
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

    /// �����̈�˂����������炻��͈�˂ł͂Ȃ�
    if (wellColumnOther > 0)
    {
        return -1;
    }

    /// ��Ԑ[����˂̈�ԉ����琔����7�i���ő�Ƃ��Ĉ�ˈ����ɂ��܂�
    (*depth) = 0;
    BOOL finish = FALSE;
    for (int i = boardHeight[wellColumn]; i < (boardHeight[wellColumn] + 7); i++)
    {
        BOOL fill = TRUE;
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
            if (finish == TRUE)
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
                finish = TRUE;
            }
            break;
        }
    }

    *out1 += (((double)(*depth)) * evalT->well);
    *out1 += column_score[wellColumn];

    return wellColumn;
}


/// �\�ʂ̓ʉ����擾���܂�
/// ��Ԑ[����˂͖������܂�
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


// ���A�����擾
void getCave(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1, double* out2)
{
    double cave = 0;
    for (int x = 0; x < BOARD_W; x++)
    {
        for (int y = 0; y < boardHeight[x]; y++)
        {
            if ((tetris->board[x + (y * BOARD_W)]) == N)
            {
                cave++;
            }
        }
    }
    *out1 = cave * evalT->cave;
    *out2 = (cave * cave) * evalT->caveSq;
}

// �ǐ����擾
void getClosedArea(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1, double* out2)
{
    /*
    ������������������
    ������������������
    ������������������
    ������������������
    ������������������
    ����Ȋ����̏ꍇ�͕ǐ�3�ɂȂ�܂��B
    */

    int ClosedBoard[21 * BOARD_W] = { 0 };

    int AreaKind = 0;
    int ClosedCount = 0;
    for (int y = 0; y < 21; y++)
    {
        for (int x = 0; x < BOARD_W; x++)
        {
            if ((ClosedBoard[(y * BOARD_W) + x] == 0) && ((tetris->board[x + (y * BOARD_W)]) == N))
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

/// �ǃG���A�����߂܂��B
int setClosedArea(MINO_TYPE* board, int* closedBoard, int x, int y, int kind, int closeCell)
{
    if (y < 0) return closeCell;
    if (y >= 21) return 0;
    if (x < 0) return closeCell;
    if (x >= BOARD_W) return closeCell;

    // ���Q�Ƃ��Ă���INDEX�������Ă����܂��B
    int index = (y * BOARD_W) + x;

    if ((closedBoard[index] == 0) && ((board[x + (y * BOARD_W)]) == N))
    {
        closedBoard[index] = kind;
        closeCell = setClosedArea(board, closedBoard, x + 1, y, kind, closeCell);
        closeCell = setClosedArea(board, closedBoard, x, y - 1, kind, closeCell);
        closeCell = setClosedArea(board, closedBoard, x - 1, y, kind, closeCell);
        closeCell = setClosedArea(board, closedBoard, x, y + 1, kind, closeCell);
    }

    return closeCell;
}


/// <summary>
/// �Η͒n�`�𔻒肵�܂��B
/// </summary>
/// <param name="evalT"></param>
/// <param name="tetris"></param>
/// <param name="boardHeight"></param>
/// <param name="out1"></param>
void getAttackPattern(EVAL_TABLE* evalT, TETRIS_DATA* tetris, int* boardHeight, double* out1)
{
    const int TEMP_BOARD_H = 30;
    int board[TEMP_BOARD_H] = { 0 };

    // ���̃{�[�h���쐬���܂��B
    for (int y = 0; y < TEMP_BOARD_H; y++)
    {
        for (int x = 0; x < BOARD_W; x++)
        {
            if (tetris->board[x + (y * BOARD_W)] != N)
            {
                board[y] |= (1 << x);
            }
        }
    }

    int lineD = 0;
    int lineTR = 0;
    int lineTL = 0;
    for (int x = 0; x < BOARD_W; x++)
    {
        for (int y = 0; y < boardHeight[x] - 5; y++)
        {
            int lineTsd = 0;
            // �T���͈�
            //    0123456789
            // 19 __________
            // 18 __________
            // 17 XXXXXXXX__
            // 3-16    :
            // 2  XXXXXXXX__
            // 1  XXXXXXXX__
            // 0  XXXXXXXX__
            if (lineD < 4)
            {
                if (x < (BOARD_W - 2))
                {
                    lineTsd = checkTsd(board, x, y);
                }
            }

            int lineTstR = 0;
            // �T���͈�
            //    0123456789
            // 19 __________
            // 18 __________
            // 17 XXXXXXX___
            // 3-16    :
            // 2  XXXXXXX___
            // 1  XXXXXXX___
            // 0  XXXXXXX___
            if (lineTR < 4)
            {
                if (x < (BOARD_W - 3))
                {
                    lineTstR = checkTstR(board, x, y);
                }
            }

            int lineTstL = 0;
            // �T���͈�
            //    0123456789
            // 19 __________
            // 18 __________
            // 17 _XXXXXXX__
            // 3-16    :
            // 2  _XXXXXXX__
            // 1  _XXXXXXX__
            // 0  _XXXXXXX__
            if (lineTL < 4)
            {
                if ((x >= 1) && (x < (BOARD_W - 2)))
                {
                    lineTstL = checkTstL(board, x, y);
                }
            }

            if (lineD > lineTsd) { lineD = lineTsd; }
            if (lineTR > lineTstR) { lineTR = lineTstR; }
            if (lineTL > lineTstL) { lineTL = lineTstL; }
        }
    }

    *out1 += 0;
    switch (lineD)
    {
    case 1: *out1 += evalT->tsdForm[0]; break;
    case 2: *out1 += evalT->tsdForm[1]; break;
    case 3: *out1 += evalT->tsdForm[2]; break;
    case 4: *out1 += evalT->tsdForm[3]; break;
    }
    switch (lineTR)
    {
    case 1: *out1 += evalT->tsdForm[0]; break;
    case 2: *out1 += evalT->tsdForm[1]; break;
    case 3: *out1 += evalT->tsdForm[2]; break;
    case 4: *out1 += evalT->tsdForm[3]; break;
    }
    switch (lineTL)
    {
    case 1: *out1 += evalT->tsdForm[0]; break;
    case 2: *out1 += evalT->tsdForm[1]; break;
    case 3: *out1 += evalT->tsdForm[2]; break;
    case 4: *out1 += evalT->tsdForm[3]; break;
    }
}

// ___ 4
// X_X 3
// ooo 2
// XoX 1
int checkTsd(int* board, int x, int y)
{
    int count = 0;
    int line = 0;
    BOOL spaceFail = FALSE;

    // XoX 1
    if ((board[y] & (0x1 << x)) == 0x1)
    {
        count++;
    }
    if ((board[y] & (0x2 << x)) != 0x0)
    {
        spaceFail = TRUE;
    }
    if ((board[y] & (0x4 << x)) == 0x1)
    {
        count++;
    }

    // ooo 2
    if ((board[y + 1] & (0x7 << x)) != 0x0)
    {
        spaceFail = TRUE;
    }

    // X_X 3
    if ((board[y + 2] & (0x1 << x)) == 0x1)
    {
        count++;
    }
    if ((board[y + 2] & (0x2 << x)) != 0x0)
    {
        spaceFail = TRUE;
    }
    if ((board[y + 2] & (0x4 << x)) == 0x1)
    {
        count++;
    }

    // ___ 4
    if ((board[y + 3] & (0x2 << x)) != 0x0)
    {
        spaceFail = TRUE;
    }

    if (spaceFail == TRUE)
    {
        return 0;
    }

    switch (count)
    {
    case 0: line = 0; break;
    case 1: line = 2; break;
    case 2: line = 3; break;
    case 3: line = 4; break;
    case 4: line = 4; break;
    }

    return line;
}

// ?X_ 4
// ?__ 3
// XoX 3
// ?oo 2
// ?o? 1
// ?X? 0
int checkTstR(int* board, int x, int y)
{
    int line = 0;

    // ?X? 0
    if ((board[y + 0] & (0x2 << x)) == 0x2)
    {
    }
    else
    {
        return line;
    }

    // ?o? 1
    if ((board[y + 1] & (0x2 << x)) == 0x0)
    {
        line = 1;
    }
    else
    {
        return line;
    }

    // ?oo 2
    if ((board[y + 2] & (0x6 << x)) == 0x0)
    {
        line = 2;
    }
    else
    {
        return line;
    }

    // XoX 3
    if ((board[y + 3] & (0x7 << x)) == 0x5)
    {
        line = 3;
    }
    else
    {
        return line;
    }

    // ?__ 3
    if ((board[y + 4] & (0x6 << x)) == 0x0)
    {
        line = 4;
    }
    else
    {
        return line;
    }

    // ?X_ 4
    if ((board[y + 5] & (0x6 << x)) == 0x2)
    {
        line = 4;
    }

    return line;
}

// _X? 4
// __? 3
// XoX 3
// oo? 2
// ?o? 1
// ?X? 0
int checkTstL(int* board, int x, int y)
{
    int line = 0;

    // ?X? 0
    if ((board[y + 0] & (0x2 << x)) == 0x2)
    {
    }
    else
    {
        return line;
    }

    // ?o? 1
    if ((board[y + 1] & (0x2 << x)) == 0x0)
    {
        line = 1;
    }
    else
    {
        return line;
    }

    // oo? 2
    if ((board[y + 2] & (0x3 << x)) == 0x0)
    {
        line = 2;
    }
    else
    {
        return line;
    }

    // XoX 3
    if ((board[y + 3] & (0x7 << x)) == 0x5)
    {
        line = 3;
    }
    else
    {
        return line;
    }

    // __? 3
    if ((board[y + 4] & (0x3 << x)) == 0x0)
    {
        line = 4;
    }
    else
    {
        return line;
    }

    // _X? 4
    if ((board[y + 5] & (0x3 << x)) == 0x2)
    {
        line = 4;
    }

    return line;
}

// �z�[���h�]�����擾
void getHold(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1, double* out2, double* out3)
{
    /// �z�[���hT�~�m�]���ł�
    if (tetris->hold == T)*out1 = evalT->holdT;
    else *out1 = 0;

    /// �z�[���hI�~�m�]���ł�
    if (tetris->hold == I)*out2 = evalT->holdI;
    else *out2 = 0;

    /// �z�[���hO�~�m�]���ł�
    if (tetris->hold == O)*out3 = evalT->holdO;
    else *out3 = 0;
}


// �p�t�F�]�����擾
void getPerfectClear(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1)
{
    int result = checkPerfectClear(tetris->board);

    if (result < 0) *out1 = 0.00;
    else *out1 = evalT->pcForm;
}

// �t�B�[���h�I�Ƀp�t�F�\���ǂ�����Ԃ��܂��B
// �\�ȏꍇ�͎萔��Ԃ��܂��B-1�̓p�t�F�s�ł��B
int checkPerfectClear(MINO_TYPE* board)
{
    int space_count = 0;
    BOOL check_split = FALSE;

    /// �p�t�F����ꂻ���ȍ����ł��B
    int height = 0;

    /// �Ƃ肠���������𒲂ׂ܂��B
    for (int y = 0; y < 7; y++)
    {
        int noFillCount = 0;
        for (int x = 0; x < BOARD_W; x++)
        {
            /// �󔒃Z�������J�E���g���܂��B
            if ((board[x + (y * BOARD_W)]) == N)
            {
                space_count++;
                noFillCount++;
            }
        }

        /// �����Ȃ��i�̏ꍇ�͂����܂łŃp�t�F�̌v�Z���s���܂��B
        if (noFillCount == 10)
        {
            /// ������ɂ��Ă�2�̔{���łȂ��Ƃ��߂ł��B
            if ((space_count % 2) == 0)
            {
                // ��s�܂߂�4�̔{���̏ꍇ�ł��B
                if ((space_count % 4) == 0)
                {
                    height = y + 1;

                    // �����Ȃ�6�i�A��Ȃ�5�i�ɂ��Ă����܂��B
                    if ((height % 2) == 0) { height = 6; }
                    if ((height % 2) == 1) { height = 5; }
                }
                else
                {
                    height = y;

                    // �����Ȃ�6�i�A��Ȃ�5�i�ɂ��Ă����܂��B
                    if ((height % 2) == 0) { height = 6; }
                    if ((height % 2) == 1) { height = 5; }
                }
            }

            break;
        }
    }

    /// ���Ǎ���0�̏ꍇ�̓p�t�F�����܂���B
    if (height == 0 || height > 6) return -1;

    /// �������(�c���܂�̌v�Z)�̃Z������4�̔{���łȂ��ƃp�t�F�����܂���B
    if (check_split == TRUE)
    {
        if (checkPerfectClearSplit(board, height) == FALSE)
        {
            if (height >= 4)
            {
                return -1;
            }

            if (checkPerfectClearSplit(board, height + 2) == FALSE)
            {
                return -1;
            }
            space_count += 20;
        }
    }

    return space_count;
}


BOOL checkPerfectClearSplit(MINO_TYPE* board, int height)
{
    // �󂫃Z�������J�E���g���܂��B
    int space_count = 0;
    for (int x = 0; x < BOARD_W; x++)
    {
        // ���܂��Ă�Z�������J�E���g���܂��B
        int fill_count = 0;

        // �c�ǂ𒲂ׂ܂��B
        BOOL side_space = FALSE;
        int line_space = 0;

        BOOL side_space_left = x > 0;
        BOOL side_space_right = x < (BOARD_W - 1);

        for (int y = 0; y < height; y++)
        {
            // �󂫃Z���̃J�E���g���s���܂��B
            if ((board[x + (y * BOARD_W)]) == N)
            {
                space_count++;
                line_space++;

                // ���T�C�h�̂����ꂩ���󂫃Z�����ǂ����𒲂ׂ܂��B
                if (side_space_left && (board[(x - 1) + (y * BOARD_W)]) == N) side_space = TRUE;
                if (side_space_right && (board[(x + 1) + (y * BOARD_W)]) == N) side_space = TRUE;
            }

            // �~�m�Z���̃J�E���g���s���܂��B
            else fill_count++;
        }

        // �X�y�[�X������T�C�h���S�����܂��Ă���p�t�F�͂ł��܂���B
        if ((line_space > 0) && (side_space == FALSE)) { return FALSE; }

        // �c���߂��������ꍇ�A�ȑO�̃X�y�[�X��4�̔{���łȂ���΃p�t�F�͂ł��܂���B
        if (fill_count == height)
        {
            if ((space_count % 4) != 0) return FALSE;
            space_count = 0;
        }
    }

    // �Ō���������󂫃Z����4�̔{���łȂ��ƃp�t�F�ł��܂���B
    if ((space_count % 4) != 0) return FALSE;

    // �����܂ł���΃p�t�F�\�ł��B
    return TRUE;
}



// ���C�������]�����擾
void GetLineClear(EVAL_TABLE* evalT, TETRIS_DATA* tetris, double* out1)
{
    double attack = 0;
    *out1 = 0;

    /// ���������Ă��Ȃ���΃X�R�A0�ł��B
    if (tetris->prevResult.deleteLine == 0)
    {
        /// T�~�m�͉Η͂̑�������̂Ń}�C�i�X�]���ł��B
        if (tetris->prevControl.current == T) { *out1 = evalT->useT; }
        if (tetris->prevControl.current == I) { *out1 = evalT->useI; }

        return;
    }

    /// �p�t�F��ԁA�p�t�F�͍ő�
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

    /// REN�Η͂����Z���܂��B
    if (tetris->control.ren >= 1)
    {
        double ren = ((double)tetris->control.ren - 1);
        attack += ren * evalT->ren;
    }

    (*out1) = attack;
}

