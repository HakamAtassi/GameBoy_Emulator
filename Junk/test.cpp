#include <cstdint>
#include <iostream>

int main(){


	union
	{
		struct
		{
			struct{
				bool u0:1;
				bool u1:1;
				bool u2:1;
				bool u3:1;
				bool C:1;
				bool H:1;
				bool N:1;
				bool Z:1;
			}F;

			uint8_t A;


		} b;
		uint16_t FA;
	}FA;


	FA.FA=0;
	printf("\nFA start 0x%X",FA.FA);
	FA.FA=9;
	printf("\nFA 0x%X",FA.FA);

	FA.FA=0;
	FA.b.F.Z=1;
	printf("\nFA 0x%X",FA.FA);

}
