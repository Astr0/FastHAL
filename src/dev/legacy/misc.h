#pragma once

#ifndef DEV_MISC_H_
#define DEV_MISC_H_

#include <inttypes.h>

namespace fasthal{
    //template<class TFieldBit>
    //class PinToggle{
    //private:
        //uint16_t _on;
        //uint16_t _off;
        //ElapsedMs _elapsed;
    //public:
        //PinToggle():_on(0), _off(-1){            
        //}
//
        //PinToggle(uint16_t on, uint16_t off):_on(on), _off(off){            
        //}
//
        //void begin(bool on){
            //TFieldBit::set(on);
            //TFieldBit::setMode(PinMode::Output);
        //}
//
        //void reset(){
            //_elapsed.reset();
            //TFieldBit::clear();
        //}
//
        //void change(uint16_t on, uint16_t off){
            //_on = on;
            //_off = off;
        //}
//
        //void update(){
            //auto time = TFieldBit::read() ? _on : _off;
            //if (_elapsed.elapsed(time)){
                //_elapsed.reset();
                //TFieldBit::toggle();
            //}
        //}
    //};
}

#endif