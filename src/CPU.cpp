#include "CPU.h"
#include <cstdint>

using namespace GameBoy;

CPU::CPU(){

	opcodeLUT=std::vector<int (CPU::*)(void)>{
			&CPU::nop,			&CPU::ld_bc_d16,	&CPU::ld_bc_a,		&CPU::inc_bc,		&CPU::inc_b,		&CPU::dec_b,		&CPU::ld_b_d8,		&CPU::rlca, 	&CPU::ld_a16_sp,	&CPU::add_hl_bc,	&CPU::ld_a_bc,		&CPU::dec_bc,		&CPU::inc_c,		&CPU::dec_c,		&CPU::ld_c_d8,		&CPU::rrca,
			&CPU::stop,			&CPU::ld_de_d16,	&CPU::ld_de_a,		&CPU::inc_de,		&CPU::inc_d,		&CPU::dec_d,		&CPU::ld_d_d8,		&CPU::rla,		&CPU::jr_s8,		&CPU::add_hl_de,	&CPU::ld_a_de,		&CPU::dec_de,		&CPU::inc_e,		&CPU::dec_e,		&CPU::ld_e_d8,		&CPU::rra,
			&CPU::jr_nz_s8,		&CPU::ld_hl_d16,	&CPU::ld_hlp_a,		&CPU::inc_hl,		&CPU::inc_h,		&CPU::dec_h,		&CPU::ld_h_d8,		&CPU::daa,		&CPU::jr_z_s8,		&CPU::add_hl_hl,	&CPU::ld_a_hlp,		&CPU::dec_hl,		&CPU::inc_l,		&CPU::dec_l,		&CPU::ld_l_d8,		&CPU::cpl,
			&CPU::jr_nc_s8,		&CPU::ld_sp_d16,	&CPU::ld_hlm_a,		&CPU::inc_sp,		&CPU::inc_hl_mem,	&CPU::dec_hl_mem,	&CPU::ld_hl_d8,		&CPU::scf,		&CPU::jr_c_s8,		&CPU::add_hl_sp,	&CPU::ld_a_hlm,		&CPU::dec_sp,		&CPU::inc_a,		&CPU::dec_a,		&CPU::ld_a_d8,		&CPU::ccf,
			&CPU::ld_b_b,		&CPU::ld_b_c,		&CPU::ld_b_d,		&CPU::ld_b_e,		&CPU::ld_b_h,		&CPU::ld_b_l,		&CPU::ld_b_hl,		&CPU::ld_b_a,	&CPU::ld_c_b,		&CPU::ld_c_c,		&CPU::ld_c_d,		&CPU::ld_c_e,		&CPU::ld_c_h,		&CPU::ld_c_l,		&CPU::ld_c_hl,		&CPU::ld_c_a,
			&CPU::ld_d_b,		&CPU::ld_d_c,		&CPU::ld_d_d,		&CPU::ld_d_e,		&CPU::ld_d_h,		&CPU::ld_d_l,		&CPU::ld_d_hl,		&CPU::ld_d_a,	&CPU::ld_e_b,		&CPU::ld_e_c,		&CPU::ld_e_d,		&CPU::ld_e_e,		&CPU::ld_e_h,		&CPU::ld_e_l,		&CPU::ld_e_hl,		&CPU::ld_e_a,
			&CPU::ld_h_b,		&CPU::ld_h_c,		&CPU::ld_h_d,		&CPU::ld_h_e,		&CPU::ld_h_h,		&CPU::ld_h_l,		&CPU::ld_h_hl,		&CPU::ld_h_a,	&CPU::ld_l_b,		&CPU::ld_l_c,		&CPU::ld_l_d,		&CPU::ld_l_e,		&CPU::ld_l_h,		&CPU::ld_l_l,		&CPU::ld_l_hl,		&CPU::ld_l_a,
			&CPU::ld_hl_b,		&CPU::ld_hl_c,		&CPU::ld_hl_d,		&CPU::ld_hl_e,		&CPU::ld_hl_h,		&CPU::ld_hl_l,		&CPU::HALT,			&CPU::ld_hl_a,	&CPU::ld_a_b,		&CPU::ld_a_c,		&CPU::ld_a_d,		&CPU::ld_a_e,		&CPU::ld_a_h,		&CPU::ld_a_l,		&CPU::ld_a_hl,		&CPU::ld_a_a,
			&CPU::add_a_b,		&CPU::add_a_c,		&CPU::add_a_d,		&CPU::add_a_e,		&CPU::add_a_h,		&CPU::add_a_l,		&CPU::add_a_hl,		&CPU::add_a_a,	&CPU::adc_a_b,		&CPU::adc_a_c,		&CPU::adc_a_d,		&CPU::adc_a_e,		&CPU::adc_a_h,		&CPU::adc_a_l,		&CPU::adc_a_hl,		&CPU::adc_a_a,
			&CPU::sub_b,		&CPU::sub_c,		&CPU::sub_d,		&CPU::sub_e,		&CPU::sub_h,		&CPU::sub_l,		&CPU::sub_hl,		&CPU::sub_a,	&CPU::sbc_a_b,		&CPU::sbc_a_c,		&CPU::sbc_a_d,		&CPU::sbc_a_e,		&CPU::sbc_a_h,		&CPU::sbc_a_l,		&CPU::sbc_a_hl,		&CPU::sbc_a_a,
			&CPU::add_b,		&CPU::add_c,		&CPU::add_d,		&CPU::add_e,		&CPU::add_h,		&CPU::add_l,		&CPU::add_hl,		&CPU::add_a,	&CPU::xor_b,		&CPU::xor_c,		&CPU::xor_d,		&CPU::xor_e,		&CPU::xor_h,		&CPU::xor_l,		&CPU::xor_hl,		&CPU::xor_a,
			&CPU::or_b,			&CPU::or_c,			&CPU::or_d,			&CPU::or_e,			&CPU::or_h,			&CPU::or_l,			&CPU::or_hl,		&CPU::or_a,		&CPU::cp_b,			&CPU::cp_c,			&CPU::cp_d,			&CPU::cp_e,			&CPU::cp_h,			&CPU::cp_l,			&CPU::cp_hl,		&CPU::cp_a,
			&CPU::ret_nz,		&CPU::pop_bc,		&CPU::jp_nz_a16,	&CPU::jp_a16,		&CPU::call_nz_a16,	&CPU::push_bc,		&CPU::add_a_d8,		&CPU::rst_a0,	&CPU::ret_z,		&CPU::ret,			&CPU::jp_z_a16,		&CPU::invalid,		&CPU::call_z_a16,	&CPU::call_z16,		&CPU::adc_a_d8,		&CPU::rst_1,
			&CPU::ret_nc,		&CPU::pop_de,		&CPU::jp_nc_a16,	&CPU::invalid,		&CPU::call_nc_a16,	&CPU::push_de,		&CPU::sub_d8,		&CPU::rst_2,	&CPU::ret_c,		&CPU::reti,			&CPU::jp_c_a16,		&CPU::invalid,		&CPU::call_c_a16,	&CPU::invalid,		&CPU::sbc_a_d8,		&CPU::rst_3,
			&CPU::ld_a8_a,		&CPU::pop_hl,		&CPU::ld_c_a,		&CPU::invalid,		&CPU::invalid,		&CPU::push_hl,		&CPU::and_d8,		&CPU::rst_4,	&CPU::add_sp_s8,	&CPU::jp_hl,		&CPU::ld_a16_a,		&CPU::invalid,		&CPU::invalid,		&CPU::invalid,		&CPU::xor_d8,		&CPU::rst_5,
			&CPU::ld_a_a8,		&CPU::pop_af,		&CPU::ld_a_c,		&CPU::di,			&CPU::invalid,		&CPU::push_af,		&CPU::or_d8,		&CPU::rst_6,	&CPU::ld_hl_sp_s8,	&CPU::ld_sp_hl,		&CPU::ld_a_a16,		&CPU::ei,			&CPU::invalid,		&CPU::invalid,		&CPU::cp_d8,		&CPU::rst_7
	};

	opcodeLUTCB=std::vector<int (CPU::*)(void)>{
			&CPU::rlc_b,	&CPU::rlc_c,	&CPU::rlc_d,	&CPU::rlc_e,	&CPU::rlc_h,	&CPU::rlc_l,	&CPU::rlc_hl,	&CPU::rlc_a,	&CPU::rrc_b,	&CPU::rrc_c,	&CPU::rrc_d,	&CPU::rrc_e,	&CPU::rrc_h,	&CPU::rrc_l,	&CPU::rrc_hl,	&CPU::rrc_a,
			&CPU::rl_b,		&CPU::rl_c,		&CPU::rl_d,		&CPU::rl_e,		&CPU::rl_h,		&CPU::rl_l,		&CPU::rl_hl,	&CPU::rl_a,		&CPU::rr_b,		&CPU::rr_c,		&CPU::rr_d,		&CPU::rr_e,		&CPU::rr_h,		&CPU::rr_l,		&CPU::rr_hl,	&CPU::rr_a,
			&CPU::sla_b,	&CPU::sla_c,	&CPU::sla_d,	&CPU::sla_e,	&CPU::sla_h,	&CPU::sla_l,	&CPU::sla_hl,	&CPU::sla_a,	&CPU::sra_b,	&CPU::sra_c,	&CPU::sra_d,	&CPU::sra_e,	&CPU::sra_h,	&CPU::sra_l,	&CPU::sra_hl,	&CPU::sra_a,
			&CPU::swap_b,	&CPU::swap_c,	&CPU::swap_d,	&CPU::swap_e,	&CPU::swap_h,	&CPU::swap_l,	&CPU::swap_hl,	&CPU::swap_a,	&CPU::srl_b,	&CPU::srl_c,	&CPU::srl_d,	&CPU::srl_e,	&CPU::srl_h,	&CPU::srl_l,	&CPU::srl_hl,	&CPU::srl_a,
			&CPU::bit_0_b,	&CPU::bit_0_c,	&CPU::bit_0_f,	&CPU::bit_0_e,	&CPU::bit_0_h,	&CPU::bit_0_l,	&CPU::bit_0_hl,	&CPU::bit_0_a,	&CPU::bit_1_b,	&CPU::bit_1_c,	&CPU::bit_1_f,	&CPU::bit_1_e,	&CPU::bit_1_h,	&CPU::bit_1_l,	&CPU::bit_1_hl,	&CPU::bit_1_a,
			&CPU::bit_2_b,	&CPU::bit_2_c,	&CPU::bit_2_f,	&CPU::bit_2_e,	&CPU::bit_2_h,	&CPU::bit_2_l,	&CPU::bit_2_hl,	&CPU::bit_2_a,	&CPU::bit_3_b,	&CPU::bit_3_c,	&CPU::bit_3_f,	&CPU::bit_3_e,	&CPU::bit_3_h,	&CPU::bit_3_l,	&CPU::bit_3_hl,	&CPU::bit_3_a,
			&CPU::bit_4_b,	&CPU::bit_4_c,	&CPU::bit_4_f,	&CPU::bit_4_e,	&CPU::bit_4_h,	&CPU::bit_4_l,	&CPU::bit_4_hl,	&CPU::bit_4_a,	&CPU::bit_5_b,	&CPU::bit_3_c,	&CPU::bit_5_f,	&CPU::bit_5_e,	&CPU::bit_5_h,	&CPU::bit_5_l,	&CPU::bit_5_hl,	&CPU::bit_5_a,
			&CPU::bit_6_b,	&CPU::bit_6_c,	&CPU::bit_6_f,	&CPU::bit_6_e,	&CPU::bit_6_h,	&CPU::bit_6_l,	&CPU::bit_6_hl,	&CPU::bit_6_a,	&CPU::bit_7_b,	&CPU::bit_3_c,	&CPU::bit_7_f,	&CPU::bit_7_e,	&CPU::bit_7_h,	&CPU::bit_7_l,	&CPU::bit_7_hl,	&CPU::bit_7_a,
			&CPU::res_0_b,	&CPU::res_0_c,	&CPU::res_0_f,	&CPU::res_0_e,	&CPU::res_0_h,	&CPU::res_0_l,	&CPU::res_0_hl,	&CPU::res_0_a,	&CPU::res_1_b,	&CPU::res_1_c,	&CPU::res_1_f,	&CPU::res_1_e,	&CPU::res_1_h,	&CPU::res_1_l,	&CPU::res_1_hl,	&CPU::res_1_a,
			&CPU::res_2_b,	&CPU::res_2_c,	&CPU::res_2_f,	&CPU::res_2_e,	&CPU::res_2_h,	&CPU::res_2_l,	&CPU::res_2_hl,	&CPU::res_2_a,	&CPU::res_3_b,	&CPU::res_3_c,	&CPU::res_3_f,	&CPU::res_3_e,	&CPU::res_3_h,	&CPU::res_3_l,	&CPU::res_3_hl,	&CPU::res_3_a,
			&CPU::res_4_b,	&CPU::res_4_c,	&CPU::res_4_f,	&CPU::res_4_e,	&CPU::res_4_h,	&CPU::res_4_l,	&CPU::res_4_hl,	&CPU::res_4_a,	&CPU::res_5_b,	&CPU::res_3_c,	&CPU::res_5_f,	&CPU::res_5_e,	&CPU::res_5_h,	&CPU::res_5_l,	&CPU::res_5_hl,	&CPU::res_5_a,
			&CPU::res_6_b,	&CPU::res_6_c,	&CPU::res_6_f,	&CPU::res_6_e,	&CPU::res_6_h,	&CPU::res_6_l,	&CPU::res_6_hl,	&CPU::res_6_a,	&CPU::res_7_b,	&CPU::res_3_c,	&CPU::res_7_f,	&CPU::res_7_e,	&CPU::res_7_h,	&CPU::res_7_l,	&CPU::res_7_hl,	&CPU::res_7_a,
			&CPU::set_0_b,	&CPU::set_0_c,	&CPU::set_0_f,	&CPU::set_0_e,	&CPU::set_0_h,	&CPU::set_0_l,	&CPU::set_0_hl,	&CPU::set_0_a,	&CPU::set_1_b,	&CPU::set_1_c,	&CPU::set_1_f,	&CPU::set_1_e,	&CPU::set_1_h,	&CPU::set_1_l,	&CPU::set_1_hl,	&CPU::set_1_a,
			&CPU::set_2_b,	&CPU::set_2_c,	&CPU::set_2_f,	&CPU::set_2_e,	&CPU::set_2_h,	&CPU::set_2_l,	&CPU::set_2_hl,	&CPU::set_2_a,	&CPU::set_3_b,	&CPU::set_3_c,	&CPU::set_3_f,	&CPU::set_3_e,	&CPU::set_3_h,	&CPU::set_3_l,	&CPU::set_3_hl,	&CPU::set_3_a,
			&CPU::set_4_b,	&CPU::set_4_c,	&CPU::set_4_f,	&CPU::set_4_e,	&CPU::set_4_h,	&CPU::set_4_l,	&CPU::set_4_hl,	&CPU::set_4_a,	&CPU::set_5_b,	&CPU::set_3_c,	&CPU::set_5_f,	&CPU::set_5_e,	&CPU::set_5_h,	&CPU::set_5_l,	&CPU::set_5_hl,	&CPU::set_5_a,
			&CPU::set_6_b,	&CPU::set_6_c,	&CPU::set_6_f,	&CPU::set_6_e,	&CPU::set_6_h,	&CPU::set_6_l,	&CPU::set_6_hl,	&CPU::set_6_a,	&CPU::set_7_b,	&CPU::set_3_c,	&CPU::set_7_f,	&CPU::set_7_e,	&CPU::set_7_h,	&CPU::set_7_l,	&CPU::set_7_hl,	&CPU::set_7_a
	};

}


void CPU::fetch(){
	instruction=ram.read(PC);
	PC++;


	//There are 2 types of instructions in the game boy: "regular" and 0xCB offset instructions
	//0xCB offset instructions are executed as such:
	//Start: 0xCB 
	//		 0xF6
	//0xCB is read, indicating that the next instruction is from the 0xCB offset table
	//not the regular opcode table
	//note that 0xCB can be an instruction in and of itself
	//ie: 
	//Start: 0xCB
	//		 0xCB 
	//is a valid program
	
	//This is important because otherwise the emulator will enter an infinite loop of setting the flag
	//Hence, if instruction is 0xCB and flag is not set, set flag and treat next instruction as normal

	if(instruction==0xCB && flagCB==0){
		flagCB=1;
		instruction=ram.read(PC);
		PC++;
	}
}

void CPU::execute(){	//dont forget interrupts 
	
	//use correct lookup table
	if(flagCB==0){
		(this->*opcodeLUT[instruction])();
	}
	else{
		(this->*opcodeLUTCB[instruction])();
	}

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


/*0xC0*/
int ret_nz();	
int pop_bc();	
int jp_nz_a16();	
int jp_a16();		
int call_nz_a16();	
int push_bc();	
int add_a_d8();	
int rst_a0();
int ret_z();	
int ret();		
int jp_z_a16();		
int call_z_a16();	
int call_z16();		
int adc_a_d8();	
int rst_1();

/*0xD0*/
int ret_nc();	
int pop_de();	
int jp_nc_a16();	
int call_nc_a16();	
int push_de();	
int sub_d8();	
int rst_2();	
int ret_c();	
int reti();		
int ret_c_a16();	
int call_c_a16();	
int sbc_a_d8();	
int rst_3();

/*0xE0*/
int ld_a8_a();	
int pop_hl();	
int ld_c_a();	
int push_hl();	
int and_d8();	
int rst_4();	
int add_sp_s8();	
int jp_hl();	
int ld_a16_a();
int xor_d8();	
int rst_5();


/*0xF0*/
int ld_a_a8();		
int pop_af();		
int ld_a_c();	
int di();	
int push_af();	
int or_d8();	
int rst_6();	
int ld_hl_sp_s8();	
int ld_sp_hl();		
int ld_a_a16();	
int ei();	
int cp_d8();	
int rst_7();


/***********16 BIT INSTRUCTIONS BELOW******************/
/*0x00*/
int rlc_b();	
int rlc_c();	
int rlc_d();	
int rlc_e();	
int rlc_h();	
int rlc_l();	
int rlc_hl();	
int rlc_a();
int rrc_b();	
int rrc_c();	
int rrc_d();	
int rrc_e();	
int rrc_h();	
int rrc_l();	
int rrc_hl();	
int rrc_a();

/*0x01*/
int rl_b();		
int rl_c();		
int rl_d();		
int rl_e();		
int rl_h();		
int rl_l();		
int rl_hl();	
int rl_a();
int rr_b();		
int rr_c();		
int rr_d();		
int rr_e();		
int rr_h();		
int rr_l();		
int rr_hl();	
int rr_a();

/*0x02*/
int sla_b();	
int sla_c();	
int sla_d();	
int sla_e();	
int sla_h();	
int sla_l();	
int sla_hl();	
int sla_a();
int sra_b();	
int sra_c();	
int sra_d();	
int sra_e();	
int sra_h();	
int sra_l();	
int sra_hl();	
int sra_a();


/*0x03*/
int swap_b();	
int swap_c();
int swap_d();	
int swap_e();	
int swap_h();	
int swap_l();	
int swap_hl();	
int swap_a();
int srl_b();	
int srl_c();	
int srl_d();	
int srl_e();	
int srl_h();	
int srl_l();	
int srl_hl();	
int srl_a();

/*0x04*/
int bit_0_b();	
int bit_0_c();	
int bit_0_f();	
int bit_0_e();	
int bit_0_h();	
int bit_0_l();
int bit_0_hl();
int bit_0_a();
int bit_1_b();	
int bit_1_c();	
int bit_1_f();	
int bit_1_e();	
int bit_1_h();	
int bit_1_l();	
int bit_1_hl();	
int bit_1_a();

/*0x05*/
int bit_2_b();	
int bit_2_c();	
int bit_2_f();	
int bit_2_e();	
int bit_2_h();	
int bit_2_l();	
int bit_2_hl();	
int bit_2_a();
int bit_3_b();	
int bit_3_c();	
int bit_3_f();	
int bit_3_e();	
int bit_3_h();	
int bit_3_l();	
int bit_3_hl();	
int bit_3_a();

/*0x06*/
int bit_4_b();	
int bit_4_c();	
int bit_4_f();	
int bit_4_e();
int bit_4_h();	
int bit_4_l();	
int bit_4_hl();	
int bit_4_a();
int bit_5_b();	
int bit_3_c();	
int bit_5_f();	
int bit_5_e();	
int bit_5_h();	
int bit_5_l();	
int bit_5_hl();	
int bit_5_a();

/*0x07*/
int bit_6_b();	
int bit_6_c();	
int bit_6_f();	
int bit_6_e();	
int bit_6_h();	
int bit_6_l();	
int bit_6_hl();
int bit_6_a();
int bit_7_b();	
int bit_3_c();
int bit_7_f();
int bit_7_e();	
int bit_7_h();	
int bit_7_l();	
int bit_7_hl();
int bit_7_a();

/*0x08*/
int res_0_b();	
int res_0_c();	
int res_0_f();	
int res_0_e();	
int res_0_h();
int res_0_l();	
int res_0_hl();	
int res_0_a();
int res_1_b();	
int res_1_c();	
int res_1_f();	
int res_1_e();	
int res_1_h();	
int res_1_l();
int res_1_hl();
int res_1_a();

/*0x09*/
int res_2_b();	
int res_2_c();	
int res_2_f();	
int res_2_e();	
int res_2_h();	
int res_2_l();	
int res_2_hl();
int res_2_a();
int res_3_b();	
int res_3_c();	
int res_3_f();	
int res_3_e();	
int res_3_h();	
int res_3_l();	
int res_3_hl();
int res_3_a();

/*0x0A*/
int res_4_b();
int res_4_c();	
int res_4_f();
int res_4_e();	
int res_4_h();	
int res_4_l();	
int res_4_hl();
int res_4_a();
int res_5_b();	
int res_3_c();	
int res_5_f();	
int res_5_e();	
int res_5_h();	
int res_5_l();	
int res_5_hl();	
int res_5_a();

/*0x0B*/
int res_6_b();	
int res_6_c();	
int res_6_f();	
int res_6_e();	
int res_6_h();	
int res_6_l();	
int res_6_hl();
int res_6_a();
int res_7_b();	
int res_3_c();	
int res_7_f();	
int res_7_e();	
int res_7_h();	
int res_7_l();	
int res_7_hl();	
int res_7_a();

/*0x0C*/
int set_0_b();	
int set_0_c();	
int set_0_f();	
int set_0_e();	
int set_0_h();	
int set_0_l();
int set_0_hl();
int set_0_a();
int set_1_b();
int set_1_c();	
int set_1_f();	
int set_1_e();	
int set_1_h();	
int set_1_l();	
int set_1_hl();
int set_1_a();

/*0x0D*/
int set_2_b();	
int set_2_c();	
int set_2_f();	
int set_2_e();	
int set_2_h();	
int set_2_l();	
int set_2_hl();
int set_2_a();
int set_3_b();
int set_3_c();	
int set_3_f();	
int set_3_e();	
int set_3_h();	
int set_3_l();	
int set_3_hl();	
int set_3_a();

/*0x0E*/
int set_4_b();	
int set_4_c();	
int set_4_f();	
int set_4_e();	
int set_4_h();
int set_4_l();
int set_4_hl();
int set_4_a();
int set_5_b();	
int set_3_c();
int set_5_f();	
int set_5_e();	
int set_5_h();	
int set_5_l();	
int set_5_hl();	
int set_5_a();

/*0x0F*/
int set_6_b();	
int set_6_c();	
int set_6_f();	
int set_6_e();	
int set_6_h();	
int set_6_l();	
int set_6_hl();
int set_6_a();
int set_7_b();	
int set_3_c();	
int set_7_f();	
int set_7_e();	
int set_7_h();	
int set_7_l();	
int set_7_hl();
int set_7_a();

