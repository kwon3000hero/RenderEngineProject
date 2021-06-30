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

VTX3D_OUT VS_3DSKY(VTX3D_IN _In)
{
    VTX3D_OUT OUT = (VTX3D_OUT) 0.0f;
    
    OUT.vPOSITION = mul(_In.vPOSITION, TransData.WVP);
    OUT.vUV = _In.vUV;
    
    return OUT;
}

Texture2D DifTex : register(t0);
SamplerState Smp : register(s0);

float4 PS_3DSKY(VTX3D_OUT _In) : SV_Target
{
    float4 TexColor = DifTex.Sample(Smp, _In.vUV.xy);
    TexColor.w = 1.0f;
    return TexColor;
}
