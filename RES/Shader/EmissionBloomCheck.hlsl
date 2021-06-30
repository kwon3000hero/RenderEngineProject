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

VTX_OUT VS_EmssionBloomCheck(VTX_IN _In)
{
    VTX_OUT OUT = (VTX_OUT) 0.0f;
    OUT.vPOSITION = _In.vPOSITION;
    OUT.vUV = _In.vUV;
    return OUT;
}

Texture2D LightTex : register(t0);
Texture2D DepthTex : register(t1);
SamplerState Smp : register(s0);


float4 PS_EmssionBloomCheck(VTX_OUT _In) : SV_Target0
{
    float4 LightSmall = LightTex.Sample(Smp, _In.vUV.xy);
    float4 DepthColor = DepthTex.Sample(Smp, _In.vUV.xy);
    
    if (0 < DepthColor.y)
    {
        return LightSmall;
    }
    
    return float4(0.0f, 0.0f, 0.0f, 0.0f);

}