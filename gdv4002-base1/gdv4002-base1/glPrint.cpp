
#include <stdio.h>
#include "glPrint.h"

// Current font glPrint will use
static GLuint currentFont = 0;

// Create a new font and return the OpenGL resource id.  This does not make the new font the current font for rendering.
GLuint glBuildFont(LPCWSTR fontName, int fontHeight, int style)
{
	HFONT		winFont = NULL;
	HDC			hDC = NULL;

	// Get device context
	hDC = GetDC(NULL);
	if (!hDC)
		return 0;

	// Generate and validate display list to hold font rendering data
	GLuint glFont = glGenLists(256);
	if (!glFont) {
		ReleaseDC(NULL, hDC);
		return 0;
	}

	// Create windows font
	winFont = CreateFont(-fontHeight,							// Height Of Font
		0,														// Width Of Font
		0,														// Angle Of Escapement
		0,														// Orientation Angle
		(style & GLFONT_STYLE::BOLD)? FW_BOLD : FW_NORMAL,		// Font Weight
		(style & GLFONT_STYLE::ITALIC)? TRUE : FALSE,			// Italic
		(style & GLFONT_STYLE::UNDERLINE)? TRUE : FALSE,		// Underline
		(style & GLFONT_STYLE::STRIKETHROUGH)? TRUE : FALSE,	// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,	// Family And Pitch
		fontName);

	if (!winFont) {

		glDeleteFont(glFont);
		ReleaseDC(NULL, hDC);
		return 0;
	}

	SelectObject(hDC, winFont); // Select current font
	wglUseFontBitmaps(hDC, 0, 256, glFont); // Builds characters

	// Release device context and dispose local resources
	ReleaseDC(NULL, hDC);
	DeleteObject(winFont);

	// Return id of newly create font (display list)
	return glFont;
}

// Set the current font to be used by glPrint.
void glSetCurrentFont(GLuint fontToSet) {

	currentFont = fontToSet;
}

// Delete the specified font and return true if deleted, false otherwise.  If the currently set font is the same as fontToDelete then this is set to 0.
bool glDeleteFont(GLuint fontToDelete)
{
	if (fontToDelete) {

		glDeleteLists(fontToDelete, 256);

		if (fontToDelete == currentFont)
			currentFont = 0;

		return true;
	}
	else {

		return false;
	}
}


// glPrint acts in a similar way to printf
void glPrint(const char *fmt, ...)
{
	char		text[256];
	va_list		ap;

	if (fmt == NULL)
		return;

	// Parse the string for variables
	va_start(ap, fmt);
	vsprintf_s(text, 256, fmt, ap);
	va_end(ap);

	// Render text
	glPushAttrib(GL_LIST_BIT);
	glListBase(currentFont);
	glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}


