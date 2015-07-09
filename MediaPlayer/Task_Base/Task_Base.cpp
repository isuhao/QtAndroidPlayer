#include "Task_Base.h"
#include "CriticalSection.h"
#include "EventImpl.h"
#include<pthread.h>
Task_Base::Task_Base(void)
{
	m_poCriSection	= new CCriticalSection;
	m_p_event		=new CEventImpl;
}

Task_Base::~Task_Base(void)
{
	wait();
	delete m_poCriSection;
	delete m_p_event;
}
void * Task_Base::svc_run( void *apParam)
{
    ((Task_Base *)apParam)->svc();
}

int Task_Base::activate( int n_threads /*= 1*/ )
{
    pthread_t threadID;

	while(n_threads)
	{
        int  err = pthread_create(&threadID, NULL, svc_run, (void*)this);

        //HANDLE hThreadRecv		= (HANDLE)_beginthreadex( NULL, 0, &Task_Base::svc_run, (LPVOID)this, 0, &threadID );//接受网络数据

        h_threads.push((int)threadID);

		n_threads --;
	}
    return true;
}

int	Task_Base::thr_count (void) const
{
	return h_threads.size();
}

int Task_Base::putq (const std::string &str)
{
	{
		CCriticalAutoLock loAutoLock(*m_poCriSection);
		msg_queue_.push(str);
    }
	m_p_event->set_event();	//设置活动事件

    return true;
}

int Task_Base::getq (std::string &str)
{
	while(msg_queue_.empty())
	{
		m_p_event->reset_event();//队列为空了 不能活动 阻塞
		m_p_event->wait_for_single_object();
	}
	
	{
		CCriticalAutoLock loAutoLock(*m_poCriSection);

		str = msg_queue_.front();
		msg_queue_.pop();
    }
    return true;
}

void Task_Base::wait()
{
	std::queue<int>  temp = h_threads;

    while(!temp.empty())
    {
        pthread_join((pthread_t)temp.front(),NULL);//  wait...
        temp.pop();
    }

    while(!h_threads.empty())
        h_threads.pop();
}

void Task_Base::discard()
{
    CCriticalAutoLock loAutoLock(*m_poCriSection);
    while(!msg_queue_.empty())
        msg_queue_.pop();
}
int Task_Base::close()
{
    return 0;
}
int Task_Base::svc()
{
    return 0;
}
//主动工作线程函数
