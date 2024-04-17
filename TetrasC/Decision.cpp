#include <vector>
#include "DxLib.h"
#include "Common.h"
#include "TetrisData.h"
#include "Decision.h"
#include "DecisionPattern.h"
#include "DecisionEval.h"
#include <algorithm>
using namespace std;



/// <summary>
/// �h���b�v�p�^�[�����擾���܂��B
/// </summary>
/// <param name="tetris"></param>
/// <param name="parent"></param>
/// <returns></returns>
static int getDropPatterns(TETRIS_DATA* tet, DECISION_TETRIS** pattern, DECISION_TETRIS* parent);

/// <summary>
// �S�h���b�v�p�^�[���̕]���l(����]��)�擾���܂�
/// </summary>
/// <param name="base"></param>
/// <param name="pattern"></param>
/// <param name="patternMax"></param>
/// <param name="evalT"></param>
/// <returns></returns>
static int getScoreList(TETRIS_DATA* base, DECISION_TETRIS** pattern, int patternMax, double* evalTable);


/// <summary>
/// ���Ԃ����{�[�h�̃f�[�^�𖳌��ɂ��܂��B
/// </summary>
/// <param name=""></param>
void distinctData(DECISION_TETRIS** pattern, int max);



void debugDrawBoard(DECISION_TETRIS** pattern, int max);

void debugDrawBoard(DECISION_TETRIS* pattern);

void debugDrawBoard(DECISION_TETRIS** pattern);



/// <summary>
/// �]���e�[�u���ł��B
/// </summary>
double evalTable[80] =
{
    /// �����̗�ɑ΂���d�݂ł��B(���E�Ώ̂Ȃ̂Ŕ��������ł��B)
    /// 0-4
    0.9,
    0.95,
    0.98,
    1,
    1,

    /// �����]��(-7, 8-14, 15-)
    /// 5-7
    -0.40,
    -0.10,
    -0.10,

    /// �����̍����ɑ΂���d�݂ł��B(���E�Ώ̂Ȃ̂Ŕ��������ł��B)
    /// 8-12
    1,
    2,
    1.5,
    1,
    1,

    /// �����̍����]��
    /// 13-14
    -0.15,
    -0.03,

    /// ��˂̗�ɑ΂���d�݂ł��B(���E�Ώ̂Ȃ̂Ŕ��������ł��B)
    /// 15-19
    -0.5,
    -0.7,
    -0.2,
    0.7,
    0.7,

    /// ��˂̐[���]��
    /// 20
    0.05,

    /// �\�ʌ`��̗�ɑ΂���d�݂ł��B(���E�Ώ̂Ȃ̂Ŕ��������ł��B)
    /// 21-23
    0.7,
    0.8,
    0.9,

    /// �\�ʌ`��]��
    /// 24-25
    -0.2,
    -0.2,

    /// ���A�]��
    /// 26-27
    -0.08,
    -0.05,

    /// �Ǖ]��
    /// 28-29
    -3.6,
    -0.3,

    /// �Η͒n�`�]��(TSD/TST)
    /// 30-39
    0.0,
    0.2,
    0.1,
    0.7,
    1.8,
    0.0,
    0.0,
    0.0,
    0.0,
    5.0,

    /// �z�[���h�]��
    /// 40-42
    0.05,
    0.10,
    -0.02,

    /// �p�t�F�n�`�]��
    /// 43
    0.5,

    /// �Η͕]��
    /// 44-63
    -3.00,
    -0.50,
    99,

    2.0,
    -2.0,
    -1.0,

    2.8,
    -1.5,
    -8.8,
    7.20,
    -1.0,
    -7.6,
    9,
    -7.4,
    4.5,
    0.6,

    3.0,
    0.1,
    0.07,
    20.0,

    /// �f�B���C�]��
    /// 64
    -0.01,

    /// Btb�]��
    /// 65
    0.5,

    /// �Η͏�Z�]��
    /// 66
    1.0,
};


/// <summary>
/// ���ڂ̃f�[�^�ł��B
/// </summary>
static DECISION_TETRIS* topPattern;

/// <summary>
/// ���ڂ̃f�[�^�ł��B
/// </summary>
static DECISION_TETRIS* secondPattern;

/// <summary>
/// �ӎv���肳�ꂽ�f�[�^�ł��B
/// </summary>
static DECISION_TETRIS* decisionPattern;

/// <summary>
/// �T���Ɏg�p����f�[�^�ł��B
/// </summary>
static DECISION_TETRIS* searchPattern;


/// <summary>
/// �T�����Ɏg�p����q���Ƃ��Ďg���f�[�^�ł��B
/// </summary>
static DECISION_TETRIS* childPattern;




int cmpQsort(const void* d1, const void* d2);




/// <summary>
/// �ӎv���蒆��TRUE�ɂȂ�܂��B
/// </summary>
static BOOL decisionRun = FALSE;


/// <summary>
/// ���f�t���O�ł��B
/// </summary>
static BOOL abortRun = FALSE;



/// <summary>
/// �ӎv����V�X�e���̏��������s���܂��B
/// ����̓������̓��I�m�ۂ��������ɍs��Ȃ��悤�ɂ��邽�߂ł��B
/// </summary>
void DecisionSystemInit(void)
{
    topPattern = (DECISION_TETRIS*)malloc(sizeof(DECISION_TETRIS) * 96);
    secondPattern = (DECISION_TETRIS*)malloc(sizeof(DECISION_TETRIS) * 96 * 96);
    childPattern = (DECISION_TETRIS*)malloc(sizeof(DECISION_TETRIS) * 96);
    searchPattern = (DECISION_TETRIS*)malloc(sizeof(DECISION_TETRIS) * 200 * 96);
}

/// <summary>
/// �ӎv����V�X�e���̉�����s���܂��B
/// ������Ăяo���ƈӎv����V�X�e���͎g�p�s�\�ɂȂ�܂��B
/// </summary>
void DecisionSystemRelease(void)
{
    free(searchPattern);
    free(childPattern);
    free(secondPattern);
    free(topPattern);
}


/// <summary>
/// �ӎv������s���܂��B
/// </summary>
void Decision(TETRIS_DATA* tet)
{
    decisionRun = TRUE;
    Debug("�ӎv������J�n���܂��B\n");

    // ���zNEXT���쐬���܂��B

    // ���ڂ����ׂĐ�ǂ݂��܂��B
    int patternCount = getDropPatterns(tet, &topPattern, NULL);
    Debug("���ڂ��`�F�b�N���܂����B[%d]�p�^�[��������܂����B\n", patternCount);

    // ���ڂ����ׂĐ�ǂ݂��܂��B
    int searchCount = 0;
    {
        for (int i = 0; i < patternCount; i++)
        {
            DECISION_TETRIS* second = &secondPattern[searchCount];

            searchCount += getDropPatterns(&topPattern[i].tetris, &second, &topPattern[i]);
        }
        Debug("���ڂ��`�F�b�N���܂����B[%d]�p�^�[��������܂����B\n", searchCount);

        // ������f�[�^���폜���܂��B
        //distinctData(&secondPattern, searchCount);

        // �]���l�Ń\�[�g���܂��B
        qsort(secondPattern, searchCount, sizeof(DECISION_TETRIS), cmpQsort);
        decisionPattern = &secondPattern[0];
    }

    // ���f���w�肳��Ă����炱�̃^�C�~���O�Œ��f���܂��B
    if (abortRun == TRUE)
    {
        decisionRun = FALSE;
        return;
    }

    // �O��ڈȍ~�𒲂ׂ܂��B���200�肾�����̗p���Ď��̎�����܂��B
    {
        int nextSearch = searchCount;
        if (nextSearch > 200)
        {
            nextSearch = 200;
        }

        int nextSearchCount = 0;
        for (int i = 0; i < nextSearch; i++)
        {
            // ���̎�����܂��B
            DECISION_TETRIS* thard = &searchPattern[nextSearchCount];
            nextSearchCount += getDropPatterns(&secondPattern[i].tetris, &thard, &secondPattern[i]);
        }

        Debug("�O��ڂ��`�F�b�N���܂����B[%d]�p�^�[��������܂����B\n", nextSearchCount);

        // ������f�[�^���폜���܂��B
        //distinctData(&searchPattern, nextSearchCount);

        // �]���l�Ń\�[�g���܂��B
        qsort(searchPattern, nextSearchCount, sizeof(DECISION_TETRIS), cmpQsort);
        decisionPattern = &searchPattern[0];
    }

    decisionRun = FALSE;
}


/// <summary>
/// �ӎv����̒��f���s���܂��B
/// </summary>
void decisionAbort()
{
    abortRun = TRUE;
}

/// <summary>
/// �h���b�v�p�^�[�����擾���܂��B
/// </summary>
/// <param name="tetris"></param>
/// <param name="parent"></param>
/// <returns></returns>
static int getDropPatterns(TETRIS_DATA* tet, DECISION_TETRIS** pattern, DECISION_TETRIS* parent)
{
    /// �J�����g�ƃz�[���h�Ŏ�肤�邷�ׂẴh���b�v�p�^�[���ƃX�R�A���擾���܂�
    int count = getScoreList(tet, pattern, 96, evalTable);

    /// �K�v�ȃf�[�^�̐e�q��ݒ肵�܂��B
    for (int i = 0; i < count; i++)
    {
        DECISION_TETRIS* d = &(*pattern)[i];

        if (d->enable == FALSE) { continue; }

        // �����NULL�Ȃ̂Ŏ������e�ɂȂ�܂��B
        if (parent == NULL)
        {
            d->top = d;
            d->parent = d;
        }
        else
        {
            d->top = parent->top;
            d->parent = parent;
        }
        d->child = NULL;
        d->childCount = 0;
    }

    return count;
}

/// <summary>
// �S�h���b�v�p�^�[���̕]���l(����]��)�擾���܂�
/// </summary>
/// <param name="base"></param>
/// <param name="pattern"></param>
/// <param name="patternMax"></param>
/// <param name="evalT"></param>
/// <returns></returns>
static int getScoreList(TETRIS_DATA* base, DECISION_TETRIS** pattern, int patternMax, double* evalTable)
{
    /// �Ƃ肠�����S�h���b�v�p�^�[�����擾���܂�
    int patternCount = GetDropPattern(base, pattern, patternMax);

    /// �h���b�v�p�^�[���ɑ΂��ăX�R�A��o�^���܂�
    for (int i = 0; i < patternCount; i++)
    {
        /// �s�v�ȃh���b�v�p�^�[���͉������܂���
        if ((*pattern)[i].enable == FALSE) continue;

        /// �h���b�v�p�^�[���ɑ΂��ĕ]����ݍ��݃f�[�^���쐬���܂�
        GetEvaluation(&(*pattern)[i].tetris, (*pattern)[i].eval.eval, evalTable);
        (*pattern)[i].totalEval = 0;
        for (int n = 0; n < 20; n++)
        {
            (*pattern)[i].totalEval += (*pattern)[i].eval.eval[n];
        }
    }

    return patternCount;
}

/// <summary>
/// �ӎv���肳�ꂽ�R�}���h���擾���܂��B
/// </summary>
/// <returns></returns>
char* getDecisionCommand(void)
{
    Debug("�@�ӎv���肵���R�}���h���v������܂����B\n");

    while (decisionRun == TRUE)
    {
        Sleep(0);
    }

    Debug("�@�ӎv���肵���R�}���h��Ԃ��܂��B[%s]�ł��B\n", decisionPattern->top->command);

    return decisionPattern->top->command;
}

/// <summary>
/// �ӎv���肳�ꂽ�f�[�^��Ԃ��܂��B
/// </summary>
/// <param name="tetris"></param>
void getDecisionTetrisData(TETRIS_DATA *tetris)
{
    Debug("�@�ӎv���肵���e�g���X�f�[�^���v������܂����B\n");

    while (decisionRun == TRUE)
    {
        Sleep(0);
    }

    Debug("�@�ӎv���肵���e�g���X�f�[�^��Ԃ��܂��B\n");

    memcpy_s(tetris, sizeof(TETRIS_DATA), &decisionPattern->top->tetris, sizeof(TETRIS_DATA));
}

/// <summary>
/// ���Ԃ����{�[�h�̃f�[�^�𖳌��ɂ��܂��B
/// </summary>
/// <param name=""></param>
void distinctData(DECISION_TETRIS** pattern, int max)
{
    for (int i = 0; i < (max - 1); i++)
    {
        if ((*pattern)[i].enable == FALSE) { continue; }

        for (int n = (i + 1); n < max; n++)
        {
            if ((*pattern)[n].enable == FALSE) { continue; }

            // �����{�[�h������
            if (
                ((*pattern)[i].boardHash[0] == (*pattern)[n].boardHash[0]) &&
                ((*pattern)[i].boardHash[1] == (*pattern)[n].boardHash[1]) &&
                ((*pattern)[i].boardHash[2] == (*pattern)[n].boardHash[2]) &&
                ((*pattern)[i].boardHash[3] == (*pattern)[n].boardHash[3])
                )
            {
                /// �f�B���C���Z���f�[�^���̗p���܂�
                if ((*pattern)[i].tetris.result.Delay > (*pattern)[n].tetris.result.Delay)
                {
                    (*pattern)[i].enable = FALSE;
                }
                else if ((*pattern)[i].tetris.result.Delay < (*pattern)[n].tetris.result.Delay)
                {
                    (*pattern)[n].enable = FALSE;
                }
                else
                {
                    (*pattern)[n].enable = FALSE;
                }
            }
        }
    }
}



int cmpQsort(const void* d1, const void* d2)
{
    if (((DECISION_TETRIS*)d1)->enable == FALSE)
    {
        return 1;
    }
    if (((DECISION_TETRIS*)d2)->enable == FALSE)
    {
        return -1;
    }

    if ((((DECISION_TETRIS*)d1)->totalEval) > (((DECISION_TETRIS*)d2)->totalEval))
    {
        return -1;
    }
    if ((((DECISION_TETRIS*)d1)->totalEval) < (((DECISION_TETRIS*)d2)->totalEval))
    {
        return 1;
    }

    return 0;
}



void debugDrawBoard(DECISION_TETRIS** pattern, int max)
{
    for (int i = 0; i < max; i++)
    {
        if ((*pattern)[i].enable == FALSE) { continue; }

        TETRIS_DATA *t = &(*pattern)[i].tetris;

        Debug("��%s\n", (*pattern)[i].command);
        Debug("eval=%lf\n", (*pattern)[i].totalEval);
        Debug("current(%d), x=%d, y=%d, r=%d\n", t->prevControl.current, t->prevControl.pos.x, t->prevControl.pos.y, t->prevControl.r);
        Debug("HASH = %I64X, %I64X, %I64X, %I64X\n", (*pattern)[i].boardHash[0], (*pattern)[i].boardHash[1], (*pattern)[i].boardHash[2], (*pattern)[i].boardHash[3]);

        for (int y = 8; y >= 0; y--)
        {
            char line[11] = { 0 };
            for (int x = 0; x < BOARD_W; x++)
            {
                if (t->board[x + (y * BOARD_W)] == 0)
                {
                    line[x] = '_';
                }
                else
                {
                    line[x] = 'X';
                }
            }
            Debug("%s\n", line);
        }
        Debug("\n");
    }
}


void debugDrawBoard(DECISION_TETRIS* pattern)
{
    if (pattern->enable == FALSE) { return; }

    TETRIS_DATA* t = &pattern->tetris;

    Debug("��%s\n", pattern->command);
    Debug("eval=%lf\n", pattern->totalEval);
    Debug("current(%d), x=%d, y=%d, r=%d\n", t->prevControl.current, t->prevControl.pos.x, t->prevControl.pos.y, t->prevControl.r);

    for (int y = 8; y >= 0; y--)
    {
        char line[11] = { 0 };
        for (int x = 0; x < BOARD_W; x++)
        {
            if (t->board[x + (y * BOARD_W)] == 0)
            {
                line[x] = '_';
            }
            else
            {
                line[x] = 'X';
            }
        }
        Debug("%s\n", line);
    }
    Debug("\n");
}


void debugDrawBoard(DECISION_TETRIS** pattern)
{
    DECISION_TETRIS* p = NULL;

    do
    {
        if (p == NULL)
        {
            p = (*pattern);
        }
        else
        {
            p = p->parent;
        }

        if (p->enable == FALSE) { return; }

        TETRIS_DATA* t = &p->tetris;

        Debug("��%s\n", p->command);
        Debug("eval=%lf\n", p->totalEval);
        Debug("current(%d), x=%d, y=%d, r=%d\n", t->prevControl.current, t->prevControl.pos.x, t->prevControl.pos.y, t->prevControl.r);

        for (int y = 8; y >= 0; y--)
        {
            char line[11] = { 0 };
            for (int x = 0; x < BOARD_W; x++)
            {
                if (t->board[x + (y * BOARD_W)] == 0)
                {
                    line[x] = '_';
                }
                else
                {
                    line[x] = 'X';
                }
            }
            Debug("%s\n", line);
        }
        Debug("\n");

    } while (p->parent != p);
}

