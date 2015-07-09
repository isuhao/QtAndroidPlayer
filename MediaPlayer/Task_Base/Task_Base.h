#pragma once
#include <queue>
#include <string>
using namespace std;

class CCriticalSection;
class CEventImpl;
// ACTIVE OBJECT  主动对象 模式
class Task_Base
{
public:
	Task_Base(void);
public:
	virtual ~Task_Base(void);
public:
	//开始主动线程运行
	virtual int activate (int n_threads = 1);
	/// 停止运行
    virtual int close();

	int		thr_count (void) const;

	int		putq (const std::string &str);

	int		getq (std::string &str);
protected:
	void	wait();//等待线程退出

    void	discard();//丢弃剩余的数据
private:
    virtual  int svc();//主动工作线程函数

    static void * svc_run( void *apParam);//TCP收包处理线程
private:
	std::queue<std::string> msg_queue_;//任务队列

    std::queue<int>		 h_threads;
	//同步锁
	CCriticalSection	*m_poCriSection;
	CEventImpl			*m_p_event;
};
