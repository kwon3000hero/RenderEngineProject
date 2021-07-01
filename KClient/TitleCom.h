#pragma once
#include <KGameComponent.h>

class KCuttingRender;
class TitleCom : public KSceneGameComponent
{
private:
	float m_progress;
	bool m_isStart;

public:

	virtual void Init();
	void PrevUpdate() override;

public:
	TitleCom() :m_progress(0.0f), m_isStart(false) {	}
	~TitleCom() {	}

};