// sync tx ostream
template<class TUart>
struct uart_sync_tx{};
namespace details{
    template<class TUart> struct is_ostream_impl<uart_sync_tx<TUart>>: std::true_type{};
}
template<class TUart>
void write(uart_sync_tx<TUart> uartw, std::uint8_t c){ tx_sync(TUart{}, c); }    

// buffered tx ostream
template<class TUart, unsigned VSize>
class uart_buf_tx{
    ring_buffer<VSize> _buf;
    using uart_t = TUart;
public:
    uart_buf_tx(){ static_assert(details::has_isr<uart_t::irq_txr.number>, "IRQ not set! Use FH_UART_TX(*this*);"); }

    void write(std::uint8_t c){
        // shortcut if empty buffer -> try write something sync
        if (ready_(uart_t::irq_txr) && _buf.empty()){
            uart_t::tx(c);
            return;
        }

        // wait for buffer space
        while (!_buf.try_write(c))
            try_irq_force(uart_t::irq_txr);
        
        // start writing
        enable_(uart_t::irq_txr);
    }
    void flush(){
        // write data buffer empty interrupt enabled (we have data in write buffer)
        // or transmission not complete (no data in write buffer, but it's not actually transmitted)            
        while (ready_(uart_t::irq_txr) || read_(uart_t::txc))
            try_irq_force(uart_t::irq_txr);
    }
    auto available() { return _buf.available(); }

    void operator()(){
        uart_t::tx(_buf.read_dirty());

        // not ok, disable async transmit
        if (_buf.empty())
            disable_(uart_t::irq_txr);
    }
};
namespace details{
    template<class TUart, unsigned VSize> 
    struct is_ostream_impl<uart_buf_tx<TUart, VSize>>: std::true_type{};
}
template<class TUart, unsigned VSize>
void write(uart_buf_tx<TUart, VSize>& uartw, std::uint8_t c){ uartw.write(c); }



// buffered rx istream
template<class TUart, unsigned VSize>
class uart_buf_rx{
    ring_buffer<VSize> _buf;
    using uart_t = TUart;
public:
    uart_buf_rx(){ static_assert(details::has_isr<uart_t::irq_rxc.number>, "IRQ not set! Use FH_UART_RX(*this*);"); }

    std::uint8_t read(){
        // block till next byte
        while (_buf.empty());
        // read
        return _buf.read_dirty();
    }
    void write(std::uint8_t c){
        // shortcut if empty buffer -> try write something sync
        if (ready_(uart_t::irq_txr) && _buf.empty()){
            uart_t::tx(c);
            return;
        }

        // wait for buffer space
        while (!_buf.try_write(c))
            try_irq_force(uart_t::irq_txr);
        
        // start writing
        enable_(uart_t::irq_txr);
    }

    auto available() { return _buf.size(); }

    void operator()(){
        auto ok = uart_t::rx_ok();
        auto v = uart_t::rx();
        if (ok && !_buf.full())
            _buf.write_dirty(v);
    }
};
namespace details{
    template<class TUart, unsigned VSize> 
    struct is_istream_impl<uart_buf_rx<TUart, VSize>>: std::true_type{};
}
template<class TUart, unsigned VSize>
std::uint8_t read(uart_buf_rx<TUart, VSize>& uartr){ return uartr.read(); }