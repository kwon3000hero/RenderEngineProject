#include "AnimationHeader.hlsli"

struct SHADOW_OUT
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

SHADOW_OUT VS_SHADOW(float4 _vLocalPos : POSITION)
{
    SHADOW_OUT SO = (SHADOW_OUT) 0.0f;
    _vLocalPos.w = 1.0f;
    SO.vPOSITION = mul(_vLocalPos, TransData.WVP);
    SO.vPROJPOSTION = SO.vPOSITION;
    return SO;
}


float4 PS_SHADOW(SHADOW_OUT _In) : SV_Target
{
    return float4(_In.vPROJPOSTION.z / _In.vPROJPOSTION.w, 0.0f, 0.0f, 0.0f);
}
