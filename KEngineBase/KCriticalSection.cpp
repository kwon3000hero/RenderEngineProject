#include "KCriticalSection.h"
#include "KCriticalSection.h"

KAutoCriticalSection KCriticalSection::operator()()
{
    return KAutoCriticalSection(*this);
}
