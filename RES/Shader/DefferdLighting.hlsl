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
    float4 vPOSITION : SV_Position;
    float4 vUV : TEXCOORD;
};

VTX3D_OUT VS_DEFFERDLIGHTING(VTX3D_IN _In)
{
    VTX3D_OUT OUT = (VTX3D_OUT) 0.0f;
    
    OUT.vPOSITION = _In.vPOSITION;
    OUT.vUV = _In.vUV;
    
    return OUT;
}

Texture2D POSTEX : register(t0);
Texture2D NORMALTEX : register(t1);
Texture2D DEPTHTEX : register(t2);
Texture2D SHADOWTEX : register(t3);

SamplerState Smp : register(s0);

struct DEFFERDLIGHTOUT
{
    float4 vDifLight : SV_Target0;
    float4 vSpcLight : SV_Target1;
    float4 vAmpLight : SV_Target2;
    float4 vLight : SV_Target3;
};


DEFFERDLIGHTOUT PS_DEFFERDLIGHTING(VTX3D_OUT _In)
{
    float4 Depth = DEPTHTEX.Sample(Smp, _In.vUV.xy);
    
    if (0 == Depth.x)
    {
        clip(-1);
    }
    
    DEFFERDLIGHTOUT OUTCOLOR = (DEFFERDLIGHTOUT) 0.0f;

    float4 pos = POSTEX.Sample(Smp, _In.vUV.xy);
    pos.w = 1.0f;

    float4 normal = NORMALTEX.Sample(Smp, _In.vUV.xy);
    normal.w = 0.0f;
    
    LIGHTRESULT LR = CalculateDirectLightPixel(pos, normal, CurLightData);
    
    OUTCOLOR.vDifLight = LR.Diffuse;
    OUTCOLOR.vSpcLight = LR.Specular;
    OUTCOLOR.vAmpLight = LR.Ambient;
    
    if (0.0f != OUTCOLOR.vDifLight.x && 0.0f != CurLightData.ShadowOption.x)
    {
        float4 vWorldPos = mul(float4(pos.xyz, 1.0f), CurLightData.CameraInverseView);
        float4 vShadowProj = mul(vWorldPos, CurLightData.LightVP);
        float fDepth = vShadowProj.z / vShadowProj.w;

        float2 vShadowUv =
            float2((vShadowProj.x / vShadowProj.w) * 0.5f + 0.5f, (vShadowProj.y / vShadowProj.w) * -0.5f + 0.5f);
        
        if ((0.01f < vShadowUv.x && vShadowUv.x < 0.99f) && (0.01f < vShadowUv.y && vShadowUv.y < 0.99f))
        {
            float fShadowDepth = SHADOWTEX.Sample(Smp, vShadowUv).r;

            if (fDepth > fShadowDepth + 0.0001f)
            {
                OUTCOLOR.vDifLight *= 0.1f;
                OUTCOLOR.vSpcLight = 0.0f;
            }
        }
    }
    
    OUTCOLOR.vLight = OUTCOLOR.vDifLight + OUTCOLOR.vAmpLight + OUTCOLOR.vSpcLight;
    
    return OUTCOLOR;
}
