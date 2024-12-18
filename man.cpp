
#include <windows.h>
#include <process.h>
#include "MultithreadProfiler.h"

int test()
{
    PROFILE(1, "Test1");
    return  1;
}

int test2()
{
    PROFILE(1, "Test2");
    return 1;
}

int test3()
{
    thread_local static int test_Index1 = PROFILER::CurrentTestIdxCnt++; PROFILE_REQUEST pr1{ test_Index1,__FUNCSIG__,"Test3" };;
    return 1;
}

unsigned ThreadProc(void* pParam)
{
    //for (int i = 0; i < 50000; ++i)
    while(1)
    {
		test();
		test();
		test();
		test();
		test();
		test();
		test2();
		test2();
    }
    return 0;
}

unsigned ThreadProc1(void* pParam)
{
    //for (int i = 0; i < 50000; ++i)
    while(1)
    {
		test();
		test();
		test();
		test();
		test();
		test();
		test2();
		test2();
        test3();
        test3();
        test3();
    }
    return 1;
}

int main()
{
	PROFILER::Init();
    HANDLE hThreadArr[3];
    for (int i = 0; i < 3; ++i)
    {
        hThreadArr[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadProc, nullptr, CREATE_SUSPENDED, nullptr);
        if (i == 2)
        {
            hThreadArr[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadProc1, nullptr, CREATE_SUSPENDED, nullptr);
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        ResumeThread(hThreadArr[i]);
    }

    while (1)
    {
        Sleep(1000);
        if (GetAsyncKeyState(VK_RETURN) & 0x01)
        {
            PROFILER::Reset();
        }

        if (GetAsyncKeyState(VK_BACK) & 0x01)
        {
            PROFILER::ProfileDataOutText("Profile.txt");
        }
    }

    WaitForMultipleObjects(3, hThreadArr, TRUE, INFINITE);
    PROFILER::ProfileDataOutText("profile.txt");
}