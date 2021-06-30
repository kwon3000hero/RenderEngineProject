struct VTX2D_IN
{
    float4 vPOSITION : POSITION;
    float4 vUV : TEXCOORD;
    float4 vCOLOR : COLOR;
};

struct VTX2D_OUT
{
    float4 vPOSITION : SV_Position;
    float4 vUV : TEXCOORD;
    float4 vCOLOR : COLOR;
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

cbuffer CUTUV : register(b1)
{
    float4 CutData;
};


VTX2D_OUT VS_2DIMAGE(VTX2D_IN _In)
{
    VTX2D_OUT OUT = (VTX2D_OUT) 0.0f;
    OUT.vPOSITION = mul(_In.vPOSITION, TransData.WVP);
	
    _In.vUV.x = (_In.vUV.x * CutData.z) + CutData.x;
    _In.vUV.y = (_In.vUV.y * CutData.w) + CutData.y;

    OUT.vUV = _In.vUV;
    OUT.vCOLOR = _In.vCOLOR;
    return OUT;
}

Texture2D DifTex : register(t0);
SamplerState Smp : register(s0);

float4 PS_2DIMAGE(VTX2D_OUT _In) : SV_Target
{
    float4 Color = DifTex.Sample(Smp, _In.vUV.xy);

    return Color;
}