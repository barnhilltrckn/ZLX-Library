/*

This effect file is designed for rending bitmap fonts output by
AngelCode Bitmap Font Generator. It is capable of rendering
from 32bit textures that pack colored icons together with outlined
characters into one texture, where the icons use all 32bits and the
characters only use 8bits each.

*/

shared float4x4 g_mWorld : WORLD;      // World matrix
shared float4x4 g_mView  : VIEW;       // View matrix
shared float4x4 g_mProj  : PROJECTION; // Projection matrix

texture  g_txScene : TEXTURE0;

void main( float4 vPos         : POSITION,
				float4 vColor       : COLOR0,
				int4   vChnl        : BLENDINDICES0,
                float2 vTex0        : TEXCOORD0,
                out float4 oDiffuse : COLOR0,
                out int4   oChnl    : TEXCOORD1,
                out float4 oPos     : POSITION,
                out float2 oTex0    : TEXCOORD0 )
{
    // Transform the position from object space to homogeneous projection space
    oPos = mul( vPos, g_mWorld );
    oPos = mul( oPos, g_mView );
    oPos = mul( oPos, g_mProj );

    // Just copy the texture coordinate and color 
    oDiffuse = vColor;
    oChnl = vChnl;
    oTex0 = vTex0;
}