#include "EventImpl.h"

CEventImpl::CEventImpl(void)
{
    pthread_mutex_init(&m_hMutex, NULL);

    pthread_cond_init(&m_hCond_t, NULL);

    //((HANDLE)m_hEventHandle) = CreateEvent(NULL, TRUE,FALSE , NULL);// ÷∂Ø
}

CEventImpl::~CEventImpl(void)
{
    pthread_mutex_destroy(&m_hMutex);

    pthread_cond_destroy(&m_hCond_t);
    //CloseHandle(((HANDLE)m_hEventHandle));
    //((HANDLE)m_hEventHandle) = NULL;
}

void CEventImpl::set_event()
{
    pthread_mutex_lock(&m_hMutex);

    pthread_cond_signal(&m_hCond_t);

    pthread_mutex_unlock(&m_hMutex);
    //::SetEvent(((HANDLE)m_hEventHandle));
}

void CEventImpl::reset_event()
{
    //::ResetEvent(((HANDLE)m_hEventHandle));
}

void CEventImpl::wait_for_single_object()
{
    pthread_mutex_lock(&m_hMutex);

    pthread_cond_wait(&m_hCond_t, &m_hMutex);

    pthread_mutex_unlock(&m_hMutex);
    //WaitForSingleObject(((HANDLE)m_hEventHandle),INFINITE);
}
