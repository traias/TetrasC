#pragma once

/// �f�o�b�O�o�͂ł��B
#define Debug(...)  {\
                        char buf[1024];\
                        sprintf_s(buf, __VA_ARGS__);\
                        OutputDebugString(buf);\
                    }

/// �E�B���h�E�T�C�Y�ł��B
#define WINDOW_W    1280
#define WINDOW_H    720
