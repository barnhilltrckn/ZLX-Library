#include "Console.h"
#include <stdarg.h>
#ifdef LIBXENON
#include "Verdana_16_abc.h"
#include "Verdana_16_png.h"
#endif

static ZLXTexture * pFontTexture = NULL;

namespace ZLX {

    Console::Console(void) {
        // Clean all vars
        m_scrollOffset = 0;
        m_currentLine = 0;
        m_currentCol = 0;

        // Enable automatic render at format
        autoRender = 1;
    }

    Console::~Console(void) {
        // Destroy();
    }

    /**
     * Create a new console instance
     * @param pFontPath
     * @param backColor
     * @param textColor
     * @return 
     */
    int Console::Create(const char * pRessourcePath, ZLXColor backColor, ZLXColor textColor) {

        // Initilise video only 1 time
        InitialiseVideo();

        // Global access to video device
        m_pVideoDevice = g_pVideoDevice;

        // Save Color
        m_backColor = backColor;
        m_textColor = textColor;

        // Load font data
        void * pFontData = NULL;
        unsigned int fontDataLength;

        char stmp[256];

        sprintf(stmp,"%s%s",pRessourcePath,"font.abc");

        LoadFile(stmp, &pFontData, &fontDataLength);

        //printf("Verdana_16_abc_size => 0%08x\r\n",Verdana_16_abc_size);
#ifndef LIBXENON
        if (pFontData == NULL) {
            FatalError("Font.abc not found");
            return X_FAIL;
        }
#else
        //printf("Verdana_16_abc_size => 0%08x\r\n",Verdana_16_abc_size);
        // if abc file is not found
        if (pFontData == NULL) {
            pFontData = (void*)Verdana_16_abc;
        }
#endif
        sprintf(stmp,"%s%s",pRessourcePath,"font.png");

        LoadTextureFromFile(m_pVideoDevice, stmp, &pFontTexture);
#ifdef LIBXENON
        if (pFontTexture == NULL) {
            printf("Load Verdana_16_png file");
            extern struct XenosSurface * loadPNGFromMemory(unsigned char *PNGdata);
            pFontTexture = loadPNGFromMemory((unsigned char*) Verdana_16_png);
            printf("Load Verdana_16_png file ok");
        }
#endif
        if (pFontTexture == NULL) {
            FatalError("Can't load texture font");
            return X_FAIL;
        }

        m_font.Create(pFontTexture, pFontData);

        // Get the size of a simple caracteres
        float w, h;
        m_font.GetTextExtent("i", &w, &h, false);

        m_lineHeight = h;

        // Create the buffers
        n_col = GetScreenWidth() / w;
        n_lines = GetScreenHeight() / h;

        //m_pBuffer = new char[(n_col*n_lines)]; //(char*)malloc(n_col*n_lines);
        m_pLines = new char * [n_lines];

        for (unsigned int i = 0; i < n_lines; i++) {
            m_pLines[i] = new char [n_col];
            memset(m_pLines[i], 0, n_col);
        }
        return X_OK;
    }

    void Console::newLine() {
        if (m_currentLine == (n_lines - 2)) {
            ScrollUp(1);
        } else {
            m_currentLine++;

        }
        m_currentCol = 0;
        memset(m_pLines[m_currentLine], 0, n_col);

    }

    void Console::ScrollUp(int nline) {
        // Copy line per line

        // Lines[x-1]=lines[x]
        for (unsigned int i = 0; i < n_lines - 1; i++) {
            memcpy(m_pLines[i], m_pLines[i + 1], n_col);
        }

    }

    void Console::Clear() {
        // Erase all text data
        for (unsigned int i = 0; i < n_lines; i++) {
            memset(m_pLines[i], 0, n_col);
        }
        m_currentLine = 0;
    }

    void Console::Add(char c) {
        // Add the current letter to the buffer
        if (c == '\r') {
            return;
        }
        if (c == '\n') {
            newLine();
            return;
        }

        m_pLines[m_currentLine][m_currentCol] = c;
        m_currentCol++;
    }

    /**
     * Output(render) a string
     * @param format
     * @param ...
     */
    void Console::Format(const char * format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        // Output the string to the console
        unsigned int uStringLength = strlen(buffer);
        for (unsigned int i = 0; i < uStringLength; i++) {
            Add(buffer[i]);
        }
        // Next Format will be on a new line
        newLine();

        if (autoRender) {
            Begin();
            Render();
            End();
        }
    }

    void Console::Begin() {
#ifndef LIBXENON
        g_pVideoDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET | D3DCLEAR_STENCIL, m_backColor, 1.0f, 0);
        g_pVideoDevice->BeginScene();
#else
        Xe_InvalidateState(m_pVideoDevice);
        Xe_SetClearColor(m_pVideoDevice, m_backColor);
#endif
    }

    void Console::End() {
#ifndef LIBXENON
        g_pVideoDevice->EndScene();
        g_pVideoDevice->Present(NULL, NULL, NULL, NULL);
#else
        Xe_Resolve(m_pVideoDevice);
        // wait for vsync
        while (!Xe_IsVBlank(g_pVideoDevice)); //slowdown ...
        Xe_Sync(m_pVideoDevice);
#endif
    }

    /**
     * Render the console
     */
    void Console::Render() {
        // Set scale to 1
        m_font.Scale(1.0f);

        for (unsigned int i = 0; i < n_lines; i++) {
            char * line = m_pLines[i];
            m_font.DrawText(line, m_textColor, 0, (i * m_lineHeight));
        }

    }

}
