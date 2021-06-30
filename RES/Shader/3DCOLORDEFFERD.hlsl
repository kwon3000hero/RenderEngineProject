#include "CalculateFunction.hlsli"
#include "AnimationHeader.hlsli"

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
VTX3D_OUT VS_3DCOLORDEFFERD(VTX3D_IN _In)
{
    VTX3D_OUT OUT = (VTX3D_OUT) 0.0f;
    
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

struct DEFFERDCOLOR
{
    float4 vDif : SV_Target0;
    float4 vPOS : SV_Target1;
    float4 vNORMAL : SV_Target2;
    float4 vDepth : SV_Target3;
};


DEFFERDCOLOR PS_3DCOLORDEFFERD(VTX3D_OUT _In)
{
    DEFFERDCOLOR ReturnColor = (DEFFERDCOLOR) 0.0f;
    ReturnColor.vDif = _In.vCOLOR;
    ReturnColor.vPOS = _In.vPOS;
    ReturnColor.vNORMAL = _In.vNORMAL;
    ReturnColor.vDepth.x = ReturnColor.vPOS.z;
    ReturnColor.vDif.w = 1.0f;
    ReturnColor.vNORMAL.w = 1.0f;
    ReturnColor.vDepth.w = 1.0f;
    ReturnColor.vPOS.w = 1.0f;

    return ReturnColor;
}
