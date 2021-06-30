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
    int IsExistEmvTex;
};

VTX3D_OUT VS_3DMESHFORWARD(VTX3D_IN _In)
{
    VTX3D_OUT OUT = (VTX3D_OUT) 0.0f;
    
    if (IsExistAnimation != 0)
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
Texture2D SpecularTex : register(t2);

SamplerState Smp : register(s0);

float4 PS_3DMESHFORWARD(VTX3D_OUT _In) : SV_Target
{
    float4 TexColor = DifTex.Sample(Smp, _In.vUV.xy);
    float4 LightColor = (float4) 0.0f;
    float4 ReturnColor = (float4) 0.0f;
    
    if (0 != IsExistSpcTex)
    {
        TexColor += SpecularTex.Sample(Smp, _In.vUV.xy);
    }
    TexColor.w = 1.0f;

    if (0 != IsExistNormalTex)
    {
        CalculateBump(_In.vNORMAL, NormalTex, Smp, _In.vUV.xy, _In.vNORMAL, _In.vTANGENT, _In.vBINORMAL);
    }
    
    for (int i = 0; i < LightCount.x; ++i)
    {
        LIGHTRESULT LR = CalculateDirectLightPixel(_In.vPOS, _In.vNORMAL, LightDataArray[i]);
        LightColor += LR.Diffuse + LR.Specular + LR.Ambient;
    }
    
    ReturnColor.xyz = TexColor.xyz * LightColor.xyz;
    ReturnColor.w = TexColor.w;
    
    return ReturnColor;
}
