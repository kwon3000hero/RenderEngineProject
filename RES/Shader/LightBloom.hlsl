// 머지용 쉐이더

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

VTX_OUT VS_LightBloom(VTX_IN _In)
{
    VTX_OUT OUT = (VTX_OUT) 0.0f;
    // WVP를 곱할필요가 없느냐?
    OUT.vPOSITION = _In.vPOSITION;
    OUT.vUV = _In.vUV;
    return OUT;
}

Texture2D BloomTarget : register(t2); // 번진에
SamplerState Smp : register(s0);

float4 PS_LightBloom(VTX_OUT _In) : SV_Target0
{
    float4 ReturnColor = 0.0f;
    
    float4 BloomColor = BloomTarget.Sample(Smp, _In.vUV.xy);
    
    if (0 < BloomColor.x)
    {
        ReturnColor += BloomColor * 10.0f;
        ReturnColor *= 1.0f;
        
        return ReturnColor;
    }
    
    clip(-1);
    return ReturnColor;
}