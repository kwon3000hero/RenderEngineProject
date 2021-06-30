struct VTX2D_OUT
{
    float4 vPOSITION : SV_Position; 
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


VTX2D_OUT VS_DEBUGFI(float4 _In : POSITION) : SV_Position
{
    VTX2D_OUT OUT = (VTX2D_OUT) 0.0f;
    OUT.vPOSITION = mul(_In, TransData.WVP);
    return OUT;
}

texture2D DifTex : register(t0);
SamplerState Smp : register(s0);

cbuffer DEBUGCOLOR : register(b0)
{
    float4 DColor;
};

float4 PS_DEBUGFI(VTX2D_OUT _In : SV_Position) : SV_Target
{
    return DColor;
}