#include "KThread.h"

unsigned __stdcall KThread::ThreadBaseFunction(void* _pArg)
{
	if (nullptr == _pArg)
	{
		AssertMsg(L"존재하지 않는 쓰레드로 콜백 진행 불가.3");
	}

	KThread* pThread = (KThread*)_pArg;
	pThread->m_Function(pThread);
	pThread->m_ThreadHandle = nullptr;

	return 0;
}

void KThread::Start()
{
	m_ThreadHandle = (HANDLE)_beginthreadex(nullptr, 0, ThreadBaseFunction, this, 0, nullptr);
}

KThread::KThread()
{
}

KThread::~KThread()
{
	if (nullptr != m_ThreadHandle)
	{
		TerminateThread(m_ThreadHandle, 0);
		CloseHandle(m_ThreadHandle);
	}
}
