#pragma once
#include <KGameLogic.h>
#include <KCamera.h>

class KFreeCamera : public KGameComponent
{
public:
	KFreeCamera(KPTR< KCamera> _camera);
	KFreeCamera(KPTR< KCamera> _camera, float _Value);

private:
	KPTR<KCamera> m_Camera;
	KPTR<KTransform> m_FollowTransform;
	KVector m_FollowPosition;
	KVector m_FollowRotation;

	float m_rotateSpeed;
	float m_translateSpeed;
	float m_ZoomSpeed;

public:
	void FollowTransform(KPTR<KTransform> _FollowTransform, KVector _FollowPosition, KVector _FollowRotation);

public:
	void Init() override;
	void Update() override;
	void FreeUpdate();
	void FollowUpdate();
};

