// Timer template
// Replace @ to timer num

#ifndef AVR_TIMER@_H_
#define AVR_TIMER@_H_

#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{	
	// Clock Source
		None    = (0 << CS@2) | (0 << CS@1) | (0 << CS@0),
	};
	#else
		None    = (0 << CS@2) | (0 << CS@1) | (0 << CS@0),
	};
	#endif
	const uint8_t Timer@WgmMask0 = (1 << WGM@1) | (1 << WGM@0);
	const uint8_t Timer@WgmMask1 = (1 << WGM@3) | (1 << WGM@2);
		/*Reserved = (1 << WGM@2) | (0 << WGM@1) | (0 << WGM@0),*/
		PwmPcA     = (1 << WGM@2) | (0 << WGM@1) | (1 << WGM@0),
		PwmFastA   = (1 << WGM@2) | (1 << WGM@1) | (1 << WGM@0)
	};
	const uint8_t Timer@WgmMask0 = (1 << WGM@1) | (1 << WGM@0);
	const uint8_t Timer@WgmMask1 = (1 << WGM@2);
	};
	const uint8_t Timer@WgmMask = (1 << WGM@1) | (1 << WGM@0);
		
	{
	};
}


#endif // defined TCCR

#undef FH_TIMER@

#endif /* TIMER0_H_ */