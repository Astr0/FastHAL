#ifndef FH_UTILS_RINGBUFFER_H_
#define FH_UTILS_RINGBUFFER_H_

#include "../mp/brigand_ex.hpp"
#include "../std/std_types.hpp"

namespace fasthal{
    template<unsigned VCapacity>
    struct ring_buffer{
        static_assert((VCapacity & (VCapacity - 1)) == 0, "Capacity should be a power of 2");
        using index_t = brigand::number_type<2 * VCapacity>;
        using data_t = std::uint8_t;
        static constexpr index_t capacity = VCapacity;
    private:
        //using index2_t = brigand::number_type<size * 2>;
        //using index1_t = brigand::number_type<size + 1>;        

        volatile index_t _write;
        volatile index_t _read;
        data_t _buffer[capacity];

        static constexpr index_t mask(index_t index){ return index & (capacity - 1);}
    public:       
        // clear it manually if needed
        //ring_buffer(): _write(0), _read(0){ }

        inline void clear(){ _write = _read = 0; }

        // doesn't makes much sense to NoInterrupt here since after no interrupt things may change
        // tail is read second so it will return <= real available
        inline index_t size(){ return _write - _read; }
        inline bool empty(){ return _write == _read; }
        inline bool full(){return size() == capacity;}
 
        inline data_t peek(){ return _buffer[mask(_read)]; }
        inline data_t read_dirty(){ return _buffer[mask(_read++)]; }
        inline data_t read() 
        { 
            return empty() ? 0 : read_dirty();; 
        }

        bool try_write(data_t c){            
            if (full()) return false;
            _buffer[mask(_write++)] = c;
            return true;
        }
        
        bsize_t write(const std::uint8_t* data, bsize_t size){
            bsize_t i = 0;
            for (; i < size && try_write(data[i]); ++i){
                // empty
            }
            return i;

            // //index_t w = _write;
            // auto canWrite = capacity - size();
            // if (sz > canWrite)
            //     sz = canWrite;
            // else
            //     canWrite = sz;
            // while(sz--)
            //     _buffer[mask(_write++)] = *data++;
            // //_write = w;
            // return canWrite;
        }
    };

    // not valid template arguments
    template<>
    struct ring_buffer<0>{
        static constexpr std::uint8_t capacity = 0;
    };

    // template<>
    // struct ring_buffer<1>{
    //     using index_t = std::uint8_t;
    //     using data_t = std::uint8_t;
    //     static constexpr std::uint8_t capacity = 1;
    // private:
    //     volatile data_t _data;
    //     volatile bool _hasData;
    // public:
    //     ring_buffer(): _hasData(false) { }

    //     inline void clear(){ _hasData = false; }
    //     index_t size(){ return _hasData ? 1 : 0; }
    //     bool empty(){ return !_hasData; }
    //     bool full(){ return _hasData; }

    //     data_t peek(){ return _data; }

    //     data_t read_dirty() { 
    //         auto d = _data;
    //         _hasData = false;
    //         return d; 
    //     }

    //     data_t read(){ return _hasData ? read_dirty() : 0; }

    //     bsize_t write(std::uint8_t* c, bsize_t size){
    //         if (!size) return 0;
    //         return try_write(*c) ? 1 : 0;
    //     }

    //     bool try_write(data_t c){
    //         if (_hasData)
    //             return false;
    //         _data = c;
    //         _hasData = true;
    //         return true;
    //     }
    // };
}

#endif
