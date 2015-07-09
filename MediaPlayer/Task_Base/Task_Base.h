#pragma once
#include <queue>
#include <string>
using namespace std;

class CCriticalSection;
class CEventImpl;
// ACTIVE OBJECT  �������� ģʽ
class Task_Base
{
public:
	Task_Base(void);
public:
	virtual ~Task_Base(void);
public:
	//��ʼ�����߳�����
	virtual int activate (int n_threads = 1);
	/// ֹͣ����
    virtual int close();

	int		thr_count (void) const;

	int		putq (const std::string &str);

	int		getq (std::string &str);
protected:
	void	wait();//�ȴ��߳��˳�

    void	discard();//����ʣ�������
private:
    virtual  int svc();//���������̺߳���

    static void * svc_run( void *apParam);//TCP�հ������߳�
private:
	std::queue<std::string> msg_queue_;//�������

    std::queue<int>		 h_threads;
	//ͬ����
	CCriticalSection	*m_poCriSection;
	CEventImpl			*m_p_event;
};
