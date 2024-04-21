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
static int getScoreList(TETRIS_DATA* base, DECISION_TETRIS** pattern, int patternMax, EVAL_TABLE* evalTable);

void debugDrawBoard(DECISION_TETRIS** pattern, int max);

void debugDrawBoard(DECISION_TETRIS* pattern);

void debugDrawBoard(DECISION_TETRIS** pattern);


/// <summary>
/// �]���e�[�u���ł��B
/// </summary>
static EVAL_TABLE evalTable;


/// <summary>
/// ���ڂ̃f�[�^�ł��B
/// </summary>
static DECISION_TETRIS* topPattern;

/// <summary>
/// ���ڂ̃f�[�^�ł��B
/// </summary>
static DECISION_TETRIS* secondPattern;

/// <summary>
/// �O��ڈȍ~�̃f�[�^�ł��B
/// (�ő�12��܂ł�Ώۂɂ��܂��B)
/// </summary>
static DECISION_TETRIS* searchPattern[10];

/// <summary>
/// �ӎv���肳�ꂽ�f�[�^�ł��B
/// </summary>
static DECISION_TETRIS* decisionPattern;





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
    topPattern = (DECISION_TETRIS*)calloc(96, sizeof(DECISION_TETRIS));
    secondPattern = (DECISION_TETRIS*)calloc(96 * 96, sizeof(DECISION_TETRIS));
    for (int i = 0; i < 10; i++)
    {
        searchPattern[i] = (DECISION_TETRIS*)calloc(200 * 96, sizeof(DECISION_TETRIS));
    }

    // �f�t�H���g�̕]���e�[�u�����擾���܂��B
    setEvalTableDefault(&evalTable);
}

/// <summary>
/// �ӎv����V�X�e���̉�����s���܂��B
/// ������Ăяo���ƈӎv����V�X�e���͎g�p�s�\�ɂȂ�܂��B
/// </summary>
void DecisionSystemRelease(void)
{
    for (int i = 0; i < 10; i++)
    {
        free(searchPattern[i]);
    }
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
            if (topPattern[i].enable == FALSE) { continue; }
            DECISION_TETRIS* p = &secondPattern[searchCount];
            searchCount += getDropPatterns(&topPattern[i].tetris, &p, &topPattern[i]);
        }
        Debug("���ڂ��`�F�b�N���܂����B[%d]�p�^�[��������܂����B\n", searchCount);

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
    int prevSearchCount = searchCount;
    DECISION_TETRIS* prevPattern = secondPattern;
    for (int next = 0; next < 10; next++)
    {
        DECISION_TETRIS* pattern = searchPattern[next];

        int nextSearch = prevSearchCount;
        if (nextSearch > 200)
        {
            nextSearch = 200;
        }

        int nextSearchCount = 0;
        for (int i = 0; i < nextSearch; i++)
        {
            if (prevPattern[i].enable == FALSE) { continue; }
            DECISION_TETRIS* p = &pattern[nextSearchCount];
            nextSearchCount += getDropPatterns(&prevPattern[i].tetris, &p, &prevPattern[i]);
        }

        Debug("%d��ڂ��`�F�b�N���܂����B[%d]�p�^�[��������܂����B\n", next + 3, nextSearchCount);

        // �]���l�Ń\�[�g���܂��B
        qsort(pattern, nextSearchCount, sizeof(DECISION_TETRIS), cmpQsort);

        // ��ԕ]���̍����f�[�^��ێ����܂��B
        decisionPattern = &pattern[0];

        // �O��f�[�^�Ƃ��č���̃f�[�^��ێ����܂��B
        prevSearchCount = nextSearchCount;
        prevPattern = searchPattern[next];

        // ���f���w�肳��Ă����炱�̃^�C�~���O�Œ��f���܂��B
        if (abortRun == TRUE)
        {
            decisionRun = FALSE;
            return;
        }
    }

    debugDrawBoard(&decisionPattern);
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
    int count = getScoreList(tet, pattern, 96, &evalTable);

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
            d->totalEval += d->parent->totalEval;
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
static int getScoreList(TETRIS_DATA* base, DECISION_TETRIS** pattern, int patternMax, EVAL_TABLE* evalTable)
{
    /// �Ƃ肠�����S�h���b�v�p�^�[�����擾���܂�
    int patternCount = GetDropPattern(base, pattern, patternMax);

    /// �h���b�v�p�^�[���ɑ΂��ăX�R�A��o�^���܂�
    for (int i = 0; i < patternCount; i++)
    {
        /// �s�v�ȃh���b�v�p�^�[���͉������܂���
        if ((*pattern)[i].enable == FALSE) continue;

        /// �h���b�v�p�^�[���ɑ΂��ĕ]����ݍ��݃f�[�^���쐬���܂�
        GetEvaluation(&(*pattern)[i].tetris, (*pattern)[i].eval, evalTable);
        (*pattern)[i].totalEval = 0;
        for (int n = 0; n < 20; n++)
        {
            (*pattern)[i].totalEval += (*pattern)[i].eval[n];
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
                switch (t->board[x + (y * BOARD_W)])
                {
                case N: line[x] = '_'; break;
                case I: line[x] = 'I'; break;
                case J: line[x] = 'J'; break;
                case L: line[x] = 'L'; break;
                case O: line[x] = 'O'; break;
                case S: line[x] = 'S'; break;
                case T: line[x] = 'T'; break;
                case Z: line[x] = 'Z'; break;
                case G: line[x] = 'G'; break;
                case C: line[x] = 'C'; break;
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

    TETRIS_DATA* t = &pattern->top->tetris;

    Debug("��%s\n", pattern->top->command);
    Debug("eval=%lf\n", pattern->top->totalEval);
    Debug("current(%d), x=%d, y=%d, r=%d\n", t->prevControl.current, t->prevControl.pos.x, t->prevControl.pos.y, t->prevControl.r);

    for (int y = 8; y >= 0; y--)
    {
        char line[11] = { 0 };
        for (int x = 0; x < BOARD_W; x++)
        {
            switch (t->board[x + (y * BOARD_W)])
            {
            case N: line[x] = '_'; break;
            case I: line[x] = 'I'; break;
            case J: line[x] = 'J'; break;
            case L: line[x] = 'L'; break;
            case O: line[x] = 'O'; break;
            case S: line[x] = 'S'; break;
            case T: line[x] = 'T'; break;
            case Z: line[x] = 'Z'; break;
            case G: line[x] = 'G'; break;
            case C: line[x] = 'C'; break;
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
        Debug("total eval=%lf\n", p->totalEval);
        for (int i = 0; i < 20; i++)
        {
            Debug("%s=%.02lf, ", EvalName[i], p->eval[i]);
        }
        Debug("\n");
        Debug("current(%d), x=%d, y=%d, r=%d\n", t->prevControl.current, t->prevControl.pos.x, t->prevControl.pos.y, t->prevControl.r);

        for (int y = 8; y >= 0; y--)
        {
            char line[11] = { 0 };
            for (int x = 0; x < BOARD_W; x++)
            {
                switch (t->board[x + (y * BOARD_W)])
                {
                case N: line[x] = '_'; break;
                case I: line[x] = 'I'; break;
                case J: line[x] = 'J'; break;
                case L: line[x] = 'L'; break;
                case O: line[x] = 'O'; break;
                case S: line[x] = 'S'; break;
                case T: line[x] = 'T'; break;
                case Z: line[x] = 'Z'; break;
                case G: line[x] = 'G'; break;
                case C: line[x] = 'C'; break;
                }
            }
            Debug("%s\n", line);
        }
        Debug("\n");

    } while (p->parent != p);
}

