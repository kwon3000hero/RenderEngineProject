#pragma once
#include <math.h>
#include <Windows.h>
#include <vector>
#include <assert.h>
#include <iostream>

#include <DirectXCollision.h>
#include <DirectXCollision.inl>
#include <DirectXPackedVector.h>
#include <DirectXPackedVector.inl>

class KGameMath
{
public:
	static const float PI;
	static const float PI2;
	static const float DTOR;
	static const float RTOD;
};

class KVector
{
public:
	static KVector LEFT;
	static KVector RIGHT;
	static KVector DOWN;
	static KVector UP;
	static KVector FORWARD;
	static KVector BACK;
	static KVector ONE;
	static KVector ZERO;
	static KVector CORD_ZERO;

	static KVector WHITE;
	static KVector BLACK;
	static KVector RED;
	static KVector BLUE;
	static KVector GREEN;

	union
	{
		struct
		{
			float PosX;
			float PosY;
			float SizeX;
			float SizeY;
		};

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};

		float Arr[4];
		float Arr2D[1][4];

		DirectX::XMVECTOR DXVector;
	};

public:
	operator DirectX::XMVECTOR()
	{
		return DXVector;
	}

	operator DirectX::XMFLOAT3()
	{
		return DirectX::XMFLOAT3(x, y, z);
	}

	operator DirectX::XMFLOAT4()
	{
		return DirectX::XMFLOAT4(x, y, z, w);
	}

	float Left() const { return x - HZ(); }
	int ILeft() const { return (int)Left(); }
	float Right() const { return x + HZ(); }
	int IRight() const { return (int)Right(); }
	float Top() const { return y - HW(); }
	int ITop() const { return (int)Top(); }
	float Bot() const { return (int)y + HW(); }
	int IBot() const { return (int)Bot(); }

	static KVector RadToDir2D(float _Rad)
	{
		return KVector{ cosf(_Rad), sinf(_Rad) };
	}

	static float Dot3D(const KVector& _Left, const KVector& _Right)
	{
		return _Left.x * _Right.x + _Left.y * _Right.y + _Left.z * _Right.z;
	}

	static KVector Cross3D(const KVector& _Left, const KVector& _Right)
	{
		return {
			_Left.y * _Right.z - _Left.z * _Right.y
			, _Left.z * _Right.x - _Left.x * _Right.z
			, _Left.x * _Right.y - _Left.y * _Right.x
		};
	}

	bool IsNearlyZero() const
	{
		return fabsf(x) <= 1.e-4f && fabsf(y) <= 1.e-4f && fabsf(z) <= 1.e-4f;
	}

	static float VectorToVectorRad2D(const KVector& _Left, const KVector& _Right)
	{
		if (_Left.y < _Right.y)
		{
			return acosf(Dot3D(_Left.NormalToReturn(), _Right.NormalToReturn()));
		}
		else {
			return acosf(Dot3D(_Left.NormalToReturn(), _Right.NormalToReturn())) + KGameMath::PI;
		}
	}

	static float VectorToVectorDeg2D(const KVector& _Left, const KVector& _Right)
	{
		return VectorToVectorRad2D(_Left, _Right) * KGameMath::RTOD;
	}


	float Len() const
	{
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	POINT ToPoint()
	{
		return POINT{ (int)x, (int)y };
	}

	void Normalize()
	{
		DXVector = DirectX::XMVector3Normalize(DXVector);
	}

	float AngleRadZ()
	{
		KVector Left = *this;
		Left.z = 0.0f;

		Left.Normalize();
		KVector Right = KVector::RIGHT;
		Right.z = 0.0f;
		Right.Normalize();

		float CosS = Dot3D(Left, Right);
		if (0 <= Left.y)
		{
			return acosf(CosS);
		}
		else
		{ 
			return KGameMath::PI2 - acosf(CosS);
		}
	}

	float AngleDegZ()
	{
		return AngleRadZ() * KGameMath::RTOD;
	}

	KVector Vector2D() const
	{
		return { x , y , 0.0f , 1.0f };
	}

	KVector HalfVector2D() const
	{
		return { x / 2 , y / 2 , 0.0F };
	}

	KVector HalfVector3D() const
	{
		return { x / 2 , y / 2 , z / 2 };
	}

	KVector AbsVector() const
	{
		return { abs(x) , abs(y), abs(z) };
	}

	KVector EulerToQuaternion() const
	{
		KVector rotation = *this;
		rotation *= KGameMath::DTOR;

		return DirectX::XMQuaternionRotationRollPitchYawFromVector(rotation.DXVector);
	}

	KVector QuaternionToEulerRad() const
	{
		KVector vector;

		float dx = x;
		float dy = y;
		float dz = z;
		float dw = w;

		float sqx = x * x;
		float sqy = y * y;
		float sqz = z * z;
		float sqw = w * w;

		vector.x = asinf(2.0f * (dw * dx - dy * dz));
		vector.y = atan2f(2.0f * (dx * dz + dw * dy), -sqx - sqy + sqz + sqw);
		vector.z = atan2f(2.0f * (dx * dy + dw * dz), -sqx + sqy - sqz + sqw);

		return vector;
	}

	KVector MultiQuaternion(const KVector& _Q)
	{
		return DirectX::XMQuaternionMultiply(DXVector, _Q.DXVector);
	}

	KVector NormalToReturn() const
	{
		float L = Len();
		return { x / L , y / L , z / L };
	}

	KVector DegToQReturn() const
	{
		KVector Return = *this;
		Return *= KGameMath::DTOR;
		Return.DXVector = DirectX::XMQuaternionRotationRollPitchYaw(Return.x, Return.y, Return.z);
		return Return;
	}

	KVector RadToQReturn() const
	{
		KVector Return = *this;
		Return.DXVector = DirectX::XMQuaternionRotationRollPitchYaw(Return.x, Return.y, Return.z);
		return Return;
	}

	KVector DegreeToQuaternion() const
	{
		KVector rotateValue = *this;
		rotateValue *= KGameMath::DTOR;
		rotateValue.DXVector = DirectX::XMQuaternionRotationRollPitchYaw(rotateValue.x, rotateValue.y, rotateValue.z);
		return rotateValue;
	}

	KVector RadianToQuaternion() const
	{
		KVector rotateValue = *this;
		rotateValue.DXVector = DirectX::XMQuaternionRotationRollPitchYaw(rotateValue.x, rotateValue.y, rotateValue.z);
		return rotateValue;
	}

	float HX() const {
		return x * 0.5f;
	}

	float HY() const {
		return y * 0.5f;
	}

	int IHX() const {
		return (int)HX();
	}

	int IHY() const {
		return (int)HY();
	}

	int IX() const {
		return (int)x;
	}

	int IY() const {
		return (int)y;
	}

	unsigned int UIX() const {
		return (int)x;
	}

	unsigned int UIY() const {
		return (int)y;
	}

	int IHZ() const {
		return (int)HZ();
	}

	int IHW() const {
		return (int)HW();
	}

	float HZ() const {
		return z * 0.5f;
	}

	float HW() const {
		return w * 0.5f;
	}

	int IZ() const {
		return (int)z;
	}

	int IW() const {
		return (int)w;
	}

	void RotToDegZ(float _Deg)
	{
		RotToRadZ(_Deg * KGameMath::DTOR);
	}

	void RotToRadZ(float _Rad)
	{
		KVector Rot = *this;
		Rot.x = (x * cosf(_Rad)) - (y * sinf(_Rad));
		Rot.y = (x * sinf(_Rad)) + (y * cosf(_Rad));
		x = Rot.x;
		y = Rot.y;
		// 내가 돌아가는 
	}

	KVector RotToDegZReturn(float _Deg)
	{
		return RotToRadZReturn(_Deg * KGameMath::DTOR);
	}

	KVector RotToRadZReturn(float _Rad)
	{
		// 어떤 벡터가 있을때
		KVector Rot = *this;
		Rot.x = (x * cosf(_Rad)) - (y * sinf(_Rad));
		Rot.y = (x * sinf(_Rad)) + (y * cosf(_Rad));
		return Rot;
	}


	void RotToDegY(float _Deg)
	{
		RotToRadY(_Deg * KGameMath::DTOR);
	}

	void RotToRadY(float _Rad)
	{
		KVector Rot = *this;
		Rot.x = (x * cosf(_Rad)) - (z * sinf(_Rad));
		Rot.z = (x * sinf(_Rad)) + (z * cosf(_Rad));
		x = Rot.x;
		z = Rot.z;
		// 내가 돌아가는 
	}

	KVector RotToDegYReturn(float _Deg)
	{
		return RotToRadYReturn(_Deg * KGameMath::DTOR);
	}

	KVector RotToRadYReturn(float _Rad)
	{
		KVector Rot = *this;
		Rot.x = (x * cosf(_Rad)) - (z * sinf(_Rad));
		Rot.z = (x * sinf(_Rad)) + (z * cosf(_Rad));
		return Rot;
	}

	void RotationToRadian(KVector _rot)
	{
		DXVector = DirectX::XMVector3Rotate(DXVector, _rot.RadianToQuaternion());
	}

	void RotationToDegree(KVector _rot)
	{
		RotationToRadian(_rot * KGameMath::DTOR);
	}

	void RotToDegX(float _Deg)
	{
		RotToRadX(_Deg * KGameMath::DTOR);
	}

	void RotToRadX(float _Rad)
	{
		KVector Rot = *this;
		Rot.z = (z * cosf(_Rad)) - (y * sinf(_Rad));
		Rot.y = (z * sinf(_Rad)) + (y * cosf(_Rad));
		z = Rot.z;
		y = Rot.y;
		// 내가 돌아가는 
	}

	KVector RotToDegXReturn(float _Deg)
	{
		return RotToRadXReturn(_Deg * KGameMath::DTOR);
	}

	KVector RotToRadXReturn(float _Rad)
	{
		KVector Rot = *this;
		Rot.z = (z * cosf(_Rad)) - (y * sinf(_Rad));
		Rot.y = (z * sinf(_Rad)) + (y * cosf(_Rad));
		return Rot;
	}

	static KVector Lerp(const KVector& _p1, const KVector& _p2, float _offset)
	{
		/*return (KVector{ 1,1,1 } - KVector{ _offset,_offset,_offset }) * _p1 +
			(KVector{ _offset,_offset,_offset } *_p2);*/

		return DirectX::XMVectorLerp(_p1.DXVector, _p2.DXVector, _offset);
	}

	static KVector QSlerp(const KVector& _p1, const KVector& _p2, float _offset)
	{
		/*float degree = acosf(Dot3D(_p1, _p2));
		return ((_p1 * sinf((1.0f - _offset) * degree)) + (_p2 * sinf(_offset * degree))) / sinf(degree);*/

		return DirectX::XMQuaternionSlerp(_p1.DXVector, _p2.DXVector, _offset);
	}

	void ColorSet(float _R, float _G, float _B, float _A)
	{
		r = _R;
		g = _G;
		b = _B;
		a = _A;
	}

	unsigned int ColorToUint() const
	{
		unsigned int TransColor = 0;
		unsigned char* Ptr = (unsigned char*)&TransColor;
		Ptr[0] = (char)(r * 255);
		Ptr[1] = (char)(g * 255);
		Ptr[2] = (char)(b * 255);
		Ptr[3] = (char)(a * 255);

		return TransColor;
	}

public:
	KVector operator-() const
	{
		KVector Result = *this;
		Result.x = -x;
		Result.y = -y;
		Result.z = -z;
		return Result;
	}

	KVector operator/(float _value) const
	{
		KVector Result = *this;

		if (_value == 0.0f)
		{
			assert(false);
		}

		Result.x /= _value == 0.0f ? 1.0f : _value;
		Result.y /= _value == 0.0f ? 1.0f : _value;
		Result.z /= _value == 0.0f ? 1.0f : _value;
		return Result;
	}

	KVector operator/(const KVector& _Other) const
	{
		KVector Result = *this;

		if (_Other.x == 0.0f ||
			_Other.y == 0.0f ||
			_Other.z == 0.0f)
		{
			assert(false);
		}


		Result.x /= _Other.x == 0.0f ? 1.0f : _Other.x;
		Result.y /= _Other.y == 0.0f ? 1.0f : _Other.y;
		Result.z /= _Other.z == 0.0f ? 1.0f : _Other.z;
		return Result;
	}


	KVector operator*(const KVector& _Other) const
	{
		KVector Result = *this;
		Result.x *= _Other.x;
		Result.y *= _Other.y;
		Result.z *= _Other.z;
		return Result;
	}

	KVector operator*(float _Other) const
	{
		KVector Result = *this;
		Result.x *= _Other;
		Result.y *= _Other;
		Result.z *= _Other;
		return Result;
	}

	KVector& operator*=(float _Other)
	{
		x *= _Other;
		y *= _Other;
		z *= _Other;
		return *this;
	}

	KVector operator+(const KVector& _Other) const
	{
		return { x + _Other.x , y + _Other.y, z + _Other.z };
	}

	KVector operator-(const KVector& _Other) const
	{
		return { x - _Other.x , y - _Other.y, z - _Other.z };
	}


	bool operator!=(const KVector& _Other)
	{
		return x != _Other.x ||
			y != _Other.y ||
			z != _Other.z || w != _Other.w;
	}

	bool operator==(const KVector& _Other)
	{
		return x == _Other.x &&
			y == _Other.y &&
			z == _Other.z;
	}


	KVector& operator+=(const KVector& _Other)
	{
		x += _Other.x;
		y += _Other.y;
		z += _Other.z;
		return *this;
	}

	KVector& operator-=(const KVector& _Other)
	{
		x -= _Other.x;
		y -= _Other.y;
		z -= _Other.z;
		return *this;
	}

	KVector& operator*=(const KVector& _Other)
	{
		x *= _Other.x;
		y *= _Other.y;
		z *= _Other.z;
		return *this;
	}

	KVector& operator=(KVector&& _Other)
	{
		x = _Other.x;
		y = _Other.y;
		z = _Other.z;
		w = _Other.w;
		return *this;
	}

	KVector& operator=(const KVector& _Other)
	{
		x = _Other.x;
		y = _Other.y;
		z = _Other.z;
		w = _Other.w;
		return *this;
	}

public:
	KVector() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{

	}

	KVector(KVector&& _Other) noexcept : x(_Other.x), y(_Other.y), z(_Other.z), w(_Other.w)
	{

	}

	KVector(const DirectX::XMVECTOR& _dxVector) : DXVector(_dxVector)
	{

	}

	KVector(const KVector& _Other) : x(_Other.x), y(_Other.y), z(_Other.z), w(_Other.w)
	{

	}

	KVector(float _X, float _Y) : x(_X), y(_Y), z(0.0f), w(0.0f)
	{

	}

	KVector(float _X, float _Y, float _Z) : x(_X), y(_Y), z(_Z), w(0.0f)
	{

	}

	KVector(float _X, float _Y, float _Z, float _W) : x(_X), y(_Y), z(_Z), w(_W)
	{

	}

};

class KMatrix
{
public:
	union
	{
		KVector ArrVector[4];
		float Arr2D[4][4];
		DirectX::XMMATRIX DXMatrix;
	};

	static float InvSqrt(float f)
	{
		const __m128 fOneHalf = _mm_set_ss(0.5f);
		__m128 Y0, X0, X1, X2, FOver2;
		float temp;

		Y0 = _mm_set_ss(f);
		X0 = _mm_rsqrt_ss(Y0);	// 1/sqrt estimate (12 bits)
		FOver2 = _mm_mul_ss(Y0, fOneHalf);

		// 1st Newton-Raphson iteration
		X1 = _mm_mul_ss(X0, X0);
		X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
		X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

		// 2nd Newton-Raphson iteration
		X2 = _mm_mul_ss(X1, X1);
		X2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X2));
		X2 = _mm_add_ss(X1, _mm_mul_ss(X1, X2));

		_mm_store_ss(&temp, X2);
		return temp;
	}


	static KVector MatrixToQuaternion(const KMatrix& _matrix)
	{
		KVector Return;

		if (_matrix.ArrVector[0].IsNearlyZero() || _matrix.ArrVector[1].IsNearlyZero() || _matrix.ArrVector[2].IsNearlyZero())
		{
			Return.x = 0.0f;
			Return.y = 0.0f;
			Return.z = 0.0f;
			Return.w = 1.0f;
			return Return;
		}

		float	s;

		// Check diagonal (trace)
		const float tr = _matrix.Arr2D[0][0] + _matrix.Arr2D[1][1] + _matrix.Arr2D[2][2];

		if (tr > 0.0f)
		{
			float InvS = InvSqrt(tr + 1.f);
			Return.w = 0.5f * (1.f / InvS);
			s = 0.5f * InvS;

			Return.x = (_matrix.Arr2D[1][2] - _matrix.Arr2D[2][1]) * s;
			Return.y = (_matrix.Arr2D[2][0] - _matrix.Arr2D[0][2]) * s;
			Return.z = (_matrix.Arr2D[0][1] - _matrix.Arr2D[1][0]) * s;
		}
		else
		{
			// diagonal is negative
			int i = 0;

			if (_matrix.Arr2D[1][1] > _matrix.Arr2D[0][0])
				i = 1;

			if (_matrix.Arr2D[2][2] > _matrix.Arr2D[i][i])
				i = 2;

			static const int nxt[3] = { 1, 2, 0 };
			const int j = nxt[i];
			const int k = nxt[j];

			s = _matrix.Arr2D[i][i] - _matrix.Arr2D[j][j] - _matrix.Arr2D[k][k] + 1.0f;

			float InvS = InvSqrt(s);

			float qt[4];
			qt[i] = 0.5f * (1.f / InvS);

			s = 0.5f * InvS;

			qt[3] = (_matrix.Arr2D[j][k] - _matrix.Arr2D[k][j]) * s;
			qt[j] = (_matrix.Arr2D[i][j] + _matrix.Arr2D[j][i]) * s;
			qt[k] = (_matrix.Arr2D[i][k] + _matrix.Arr2D[k][i]) * s;

			Return.x = qt[0];
			Return.y = qt[1];
			Return.z = qt[2];
			Return.w = qt[3];
		}

		return Return;
	}


public:
	operator DirectX::FXMMATRIX()
	{
		return DXMatrix;
	}

	KMatrix& operator=(KMatrix&& _Other)
	{
		DXMatrix = _Other.DXMatrix;
		return *this;
	}

	KMatrix& operator=(const KMatrix& _Other)
	{
		DXMatrix = _Other.DXMatrix;
		return *this;
	}


	KMatrix operator*(const KMatrix& _Other)
	{
		return 	DirectX::XMMatrixMultiply(DXMatrix, _Other.DXMatrix);
	}

	static KMatrix Transformation(KVector _Scale, KVector _RotQ, KVector _Pos)
	{
		return DirectX::XMMatrixTransformation(KVector::ZERO, KVector::ZERO, _Scale.DXVector, KVector::ZERO, _RotQ, _Pos);
	}

public:
	void Scale(KVector _Value)
	{
		Identity();
		DXMatrix = DirectX::XMMatrixScaling(_Value.x, _Value.y, _Value.z);
	}
	void View(KVector _EyePos, KVector _EyeDir, KVector _EyeUp)
	{
		Identity();
		DXMatrix = DirectX::XMMatrixLookToLH(_EyePos, _EyeDir, _EyeUp);
	}
	void ProjDeg(float _Fov, float _W, float _H, float _Near, float _Far)
	{
		ProjRad(_Fov * KGameMath::DTOR, _W, _H, _Near, _Far);
	}

	void ProjRad(float _Fov, float _W, float _H, float _Near, float _Far)
	{
		Identity();
		DXMatrix = DirectX::XMMatrixPerspectiveFovLH(_Fov, _W / _H, _Near, _Far);
	}

	void Orth(float _W, float _H, float _Near, float _Far)
	{
		Identity();
		DXMatrix = DirectX::XMMatrixOrthographicLH(_W, _H, _Near, _Far);
	}

	void OrthOffCenter(float _StartX, float _StartY, float _EndX, float _EndY, float _Near, float _Far)
	{
		Identity();
		DXMatrix = DirectX::XMMatrixOrthographicOffCenterLH(_StartX, _StartY, _EndX, _EndY, _Near, _Far);
	}

	void SetInverse()
	{
		DXMatrix = DirectX::XMMatrixInverse(nullptr, DXMatrix);
	}

	KMatrix Inverse()
	{
		KMatrix Inverse = DirectX::XMMatrixInverse(nullptr, DXMatrix);
		return Inverse;
	}

	void TransPose()
	{
		DXMatrix = DirectX::XMMatrixTranspose(DXMatrix);
	}
	void RotZDeg(float _Value)
	{
		RotZRad(_Value * KGameMath::DTOR);
	}

	void RotZRad(float _Value)
	{
		DXMatrix = DirectX::XMMatrixRotationZ(_Value);
	}
	void RotYDeg(float _Value)
	{
		RotYRad(_Value * KGameMath::DTOR);
	}

	void RotYRad(float _Value)
	{
		DXMatrix = DirectX::XMMatrixRotationY(_Value);
	}

	void RotXDeg(float _Value)
	{
		RotXRad(_Value * KGameMath::DTOR);
	}

	void RotXRad(float _Value)
	{
		DXMatrix = DirectX::XMMatrixRotationX(_Value);
	}

	void RotRadian(KVector _value)
	{
		DXMatrix = DirectX::XMMatrixRotationRollPitchYaw(_value.x, _value.y, _value.z);
	}

	void RotDegree(KVector _value)
	{
		RotRadian(_value * KGameMath::DTOR);
	}

	void Pos(KVector _Pos)
	{
		DXMatrix = DirectX::XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);
	}

	void Identity()
	{
		DXMatrix = DirectX::XMMatrixIdentity();
	}

	void World(KVector _Scale, KVector _RotDeg, KVector _Pos)
	{
		KMatrix Scale;
		KMatrix Rot;
		KMatrix Pos;

		Scale.Scale(_Scale);
		Rot.RotDegree(_RotDeg);
		Pos.Pos(_Pos);

		DXMatrix = Scale * Rot * Pos;
	}

	static KMatrix Affine(KVector _scale, KVector _rotateQuaternion, KVector _position)
	{
		return DirectX::XMMatrixAffineTransformation(_scale, KVector::CORD_ZERO, _rotateQuaternion, _position);
	}

	//w 1: coordinate, w 0: normal 
	KVector Multi(KVector _Vector, float _w) const
	{
		_Vector.w = _w;

		return DirectX::XMVector4Transform(_Vector, DXMatrix);
	}

	KVector ExtractScaling()
	{
		KVector ret = KVector::ZERO;

		float Tolerance = 1.e-8f;

		const float SquareSum0 = (Arr2D[0][0] * Arr2D[0][0]) + (Arr2D[0][1] * Arr2D[0][1]) + (Arr2D[0][2] * Arr2D[0][2]);
		const float SquareSum1 = (Arr2D[1][0] * Arr2D[1][0]) + (Arr2D[1][1] * Arr2D[1][1]) + (Arr2D[1][2] * Arr2D[1][2]);
		const float SquareSum2 = (Arr2D[2][0] * Arr2D[2][0]) + (Arr2D[2][1] * Arr2D[2][1]) + (Arr2D[2][2] * Arr2D[2][2]);

		if (SquareSum0 > Tolerance)
		{
			float Scale0 = sqrtf(SquareSum0);
			ret.x = Scale0;
			float InvScale0 = 1.f / Scale0;
			Arr2D[0][0] *= InvScale0;
			Arr2D[0][1] *= InvScale0;
			Arr2D[0][2] *= InvScale0;
		}
		else
		{
			ret.x = 0;
		}

		if (SquareSum1 > Tolerance)
		{
			float Scale1 = sqrtf(SquareSum1);
			ret.y = Scale1;
			float InvScale1 = 1.f / Scale1;
			Arr2D[1][0] *= InvScale1;
			Arr2D[1][1] *= InvScale1;
			Arr2D[1][2] *= InvScale1;
		}
		else
		{
			ret.y = 0;
		}

		if (SquareSum2 > Tolerance)
		{
			float Scale2 = sqrtf(SquareSum2);
			ret.z = Scale2;
			float InvScale2 = 1.f / Scale2;
			Arr2D[2][0] *= InvScale2;
			Arr2D[2][1] *= InvScale2;
			Arr2D[2][2] *= InvScale2;
		}
		else
		{
			ret.z = 0;
		}

		return ret;
	}

	inline float Determinant() const
	{
		return	Arr2D[0][0] * (
			Arr2D[1][1] * (Arr2D[2][2] * Arr2D[3][3] - Arr2D[2][3] * Arr2D[3][2]) -
			Arr2D[2][1] * (Arr2D[1][2] * Arr2D[3][3] - Arr2D[1][3] * Arr2D[3][2]) +
			Arr2D[3][1] * (Arr2D[1][2] * Arr2D[2][3] - Arr2D[1][3] * Arr2D[2][2])
			) -
			Arr2D[1][0] * (
				Arr2D[0][1] * (Arr2D[2][2] * Arr2D[3][3] - Arr2D[2][3] * Arr2D[3][2]) -
				Arr2D[2][1] * (Arr2D[0][2] * Arr2D[3][3] - Arr2D[0][3] * Arr2D[3][2]) +
				Arr2D[3][1] * (Arr2D[0][2] * Arr2D[2][3] - Arr2D[0][3] * Arr2D[2][2])
				) +
			Arr2D[2][0] * (
				Arr2D[0][1] * (Arr2D[1][2] * Arr2D[3][3] - Arr2D[1][3] * Arr2D[3][2]) -
				Arr2D[1][1] * (Arr2D[0][2] * Arr2D[3][3] - Arr2D[0][3] * Arr2D[3][2]) +
				Arr2D[3][1] * (Arr2D[0][2] * Arr2D[1][3] - Arr2D[0][3] * Arr2D[1][2])
				) -
			Arr2D[3][0] * (
				Arr2D[0][1] * (Arr2D[1][2] * Arr2D[2][3] - Arr2D[1][3] * Arr2D[2][2]) -
				Arr2D[1][1] * (Arr2D[0][2] * Arr2D[2][3] - Arr2D[0][3] * Arr2D[2][2]) +
				Arr2D[2][1] * (Arr2D[0][2] * Arr2D[1][3] - Arr2D[0][3] * Arr2D[1][2])
				);
	}

public:
	KMatrix()
	{
		Identity();
	}

	KMatrix(KMatrix&& _Other) noexcept : DXMatrix(_Other.DXMatrix)
	{
	}

	KMatrix(const KMatrix& _Other)
	{
		memcpy_s(this, sizeof(KMatrix), &_Other.DXMatrix, sizeof(KMatrix));
	}

	KMatrix(const DirectX::XMMATRIX& _DXMAT) : DXMatrix(_DXMAT)
	{
	}

};

KVector operator*(const KVector& _Left, const KMatrix& _Right);


typedef KVector KCOLOR;
typedef KVector KCOLDATA;
typedef KVector float4;