#ifndef CPU_H_
#define CPU_H_


#include "Registers.h"
#include <cstdint>

namespace GameBoy{
	class CPU{

		public:
			CPU();

		private:

			Registers registers;	//General purpose registers

			uint16_t PC;	//program counter
			uint16_t SP;	//stack pointer


			

	

	};
}





#endif 
