#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <cstdint>



namespace GameBoy{
	
			//register consists of 6 16 bit addresses
			//but in each(almost each) address is split down the middle
			//it can be addresses as a whole or by each resigers
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
				uint8_t A;
				struct{
					unsigned U0:1;	//unused
					unsigned U1:1;	//unused
					unsigned U2:1;	//unused
					unsigned U3:1;	//unused
					unsigned C:1;	//carry
					unsigned H:1;	//half carry 
					unsigned N:1;	//subtraction flag
					unsigned Z:1;	//zero flag
				}F;
			} Byte;
			uint16_t Word;
		}FA;

		union
		{
			struct
			{
				uint8_t C, B;
			} Byte;
			uint16_t Word;
		}BC;

		union
		{
			struct
			{
				uint8_t E, D;
			} Byte;
			uint16_t Word;
		}DE;

		union
		{
			struct
			{
				uint8_t L, H;
			} Byte;
			uint16_t Word;
		}HL;



	};
}


#endif




