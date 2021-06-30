#include "LightHeader.hlsli"
#include "AnimationHeader.hlsli"
#include "CalculateFunction.hlsli"

struct VTX3D_IN
{
    float4 vPOSITION : POSITION;
    float4 vUV : TEXCOORD;
    float4 vCOLOR : COLOR;
    float4 vNORMAL : NORMAL;
    float4 vBINORMAL : BINORMAL;
    float4 vTANGENT : TANGENT;
    float4 vWEIGHT : BLENDWEIGHT;
    int4 vINDEX : BLENDINDICES;
};

struct VTX3D_OUT
{
    float4 vPOSITION : SV_Position;
    float4 vUV : TEXCOORD;
    float4 vCOLOR : COLOR;
    float4 vPOS : POSITION;
    float4 vNORMAL : NORMAL;
    float4 vBINORMAL : BINORMAL;
    float4 vTANGENT : TANGENT;
};

struct TRANSFORMMAT
{
    row_major float4x4 SCALE;
    row_major float4x4 ROT;
    row_major float4x4 ROTX;
    row_major float4x4 ROTY;
    row_major float4x4 ROTZ;
    row_major float4x4 POS;
    row_major float4x4 Offset;
    row_major float4x4 Final;
    row_major float4x4 REVOLUTION;
    row_major float4x4 PARENT;
    row_major float4x4 VIEW;
    row_major float4x4 PROJ;
    row_major float4x4 WV;
    row_major float4x4 WVP;
};

cbuffer TRANSFORMDATA : register(b0)
{
    TRANSFORMMAT TransData;
};

cbuffer RENDEROPTION : register(b3)
{
    int IsExistAnimation;
    int IsExistNormalTex;
    int IsExistSpcTex;
    int IsExistEmiTex;
};

VTX3D_OUT VS_3DMESHDEFFERD(VTX3D_IN _In)
{
    VTX3D_OUT OUT = (VTX3D_OUT) 0.0f;
    
    if (0 != IsExistAnimation)
    {
        SkinningTex(_In.vPOSITION, _In.vNORMAL, _In.vWEIGHT, _In.vINDEX);
    }

    OUT.vPOSITION = mul(_In.vPOSITION, TransData.WVP);
    OUT.vUV = _In.vUV;
    OUT.vCOLOR = _In.vCOLOR;
    OUT.vPOS = mul(_In.vPOSITION, TransData.WV);
    OUT.vNORMAL = normalize(mul(_In.vNORMAL, TransData.WV));
    OUT.vBINORMAL = normalize(mul(_In.vBINORMAL, TransData.WV));
    OUT.vTANGENT = normalize(mul(_In.vTANGENT, TransData.WV));
    OUT.vNORMAL.w = OUT.vBINORMAL.w = OUT.vTANGENT.w = 0.0f;
    
    return OUT;
}

Texture2D DifTex : register(t0);
Texture2D NormalTex : register(t1);
Texture2D SpcTex : register(t2);
Texture2D EmiTex : register(t3);
SamplerState Smp : register(s0);

struct DEFFERDCOLOR
{
    float4 vDif : SV_Target0;
    float4 vPOS : SV_Target1;
    float4 vNORMAL : SV_Target2;
    float4 vDepth : SV_Target3;
};


DEFFERDCOLOR PS_3DMESHDEFFERD(VTX3D_OUT _In)
{
    DEFFERDCOLOR ReturnColor = (DEFFERDCOLOR) 0.0f;
    ReturnColor.vDif = DifTex.Sample(Smp, _In.vUV.xy);
    
    ReturnColor.vPOS = _In.vPOS;
    ReturnColor.vNORMAL = _In.vNORMAL;
    ReturnColor.vDepth.x = ReturnColor.vPOS.z;

    if (0 != IsExistSpcTex)
    {
        ReturnColor.vDif += SpcTex.Sample(Smp, _In.vUV.xy);
    }
    
    if (0 != IsExistNormalTex)
    {
        CalculateBump(ReturnColor.vNORMAL, NormalTex, Smp, _In.vUV.xy, _In.vNORMAL, _In.vTANGENT, _In.vBINORMAL);
    }
    
    if (0 != IsExistEmiTex)
    {
        ReturnColor.vDepth.y = EmiTex.Sample(Smp, _In.vUV.xy);
    }
    
    ReturnColor.vDif.w = 1.0f;
    ReturnColor.vNORMAL.w = 1.0f;
    ReturnColor.vDepth.w = 1.0f;
    ReturnColor.vPOS.w = 1.0f;
    
    return ReturnColor;
}