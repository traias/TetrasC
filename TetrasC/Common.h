#pragma once

/// デバッグ出力です。
#define Debug(...)  {\
                        char buf[1024];\
                        sprintf_s(buf, __VA_ARGS__);\
                        OutputDebugString(buf);\
                    }

/// ウィンドウサイズです。
#define WINDOW_W    1280
#define WINDOW_H    720
