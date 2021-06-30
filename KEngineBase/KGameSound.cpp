#include "KGameSound.h"
#include "KGAMEDEBUG.h"
#include "KGAMEPATH.h"

FMOD::System* KGameSound::m_pSystem = nullptr;
std::map<KGAMESTR, KGameSound*> KGameSound::m_soundContainer;
std::map<KGAMESTR, FMOD::Channel*> KGameSound::m_channelContainer;
KGameSound::Creator KGameSound::m_creator = KGameSound::Creator();

void KGameSound::Init()
{
	FMOD::System_Create(&m_pSystem);

	if (nullptr == m_pSystem)
	{
		BOOM;
		return;
	}

	FMOD_RESULT result = m_pSystem->init(32, FMOD_DEFAULT, nullptr);

	if (FMOD_OK != result)
	{
		BOOM;
		return;
	}
}

void KGameSound::Destroy()
{
	{
		static std::map<KGAMESTR, KGameSound*>::iterator iter = m_soundContainer.begin();
		static std::map<KGAMESTR, KGameSound*>::iterator end = m_soundContainer.end();

		for (; iter != end; ++iter)
		{
			if (nullptr != iter->second)
			{
				delete iter->second;
			}
		}
	}

	{
		static std::map<KGAMESTR, FMOD::Channel*>::iterator iter = m_channelContainer.begin();
		static std::map<KGAMESTR, FMOD::Channel*>::iterator end = m_channelContainer.end();

		for (; iter != end; ++iter)
		{
			if (nullptr != iter->second)
			{
				delete iter->second;
			}
		}
	}

	if (nullptr != m_pSystem)
	{
		m_pSystem->release();
	}
}


bool KGameSound::Load(const KGAMESTR& path)
{
	return Load(KGAMEPATH::GetFileName(path), path);
}

bool KGameSound::Load(const KGAMESTR& key, const KGAMESTR& path)
{
	if (m_soundContainer.end() != m_soundContainer.find(path))
	{
		BOOM;
		return false;
	}

	KGameSound* pNewSound = new KGameSound();
	pNewSound->Load(path);

	m_soundContainer.insert(std::map<KGAMESTR, KGameSound*>::value_type(key, pNewSound));

	return true;
}

void KGameSound::Play(const KGAMESTR& key)
{
	std::map<KGAMESTR, KGameSound*>::iterator findSound = m_soundContainer.find(key);

	if (m_soundContainer.end() == findSound)
	{
		BOOM;
	}

	m_pSystem->playSound(findSound->second->m_pSound, nullptr, false, nullptr);
}

void KGameSound::Play(const KGAMESTR& key, const KGAMESTR& path)
{
	std::map<KGAMESTR, KGameSound*>::iterator findSound = m_soundContainer.find(key);

	if (m_soundContainer.end() == findSound)
	{
		BOOM;
	}

	FMOD::Channel* pChannel = nullptr;
	m_pSystem->playSound(findSound->second->m_pSound, nullptr, false, &pChannel);
	m_channelContainer.insert(std::map < KGAMESTR, FMOD::Channel*>::value_type(key, pChannel));
}

void KGameSound::Stop(const KGAMESTR& key)
{
	std::map<KGAMESTR, FMOD::Channel*>::iterator findControl = m_channelContainer.find(key);

	if (m_channelContainer.end() == findControl)
	{
		BOOM;
	}

	bool isPlaying = false;
	findControl->second->isPlaying(&isPlaying);
	if (!isPlaying)
	{
		return;
	}

	findControl->second->stop();
}

void KGameSound::Update()
{
	m_pSystem->update();

	std::map<KGAMESTR, FMOD::Channel*>::iterator iter = m_channelContainer.begin();
	std::map<KGAMESTR, FMOD::Channel*>::iterator end = m_channelContainer.end();

	for (; iter != end; )
	{
		bool Check = false;
		iter->second->isPlaying(&Check);
		if (true == Check)
		{
			++iter;
			continue;
		}

		iter = m_channelContainer.erase(iter);
	}
}

void KGameSound::InnerLoad(const KGAMESTR& path)
{
	std::string strPath = path;

	FMOD_RESULT result = m_pSystem->createSound(strPath.c_str(), FMOD_DEFAULT, nullptr, &m_pSound);

	if (nullptr == m_pSound || FMOD_OK != result)
	{
		BOOM;
		return;
	}
}

KGameSound::KGameSound() :m_pSound(nullptr)
{

}

KGameSound::~KGameSound()
{
	if (nullptr != m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}
}