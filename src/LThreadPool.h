/*******************************************
 * LThreadPool.h
 * a thread pool for multi-thread&multi-task-in-quene
 * @author buptlong@126.com
 * 2015-11-25
 *******************************************/

#ifndef _L_THREADPOOL_H_
#define _L_THREADPOOL_H_
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include "define_l.h"
#include "LTask.h"


NS_LONG_BEGIN

class LThreadPool
{
public:
	LThreadPool();
	virtual ~LThreadPool();
	static LThreadPool * getInstance();

public:
	void addThread(int count);
	void pushTask(std::function<void()> taskFunction);
	void pushCommonTask(std::function<void(TaskDataParam *)> taskFunction, TaskDataParam * dataParam);
    void popTask();
	void open();
	void close();
private:
	static LThreadPool * create();
	bool initialize();
	void taskSchedule();
	LTask * getOneTask();
private:
	std::vector<std::thread> m_theadPool;
	std::queue<LTask *> m_taskQueue;
	std::mutex m_mutexTask;
	std::mutex m_mutexTask_1;
	int m_defaultThreadCount;
	std::atomic<bool> m_stopFlag;
	std::condition_variable m_cvTask;
    int m_taskKey;
};

NS_LONG_END

#endif //_L_THREADPOOL_H_