#ifndef FH_LIBS_MYSENSORS_GTRANSPORT_UART_H_
#define FH_LIBS_MYSENSORS_GTRANSPORT_UART_H_

#include "../../mp/holder.hpp"
#include "mymessage.hpp"
#include "myprotocol_text.hpp"

namespace fasthal::mysensors{
    struct gtransport_streams_default_config{
        static constexpr std::uint8_t max_receive_length = 100; 
    };

    template <typename TOutputPtr, typename TInputPtr, typename TConfig = gtransport_streams_default_config>
    class gtransport_streams
        : mp::holder<TOutputPtr>
        , mp::holder<TInputPtr>{
        auto& output() const{ return *(this->mp::holder<TOutputPtr>::get()); }
        auto& input() const{ return *(this->mp::holder<TInputPtr>::get()); }
        using protocol_t = myprotocol_text;
        using config_t = TConfig;
        bsize_t _input_index;
        std::uint8_t _input[config_t::max_receive_length];
    public:
        gtransport_streams(TOutputPtr output, TInputPtr input, TConfig config = gtransport_streams_default_config{})
            : mp::holder<TOutputPtr>(output)
            , mp::holder<TInputPtr>(input){}

        bool send(mymessage& msg) const{
            protocol_t::write(output(), msg);
            return true;
        }

        bool update(mymessage& msg){
            while (input().available()) {
                // get the new byte:
                const char c = read_char(input());
                // if the incoming character is a newline, set a flag
                // so the main loop can do something aboutput it:
                if (_input_index < config_t::max_receive_length - 1) {
                    if (c == '\n') {
                        //_input[_input_index] = 0;
                        // TODO
                        auto stream = buffer_istream{_input, _input_index};
                        _input_index = 0;
                        return protocol_t::read(stream, msg);
                    } else {
                        // add it to the _input:
                        _input[_input_index] = c;
                        _input_index++;
                    }
                } else {
                    // Incoming message too long. Throw away
                    _input_index = 0;
                }
            }
            return false;
        }

        bool begin() const{
            auto msg = mymessage{};
            send(msg.build_gw(my_internal::gateway_ready).set("Gateway startup complete."));
            // Send presentation of locally attached sensors (and node if applicable)
	        // This will be done on gateway level after transport is ok
            // presentNode();
            return true;
        }
    };
}

#endif