#pragma once

#ifndef UTILS_RINGBUFFER_H_
#define UTILS_RINGBUFFER_H_

#include "../mp/brigand_ex.hpp"
#include <inttypes.h>

namespace fasthal{
    template<unsigned BufferSize>
    class RingBuffer{
    public:
        using BufferIndex = brigand::number_type<BufferSize>;
    private:
        using BufferIndex2 = brigand::number_type<BufferSize * 2>;
        using BufferIndex1 = brigand::number_type<BufferSize + 1>;

        volatile BufferIndex _head;
        volatile BufferIndex _tail;
        unsigned char _buffer[BufferSize];
    public:       
        RingBuffer(): _head(0), _tail(0){

        }

        inline void clear(){
            _head = _tail;
        }

        BufferIndex available(){
            // doesn't makes much sense to NoInterrupt here since after no interrupt things may change
            // tail is read second so it will return <= real available
            return ((BufferIndex2)(BufferSize + _head - _tail)) % BufferSize;
        }

        bool empty(){
            return _head == _tail;
        }

        uint8_t peek(){
            return _buffer[_tail];
        }

        uint8_t readDirty(){
            uint8_t c = peek();
            _tail = ((BufferIndex1)(_tail + 1)) % BufferSize;
            return c;
        }

        uint8_t read(){
            return empty() ? 0 : readDirty();
        }

        BufferIndex nextIndex(){
            return ((BufferIndex1)(_head + 1)) % BufferSize;
        }

        bool canWriteNext(BufferIndex i){
            return i != _tail;
        }

        void writeNext(BufferIndex i, uint8_t c){
            _buffer[_head] = c;
            _head = i;
        }

        bool tryWrite(uint8_t c){
            BufferIndex i = nextIndex();
            
            // if we should be storing the received character into the location
            // just before the tail (meaning that the head would advance to the
            // current location of the tail), we're about to overflow the buffer
            // and so we don't write the character or advance the head.
            if (canWriteNext(i)) {
                writeNext(i, c);
                return true;
            }
            return false;
        }
    };

    // not valid template arguments
    template<>
    class RingBuffer<0>{};

    template<>
    class RingBuffer<1>{};
}

#endif
