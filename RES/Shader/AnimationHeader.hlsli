Texture2D FrameAnimationTexture : register(t0);

float4x4 TextureToMatrix(int _Idx)
{
    float4x4 BoneMat =
    {
        FrameAnimationTexture.Load(int3((_Idx * 4) + 0, 0, 0)),
        FrameAnimationTexture.Load(int3((_Idx * 4) + 1, 0, 0)),
        FrameAnimationTexture.Load(int3((_Idx * 4) + 2, 0, 0)),
        FrameAnimationTexture.Load(int3((_Idx * 4) + 3, 0, 0)),
    };
    return BoneMat;
}

void SkinningTex(inout float4 _Pos, inout float4 _Normal, inout float4 _Weight, inout int4 _Indices)
{
    float4 CalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 CalNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float fWeight[4];
    fWeight[0] = _Weight.x;
    fWeight[1] = _Weight.y;
    fWeight[2] = _Weight.z;
    fWeight[3] = 1.f - _Weight.x - _Weight.y - _Weight.z;
    
    _Pos.w = 1.0f;
    _Normal.w = 0.0f;
    
    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = TextureToMatrix(_Indices[i]);
        
        CalPos += fWeight[i] * mul(_Pos, matBone);
        CalNormal += fWeight[i] * mul(_Normal, matBone);
    }
    
    _Pos = CalPos;
    _Normal = CalNormal;

    return;
}