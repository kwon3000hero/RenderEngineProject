
void CalculateBump(inout float4 _Nomral, Texture2D _BumpTex, SamplerState _Smp,
float2 _Uv, float4 _ViewNormal, float4 _ViewTangent, float4 _ViewBiNormal)
{
    float4 BumpNormal = _BumpTex.Sample(_Smp, _Uv);
    BumpNormal = BumpNormal * 2.0f - 1.0f;
    BumpNormal.w = 0.0F;
    BumpNormal = normalize(BumpNormal);
    
    float3x3 TangentSpace = float3x3(_ViewTangent.xyz, _ViewBiNormal.xyz, _ViewNormal.xyz);
    BumpNormal.xyz = mul(BumpNormal.xyz, TangentSpace);
    BumpNormal.w;
    BumpNormal = normalize(BumpNormal);
    
    _Nomral = BumpNormal;
}