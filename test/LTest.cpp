#include "LTest.h"
#include "../src/LTask.h"
#include "../src/LThreadPool.h"

NS_LONG_BEGIN

void LTest::test_single_thread_multi_task()
{
	// sample 1
//	LTask task1([=]() {
//		LLOG("i'm a sub thread task 1");
//		int count = 0;
//		do {
//			LLOG("i'm task 1");
//			count = count + 1;
//		} while (count < 10);
//	});
//	task1.commit();

	// sample 2
	auto task2 = LTask::create([=]() {
		LLOG("i'm a sub thread task 2");
		int count = 0;
		do {
			LLOG("i'm task 2");
			count = count + 1;
		} while (count < 8);
	});
	task2->commit();

	// sample 3
	auto pool = LThreadPool::getInstance();
	pool->pushTask([=]() {
		LLOG("i'm a sub thread task 3");
		int count = 0;
		do {
			LLOG("i'm task 3");
			count = count + 1;
		} while (count < 12);
	});
}

void LTest::test_multi_thread_multi_task()
{
	auto pool = LThreadPool::getInstance();
	pool->pushTask([=]() {
		LLOG("i'm a sub thread task 1");
		do {
			LLOG("i'm task 1");
		} while (1);
	});
	/////////////////////
	pool->addThread(1);

	const char * name = "longhui";
	int age = 25;
	std:string desc("master of BUPT");
	TaskDataParam * dataParam = new TaskDataParam();
	dataParam->pushString("name", std::string(name));
	dataParam->pushNumber("age", age);
	dataParam->pushString("desc", std::string(desc));
	pool->pushCommonTask([=](TaskDataParam * _dataParam) {
		LLOG("i'm a sub thread task 2");
		do {
		    LLOG("i'm task 2");
			LLOG("name = %s", _dataParam->getStringValue("name").c_str());
			LLOG("age = %ld", _dataParam->getNumberValue("age"));
			LLOG("desc = %s", _dataParam->getStringValue("desc").c_str());
		} while (1);
	}, dataParam);
	////////////////////
	pool->pushTask([=]() {
		LLOG("i'm a sub thread task 3");
		do {
			LLOG("i'm task 3");
		} while (1);
	});
	pool->addThread(1);
}


NS_LONG_END