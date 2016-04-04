#include "LThreadPool.h"

NS_LONG_BEGIN

static LThreadPool * g_pLThreadPool = 0;

LThreadPool::LThreadPool():
	m_defaultThreadCount(1),
	m_stopFlag(false),
    m_taskKey(1)
{
}

LThreadPool::~LThreadPool()
{

}

LThreadPool * LThreadPool::getInstance()
{
	if (!g_pLThreadPool)
	{
		g_pLThreadPool = LThreadPool::create();
	}
	return g_pLThreadPool;
}

LThreadPool * LThreadPool::create()
{
	auto pool = new LThreadPool();
	if (pool && pool->initialize())
	{
		return pool;
	}
	else
	{
		//pool->release();
	}
	return nullptr;
}

bool LThreadPool::initialize()
{
	this->addThread(m_defaultThreadCount);
	return true;
}

void LThreadPool::pushTask(std::function<void()> taskFunction)
{
	if (m_stopFlag.load())
	{
		LLOG("LTheadPool: closed. no task can be added");
		return;
	}
	m_mutexTask.lock();
	TaskData * _pData = new TaskData("normal", taskFunction);
	auto _pTask = LTask::create(_pData);
	m_taskQueue.push(_pTask);
	m_mutexTask.unlock();

	m_cvTask.notify_all();
}

void LThreadPool::pushCommonTask(std::function<void(TaskDataParam *)> taskFunction, TaskDataParam * dataParam)
{
	if (m_stopFlag.load())
	{
		LLOG("LTheadPool: closed. no task can be added");
		return;
	}
	m_mutexTask.lock();
	TaskDataCommon * _pData = new TaskDataCommon("common", taskFunction, dataParam);
	auto _pTask = LTask::create(_pData);
	m_taskQueue.push(_pTask);
	m_mutexTask.unlock();

	m_cvTask.notify_all();
}

void LThreadPool::popTask()
{
	m_mutexTask.lock();
	m_taskQueue.pop();
	m_mutexTask.unlock();
}

LTask * LThreadPool::getOneTask()
{
	std::unique_lock<std::mutex> _lock { m_mutexTask_1 };
	m_cvTask.wait(_lock, [this](){
		return !this->m_taskQueue.empty();
	});
	LTask * task = m_taskQueue.front();
	this->popTask();
	return task;
}


void LThreadPool::taskSchedule()
{
	while (true)
	{
		LTask * task = this->getOneTask();
		if (task)
		{
			//LLOG("LTheadPool: begin to execute a task: %s", task->getID().c_str());
			task->execute();
            
			delete task;
            task = nullptr;
		}
	}
}

void LThreadPool::open()
{
	m_stopFlag.store(false);
}

void LThreadPool::close()
{
	m_stopFlag.store(true);
}

void LThreadPool::addThread(int count)
{
	if (count >= 1)
	{
		for (size_t i = 0; i < count; i++)
		{
			m_theadPool.emplace_back(&LThreadPool::taskSchedule, this);
		}
	}
}

NS_LONG_END