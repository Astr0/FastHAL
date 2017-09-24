#pragma once

#ifndef TIME_H_
#define TIME_H_

#include <inttypes.h>
#include "../platforms/time.h"
#include "../sys/maskutils.h"

namespace fasthal{
	
	// elapsed checker suitable for Bounce. elapsed() when called Checks count.
	template <unsigned Checks>
	class ElapsedChecks{
		typedef typename common::NumberType<Checks + 1>::Result checks_t;
		
		checks_t _checks;
	public:
		ElapsedChecks(){ reset(); }
		void reset(){ _checks = 0; }
		bool elapsed(){
			if (_checks == Checks)
				return true;
			_checks++;
			return false;
		}		
	};
	
    //class TimeUtils{
    //public:
        //static inline constexpr Time::micros_t freqToMicros(uint8_t freq)
        //{
            //return 1000000 / freq;
        //}
    //};
//
    //template<typename T, T (*GetTime)()>
    //class Elapsed{
    //private:
        //T _lastTime;
    //public:
        //Elapsed(): _lastTime(GetTime()){            
        //}
//
        //void reset(){
            //_lastTime = GetTime();
        //}
//
        //T elapsed(){
            //return GetTime() - _lastTime;
        //}
//
        //bool elapsed(T time){
            //return elapsed() >= time;
        //}
    //};
//
    //typedef Elapsed<Time::millis_t, Time::millis> ElapsedMs;
}

#endif