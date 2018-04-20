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
    void write(std::uint8_t c){
        // shortcut if empty buffer -> try write something sync
        if (uart_t::tx_ready() && _buf.empty()){
            uart_t::tx(c);
            return;
        }

        // wait for buffer space
        while (!_buf.try_write(c))
            uart_t::try_tx_sync();
        
        // start writing
        uart_t::tx_start();
    }
    void flush(){
        // write data buffer empty interrupt enabled (we have data in write buffer)
        // or transmission not complete (no data in write buffer, but it's not actually transmitted)            
        while (uart_t::tx_started() || read_(uart_t::txc))
            uart_t::try_tx_sync();
    }
    auto available() { return _buf.available(); }

    void operator()(){
        uart_t::tx(_buf.read_dirty());

        // not ok, disable async transmit
        if (_buf.empty())
            uart_t::tx_stop();
    }
};
namespace details{
    template<class TUart, unsigned VSize> 
    struct is_ostream_impl<uart_buf_tx<TUart, VSize>>: std::true_type{};
}
template<class TUart, unsigned VSize>
void write(uart_buf_tx<TUart, VSize>& uartw, std::uint8_t c){ uartw.write(c); }    