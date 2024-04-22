#include <stdio.h>
#include "TetrisData.h"
#include "Decision.h"
#include "DecisionPattern.h"



/// 定数宣言です
/// このファイルでしか使用しない定数を宣言します


#define BASE_MOVE_MAX       43

/// ベースになるドロップコマンドです。
const char bsseDropCommand[BASE_MOVE_MAX][32]
{
    "lllllu",
    "llllu",
    "lllu",
    "llu",
    "lu",
    "u",
    "ru",
    "rru",
    "rrru",
    "rrrru",
    "rrrrru",
    "Rlllllu",
    "Rllllu",
    "Rlllu",
    "Rllu",
    "Rlu",
    "Ru",
    "Rru",
    "Rrru",
    "Rrrru",
    "Rrrrru",
    "Lllllu",
    "Llllu",
    "Lllu",
    "Llu",
    "Lu",
    "Lru",
    "Lrru",
    "Lrrru",
    "Lrrrru",
    "Lrrrrru",
    "RlRllllu",
    "RlRlllu",
    "RlRllu",
    "RlRlu",
    "RlRu",
    "RRu",
    "RrRu",
    "RrRru",
    "RrRrru",
    "RrRrrru",
    "RrRrrrru"
    "LLu",
};

/// ここから関数プロトタイプ宣言です

/// ドロップ全パターンをテストします
/// ドロップ全パターンをテストします。
int GetDropAllPattern(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int indexStart, int indexMax, const char prefix);

/// ベースになるドロップ状態を全データ作成します。
int BaseDropTest(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int indexStart, int indexMax, const char* command);

/// ドロップ状態を全データ作成します。
int DropTest(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int registCount, int PatternMax, char* command, const char* addCommand);

/// すでにパターン登録済みかどうかをチェックします
BOOL findRegistPattern(DECISION_TETRIS* check, DECISION_TETRIS** pattern, int start, int end);

/// パターン登録を行います
void registPattern(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int index, char* command, int harddropCell);

/// ハッシュコードを得ます
unsigned __int64 getHash(MINO_TYPE mino, int r, POINT* cell);

/// 形状ハッシュコードが同一のデータを削除します
void distinctHash(DECISION_TETRIS** pattern, int start, int end);

/// ドロップに関するディレイを取得します
int getDropDelay(TETRIS_DATA* Base, POINT* MinoPoint, int R);


/// 関数の実装です

/// 現在の盤面から次の盤面に向かう全ドロップパターンを取得します。
int GetDropPattern(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int patternMax)
{
    if (tetris->control.current == N) return 0;

    /// ベースデータを変更するため、ベースをコピーします
    TETRIS_DATA copyTetris;
    memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));

    /// ドロップパターンを検索します
    int registCount = GetDropAllPattern(&copyTetris, pattern, 0, patternMax, ' ');

    /// 設置形状が同一なデータを重複として削除します。
    distinctHash(pattern, 0, registCount);

    /// ホールド処理です。
    memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));
    if ((copyTetris.hold != copyTetris.control.current))
    {
        /// ホールドの取り出しを行います
        Hold(&copyTetris);

        /// 取り出した結果ホールドが使えなかった場合は処理しません
        if (copyTetris.control.current != N)
        {
            int prevRegistCount = registCount;

            /// ドロップパターンを検索します
            registCount = GetDropAllPattern(&copyTetris, pattern, registCount, patternMax, 'h');

            /// 設置形状が同一なデータを重複として削除します。
            distinctHash(pattern, prevRegistCount, registCount);
        }
    }

    /// 余ったデータは無効にしておきます。
    for (int i = registCount; i < patternMax; i++)
    {
        (*pattern)[i].enable = FALSE;
    }

    return registCount;
}




/// ドロップ全パターンをテストします。
int GetDropAllPattern(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int startIndex, int patternMax, const char prefix)
{
    /// ミノを初期位置に設定します。
    GetMinoPisitionStart(tetris->board, tetris->control.current, &tetris->control.pos);

    /// 最初からぶつかっていたら終了します。
    {
        POINT cell[4];
        GetCellPoints(tetris->control.current, &tetris->control.pos, tetris->control.r, cell);
        if (CheckCollision(tetris->board, cell) == TRUE) { return 0; }
    }

    // 登録したデータの数です。
    int registCount = startIndex;

    /// 最初はベースを登録します
    /// ミノ出現直後の動きです
    for (int i = 0; i < BASE_MOVE_MAX; i++)
    {
        if (registCount >= (patternMax - 1)) { return registCount; }

        // 動かすコマンドを用意します。
        char command[32] = { 0 };
        int index = 0;
        if (prefix == 'h')
        {
            command[0] = prefix;
            index++;
        }
        strcpy_s(&command[index], 32 - 1 - index, (const char*)bsseDropCommand[i]);

        // ベースデータのコピーをしておきます。
        // このコピーを動かしてデータを作成します。
        TETRIS_DATA copyTetris;
        memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));

        registCount = BaseDropTest(&copyTetris, pattern, registCount, patternMax, command);
    }

    /// 登録されるデータベースのカウントです。
    if (registCount >= (patternMax - 1)) { return registCount; }
    
    /// ハードドロップの代わりにソフドロを入れ、設置後の動きを登録します
    int checkRegistCount = startIndex;
    int prevRegistCount = registCount;
    do
    {
        prevRegistCount = registCount;
        for (int i = checkRegistCount; i < prevRegistCount; i++)
        {
            if ((*pattern)[i].enable == FALSE) { continue; }

            // 途中までのコマンドを用意する
            char command[32] = { 0 };
            strcpy_s(command, 32, (*pattern)[i].command);

            command[strlen(command) - 1] = '\0';
            if ((*pattern)[i].harddropCell > 0)
            {
                strcat_s(command, 32, "d");
            }

            // ドロップのテストを実行[回転]
            {
                const char* addCommand[2] = { "Lu", "Ru" };

                for (int n = 0; n < 2; n++)
                {
                    // 結果のデータから探索用データを復旧させる
                    TETRIS_DATA copyTetris;
                    memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));
                    memcpy_s(&copyTetris.control, sizeof(CONTROL), &(*pattern)[i].tetris.prevControl, sizeof(CONTROL));
                    copyTetris.control.delay += (*pattern)[i].harddropCell * 3;

                    int regist = DropTest(&copyTetris, pattern, registCount, patternMax, command, addCommand[n]);
                    if (regist == registCount) { continue; }

                    // 登録したものが前回と同一データに存在していたら処理しません。
                    if (findRegistPattern(&(*pattern)[registCount], pattern, startIndex, registCount) == TRUE)
                    {
                        (*pattern)[registCount].enable = FALSE;
                    }
                    else
                    {
                        registCount = regist;
                    }
                }
            }

            // ドロップのテストを実行[左移動]
            {
                const char* addCommand[6] = { "lu", "llu", "lllu", "llllu", "lllllu", "llllllu" };

                for (int n = 0; n < 6; n++)
                {
                    // 結果のデータから探索用データを復旧させる
                    TETRIS_DATA copyTetris;
                    memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));
                    memcpy_s(&copyTetris.control, sizeof(CONTROL), &(*pattern)[i].tetris.prevControl, sizeof(CONTROL));
                    copyTetris.control.delay += (*pattern)[i].harddropCell * 3;

                    int regist = DropTest(&copyTetris, pattern, registCount, patternMax, command, addCommand[n]);
                    if (regist == registCount) { break; }

                    // 登録したものが前回と同一データに存在していたら処理しません。
                    if (findRegistPattern(&(*pattern)[registCount], pattern, startIndex, registCount) == TRUE)
                    {
                        (*pattern)[registCount].enable = FALSE;
                    }
                    else
                    {
                        registCount = regist;
                    }
                }
            }

            {
                // ドロップのテストを実行[右移動]
                const char* addCommand[6] = { "ru", "rru", "rrru", "rrrru", "rrrrru", "rrrrrru" };

                for (int n = 0; n < 6; n++)
                {
                    // 結果のデータから探索用データを復旧させる
                    TETRIS_DATA copyTetris;
                    memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));
                    memcpy_s(&copyTetris.control, sizeof(CONTROL), &(*pattern)[i].tetris.prevControl, sizeof(CONTROL));
                    copyTetris.control.delay += (*pattern)[i].harddropCell * 3;

                    int regist = DropTest(&copyTetris, pattern, registCount, patternMax, command, addCommand[n]);
                    if (regist == registCount) { break; }

                    // 登録したものが前回と同一データに存在していたら処理しません。
                    if (findRegistPattern(&(*pattern)[registCount], pattern, startIndex, registCount) == TRUE)
                    {
                        (*pattern)[registCount].enable = FALSE;
                    }
                    else
                    {
                        registCount = regist;
                    }
                }
            }
        }
        checkRegistCount = prevRegistCount;
    } while (prevRegistCount != registCount);

    return registCount;
}

/// ベースになるドロップ状態を全データ作成します。
int BaseDropTest(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int registCount, int patternMax, const char* command)
{
    // コマンド通りに動かします。
    for (int i = 0; command[i]; i++)
    {
        switch (command[i])
        {
            case 'L':
                if (MoveB(tetris) == FALSE) { return registCount; }
                break;

            case 'R':
                if (MoveA(tetris) == FALSE) { return registCount; }
                break;

            case 'l':
                if (MoveLeft(tetris) == FALSE) { return registCount; }
                break;

            case 'r':
                if (MoveRight(tetris) == FALSE) { return registCount; }
                break;

            // 最下段まで落として登録して終わりにする
            case 'u':
                int dropCount = MoveDD(tetris);

                /// 新規登録です
                registPattern(tetris, pattern, registCount, (char*)command, dropCount);
                return registCount + 1;
        }
    }

    return registCount;
}

/// ドロップ状態を全データ作成します。
int DropTest(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int registCount, int PatternMax, char* command, const char* addCommand)
{
    // コマンド通りに動かします。
    for (int i = 0; addCommand[i]; i++)
    {
        switch (addCommand[i])
        {
        case 'L':
            if (MoveB(tetris) == FALSE) { return registCount; }
            break;

        case 'R':
            if (MoveA(tetris) == FALSE) { return registCount; }
            break;

        case 'l':
            if (MoveLeft(tetris) == FALSE) { return registCount; }
            break;

        case 'r':
            if (MoveRight(tetris) == FALSE) { return registCount; }
            break;

            // 最下段まで落として登録して終わりにする
        case 'u':
            int dropCount = MoveDD(tetris);

            /// 新規登録です
            char c[32] = { 0 };
            strcpy_s(c, 32, command);
            strcat_s(c, 32, addCommand);
            registPattern(tetris, pattern, registCount, c, dropCount);
            return registCount + 1;
        }
    }
    
    return registCount;
}

/// すでにパターン登録済みかどうかをチェックします
BOOL findRegistPattern(DECISION_TETRIS* check, DECISION_TETRIS** pattern, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        if ((*pattern)[i].enable == FALSE) { continue; }
        if ((*pattern)[i].hash == check->hash)
        {
            return TRUE;
        }
    }

    return FALSE;
}


/// パターン登録を行います
void registPattern(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int index, char* command, int harddropCell)
{
    /// データを複製します。
    memcpy_s(&(*pattern)[index].tetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));

    // 設置します。
    MovePut(&(*pattern)[index].tetris);
    finishDrop(&(*pattern)[index].tetris);
    CorrectBoard((*pattern)[index].tetris.board);
    newMino(&(*pattern)[index].tetris);

    (*pattern)[index].harddropCell = harddropCell;
    strcpy_s((*pattern)[index].command, 32, command);
    {
        POINT cell[4];
        GetCellPoints((*pattern)[index].tetris.prevControl.current, &(*pattern)[index].tetris.prevControl.pos, (*pattern)[index].tetris.prevControl.r, cell);
        (*pattern)[index].hash = getHash((*pattern)[index].tetris.prevControl.current, (*pattern)[index].tetris.prevControl.r, cell);
    }

    /// データを有効に設定します
    (*pattern)[index].enable = TRUE;
}



/// ハッシュコードを得ます
unsigned __int64 getHash(MINO_TYPE mino, int r, POINT* cell)
{
    return (
        ((((unsigned __int64)mino) << 56UL) & 0xFF00000000000000) |
        ((((unsigned __int64)r) << 48UL) & 0x00FF000000000000) |
        ((((unsigned __int64)cell[3].y) << 40UL) & 0x0000FF0000000000) |
        ((((unsigned __int64)cell[3].x) << 36UL) & 0x000000F000000000) |
        ((((unsigned __int64)cell[2].y) << 28UL) & 0x0000000FF0000000) |
        ((((unsigned __int64)cell[2].x) << 24UL) & 0x000000000F000000) |
        ((((unsigned __int64)cell[1].y) << 16UL) & 0x0000000000FF0000) |
        ((((unsigned __int64)cell[1].x) << 12UL) & 0x000000000000F000) |
        ((((unsigned __int64)cell[0].y) << 4UL) & 0x0000000000000FF0) |
        ((((unsigned __int64)cell[0].x) << 0UL) & 0x000000000000000F)
        );
}

/// ハッシュが同一のデータを削除します
void distinctHash(DECISION_TETRIS** pattern, int start, int end)
{
    for (int i = start; i < (end - 1); i++)
    {
        if ((*pattern)[i].enable == FALSE) { continue; }

        for (int n = (i + 1); n < end; n++)
        {
            if ((*pattern)[n].enable == FALSE) { continue; }

            /// 回転を除く同一のデータが見つかった
            if (((*pattern)[i].hash & 0xFF00FFFFFFFFFFFF) == ((*pattern)[n].hash & 0xFF00FFFFFFFFFFFF))
            {
                /// ディレイが短いデータを採用します
                if ((*pattern)[i].tetris.prevResult.Delay > (*pattern)[n].tetris.prevResult.Delay)
                {
                    (*pattern)[i].enable = FALSE;
                }
                else if ((*pattern)[i].tetris.prevResult.Delay < (*pattern)[n].tetris.prevResult.Delay)
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


/// ドロップに関するディレイを取得します
int getDropDelay(TETRIS_DATA* tetris, POINT* pos, int r)
{
    if (pos == NULL) return 0;

    /// ミノ配置をしたボードを用意します。
    MINO_TYPE Board[BOARD_W * BOARD_H] = { N };
    memcpy_s(Board, sizeof(MINO_TYPE) * BOARD_W * BOARD_H, tetris->board, sizeof(MINO_TYPE) * BOARD_W * BOARD_H);
    putBoard(Board, tetris->control.current, tetris->control.r, pos);

    // そろった行を削除します。
    int deleteLine = CorrectBoard(Board);

    BOOL pc = TRUE;
    for (int x = 0; x < BOARD_W; x++)
    {
        if (Board[x] != N) { pc = FALSE; break; }
    }

    /// ディレイを計算します。
    int delay = 0;
    if (pc == TRUE)
    {
        delay += 1;
    }
    else
    {
        switch (deleteLine)
        {
        case 1:
            delay += 40;
            break;
        case 2:
        case 3:
            delay += 45;
            break;
        case 4:
            delay += 50;
            break;
        }
    }

    return delay;
}
