#ifndef FH_ISTREAM_PARSER_H_
#define FH_ISTREAM_PARSER_H_

#include "../mp/holder.hpp"
#include "../std/type_traits.hpp"
#include "stream.hpp"

namespace fasthal{
    // limits stream to sequence
    template<typename TStreamPtr>
    class istream_parser: mp::holder<TStreamPtr>{
        auto& stream(){return *(this->mp::holder<TStreamPtr>::get()); }
        bool _end;
        std::uint8_t _to;
    public:
        istream_parser(TStreamPtr stream): mp::holder<TStreamPtr>(stream), _end(true) {}

        auto& to(std::uint8_t b){
            _to = b;
            _end = false;
            return *this;
        }

        bool end() const {return _end;}

        std::uint8_t read(){
            if (_end) return 0;
            auto c = ::fasthal::read(stream());
            if (c == _to) { 
                _end = true; 
                return 0; 
            }
            return c;
        }
    };

    namespace details{
        template<typename TStreamPtr>
        struct is_istream_impl<istream_parser<TStreamPtr>>: std::true_type{};
    }

    template<typename TStreamPtr>
    std::uint8_t read(istream_parser<TStreamPtr>& stream){
        return stream.read();
    }
}

#endif