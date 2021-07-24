#include "KCriticalSection.h"

KAutoCriticalSection KCriticalSection::operator()()
{
    return KAutoCriticalSection(*this);
}
