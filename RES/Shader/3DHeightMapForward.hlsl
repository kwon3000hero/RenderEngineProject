#include "CalculateFunction.hlsli"
#include "LightHeader.hlsli"

struct VTX3D_IN
{
    float4 vPOSITION : POSITION;
    float4 vUV : TEXCOORD;
    float4 vCOLOR : COLOR;
    float4 vNORMAL : NORMAL;
    float4 vBINORMAL : BINORMAL;
    float4 vTANGENT : TANGENT;
};

struct VTX3D_OUT
{
    float4 vPOSITION : SV_Position; // 뷰포트가 곱해진다.
    float4 vUV : TEXCOORD; // 선형보간
    float4 vCOLOR : COLOR; // 선형보간
    float4 vPOS : POSITION; // 선형보간
    float4 vNORMAL : NORMAL; // 구면선형보간 // 방향을 나타내는 시맨틱은 위치가 아니죠?
    float4 vBINORMAL : BINORMAL; // 구면선형보간
    float4 vTANGENT : TANGENT; // 구면선형보간
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

cbuffer MAPDATA : register(b1)
{
    float4 HeightMapData;
};

Texture2D HeightTex : register(t0);
SamplerState Smp : register(s0);

VTX3D_OUT VS_HMAPFORWARD(VTX3D_IN _In)
{
    VTX3D_OUT OUT = (VTX3D_OUT) 0.0f;

    float4 Uv = float4(_In.vUV.x / (float) HeightMapData.x, _In.vUV.y / (float) HeightMapData.z, 0.0f, 0.0f);
    
    float4 Pos = _In.vPOSITION;
    float4 Color = HeightTex.SampleLevel(Smp, Uv.xy, 0);
    Pos.y = Color.r * 0.5f;
    
    OUT.vUV = _In.vUV;
    OUT.vPOSITION = mul(Pos, TransData.WVP);

    OUT.vCOLOR = _In.vCOLOR; // LightColor;
    OUT.vPOS = mul(_In.vPOSITION, TransData.WV);
    OUT.vNORMAL = normalize(mul(_In.vNORMAL, TransData.WV));
    OUT.vBINORMAL = normalize(mul(_In.vBINORMAL, TransData.WV));
    OUT.vTANGENT = normalize(mul(_In.vTANGENT, TransData.WV));
    OUT.vNORMAL.w = OUT.vBINORMAL.w = OUT.vTANGENT.w = 0.0f;
    
    return OUT;
}

Texture2D DifTex0 : register(t0);
Texture2D DifBump0 : register(t1);

Texture2D DifTex1 : register(t2);
Texture2D DifTexSp1 : register(t3);
Texture2D DifBump1 : register(t4);

float4 PS_HMAPFORWARD(VTX3D_OUT _In) : SV_Target
{
    float4 TexColor0 = DifTex0.Sample(Smp, _In.vUV.xy);
    
    float4 UV = float4(_In.vUV.x / (float) HeightMapData.x, _In.vUV.y / (float) HeightMapData.z, 0.0f, 0.0f);
    
    if (1 <= HeightMapData.w)
    {
        float textureColor1 = DifTex1.Sample(Smp, _In.vUV.xy);
        float textureSpColor1 = DifTexSp1.Sample(Smp, _In.vUV.xy);
        TexColor0 = (textureColor1 * textureSpColor1.r) + (textureColor1 * (1.0f - textureSpColor1.r));
    }
    
    TexColor0.w = 1.0f;
    
    float4 LightColor = (float4) 0.0f;
    float4 ReturnColor = (float4) 0.0f;
    
    for (int i = 0; i < LightCount.x; ++i)
    {
        LIGHTRESULT LR = CalculateDirectLightPixel(_In.vPOS, _In.vNORMAL, LightDataArray[i]);
        LightColor += LR.Diffuse + LR.Specular + LR.Ambient;
    }
    ReturnColor.xyz = TexColor0.xyz * LightColor.xyz;
    ReturnColor.w = TexColor0.w;
    return ReturnColor;
}
