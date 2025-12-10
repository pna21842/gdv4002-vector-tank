
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "GraphicsCore.h"

enum GLFONT_STYLE {BOLD = 0x01, ITALIC = 0x02, UNDERLINE = 0x04, STRIKETHROUGH = 0x08 };

GLuint glBuildFont(LPCWSTR fontName, int fontHeight, int style = 0);
void glSetCurrentFont(GLuint fontToSet);
bool glDeleteFont(GLuint fontToDelete);
void glPrint(const char *fmt, ...);

