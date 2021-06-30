#include "AnimationHeader.hlsli"

struct SHADOWANI_IN
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


struct SHADOWANI_OUT
{
    float4 vPOSITION : SV_Position;
    float4 vPROJPOSTION : POSITION;
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

SHADOWANI_OUT VS_SHADOWANI(SHADOWANI_IN _In)
{
    SkinningTex(_In.vPOSITION, _In.vNORMAL, _In.vWEIGHT, _In.vINDEX);
    
    SHADOWANI_OUT SO = (SHADOWANI_OUT) 0.0f;
    SO.vPOSITION = mul(_In.vPOSITION, TransData.WVP);
    SO.vPROJPOSTION = SO.vPOSITION;
    return SO;
}


float4 PS_SHADOWANI(SHADOWANI_OUT _In) : SV_Target
{
    return float4(_In.vPROJPOSTION.z / _In.vPROJPOSTION.w, 0.0f, 0.0f, 0.0f);
}
