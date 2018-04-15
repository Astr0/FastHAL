#ifndef FH_UTILS_LIST_H_
#define FH_UTILS_LIST_H_

#include "../mp/brigand_ex.hpp"

namespace fasthal{
    template<typename T, unsigned VCapacity>
    class list{
    public:
        static constexpr auto capacity = VCapacity;
        using index_t = brigand::number_type<VCapacity>;
    private:
        index_t _size;
        T _list[capacity];
    public:
        list(): _size(0) {}

        void clear() { _size = 0; }
        bool full() { return _size == capacity; }
        bool empty() { return _size == 0; }
        index_t size() { return _size; }
        void add(T item) { _list[_size++] = item; }
        T& at(index_t index) { return _list[index]; }
        void remove_at(index_t index) {
            _size--;
            // swap if it wasn't last
            if (index < _size)
                _list[index] = _list[_size];        
        }
    };
};

#endif