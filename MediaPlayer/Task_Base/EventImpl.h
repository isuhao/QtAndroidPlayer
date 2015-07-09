#pragma once
#include <pthread.h>
//#include <sys/types.h>
#include <sys/timeb.h>
class CEventImpl
{
public:
	CEventImpl(void);

	virtual void set_event();
	virtual void reset_event();
	virtual void wait_for_single_object();

public:
	virtual ~CEventImpl(void);
private:
    pthread_mutex_t m_hMutex;
    pthread_cond_t  m_hCond_t;
};
