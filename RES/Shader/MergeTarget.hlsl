struct VTX_IN
{
    float4 vPOSITION : POSITION;
    float4 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPOSITION : SV_Position;
    float4 vUV : TEXCOORD;
};

VTX_OUT VS_MergeTarget(VTX_IN _In)
{
    VTX_OUT OUT = (VTX_OUT) 0.0f;
    // WVP를 곱할필요가 없느냐?
    OUT.vPOSITION = _In.vPOSITION;
    OUT.vUV = _In.vUV;
    return OUT;
}

Texture2D MergeTex : register(t0);
SamplerState Smp : register(s0);

float4 PS_MergeTarget(VTX_OUT _In) : SV_Target
{
    float4 Color = MergeTex.Sample(Smp, _In.vUV.xy);
    
    return Color;
}