#ifndef FH_LIBS_MYSENSORS_MYTRANSPORT_H_
#define FH_LIBS_MYSENSORS_MYTRANSPORT_H_

#include "../../std/std_types.hpp"
#include "mymessage.hpp"

namespace fasthal::mysensors{
    struct mytransport{
        template <class TTransport>
        static void handle_ack(TTransport& transport, mymessage& msg, std::uint8_t sender){
            if (msg.request_ack())
                send_ack(transport, msg, sender);
        }

        template <class TTransport>
        static void send_ack(TTransport& transport, mymessage& msg, std::uint8_t myaddress){
            auto copy = msg;
            // Reply without ack flag (otherwise we would end up in an eternal loop)
            copy.request_ack(false);
            // it's ack
            copy.ack(true);
            // reverse destination
            copy.destination = msg.sender;
            // sender is us
            copy.sender = myaddress;
            transport.send(msg);
        }
    };
}

// this my sensors implementation is purely network oriented, but highly customizable
// based on chain-of-responsibility / piping principle
// basic "pipe" interface: bool begin(), bool update(msg) - receive message, bool send(msg) - to send 
// update may possibly process message internally and return true/false depending on processing

#endif