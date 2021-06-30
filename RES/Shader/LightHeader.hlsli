class LIGHTDATA
{
    float4 DiffuseColor;
    float4 SpecularColor;
    float4 AmbientColor;
    float4 LightPower; // X´Â DifDiffuseColor, Y SpecularColor, Z AmbientColor
    float4 LightColor;
    float4 LightPos;
    float4 LightDir;
    float4 LightDirInv;
    float4 LightRange;
    float4 CameraPos;
    float4 ShadowOption;
    float4 Dummy;
    row_major float4x4 CameraInverseView;
    row_major float4x4 LightView;
    row_major float4x4 LightProj;
    row_major float4x4 LightVP;
};

cbuffer LIGHTDATABUFFER : register(b10)
{
    int4 LightCount;
    LIGHTDATA LightDataArray[128];
};

cbuffer CURRENTLIGHTBUFFER : register(b11)
{
    LIGHTDATA CurLightData;
};

struct LIGHTRESULT
{
    float4 Diffuse;
    float4 Specular;
    float4 Ambient;
};

cbuffer CURLIGHTDATA : register(b11)
{
    LIGHTDATA CurData;
};

LIGHTRESULT CalculateDirectLightPixel(float4 _vViewPos, float4 _vViewNormal, LIGHTDATA _LightData)
{
    LIGHTRESULT result = (LIGHTRESULT) 0.0f;
    
    result.Diffuse = max(0, dot(_vViewNormal.xyz, _LightData.LightDirInv.xyz)) * _LightData.LightPower.x;
    result.Ambient = _LightData.AmbientColor;
    
    float3 vReflection;
    float3 vEye;
    
    vReflection = normalize(2.0f * _vViewNormal.xyz * dot(_LightData.LightDirInv.xyz, _vViewNormal.xyz) - _LightData.LightDirInv.xyz);
    
    vEye = normalize(_LightData.CameraPos.xyz - _vViewPos.xyz);
    
    result.Specular = max(0, dot(vReflection.xyz, vEye.xyz)) * _LightData.LightPower.y;
    
    result.Specular.xyzw = pow(result.Specular.x, _LightData.LightRange.y);
    
    result.Diffuse *= _LightData.LightPower.w;
    result.Specular *= _LightData.LightPower.w;
    
    return result;
}