#include <windows.h>
#include "GameTimer.h"

GameTimer::GameTimer() : mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
                         mPasedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
    __int64 countsPerSec;
    QueryPerformanceFrequency((LARGE_INTEGER*)& countsPerSec);
    // 每统计一次经历多少秒
    mSecondsPerCount = 1.0 / (double) countsPerSec;
}
/**
 * @brief 
 * Returns the total time elapsed since Reset() was called, 
 * NOT counting any time when the clock is stopped
 * 
 * 返回自调用 Reset() 以来经过的总时间，
 * 不计算时钟停止的时间。
 * 
 * @return float 
 */
float GameTimer::TotalTime() const{

    // If we are stopped, do not count the time that has passed since we stopped.
    // Moreover, if we previously already had a pause, the distance 
    // mStopTime - mBaseTime includes paused time, which we do not want to count.
    // To correct this, we can subtract the paused time from mStopTime:  
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

    // 如果暂停: 不计算自从停下来已经过去的时间
    // 同时，如果之前已经暂停过了，那么不用计算暂停时间mPasedTime: mStopTime - mBaseTime
    if (mStopped){
        return (float) (((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
    }
    // 没有停止，经过的总时间
    else{
        return (float) (((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
    }
}

/**
 * @brief 返回当前帧和上一帧之间的时间差
 * 
 * @return float 
 */
float GameTimer::DeltaTime() const{
    return (float) mDeltaTime;
}

/**
 * @brief 重置(重新开始)
 * 
 */
void GameTimer::Reset(){
    __int64 currTime;
    // 统计了多少次
    QueryPerformanceCounter((LARGE_INTEGER*)& currTime);
    mBaseTime = currTime;
    mPrevTime = currTime;
    mStopTime = 0;
    mStopped = false;
}

/**
 * @brief 开始
 * 
 */
void GameTimer::Start(){
    __int64 startTime;
    QueryPerformanceCounter((LARGE_INTEGER*) &startTime);

    // 如果停止 -> 重新开始: 统计暂停时间
    if (mStopped){
        mPausedTime += (startTime - mStopTime);

        mPrevTime = startTime;
        mStopTime = 0;
        mStopped = false;
    }
}

/**
 * @brief 停止
 * 
 */
void GameTimer::Stop(){
    if (!mStopped){
        // 记录停止时间
        __int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*) &currTime);

        mStopTime = currTime;
        mStopped = true;
    }
}

/**
 * @brief 记录每一帧之间的时间差
 * 
 */
void GameTimer::Tick(){
    if (mStopped){
        mDeltaTime = 0.0;
        return;
    }
    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*) &currTime);
    mCurrTime = currTime;

    // Time difference between this frame and the previous.
    // 当前帧和上一帧之间的时间差
    mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;
	
    // Prepare for next frame.
    // 为下一帧做准备
    mPrevTime = mCurrTime;

    // Force nonnegative. The DXSDK CD UTTimer mentions that 
    // if the processor goes into a power save mode or we get shuffled to another processor, 
    // then mDeltaTime can be negative.
    // 强制非负数。 DXSDK 的 CDXUTTimer 提到，
    // 如果处理器进入省电模式或者我们被改组到另一个处理器，
    // 那么 mDeltaTime 可能是负数。
    if (mDeltaTime < 0.0){
        mDeltaTime = 0.0;
    }

}


