#ifndef FH_HAL_NET_H_
#define FH_HAL_NET_H_

namespace fasthal{
    template<typename TBuf = buffer_t>
    class net_args {
        using args_t = net_args<TBuf>;

        //using callback_t = void(*)(args_t&); 
        using callback_t = mp::static_func<void(args_t&)>; 
        // size of buffer
        bsize_t _count;
        // operation status
        std::uint8_t _status;
        // buffer
        TBuf _buf;
        callback_t _callback;
    public: 
        constexpr net_args(){}

        bsize_t count()const {return _count;}
        void count(bsize_t c) { _count = c; }
        
        template<typename... T>
        bool status_any(T... s)const { return is_any(_status, static_cast<std::uint8_t>(s)...); }
        template<typename T>
        void status(T s) { _status = static_cast<std::uint8_t>(s); } 
        
        void buffer(TBuf buf) { _buf = buf; }
        TBuf& buffer() { return _buf; }
        std::uint8_t& operator[](bsize_t i) { return _buf[i]; }
        std::uint8_t operator[](bsize_t i)const { return _buf[i]; }

        void operator()() { _callback(*this); }
        
        args_t& callback(callback_t cb){ _callback = cb; return *this; } 
    };
}

#endif