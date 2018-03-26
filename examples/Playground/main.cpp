/*
 * Playground.cpp
 *
 * Created: 10/23/2017 3:48:05 PM
 * Author : astr0
 */ 

#include <avr/io.h>


class Port{
	private:
	//volatile uint8_t _value;
	
	public:	
	inline void set(uint8_t mask) const { PORTB |= mask;}
};


template<uint8_t num>
class Pin{
private:
	const Port _port;
public:
	constexpr Pin(const Port port): _port(port){
		
	}

	void set() const{
		_port.set(1 << num);
	}		
};

constexpr auto p = Port{};
constexpr auto p1 = Pin<1>{p};
auto p2 = Pin<2>{p};

//Port& p = *(Port*)(&PORTB);
//Pin p1 = {p, 1};
	
template<class Pin1, class Pin2>
void setPins(Pin1 pin1, Pin2 pin2){
	pin1.set();
	pin2.set();
}

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		setPins(p1, p2);
		//p.set(2);
    }
}

