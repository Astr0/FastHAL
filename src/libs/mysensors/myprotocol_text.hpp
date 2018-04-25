#ifndef FH_LIBS_MYSENSORS_MYPROTOCOL_TEXT_H_
#define FH_LIBS_MYSENSORS_MYPROTOCOL_TEXT_H_

#include "mymessage.hpp"

namespace fasthal::mysensors{
    struct myprotocol_text{
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
            msg.print_to(stream);            
            print(stream, '\n');

            
// snprintf_P(_fmtBuffer, MY_GATEWAY_MAX_SEND_LENGTH,
// 	           PSTR("%" PRIu8 ";%" PRIu8 ";%" PRIu8 ";%" PRIu8 ";%" PRIu8 ";%s\n"), message.sender,
// 	           message.sensor, (uint8_t)mGetCommand(message), (uint8_t)mGetAck(message), message.type,
// 	           message.getString(_convBuffer));
            
        }
    };
}

#endif