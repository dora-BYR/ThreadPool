#include "LTask.h"

NS_LONG_BEGIN

void TaskDataParam::pushString(std::string key, std::string value)
{
    m_strings[key] = value;
}

void TaskDataParam::pushNumber(std::string key, double value)
{
    m_numbers[key] = value;
}

std::string TaskDataParam::getStringValue(std::string key)
{
	if (m_numbers.empty())
	{
		return "";
	}
    for (auto &iter: m_strings)
    {
        if (iter.first == key)
        {
            return iter.second;
        }
    }
    return "";
}

double TaskDataParam::getNumberValue(std::string key)
{
	if (m_numbers.empty())
	{
		return 0;
	}
    for (auto &iter: m_numbers)
    {
        if (iter.first == key)
        {
            return iter.second;
        }
    }
    return 0;
}

std::string TaskData::getID()
{
    return m_id;
};

std::function<void()> TaskData::getTaskFunction()
{
    return m_taskFunction;
};

void TaskData::execute()
{
    if (m_taskFunction)
    {
        m_taskFunction();
    }
};

std::function<void(TaskDataParam *)> TaskDataCommon::getTaskFunction()
{
    return m_taskFunction;
};

void TaskDataCommon::execute()
{
    if (m_taskFunction)
    {
        m_taskFunction(m_TaskDataParam);
    }
};

LTask::LTask():
	m_pTaskData(nullptr)
{
}

LTask::~LTask()
{

}

LTask * LTask::create(TaskData * taskData)
{
	auto task = new LTask();
	if (task && task->initialize(taskData))
	{
		return task;
	}
	else
	{
		//task->release();
	}
	return nullptr;
}

bool LTask::initialize(TaskData * taskData)
{
	m_pTaskData = taskData;
	return true;
}

TaskData * LTask::getTaskData()
{
	return m_pTaskData;
}

std::string LTask::getID()
{
	if (m_pTaskData)
	{
		return m_pTaskData->getID();
	}
	return "";
}

std::function<void()> LTask::getTaskFunction()
{
	if (m_pTaskData)
	{
		return m_pTaskData->getTaskFunction();
	}
	return nullptr;
}

void LTask::execute()
{
    if (m_pTaskData)
    {
        return m_pTaskData->execute();
    }
}

NS_LONG_END