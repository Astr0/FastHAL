#ifndef FH_UTILS_RINGBUFFER_H_
#define FH_UTILS_RINGBUFFER_H_

#include "../mp/brigand_ex.hpp"
#include "../std/std_types.hpp"

// TODO: Fix ringbuffer so 1 bytes is ok (tail and head moves out of range and % later)
namespace fasthal{
    template<unsigned VBufferSize>
    class ring_buffer{
    public:        
        using index_t = brigand::number_type<VBufferSize>;
        using data_t = std::uint8_t;
        static constexpr index_t size = VBufferSize;
    private:
        using index2_t = brigand::number_type<size * 2>;
        using index1_t = brigand::number_type<size + 1>;
        

        volatile index_t _head;
        volatile index_t _tail;
        data_t _buffer[size];
    public:       
        ring_buffer(): _head(0), _tail(0){ }

        inline void clear(){
            _head = _tail;
        }

        index_t available(){
            // doesn't makes much sense to NoInterrupt here since after no interrupt things may change
            // tail is read second so it will return <= real available
            return ((index2_t)(size + _head - _tail)) % size;
        }

        bool empty(){
            return _head == _tail;
        }

        data_t peek(){
            return _buffer[_tail];
        }

        data_t read_dirty(){
            uint8_t c = peek();
            _tail = ((index1_t)(_tail + 1)) % size;
            return c;
        }

        data_t read(){
            return empty() ? 0 : read_dirty();
        }

        index_t next_i(){
            return ((index1_t)(_head + 1)) % size;
        }

        bool can_write_i(index_t i){
            return i != _tail;
        }

        void write_i(index_t i, data_t c){
            _buffer[_head] = c;
            _head = i;
        }

        bool try_write_i(index_t i, data_t c){
            // if we should be storing the received character into the location
            // just before the tail (meaning that the head would advance to the
            // current location of the tail), we're about to overflow the buffer
            // and so we don't write the character or advance the head.
            if (can_write_i(i))                
            {
                write_i(i, c);
                return true;
            }
            return false;            
        }

        bool try_write(data_t c){
            return try_write_i(next_i(), c);
        }
    };

    // not valid template arguments
    template<>
    struct ring_buffer<0>{
        static constexpr std::uint8_t size = 0;
    };

    template<>
    struct ring_buffer<1>: ring_buffer<0>{};
}

#endif
