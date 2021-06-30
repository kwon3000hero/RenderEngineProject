#include "CalculateFunction.hlsli"
#include "AnimationHeader.hlsli"
#include "LightHeader.hlsli"

struct VTX3D_IN
{
    float4 vPOSITION : POSITION;
    float4 vUV : TEXCOORD;
};

struct VTX3D_OUT
{
    float4 vPOSITION : SV_Position; // 뷰포트가 곱해진다.
    float4 vUV : TEXCOORD; // 선형보간
};

VTX3D_OUT VS_DEFFERDMERGE(VTX3D_IN _In)
{
    VTX3D_OUT OUT = (VTX3D_OUT) 0.0f;
    
    OUT.vPOSITION = _In.vPOSITION;
    OUT.vUV = _In.vUV;
    
    return OUT;
}

Texture2D ColorTex : register(t0);
Texture2D LightTex : register(t1);
SamplerState Smp : register(s0);

float4 PS_DEFFERDMERGE(VTX3D_OUT _In) : SV_Target
{
    float4 TexColor = ColorTex.Sample(Smp, _In.vUV.xy);
    float4 Light = LightTex.Sample(Smp, _In.vUV.xy);

    if (TexColor.a == 0.0f)
    {
        clip(-1);
    }
 
    
    float4 DefferdColor = TexColor * Light;
    DefferdColor.w = 1.0f;
    
    return DefferdColor;
}
