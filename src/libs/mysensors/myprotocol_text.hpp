#ifndef FH_LIBS_MYSENSORS_MYPROTOCOL_TEXT_H_
#define FH_LIBS_MYSENSORS_MYPROTOCOL_TEXT_H_

#include "mymessage.hpp"

namespace fasthal::mysensors{
    class myprotocol_text{
        //static constexpr auto is_end = [](char c){return c == ';'; };
    public:
        template<class TStream>
        static void write(TStream& stream, const mymessage& msg){
            print(stream, msg.sender);
            print(stream, ';');
            print(stream, msg.sensor);
            print(stream, ';');
            print(stream, static_cast<std::uint8_t>(msg.command()));
            print(stream, ';');
            print(stream, msg.ack());
            print(stream, ';');
            print(stream, msg.type);
            print(stream, ';');
            msg.print_value_to(stream);
            print(stream, '\n');
        }

        template<class TStream>
        static bool read(TStream& stream, mymessage& msg){
            auto parser = istream_parser{&stream};
            msg.destination = parse_u8(parser.to(';'));
            msg.sensor = parse_u8(parser.to(';'));
            auto command = static_cast<my_command>(parse_u8(parser.to(';')));
            msg.request_ack(parse_u8(parser.to(';')));
            msg.type = parse_u8(parser.to(';'));
            // check if something wen't wrong
            if (!parser.end())
                return false;

            // we don't have data type in this protocol, so everything suxx...
            if (command == my_command::stream){
                // custom HEX
                msg.parse_custom_from(stream);
            } else {
                // plain old string...
                msg.parse_string_from(stream);
            }            
            
            msg.command(command);
            msg.ack(false);
            msg.sender = gateway_address;
            msg.last = gateway_address;

            return true;
        }
    };
}

#endif