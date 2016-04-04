/*******************************************
 * LTask.h
 * A common task object to do by thread
 * @author buptlong@126.com
 * 2015-11-25
 *******************************************/

#ifndef __L_TASK_H__
#define __L_TASK_H__

#include "define_l.h"
#include <string>
#include <map>
#include <functional>

NS_LONG_BEGIN

using namespace std;

/**
 * TaskDataParam: use to transfer params to the thread.
 */
class TaskDataParam
{
public:
    TaskDataParam(){
		m_strings.clear();
		m_numbers.clear();
	};
    virtual ~TaskDataParam(){};
    
public:
    void pushString(std::string key, std::string value);
    void pushNumber(std::string key, double value);
    std::string getStringValue(std::string key);
    double getNumberValue(std::string key);
    
private:
    std::map<std::string, std::string> m_strings;
    std::map<std::string, double> m_numbers;
};

/**
 * TaskData: a base taskData with no-params-input function.
 */
class TaskData
{
public:
    TaskData(std::string id, std::function<void()> taskFunction):
    m_id(id), m_taskFunction(taskFunction)
    {
        
    };
    virtual ~TaskData()
	{

	};
    
    /**
     * Get a task ID as a string, which marks a unique task.
     * This ID can be set when a task pushed in the thread pool.
     * This ID is required when multi-task pushed.
     */
    virtual std::string getID();
    
    /**
     * Get the task function which will be executed by a thread.
     */
    std::function<void()> getTaskFunction();
    
    /**
     * Execute the task.
     */
    virtual void execute();
    
private:
    std::string m_id;
    
    /**
     * Task function which will be executed by a thread.
     */
    std::function<void()> m_taskFunction;
};

/**
 * TaskDataCommon:  a common taskData class inherited from TaskData.
 *                  struct TaskDataParam used to set args as you want.
 */
class TaskDataCommon : public TaskData
{
public:
	TaskDataCommon(std::string id, std::function<void(TaskDataParam *)> taskFunction, TaskDataParam * dataParam) :
        TaskData(id, nullptr), m_taskFunction(taskFunction), m_TaskDataParam(dataParam)
	{

	};
	virtual ~TaskDataCommon() 
	{
		if (m_TaskDataParam)
		{
			delete m_TaskDataParam;
			m_TaskDataParam = nullptr;
		}
	};
    
    /**
     * Get the task function which will be executed by a thread.
     */
	std::function<void(TaskDataParam *)> getTaskFunction();

    /**
     * Execute the task.
     */
	virtual void execute();

private:
	TaskDataParam * m_TaskDataParam;
	std::function<void(TaskDataParam *)> m_taskFunction;
};

class LTask
{
public:
    LTask();
	virtual ~LTask();
	static LTask * create(TaskData * taskData);
	void execute();
public:
	TaskData * getTaskData();
	std::string getID();
	std::function<void()> getTaskFunction();

private:
	bool initialize(TaskData * taskData);

private:
	TaskData * m_pTaskData;
};

NS_LONG_END

#endif //__L_TASK_H__