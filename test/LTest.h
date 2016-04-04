/*******************************************
 * LTest.h
 * A test for threadpool
 * @author buptlong@126.com
 * 2015-11-25
 *******************************************/

#ifndef __L_TEST_H__
#define __L_TEST_H__

#include "../src/define_l.h"

NS_LONG_BEGIN

using namespace std;

class LTest
{
public:
	LTest(){};
    virtual ~LTest(){};
    
public:
	static void test_single_thread_multi_task();
	static void test_multi_thread_multi_task();
    
private:

};

__L_TEST_H__

NS_LONG_END

#endif //__L_TEST_H__