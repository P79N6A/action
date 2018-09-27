/*
 * pp_time.h
 *
 *  Created on: 2015��11��11��
 *      Author: purperzhang
 */

#ifndef APPS_APOLLO_CHAT_PP_COMMON_PP_TIME_H_
#define APPS_APOLLO_CHAT_PP_COMMON_PP_TIME_H_
#ifdef linux
#include <sys/time.h>
#include <unistd.h>
#elif defined(WIN32) || defined(WIN64)
#include <winsock2.h>
#endif
#include <time.h>


#ifdef linux
namespace pp
{
	inline unsigned int gettime_ms()
	{
		struct timeval tv;
		gettimeofday(&tv,0);
		return tv.tv_sec*1000+tv.tv_usec/1000;
	}
	inline unsigned int gettime_s()
	{
		return static_cast<unsigned int>(time(0));
	}
	inline unsigned int gettime_us()
	{
		struct timeval tv;
		gettimeofday(&tv,0);
		return tv.tv_sec*1000000+tv.tv_usec;
	}
	inline void sleep_ms(unsigned int t)
	{
		usleep(1000*t);
	}
	inline void gettime(int*ts,unsigned long long*tms,unsigned long long*tus)
	{
		struct timeval tv;
		gettimeofday(&tv,0);
		if(ts) *ts=tv.tv_sec;
		if(tms) *tms=tv.tv_sec*1000ULL+tv.tv_usec/1000ULL;
		if(tus) *tus=tv.tv_sec*1000000ULL+tv.tv_usec;
	}
}
#elif defined(WIN32) || defined(WIN64)
namespace pp
{
	inline unsigned int gettime_ms()
	{
		return GetTickCount();
	}
	inline unsigned int gettime_s()
	{
		return static_cast<unsigned int>(time(0));
	}
	inline unsigned int gettime_us()
	{
		LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
		LARGE_INTEGER Frequency;

		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartingTime);
//
//		// Activity to be timed
//
//		QueryPerformanceCounter(&EndingTime);
//		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
//
//
//		//
//		// We now have the elapsed number of ticks, along with the
//		// number of ticks-per-second. We use these values
//		// to convert to the number of elapsed microseconds.
//		// To guard against loss-of-precision, we convert
//		// to microseconds *before* dividing by ticks-per-second.
//		//
//
//		ElapsedMicroseconds.QuadPart *= 1000000;
//		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
//		return ElapsedMicroseconds.QuadPart;
		{
			return StartingTime.QuadPart*1000000/Frequency.QuadPart;
		}
	}
	inline void sleep_ms(unsigned int t)
	{
		Sleep(t);
	}
}
#endif

#endif /* APPS_APOLLO_CHAT_PP_COMMON_PP_TIME_H_ */
