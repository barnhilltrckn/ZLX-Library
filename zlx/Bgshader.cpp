/* 
 * File:   Bgshader.cpp
 * Author: cc
 * 
 * Created on 12 août 2011, 21:23
 */
#include "Draw.h"
#include "zlx.h"


#ifndef LIBXENON
#include "draw_shader.h"
#else
#include "bg_pixel_psu.h"
#include "bg_vertex_vsu.h"
#endif

#include "Bgshader.h"
namespace ZLX {

    //Parametre a envoyer au pixel shaders
    static float fTime = 1.0f;
    static float fRate = 1.1f;
    static float fAmplitude = 15.0f;
    static float fAmplitudeMul = 4.0f;

    static ZLXVertexShader * m_pVertexShader = NULL;
    static ZLXPixelShader * m_pPixelShader = NULL;

    static int isShaderLoaded = FALSE;

    static int CreateShader();
	
	void Bgshader::SetAmplitude(float val){
		fAmplitude = val;
	}
	
	void Bgshader::SetAmplitudeMul(float val){
		fAmplitudeMul = val;
	}

    void Bgshader::RenderBg() {
        if (isShaderLoaded == FALSE) {
            printf("Bgshader CreateShader\r\n");
            CreateShader();
        }
       // printf("RenderBg\r\n");
        float uleft = 0, uright = 1, vtop = 0, vbottom = 1;
        float dstx = -1.0f;
        float dsty = -1.0f;
        float width = 2.0f;
        float height = 2.0f;

        float z = 1.0f;

        unsigned int color = 0xFF00FF00;

        float vertices[] = {
            dstx,
            dsty + height,
            z,
            color,
            uleft,
            vtop,

            dstx + width,
            dsty + height,
            z,
            color,
            uright,
            vtop,

            dstx,
            dsty,
            z,
            color,
            uleft,
            vbottom,

            dstx,
            dsty,
            z,
            color,
            uleft,
            vbottom,

            dstx + width,
            dsty + height,
            z,
            color,
            uright,
            vtop,

            dstx + width,
            dsty,
            z,
            color,
            uright,
            vbottom
        };

        fTime += 0.1f;
        
        //send param to shader
        Xe_SetPixelShaderConstantF(g_pVideoDevice, 0, &fTime, 1);
        Xe_SetPixelShaderConstantF(g_pVideoDevice, 1, &fRate, 1);
        Xe_SetPixelShaderConstantF(g_pVideoDevice, 2, &fAmplitude, 1);
        Xe_SetPixelShaderConstantF(g_pVideoDevice, 3, &fAmplitudeMul, 1);
        
        Xe_VBBegin(g_pVideoDevice, 6);
        //memcpy(v, Rect, len);
        Xe_VBPut(g_pVideoDevice, vertices, 6*6);

        XenosVertexBuffer *vb = Xe_VBEnd(g_pVideoDevice);
        Xe_VBPoolAdd(g_pVideoDevice, vb);

        Xe_SetShader(g_pVideoDevice, SHADER_TYPE_PIXEL, m_pPixelShader, 0);
        Xe_SetShader(g_pVideoDevice, SHADER_TYPE_VERTEX, m_pVertexShader, 0);

        Xe_Draw(g_pVideoDevice, vb, 0);
    }

    int CreateShader() {
        // Load already compiled shader
        static const struct XenosVBFFormat vbf = {
            3,
            {
                {XE_USAGE_POSITION, 0, XE_TYPE_FLOAT3},
                {XE_USAGE_COLOR, 0, XE_TYPE_UBYTE4},
                {XE_USAGE_TEXCOORD, 0, XE_TYPE_FLOAT2},
            }
        };


        m_pPixelShader = Xe_LoadShaderFromMemory(g_pVideoDevice, (void*) bg_pixel_psu);
        Xe_InstantiateShader(g_pVideoDevice, m_pPixelShader, 0);

        m_pVertexShader = Xe_LoadShaderFromMemory(g_pVideoDevice, (void*) bg_vertex_vsu);
        Xe_InstantiateShader(g_pVideoDevice, m_pVertexShader, 0);
        Xe_ShaderApplyVFetchPatches(g_pVideoDevice, m_pVertexShader, 0, &vbf);

        isShaderLoaded = TRUE;

        return 0;
    }

}
