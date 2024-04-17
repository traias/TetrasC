#pragma once

#include "TetrisData.h"
#include "DecisionEval.h"





typedef struct tag_DECISION_TETRIS
{
    // �L������
    BOOL enable;

    // �e�g���X�f�[�^�ł��B
    TETRIS_DATA tetris;

    // �]���l�ł��B
    EVAL eval;

    // �����]���l�ł��B
    double totalEval;

    // �ݒu���̃n�b�V���R�[�h�ł��B
    // [�~�m��:8bit][��]:4bit][[�Z���̍��Wy:8bit,x:4bit]*4]
    __int64 hash;

    // �{�[�h�̃n�b�V���R�[�h�ł��B���ׂăr�b�g�ŕ\�����Ă��܂��B
    __int64 boardHash[4];

    // ���̓R�}���h�ł��B
    char command[32];

    // �n�[�h�h���b�v�����Z�����ł��B
    int harddropCell;

    // �e�̃|�C���^�ł��B�e�����Ȃ���null�ł��B
    tag_DECISION_TETRIS* parent;

    // ��Ԃ̐e�f�[�^�ł��B
    // ���̃f�[�^�͌��ݔՖʂ̎��ɂȂ�܂��B
    tag_DECISION_TETRIS* top;

    // �q���̃f�[�^�ł��B��������܂��B
    tag_DECISION_TETRIS* child;
    int childCount;

} DECISION_TETRIS;



/// <summary>
/// �ӎv����V�X�e���̏��������s���܂��B
/// ����̓������̓��I�m�ۂ��������ɍs��Ȃ��悤�ɂ��邽�߂ł��B
/// </summary>
extern void DecisionSystemInit(void);

/// <summary>
/// �ӎv����V�X�e���̉�����s���܂��B
/// ������Ăяo���ƈӎv����V�X�e���͎g�p�s�\�ɂȂ�܂��B
/// </summary>
extern void DecisionSystemRelease(void);

/// <summary>
/// �ӎv������s���܂��B
/// ���O�ɏ��������ĂԕK�v������܂��B
/// </summary>
/// <param name="tet"></param>
extern void Decision(TETRIS_DATA* tet);


/// <summary>
/// �ӎv����̒��f���s���܂��B
/// </summary>
extern void decisionAbort();


/// <summary>
/// �ӎv���肳�ꂽ�R�}���h���擾���܂��B
/// </summary>
/// <returns></returns>
extern char* getDecisionCommand(void);

/// <summary>
/// �ӎv���肳�ꂽ�f�[�^��Ԃ��܂��B
/// </summary>
/// <param name="tetris"></param>
extern void getDecisionTetrisData(TETRIS_DATA* tetris);
