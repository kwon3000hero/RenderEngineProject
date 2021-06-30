// 머지용 쉐이더

// 텍스처.
static float g_vGAU[25] =
{
    1.0F, 2.0f, 4.0f, 2.0f, 1.0f, // 10
    2.0F, 4.0f, 8.0f, 4.0f, 2.0f, // 20
    4.0F, 8.0f, 16.0f, 8.0f, 4.0f, // 40
    2.0F, 4.0f, 8.0f, 4.0f, 2.0f, // 20
    1.0F, 2.0f, 4.0f, 2.0f, 1.0f, // 10
};


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

VTX_OUT VS_EmissionBloom(VTX_IN _In)
{
    VTX_OUT OUT = (VTX_OUT) 0.0f;
    // WVP를 곱할필요가 없느냐?
    OUT.vPOSITION = _In.vPOSITION;
    OUT.vUV = _In.vUV;
    return OUT;
}

cbuffer ScreenSize : register(b0)
{
    float4 SSize;
}

Texture2D CheckTex : register(t0);
SamplerState Smp : register(s0);

float4 PS_EmissionBloom(VTX_OUT _In) : SV_Target0
{
    float PixelU = 1.0f / SSize.x;
    float PixelV = 1.0f / SSize.y;
    
    float4 Sum;
    
    for (int y = 0; y < 5; ++y)
    {
        for (int x = 0; x < 5; ++x)
        {
            float2 Uv = _In.vUV.xy + float2((y - 2) * PixelU, (x - 2) * PixelV);
            Sum += CheckTex.Sample(Smp, Uv) * g_vGAU[y * 5 + x];
        }
    }
    
    Sum /= 100.0f;
    

    return Sum;

}