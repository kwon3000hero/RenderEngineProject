#include <KGameLogic.h>

class KFreeObject : public KGameComponent
{
public:
	KFreeObject();
	KFreeObject(float _Value);

public:
	float m_speed;

public:
	void Init() override;
	void Update() override;
};

