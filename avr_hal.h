#pragma once


#ifndef AVR_HAL_H_
#define AVR_HAL_H_

#define FASTHAL_WRAPVARIABLE(CLASSNAME, VARNAME)\
struct CLASSNAME\
{\
static decltype(VARNAME)& value(){return VARNAME;}\
};

#define FASTHAL_DECLAREPORT(CODE)\
namespace priv\
{\
	FASTHAL_WRAPVARIABLE(PortReg ## CODE, PORT ## CODE)\
	FASTHAL_WRAPVARIABLE(PinReg ## CODE, PIN ## CODE)\
	FASTHAL_WRAPVARIABLE(DdrReg ## CODE, DDR ## CODE)\
}\
typedef AVRPort<priv::DdrReg ## CODE, priv::PortReg ## CODE, priv::PinReg ## CODE> Port ## CODE;\
typedef Pin<Port ## CODE, 0> Pin ## CODE ## 0;\
typedef Pin<Port ## CODE, 1> Pin ## CODE ## 1;\
typedef Pin<Port ## CODE, 2> Pin ## CODE ## 2;\
typedef Pin<Port ## CODE, 3> Pin ## CODE ## 3;\
typedef Pin<Port ## CODE, 4> Pin ## CODE ## 4;\
typedef Pin<Port ## CODE, 5> Pin ## CODE ## 5;\
typedef Pin<Port ## CODE, 6> Pin ## CODE ## 6;\
typedef Pin<Port ## CODE, 7> Pin ## CODE ## 7;\
typedef InvertedPin<Pin ## CODE ## 0> Pin ## CODE ## 0Inv;\
typedef InvertedPin<Pin ## CODE ## 1> Pin ## CODE ## 1Inv;\
typedef InvertedPin<Pin ## CODE ## 2> Pin ## CODE ## 2Inv;\
typedef InvertedPin<Pin ## CODE ## 3> Pin ## CODE ## 3Inv;\
typedef InvertedPin<Pin ## CODE ## 4> Pin ## CODE ## 4Inv;\
typedef InvertedPin<Pin ## CODE ## 5> Pin ## CODE ## 5Inv;\
typedef InvertedPin<Pin ## CODE ## 6> Pin ## CODE ## 6Inv;\
typedef InvertedPin<Pin ## CODE ## 7> Pin ## CODE ## 7Inv;\


namespace fasthal{
    class NoInterrupts{
        uint8_t _sreg;
    public:
        NoInterrupts()
        {
            _sreg = SREG;
            cli();
        }

        ~NoInterrupts(){
            SREG = _sreg;	
        }
    };

    template<class DDRREG, class PORTREG, class PINREG>
	class AVRPort
	{
		public:
		constexpr AVRPort(){}
		
		static constexpr uint8_t width(){return 8;}			
	
		inline static void write(uint8_t value) {PORTREG::value() = value;}
		inline static void clearAndSet(uint8_t clearMask, uint8_t setMask) {PORTREG::value() = PORTREG::value() & ~clearMask | setMask;}
		inline static void set(uint8_t mask) {PORTREG::value() |= mask;}
		inline static void clear(uint8_t mask) {PORTREG::value() &= ~mask;}
		inline static void toggle(uint8_t mask) {PORTREG::value() ^= mask;}
		inline static uint8_t read() {return PINREG::value();}
		inline static bool read(uint8_t mask){return PINREG::value() & mask;}

		static void setMode(uint8_t mask, uint8_t mode)
		{
			NoInterrupts noInterrupts;
			switch (mode)
			{
				case INPUT:
					DDRREG::value() &= ~mask;
					PORTREG::value() &= ~mask;
					break;
				case INPUT_PULLUP:
					DDRREG::value() &= ~mask;
					PORTREG::value() |= mask;
					break;
				default:
					DDRREG::value() |= mask;
			}
		}
			
		template<unsigned VNumber>
		struct Pin{
			typedef typename fasthal::Pin<AVRPort<DDRREG, PORTREG, PINREG>, VNumber> Type;
		};
    };	
    
    #ifdef PORTA
	FASTHAL_DECLAREPORT(A)
	#endif
	#ifdef PORTB
	FASTHAL_DECLAREPORT(B)
	#endif
	#ifdef PORTC
	FASTHAL_DECLAREPORT(C)
	#endif
	#ifdef PORTD
	FASTHAL_DECLAREPORT(D)
	#endif
	#ifdef PORTE
	FASTHAL_DECLAREPORT(E)
	#endif
	#ifdef PORTF
	FASTHAL_DECLAREPORT(F)
	#endif
	#ifdef PORTG
	FASTHAL_DECLAREPORT(G)
	#endif
	#ifdef PORTH
	FASTHAL_DECLAREPORT(H)
	#endif
}

#undef FASTHAL_DECLAREPORT
#undef FASTHAL_WRAPVARIABLE

#endif /* AVR_HAL_H_ */