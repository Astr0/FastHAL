#ifndef FH_LIBS_MYSENSORS_MYNODE_H_
#define FH_LIBS_MYSENSORS_MYNODE_H_

namespace fasthal::mysensors{
    template<typename TTransportPtr>
    class mynode
        : mp::holder<TTransportPtr>{
        auto& transport() const { return *(this->mp::holder<TTransportPtr>::get()); }        

    public:
        constexpr mynode(TTransportPtr transport):
            mp::holder<TTransportPtr>(transport) {}

        // init
        bool begin() const {
            // init network and gateway transport           
            return transport().begin();
            // TODO: present this node after init
        }

        // update
        bool update(mymessage& msg) const {
            // got message from gateway transport or net transport
            return transport().update(msg);
        }

        // send message
        bool send(mymessage& msg) const {
            // Send through normal transport
            return transport().send(msg);
        }
    };
}

#endif