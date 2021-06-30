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

VTX2D_OUT VS_COLOR(VTX2D_IN _In)
{
    VTX2D_OUT OUT = (VTX2D_OUT)0.0f;
    OUT.vPOSITION = mul(_In.vPOSITION, TransData.WVP);
    OUT.vUV = _In.vUV;
    OUT.vCOLOR = _In.vCOLOR;
    return OUT;
}

cbuffer PLANECOLOR : register(b0)
{
    float4 PlaneColor;
};

cbuffer LINECOLOR : register(b1)
{
    float4 LineColor;
};


float4 PS_COLOR(VTX2D_OUT _In) : SV_Target
{
    if (
        _In.vUV.x > 0.1f &&
        _In.vUV.x < 0.9f &&
        _In.vUV.y > 0.1f &&
        _In.vUV.y < 0.9f
        )
    {
        return PlaneColor;
    }

    return LineColor;

    // return float4(1.0f, 0.0f, 0.0f, 1.0f);

}