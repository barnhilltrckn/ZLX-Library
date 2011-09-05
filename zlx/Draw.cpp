#include "Draw.h"
#include "zlx.h"

#ifndef LIBXENON
#include "draw_shader.h"
#else
#include "draw_c_p_psu.h"
#include "draw_t_p_psu.h"
#include "draw_v_vsu.h"
#endif

#define MAX_VERTEX_COUNT 16*1024

namespace ZLX {
    static ZLXVertexShader * m_pVertexShader;
    static ZLXPixelShader * m_pPixelShader;
    static ZLXPixelShader * m_pPixelTexturedShader;
    static XenosVertexBuffer *vb;

    static int ivertices = 0;

    static int isShaderLoaded = FALSE;

    typedef struct DrawVerticeFormats {
        float x, y; //z/w remove it ...
        float u, v;
        ZLXColor color;
    } DrawVerticeFormats;

    DrawVerticeFormats * VerticesRect;
#ifndef LIBXENON
    LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl;
#endif

    static int CreateShader();

    void Draw::Reset() {
        ivertices = 0;
    }

    static void PrepareVb() {
        VerticesRect = (DrawVerticeFormats *) Xe_VB_Lock(g_pVideoDevice, vb, ivertices, 6 * sizeof (DrawVerticeFormats), XE_LOCK_WRITE);
    }

    static void UpdateVb() {
        Xe_VB_Unlock(g_pVideoDevice, vb);
        Xe_SetStreamSource(g_pVideoDevice, 0, vb, ivertices, sizeof (DrawVerticeFormats));
        
        ivertices += 6 * sizeof (DrawVerticeFormats);
    }

    void Draw::DrawGradientRect(float x, float y, float w, float h, ZLXColor TopColor, ZLXColor BackColor) {
        if (isShaderLoaded == FALSE) {
            CreateShader();
        }

        PrepareVb();

        // Uv
        float bottom = 0.0f;
        float top = 1.0f;
        float left = 0.0f;
        float right = 1.0f;

        // Bottom left
        VerticesRect[0].x = x;
        VerticesRect[0].y = y;
        VerticesRect[0].u = bottom;
        VerticesRect[0].v = left;
        VerticesRect[0].color = BackColor;

        // top left
        VerticesRect[1].x = x;
        VerticesRect[1].y = y + h;
        VerticesRect[1].u = top;
        VerticesRect[1].v = left;
        VerticesRect[1].color = TopColor;

        // bottom right
        VerticesRect[2].x = x + w;
        VerticesRect[2].y = y;
        VerticesRect[2].u = bottom;
        VerticesRect[2].v = right;
        VerticesRect[2].color = BackColor;

        // bottom right
        VerticesRect[3].x = x + w;
        VerticesRect[3].y = y;
        VerticesRect[3].u = bottom;
        VerticesRect[3].v = right;
        VerticesRect[3].color = BackColor;

        // top left
        VerticesRect[4].x = x;
        VerticesRect[4].y = y + h;
        VerticesRect[4].u = top;
        VerticesRect[4].v = left;
        VerticesRect[4].color = TopColor;

        // top right
        VerticesRect[5].x = x + w;
        VerticesRect[5].y = y + h;
        VerticesRect[5].u = top;
        VerticesRect[5].v = right;
        VerticesRect[5].color = TopColor;

        //        int len = 6 * sizeof (DrawVerticeFormats);

        // refresh vb caches
        UpdateVb();

        Xe_SetTexture(g_pVideoDevice, 0, NULL);
        Xe_SetShader(g_pVideoDevice, SHADER_TYPE_PIXEL, m_pPixelShader, 0);
        Xe_SetShader(g_pVideoDevice, SHADER_TYPE_VERTEX, m_pVertexShader, 0);
        Xe_DrawPrimitive(g_pVideoDevice, XE_PRIMTYPE_TRIANGLELIST, 0, 2);
    }

    void Draw::DrawTexturedRect(float x, float y, float w, float h, ZLXTexture * p_texture) {
        if (isShaderLoaded == FALSE) {
            CreateShader();
        }

        PrepareVb();

        // Uv
        float bottom = 1.0f;
        float top = 0.0f;
        float left = 0.0f;
        float right = 1.0f;

        // Bottom left
        VerticesRect[0].x = x;
        VerticesRect[0].y = y;
        VerticesRect[0].u = left;
        VerticesRect[0].v = bottom;

        // top left
        VerticesRect[1].x = x;
        VerticesRect[1].y = y + h;
        VerticesRect[1].u = left;
        VerticesRect[1].v = top;

        // bottom right
        VerticesRect[2].x = x + w;
        VerticesRect[2].y = y;
        VerticesRect[2].u = right;
        VerticesRect[2].v = bottom;

        // bottom right
        VerticesRect[3].x = x + w;
        VerticesRect[3].y = y;
        VerticesRect[3].u = right;
        VerticesRect[3].v = bottom;

        // top left
        VerticesRect[4].x = x;
        VerticesRect[4].y = y + h;
        VerticesRect[4].u = left;
        VerticesRect[4].v = top;

        // top right
        VerticesRect[5].x = x + w;
        VerticesRect[5].y = y + h;
        VerticesRect[5].u = right;
        VerticesRect[5].v = top;

        //int len = 6 * sizeof (DrawVerticeFormats);

        UpdateVb();
        Xe_SetTexture(g_pVideoDevice, 0, p_texture);
        Xe_SetShader(g_pVideoDevice, SHADER_TYPE_PIXEL, m_pPixelTexturedShader, 0);
        Xe_SetShader(g_pVideoDevice, SHADER_TYPE_VERTEX, m_pVertexShader, 0);
        Xe_DrawPrimitive(g_pVideoDevice, XE_PRIMTYPE_TRIANGLELIST, 0, 2);

        
    }

    void Draw::DrawColoredRect(float x, float y, float w, float h, ZLXColor color) {

        if (isShaderLoaded == FALSE) {
            CreateShader();
        }

        PrepareVb();

        // Uv
        float bottom = 0.0f;
        float top = 1.0f;
        float left = 0.0f;
        float right = 1.0f;

        // Bottom left
        VerticesRect[0].x = x;
        VerticesRect[0].y = y;
        VerticesRect[0].u = bottom;
        VerticesRect[0].v = left;
        VerticesRect[0].color = color;

        // top left
        VerticesRect[1].x = x;
        VerticesRect[1].y = y + h;
        VerticesRect[1].u = top;
        VerticesRect[1].v = left;
        VerticesRect[1].color = color;

        // bottom right
        VerticesRect[2].x = x + w;
        VerticesRect[2].y = y;
        VerticesRect[2].u = bottom;
        VerticesRect[2].v = right;
        VerticesRect[2].color = color;

        // bottom right
        VerticesRect[3].x = x + w;
        VerticesRect[3].y = y;
        VerticesRect[3].u = bottom;
        VerticesRect[3].v = right;
        VerticesRect[3].color = color;

        // top left
        VerticesRect[4].x = x;
        VerticesRect[4].y = y + h;
        VerticesRect[4].u = top;
        VerticesRect[4].v = left;
        VerticesRect[4].color = color;

        // top right
        VerticesRect[5].x = x + w;
        VerticesRect[5].y = y + h;
        VerticesRect[5].u = top;
        VerticesRect[5].v = right;
        VerticesRect[5].color = color;

        //int len = 6 * sizeof (DrawVerticeFormats);
        UpdateVb();
        Xe_SetTexture(g_pVideoDevice, 0, NULL);
        Xe_SetShader(g_pVideoDevice, SHADER_TYPE_PIXEL, m_pPixelShader, 0);
        Xe_SetShader(g_pVideoDevice, SHADER_TYPE_VERTEX, m_pVertexShader, 0);
        Xe_DrawPrimitive(g_pVideoDevice, XE_PRIMTYPE_TRIANGLELIST, 0, 2);

        
    };

    int CreateShader() {
        // Load already compiled shader
        static const struct XenosVBFFormat vbf = {
            3,
            {
                {XE_USAGE_POSITION, 0, XE_TYPE_FLOAT2},
                {XE_USAGE_TEXCOORD, 0, XE_TYPE_FLOAT2},
                {XE_USAGE_COLOR, 0, XE_TYPE_UBYTE4},
            }
        };

        m_pPixelShader = Xe_LoadShaderFromMemory(g_pVideoDevice, (void*) draw_c_p_psu);
        Xe_InstantiateShader(g_pVideoDevice, m_pPixelShader, 0);

        m_pPixelTexturedShader = Xe_LoadShaderFromMemory(g_pVideoDevice, (void*) draw_t_p_psu);
        Xe_InstantiateShader(g_pVideoDevice, m_pPixelTexturedShader, 0);

        m_pVertexShader = Xe_LoadShaderFromMemory(g_pVideoDevice, (void*) draw_v_vsu);
        Xe_InstantiateShader(g_pVideoDevice, m_pVertexShader, 0);
        Xe_ShaderApplyVFetchPatches(g_pVideoDevice, m_pVertexShader, 0, &vbf);

        // Create Vb
        vb = Xe_CreateVertexBuffer(g_pVideoDevice, MAX_VERTEX_COUNT * sizeof (DrawVerticeFormats));

        //PrepareVb();

        isShaderLoaded = TRUE;

        return 0;
    }
}