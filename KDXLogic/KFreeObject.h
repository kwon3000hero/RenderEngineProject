#include <KGameComponent.h>

class KFreeObject : public KActorGameComponent
{
public:
	KFreeObject();


public:
	void StartData(float _Value = 1.0f);

public:
	float Speed;

public:
	void Init() override;
	void Update() override;


};

