// UCSRA: RXC   TXC   UDRE  FE   DOR  UPE   U2X  MPCM
// UCSRB: RXCIE TXCIE UDRIE RXEN TXEN UCSZ2 RXB8 TXB8
// UCSRC: URSEL UMSEL UPM1  UPM0 USBS UCSZ1 UCS0 UCPOL

#define FH_DECLARE_UART_REGISTERS(NAME, CODE)\
    FH_DECLARE_REGISTER_ONLY(ubrr ## NAME, UBRR ## CODE)\
	FH_DECLARE_REGISTER_ONLY(ubrr ## NAME ## h, UBRR ## CODE ## H)\
	FH_DECLARE_REGISTER_ONLY(ubrr ## NAME ## l, UBRR ## CODE ## L)\
	FH_DECLARE_REGISTER_ONLY(ucsr ## NAME ## a, UCSR ## CODE ## A)\
	FH_DECLARE_REGISTER_ONLY(ucsr ## NAME ## b, UCSR ## CODE ## B)\
	FH_DECLARE_REGISTER_ONLY(ucsr ## NAME ## c, UCSR ## CODE ## C)\
	FH_DECLARE_REGISTER_ONLY(udr ## NAME, UDR ## CODE)\
	constexpr auto rxc ## NAME = fieldBit<RXC ## CODE>(ucsr ## NAME ## a);\
	constexpr auto txc ## NAME = fieldBit<TXC ## CODE>(ucsr ## NAME ## a);\
	constexpr auto udre ## NAME = fieldBit<UDRE ## CODE>(ucsr ## NAME ## a);\
	constexpr auto u2x ## NAME = fieldBit<U2X ## CODE>(ucsr ## NAME ## a);\
	constexpr auto upe ## NAME = fieldBit<UPE ## CODE>(ucsr ## NAME ## a);\
	constexpr auto rxen ## NAME = fieldBit<RXEN ## CODE>(ucsr ## NAME ## b);\
	constexpr auto txen ## NAME = fieldBit<TXEN ## CODE>(ucsr ## NAME ## b);\
	constexpr auto rxcie ## NAME = fieldBit<RXCIE ## CODE>(ucsr ## NAME ## b);\
	constexpr auto txcie ## NAME = fieldBit<TXCIE ## CODE>(ucsr ## NAME ## b);\
	constexpr auto udrie ## NAME = fieldBit<UDRIE ## CODE>(ucsr ## NAME ## b);\

#ifdef UBRR
FH_DECLARE_UART_REGISTERS(0,);
#define FH_HAS_UART0
#endif
#ifdef UBRR0
FH_DECLARE_UART_REGISTERS(0,0);
#define FH_HAS_UART0
#endif
#ifdef UBRR1
FH_DECLARE_UART_REGISTERS(1,1);
#define FH_HAS_UART1
#endif
#ifdef UBRR2
FH_DECLARE_UART_REGISTERS(2,2);
#define FH_HAS_UART2
#endif
#ifdef UBRR3
FH_DECLARE_UART_REGISTERS(3,3);
#define FH_HAS_UART3
#endif

#undef FH_DECLARE_UART_REGISTERS