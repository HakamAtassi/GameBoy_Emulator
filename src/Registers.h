#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <cstdint>

namespace GameBoy{

	//register consists of 6 16 bit addresses
	//but in each(almost each) address is split down the middle
	//it can be addresses as a whole or by each Registers
	//		A | F
	//		B | C
	//		D | E
	//		H | L
	//		 SP
	//		 PC
	//A is the accumulator, F is flags. Remainder are mostly GP.

	//since F is a flag register, it consists of several variables
	//bit 7 is Zero flag,
	//bit 6 is sub flag (BCD)
	//bit 5 is half carry flag (BCD)
	//bit 4 is carry flag


	struct Registers{

		union
		{
			struct
			{
				union
				{
					struct{
						uint8_t U0:1;	//unused
						uint8_t U1:1;	//unused
						uint8_t U2:1;	//unused
						uint8_t U3:1;	//unused
						uint8_t carry:1;	//carry
						uint8_t halfCarry:1;	//half carry 
						uint8_t negative:1;	//subtraction flag
						uint8_t zero:1;	//zero flag
					};
					uint8_t	F;
				};
				uint8_t A;
			};
			uint16_t AF;
		};

		union
		{
			struct
			{
				uint8_t C, B;
			};
			uint16_t BC;
		};

		union
		{
			struct
			{
				uint8_t E, D;
			};
			uint16_t DE;
		};

		union
		{
			struct
			{
				uint8_t L, H;
			};
			uint16_t HL;
		};
	};
}


#endif




