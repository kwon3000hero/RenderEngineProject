#include "KGAMEMATH.h"

const float KGameMath::PI = 3.14159265358979323846264338327950288419f;
const float KGameMath::PI2 = KGameMath::PI * 2;
const float KGameMath::DTOR = PI / 180.0F;
const float KGameMath::RTOD = 180.0F / PI;

KVector KVector::LEFT = { -1.0f, 0.0f, 0.0f, 0.0f };
KVector KVector::RIGHT = { 1.0f, 0.0f, 0.0f, 0.0f };
KVector KVector::DOWN = { 0.0f, -1.0f, 0.0f, 0.0f };
KVector KVector::UP = { 0.0f, 1.0f, 0.0f, 0.0f };
KVector KVector::FORWARD = { 0.0f, 0.0f, 1.0f, 0.0f };
KVector KVector::BACK = { 0.0f, 0.0f, -1.0f, 0.0f };
KVector KVector::ONE = { 1.0f, 1.0f, 1.0f, 1.0f };
KVector KVector::ZERO = { 0.0f, 0.0f, 0.0f, 0.0f };
KVector KVector::CORD_ZERO = { 0.0f, 0.0f, 0.0f, 1.0f };

KVector KVector::WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
KVector KVector::BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
KVector KVector::RED = { 1.0f, 0.0f, 0.0f, 1.0f };
KVector KVector::BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
KVector KVector::GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };

KVector operator*(const KVector& _Left, const KMatrix& _Right)
{
	return KVector();
}