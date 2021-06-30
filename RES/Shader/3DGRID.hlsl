struct VTX3D_IN
{
    float4 vPOSITION : POSITION;
    float4 vUV : TEXCOORD;
};

struct VTX3D_OUT
{
    float4 vPOSITION : SV_Position;
    float4 vVIEWPOS : POSITION;
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

cbuffer CAMPOSMAT : register(b1)
{
    float4 CamWorldPos;
};

VTX3D_OUT VS_GRID(VTX3D_IN _In)
{
    VTX3D_OUT OUT = (VTX3D_OUT) 0.0f;

   
    OUT.vPOSITION = mul(_In.vPOSITION, TransData.WVP);
    OUT.vVIEWPOS = mul(_In.vPOSITION, TransData.Final);
    OUT.vUV = _In.vUV;
    
    return OUT;
}

float4 PS_GRID(VTX3D_OUT _In) : SV_Target
{
    float X = abs(_In.vVIEWPOS.x % 10.0f);
    float Z = abs(_In.vVIEWPOS.z % 10.0f);
    
    float Len = length(_In.vVIEWPOS.xz - CamWorldPos.xz);
    float Ratio = 1.0f;
    float GridRange = max(10.0f, abs(CamWorldPos.y) * 10.0f);
    Ratio = max(0, (GridRange - Len) / GridRange);
    if (X <= 0.2F ||
        Z <= 0.2F)
    {
        return float4(1.0f, 1.0f, 1.0f, Ratio);
    }
 
    clip(-1.0F);
    
    return float4(1.0f, 1.0f, 1.0f, 1.0f);


}
