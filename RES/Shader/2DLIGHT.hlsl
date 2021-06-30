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

VTX_OUT VS_2DLIGHT(VTX_IN _In)
{
    VTX_OUT OUT = (VTX_OUT) 0.0f;
    OUT.vPOSITION = _In.vPOSITION;
    OUT.vUV = _In.vUV;
    return OUT;
}

cbuffer SCREENRATIO : register(b0)
{
    float4 Ratio;
};

Texture2D EffectTex : register(t0);
SamplerState Smp : register(s0);

float4 PS_2DLIGHT(VTX_OUT _In) : SV_Target0
{
    float4 Color = EffectTex.Sample(Smp, _In.vUV.xy);
    
    _In.vUV.x -= 0.5f;
    _In.vUV.x *= Ratio.z;
    _In.vUV.y = (_In.vUV.y * -1.0f) + 0.5f;
    float Len = length(_In.vUV.xy);
    float CalRatio = 1.0f - Len;
    
    if ((Color.x + Color.y + Color.z + Color.w) == 0.0f)
    {
        return float4(0.0f, 0.0f, 0.0f, Len);
    }
    
    return Color * CalRatio;

}