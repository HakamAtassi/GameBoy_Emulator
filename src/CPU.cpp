#include "CPU.h"
#include <cstdint>

using namespace GameBoy;

CPU::CPU(){
	RAM ram;
}

int CPU::nop(){	//0x00
	cycles=1;
	return 0;
}

int CPU::ld_bc_d16(){	//0x01
	cycles=3; 
	uint16_t imm=ram.read(PC);
	PC++;
	imm|=ram.read(PC)<<8;
	PC++;
	regs.BC.Word=imm;

	return 0;
}

int CPU::ld_bc_a(){	//0x02
	cycles=2;
	ram.write(regs.BC.Word,regs.FA.Byte.A);
	return 0;
}

int CPU::inc_bc(){	//0x03
	cycles=2;
	regs.BC.Word++;
	return 0;
}

int CPU::inc_b(){	//0x04
	cycles=1;
	//carry flag unchanged
	regs.FA.Byte.F.H=((regs.BC.Byte.B&0x0F)==0x0F);
	regs.FA.Byte.F.N=0;
	regs.FA.Byte.F.Z=(regs.BC.Byte.B==0xFF);
	regs.BC.Byte.B++;

}

int CPU::dec_b(){	//0x05
	//carry flag unchanged
	regs.FA.Byte.F.H=((regs.BC.Byte.B&0x0F)==0x00);
	regs.FA.Byte.F.N=1;
	regs.FA.Byte.F.Z=(regs.BC.Byte.B==0x01);
	regs.BC.Byte.B--;
	return 0;
}

int CPU::ld_b_d8(){	//0x06
	cycles=2;
	uint8_t imm=ram.read(PC);
	PC++;
	regs.BC.Byte.B=imm;
	return 0;
}


int CPU::rlca(){	//0x07
	cycles=1;

	regs.FA.Byte.F.Z=0;
	regs.FA.Byte.F.N=0;
	regs.FA.Byte.F.H=0;

	regs.FA.Byte.F.C=(regs.FA.Byte.A&0x80)>>8;
	regs.FA.Byte.A=regs.FA.Byte.A<<1;
	regs.FA.Byte.A|=regs.FA.Byte.F.C;	//to rotate, use the same value that was stored in the carry bit
	return 0;
}

int CPU::ld_a16_sp(){	//0x08
	int cycles=5;

	uint16_t a16=ram.read(PC);
	PC++;
	a16|=ram.read(PC)<<8;
	PC++;

	ram.write(a16,SP&0x00FF);
	ram.write(a16+1,(SP&0xFF00)>>8);
	return 0;
}

int CPU::add_hl_bc(){	//0x09

}

int CPU::ld_a_bc(){	//0x0A
	cycles=2;
	uint16_t operand=ram.read(regs.BC.Word);
	regs.FA.Byte.A=operand;
	return 0;
}

int CPU::dec_bc(){	//0x0B
	cycles=2;
	regs.BC.Word--;
	PC++;
	return 0;
}

int CPU::inc_c(){	//0x0C

}

int CPU::dec_c(){	//0x0D

}

int CPU::ld_c_d8(){	//0x0E
	cycles=2;
	uint8_t imm=ram.read(PC);
	regs.BC.Byte.C=imm;
	PC++;
	return 0;
}

int CPU::rrca(){	//0x0F

}

int CPU::stop(){	//0x10


}

int CPU::ld_de_d16(){	//0x11
	cycles=3;

	uint16_t imm=ram.read(PC);
	PC++;
	imm|=ram.read(PC)<<8;
	PC++;
	regs.DE.Word=imm;

	return 0;
}

int CPU::ld_de_a(){	//0x12
	cycles=2;
	ram.write(regs.DE.Word,regs.FA.Byte.A);
	return 0;
}

int CPU::inc_de(){	//0x13
	cycles=2;
	regs.DE.Word++;
	return 0;

}

int CPU::inc_d(){	//0x14
	cycles=1;
	regs.FA.Byte.F.H=((regs.DE.Byte.D&0x0F)==0x0F);
	regs.FA.Byte.F.N=0;
	regs.FA.Byte.F.Z=(regs.DE.Byte.D==0xFF);
	regs.BC.Byte.B++;
	return 0;
}

int CPU::dec_d(){	//0x15
	//carry flag unchanged
	regs.FA.Byte.F.H=((regs.DE.Byte.D&0x0F)==0x00);
	regs.FA.Byte.F.N=1;
	regs.FA.Byte.F.Z=(regs.DE.Byte.D==0x01);
	regs.DE.Byte.D--;
	return 0;
}

int CPU::ld_d_d8(){	//0x16
	cycles=2;
	uint8_t imm=ram.read(PC);
	PC++;
	regs.DE.Byte.D=imm;
	return 0;
}

int CPU::rla(){	//0x17
	//possible error
	regs.FA.Byte.F.Z=0;
	regs.FA.Byte.F.N=0;
	regs.FA.Byte.F.H=0;

	int MSB=(regs.FA.Byte.A&0x80)>>8;
	int carry=regs.FA.Byte.F.C;
	regs.FA.Byte.F.C=MSB;
	regs.FA.Byte.A=regs.FA.Byte.A<<1;
	regs.FA.Byte.A|=carry;
	return 0;
}

int CPU::jr_s8(){	//0x18
					//possible error here (signed or unsigned number?)
	cycles=3;
	int8_t s8=ram.read(PC);
	PC++;
	PC=PC+s8;
	return 0;
}

int CPU::add_hl_de(){	//0x19

}

int CPU::ld_a_de(){	//0x1A
	cycles=2;
	uint16_t operand=ram.read(regs.DE.Word);
	regs.FA.Byte.A=operand;
	return 0;
}


int CPU::dec_de(){	//0x1B
	cycles=2;
	regs.DE.Word--;
	PC++;
	return 0;
}


int CPU::inc_e(){	//0x1C

}

int CPU::dec_e(){	//0x1D

}

int CPU::ld_e_d8(){	//0x1E
	cycles=2;
	uint8_t imm=ram.read(PC);
	regs.DE.Byte.E=imm;
	PC++;
	return 0;
}

int CPU::rra(){	//0x1F

}

int CPU::jr_nz_s8(){	//0x20

	if(regs.FA.Byte.F.Z==0){
		cycles=3;
		int8_t s8=ram.read(PC);
		PC++;
		PC=PC+s8;
	}
	else{
		cycles=2;
		PC++;
	}
	return 0;
}

int CPU::ld_hl_d16(){	//0x21
	cycles=3;
	uint16_t imm=ram.read(PC);
	PC++;
	imm|=ram.read(PC)<<8;
	PC++;
	regs.HL.Word=imm;

	return 0;
}

int CPU::ld_hlp_a(){	//0x22
	cycles=2;
	ram.write(regs.HL.Word,regs.FA.Byte.A);
	regs.HL.Word++;
	return 0;

}

int CPU::inc_hl(){	//0x23
	cycles=2;
	regs.HL.Word++;
	return 0;
}

int CPU::inc_h(){	//0x24
	cycles=1;
	regs.FA.Byte.F.H=((regs.HL.Byte.H&0x0F)==0x0F);
	regs.FA.Byte.F.N=0;
	regs.FA.Byte.F.Z=(regs.HL.Byte.H==0xFF);
	regs.BC.Byte.B++;
}

int CPU::dec_h(){	//0x25
	//carry flag unchanged
	cycles=1;
	regs.FA.Byte.F.H=((regs.HL.Byte.H&0x0F)==0x00);
	regs.FA.Byte.F.N=1;
	regs.FA.Byte.F.Z=(regs.HL.Byte.H==0x01);
	regs.HL.Byte.H--;
	return 0;
}

int CPU::ld_h_d8(){	//0x26
	cycles=2;
	uint8_t imm=ram.read(PC);
	PC++;
	regs.HL.Byte.H=imm;
	return 0;
}

int CPU::daa(){	//0x27



}

int CPU::jr_z_s8(){	//0x28

	if(regs.FA.Byte.F.Z==1){
		cycles=3;
		int8_t s8=ram.read(PC);
		PC++;
		PC=PC+s8;
	}
	else{
		cycles=2;
		PC++;
	}
	return 0;
}

int CPU::add_hl_hl(){
	
}


int CPU::ld_a_hlp(){	//0x2A
	cycles=2;
	regs.FA.Byte.A=ram.read(regs.HL.Word);
	regs.HL.Word++;
	return 0;

}

int CPU::dec_hl(){	//0x2B
	cycles=2;
	regs.HL.Word--;
	PC++;
	return 0;
}

int CPU::inc_l(){	//0x2C
	

}


int CPU::dec_l(){

}

int CPU::ld_l_d8(){	//0x2E
	cycles=2;
	uint8_t imm=ram.read(PC);
	regs.HL.Byte.L=imm;
	PC++;
	return 0;
}

int CPU::cpl(){	//0x2F

}

int CPU::jr_nc_s8(){	//0x30
	
	if(regs.FA.Byte.F.C==0){
		cycles=3;
		int8_t s8=ram.read(PC);
		PC++;
		PC=PC+s8;
	}
	else{
		cycles=2;
		PC++;
	}
	return 0;
}

int CPU::ld_sp_d16(){	//0x31
	cycles=3;
	uint16_t imm=ram.read(PC);
	PC++;
	imm|=ram.read(PC)<<8;
	PC++;
	SP=imm;

	return 0;
}

int CPU::ld_hlm_a(){	//0x32

	cycles=2;
	ram.write(regs.HL.Word, regs.FA.Byte.A);
	regs.HL.Word--;
	return 0;
}

int CPU::inc_sp(){	//0x33
	cycles=2;
	SP++;
	return 0;
}

int CPU::inc_hl_mem(){	//0x34
	cycles=1;
	uint8_t contents=ram.read(regs.HL.Word);
	regs.FA.Byte.F.H=((contents&0x0F)==0x0F);
	regs.FA.Byte.F.N=0;
	regs.FA.Byte.F.Z=(contents==0xFF);
	ram.write(regs.HL.Word,contents++);
	return 0;
}

int CPU::dec_hl_mem(){	//0x35
	uint8_t contents=ram.read(regs.HL.Word);
	regs.FA.Byte.F.H=((contents&0x0F)==0x00);
	regs.FA.Byte.F.N=1;
	regs.FA.Byte.F.Z=(contents==0x01);
	ram.write(regs.HL.Word,contents--);
	return 0;
}

int CPU::ld_hl_d8(){	//0x36
	cycles=3;
	uint8_t imm=ram.read(PC);
	PC++;
	ram.write(regs.HL.Word,imm);
	return 0;
}

int CPU::scf(){	//0x37
	cycles=1;

	regs.FA.Byte.F.C=1;
	regs.FA.Byte.F.H=0;
	regs.FA.Byte.F.N=0;
	regs.FA.Byte.F.Z=0;
	return 0;
}

int CPU::jr_c_s8(){ //0x38

	if(regs.FA.Byte.F.C==1){
		cycles=3;
		int8_t s8=ram.read(PC);
		PC++;
		PC=PC+s8;
	}
	else{
		cycles=2;
		PC++;
	}
	return 0;
}

int CPU::add_hl_sp(){	//0x39

}

int CPU::ld_a_hlm(){	//0x3A

	cycles=2;
	regs.FA.Byte.A=ram.read(regs.HL.Word);
	regs.HL.Word--;
	return 0;
}

int CPU::dec_sp(){	//0x3B
	cycles=2;
	SP--;
	PC++;
	return 0;
}

int CPU::inc_a(){	//0x3C
	
}

int CPU::dec_a(){	//0x3D

}

int CPU::ld_a_d8(){	//0x3E
	cycles=2;
	uint8_t imm=ram.read(PC);
	PC++;
	regs.FA.Byte.A=imm;
	return 0;

}

int CPU::ccf(){	//0x3F

	return 0;
}


int CPU::ld_b_b(){	//0x40
	cycles=1;
	regs.BC.Byte.B=regs.BC.Byte.B;
	return 0;
}
int CPU::ld_b_c(){	//0x41
	cycles=1;
	regs.BC.Byte.B=regs.BC.Byte.C;
	return 0;
}
int CPU::ld_b_d(){	//0x42
	cycles=1;
	regs.BC.Byte.B=regs.DE.Byte.D;
	return 0;
}	
int CPU::ld_b_e(){	//0x43
	cycles=1;
	regs.BC.Byte.B=regs.DE.Byte.E;
	return 0;
}
int CPU::ld_b_h(){	//0x44
	cycles=1;
	regs.BC.Byte.B=regs.HL.Byte.H;
	return 0;
}
int CPU::ld_b_l(){	//0x45
	cycles=1;
	regs.BC.Byte.B=regs.HL.Byte.L;
	return 0;
}
int CPU::ld_b_hl(){	//0x46
	cycles=2;
	uint8_t operand=ram.read(regs.HL.Word);
	regs.BC.Byte.B=operand;
	return 0;

}
int CPU::ld_b_a(){	//0x47
	cycles=1;
	regs.BC.Byte.B=regs.FA.Byte.A;
	return 0;
}
int CPU::ld_c_b(){	//0x48
	cycles=1;
	regs.BC.Byte.C=regs.BC.Byte.B;
	return 0;
}
int CPU::ld_c_c(){	//0x49
	cycles=1;
	regs.BC.Byte.C=regs.BC.Byte.C;
	return 0;
}
int CPU::ld_c_d(){	//0x4A
	cycles=1;
	regs.BC.Byte.C=regs.BC.Byte.C;
	return 0;
}
int CPU::ld_c_e(){	//0x4B
	cycles=1;
	regs.BC.Byte.C=regs.DE.Byte.E;
	return 0;
}
int CPU::ld_c_h(){	//0x4C
	cycles=1;
	regs.BC.Byte.C=regs.HL.Byte.H;
	return 0;
}
int CPU::ld_c_l(){	//0x4D
	cycles=1;
	regs.BC.Byte.C=regs.HL.Byte.L;
	return 0;
}
int CPU::ld_c_hl(){	//0x4E
	cycles=2;
	uint8_t operand=ram.read(regs.HL.Word);
	regs.BC.Byte.C=operand;
	return 0;
}
int CPU::ld_c_a(){	//0x4F
	cycles=1;
	regs.BC.Byte.C=regs.FA.Byte.A;
	return 0;
}



/*0x50 instructions*/
int CPU::ld_d_b(){	//0x50
	cycles=1;
	regs.DE.Byte.D=regs.BC.Byte.B;
	return 0;
}
int CPU::ld_d_c(){	//0x51
	cycles=1;
	regs.DE.Byte.D=regs.BC.Byte.C;
	return 0;
}
int CPU::ld_d_d(){	//0x52
	cycles=1;
	regs.DE.Byte.D=regs.DE.Byte.D;
	return 0;
}
int CPU::ld_d_e(){	//0x53
	cycles=1;
	regs.DE.Byte.D=regs.DE.Byte.E;
	return 0;
}
int CPU::ld_d_h(){	//0x54
	cycles=1;
	regs.DE.Byte.D=regs.HL.Byte.H;
	return 0;
}
int CPU::ld_d_l(){	//0x55
	cycles=1;
	regs.DE.Byte.D=regs.HL.Byte.L;
	return 0;
}
int CPU::ld_d_hl(){	//0x56
	cycles=2;
	uint8_t operand=ram.read(regs.HL.Word);
	regs.DE.Byte.D=operand;
	return 0;
}	
int CPU::ld_d_a(){	//0x57
	cycles=1;
	regs.DE.Byte.D=regs.FA.Byte.A;
	return 0;
}
int CPU::ld_e_b(){	//0x58
	cycles=1;
	regs.DE.Byte.E=regs.BC.Byte.B;
	return 0;
}
int CPU::ld_e_c(){	//0x59
	cycles=1;
	regs.DE.Byte.E=regs.BC.Byte.C;
	return 0;
}
int CPU::ld_e_d(){	//0x5A
	cycles=1;
	regs.DE.Byte.E=regs.DE.Byte.D;
	return 0;
}
int CPU::ld_e_e(){	//0x5B
	cycles=1;
	regs.DE.Byte.E=regs.DE.Byte.E;
	return 0;
}
int CPU::ld_e_h(){	//0x5C
	cycles=1;
	regs.DE.Byte.E=regs.HL.Byte.H;
	return 0;
}	
int CPU::ld_e_l(){	//0x5D
	cycles=1;
	regs.DE.Byte.E=regs.HL.Byte.L;
	return 0;
} 	
int CPU::ld_e_hl(){	//0x5E
	cycles=2;
	uint8_t operand=ram.read(regs.HL.Word);
	regs.DE.Byte.E=operand;
	return 0;
}	
int CPU::ld_e_a(){	//0x5F
	cycles=1;
	regs.DE.Byte.E=regs.FA.Byte.A;
	return 0;
}

/*0x60 instructions*/
int CPU::ld_h_b(){
	cycles=1;
	regs.HL.Byte.H=regs.BC.Byte.B;
	return 0;
}	
int CPU::ld_h_c(){
	cycles=1;
	regs.HL.Byte.H=regs.BC.Byte.C;
	return 0;
}	
int CPU::ld_h_d(){
	cycles=1;
	regs.HL.Byte.H=regs.DE.Byte.D;
	return 0;
}	
int CPU::ld_h_e(){
	cycles=1;
	regs.HL.Byte.H=regs.DE.Byte.E;
	return 0;
}	
int CPU::ld_h_h(){
	cycles=1;
	regs.HL.Byte.H=regs.HL.Byte.H;
	return 0;
}	
int CPU::ld_h_l(){
	cycles=1;
	regs.HL.Byte.H=regs.HL.Byte.L;
	return 0;
}	
int CPU::ld_h_hl(){
	cycles=2;
	uint8_t operand=ram.read(regs.HL.Word);
	regs.HL.Byte.H=operand;
	return 0;
}	
int CPU::ld_h_a(){
	cycles=1;
	regs.HL.Byte.H=regs.FA.Byte.A;
	return 0;
}	
int CPU::ld_l_b(){
	cycles=1;
	regs.HL.Byte.L=regs.BC.Byte.B;
	return 0;
}	
int CPU::ld_l_c(){
	cycles=1;
	regs.HL.Byte.L=regs.BC.Byte.C;
	return 0;
}	
int CPU::ld_l_d(){
	cycles=1;
	regs.HL.Byte.L=regs.DE.Byte.D;
	return 0;
}	
int CPU::ld_l_e(){
	cycles=1;
	regs.HL.Byte.L=regs.DE.Byte.E;
	return 0;
}	
int CPU::ld_l_h(){
	cycles=1;
	regs.HL.Byte.L=regs.HL.Byte.H;
	return 0;
}	
int CPU::ld_l_l(){
	cycles=1;
	regs.HL.Byte.L=regs.HL.Byte.L;
	return 0;
} 	
int CPU::ld_l_hl(){
	cycles=2;
	uint8_t operand=ram.read(regs.HL.Word);
	regs.HL.Byte.L=operand;
	return 0;
}	
int CPU::ld_l_a(){
	cycles=1;
	regs.HL.Byte.L=regs.FA.Byte.A;
	return 0;
}

/*0x70 instructions*/
int CPU::ld_hl_b(){
	cycles=2;
	ram.write(regs.HL.Word,regs.BC.Byte.B);

}	
int CPU::ld_hl_c(){
	cycles=2;
	ram.write(regs.HL.Word,regs.BC.Byte.C);
}
int CPU::ld_hl_d(){
	cycles=2;
	ram.write(regs.HL.Word,regs.DE.Byte.D);
}	
int CPU::ld_hl_e(){
	cycles=2;
	ram.write(regs.HL.Word,regs.DE.Byte.E);
}	
int CPU::ld_hl_h(){
	cycles=2;
	ram.write(regs.HL.Word,regs.HL.Byte.H);
}	
int CPU::ld_hl_l(){
	cycles=2;
	ram.write(regs.HL.Word,regs.HL.Byte.L);
}	
int CPU::HALT(){

}		
int CPU::ld_hl_a(){
	cycles=2;
	ram.write(regs.HL.Word,regs.FA.Byte.A);
}	
int CPU::ld_a_b(){
	cycles=1;
	regs.FA.Byte.A=regs.BC.Byte.B;
	return 0;
}	
int CPU::ld_a_c(){
	cycles=1;
	regs.FA.Byte.A=regs.BC.Byte.C;
	return 0;
}	
int CPU::ld_a_d(){
	cycles=1;
	regs.FA.Byte.A=regs.DE.Byte.D;
	return 0;
}	
int CPU::ld_a_e(){
	cycles=1;
	regs.FA.Byte.A=regs.DE.Byte.D;
	return 0;
}	
int CPU::ld_a_h(){
	cycles=1;
	regs.FA.Byte.A=regs.HL.Byte.H;
	return 0;
}	
int CPU::ld_a_l(){
	cycles=1;
	regs.FA.Byte.A=regs.HL.Byte.L;
	return 0;
} 	
int CPU::ld_a_hl(){
	cycles=2;
	uint8_t operand=ram.read(regs.HL.Word);
	regs.FA.Byte.A=operand;
	return 0;
}	
int CPU::ld_a_a(){
	cycles=1;
	regs.FA.Byte.A=regs.FA.Byte.A;
	return 0;
};


/*0x80 instructions*/
int CPU::add_a_b(){}	
int CPU::add_a_c(){}	
int CPU::add_a_d(){}	
int CPU::add_a_e(){}	
int CPU::add_a_h(){}	
int CPU::add_a_l(){}	
int CPU::add_a_hl(){}	
int CPU::add_a_a(){}
int CPU::adc_a_b(){}	
int CPU::adc_a_c(){}	
int CPU::adc_a_d(){}	
int CPU::adc_a_e(){}	
int CPU::adc_a_h(){}	
int CPU::adc_a_l(){}
int CPU::adc_a_hl(){}	
int CPU::adc_a_a(){}

/*0x90 instructions*/
int CPU::sub_b(){}	
int CPU::sub_c(){}	
int CPU::sub_d(){}	
int CPU::sub_e(){}	
int CPU::sub_h(){}	
int CPU::sub_l(){}	
int CPU::sub_hl(){}	
int CPU::sub_a(){}
int CPU::sbc_a_b(){}	
int CPU::sbc_a_c(){}	
int CPU::sbc_a_d(){}	
int CPU::sbc_a_e(){}	
int CPU::sbc_a_h(){}	
int CPU::sbc_a_l(){}	
int CPU::sbc_a_hl(){}	
int CPU::sbc_a_a(){}


/*0xA0 instructions*/
int CPU::add_b(){}	
int CPU::add_c(){}	
int CPU::add_d(){}	
int CPU::add_e(){}	
int CPU::add_h(){}	
int CPU::add_l(){}	
int CPU::add_hl(){}	
int CPU::add_a(){}
int CPU::xor_b(){}	
int CPU::xor_c(){}	
int CPU::xor_d(){}	
int CPU::xor_e(){}	
int CPU::xor_h(){}	
int CPU::xor_l(){}	
int CPU::xor_hl(){}
int CPU::xor_a(){}

/*0xB0 instructions*/
int CPU::or_b(){}	
int CPU::or_c(){}
int CPU::or_d(){}	
int CPU::or_e(){}	
int CPU::or_h(){}	
int CPU::or_l(){}	
int CPU::or_hl(){}
int CPU::or_a(){}
int CPU::cp_b(){}	
int CPU::cp_c(){}	
int CPU::cp_d(){}	
int CPU::cp_e(){}	
int CPU::cp_h(){}	
int CPU::cp_l(){}	
int CPU::cp_hl(){}
int CPU::cp_a(){}