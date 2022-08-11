#include "CPU.h"
#include <cstdint>
#include <iostream>

using namespace GameBoy;

CPU::CPU() {

	PC=0x100;	//start up value

	//initialize registers

	regs.AF=0;
	regs.BC=0;
	regs.DE=0;

	opcodeLUT = std::vector<int (CPU::*)(void)>{
		&CPU::nop,         &CPU::ld_bc_d16,  &CPU::ld_bc_a,   &CPU::inc_bc,
			&CPU::inc_b,       &CPU::dec_b,      &CPU::ld_b_d8,   &CPU::rlca,
			&CPU::ld_a16_sp,   &CPU::add_hl_bc,  &CPU::ld_a_bc,   &CPU::dec_bc,
			&CPU::inc_c,       &CPU::dec_c,      &CPU::ld_c_d8,   &CPU::rrca,
			&CPU::stop,        &CPU::ld_de_d16,  &CPU::ld_de_a,   &CPU::inc_de,
			&CPU::inc_d,       &CPU::dec_d,      &CPU::ld_d_d8,   &CPU::rla,
			&CPU::jr_s8,       &CPU::add_hl_de,  &CPU::ld_a_de,   &CPU::dec_de,
			&CPU::inc_e,       &CPU::dec_e,      &CPU::ld_e_d8,   &CPU::rra,
			&CPU::jr_nz_s8,    &CPU::ld_hl_d16,  &CPU::ld_hlp_a,  &CPU::inc_hl,
			&CPU::inc_h,       &CPU::dec_h,      &CPU::ld_h_d8,   &CPU::daa,
			&CPU::jr_z_s8,     &CPU::add_hl_hl,  &CPU::ld_a_hlp,  &CPU::dec_hl,
			&CPU::inc_l,       &CPU::dec_l,      &CPU::ld_l_d8,   &CPU::cpl,
			&CPU::jr_nc_s8,    &CPU::ld_sp_d16,  &CPU::ld_hlm_a,  &CPU::inc_sp,
			&CPU::inc_hl_mem,  &CPU::dec_hl_mem, &CPU::ld_hl_d8,  &CPU::scf,
			&CPU::jr_c_s8,     &CPU::add_hl_sp,  &CPU::ld_a_hlm,  &CPU::dec_sp,
			&CPU::inc_a,       &CPU::dec_a,      &CPU::ld_a_d8,   &CPU::ccf,
			&CPU::ld_b_b,      &CPU::ld_b_c,     &CPU::ld_b_d,    &CPU::ld_b_e,
			&CPU::ld_b_h,      &CPU::ld_b_l,     &CPU::ld_b_hl,   &CPU::ld_b_a,
			&CPU::ld_c_b,      &CPU::ld_c_c,     &CPU::ld_c_d,    &CPU::ld_c_e,
			&CPU::ld_c_h,      &CPU::ld_c_l,     &CPU::ld_c_hl,   &CPU::ld_c_a,
			&CPU::ld_d_b,      &CPU::ld_d_c,     &CPU::ld_d_d,    &CPU::ld_d_e,
			&CPU::ld_d_h,      &CPU::ld_d_l,     &CPU::ld_d_hl,   &CPU::ld_d_a,
			&CPU::ld_e_b,      &CPU::ld_e_c,     &CPU::ld_e_d,    &CPU::ld_e_e,
			&CPU::ld_e_h,      &CPU::ld_e_l,     &CPU::ld_e_hl,   &CPU::ld_e_a,
			&CPU::ld_h_b,      &CPU::ld_h_c,     &CPU::ld_h_d,    &CPU::ld_h_e,
			&CPU::ld_h_h,      &CPU::ld_h_l,     &CPU::ld_h_hl,   &CPU::ld_h_a,
			&CPU::ld_l_b,      &CPU::ld_l_c,     &CPU::ld_l_d,    &CPU::ld_l_e,
			&CPU::ld_l_h,      &CPU::ld_l_l,     &CPU::ld_l_hl,   &CPU::ld_l_a,
			&CPU::ld_hl_b,     &CPU::ld_hl_c,    &CPU::ld_hl_d,   &CPU::ld_hl_e,
			&CPU::ld_hl_h,     &CPU::ld_hl_l,    &CPU::HALT,      &CPU::ld_hl_a,
			&CPU::ld_a_b,      &CPU::ld_a_c,     &CPU::ld_a_d,    &CPU::ld_a_e,
			&CPU::ld_a_h,      &CPU::ld_a_l,     &CPU::ld_a_hl,   &CPU::ld_a_a,
			&CPU::add_a_b,     &CPU::add_a_c,    &CPU::add_a_d,   &CPU::add_a_e,
			&CPU::add_a_h,     &CPU::add_a_l,    &CPU::add_a_hl,  &CPU::add_a_a,
			&CPU::adc_a_b,     &CPU::adc_a_c,    &CPU::adc_a_d,   &CPU::adc_a_e,
			&CPU::adc_a_h,     &CPU::adc_a_l,    &CPU::adc_a_hl,  &CPU::adc_a_a,
			&CPU::sub_b,       &CPU::sub_c,      &CPU::sub_d,     &CPU::sub_e,
			&CPU::sub_h,       &CPU::sub_l,      &CPU::sub_hl,    &CPU::sub_a,
			&CPU::sbc_a_b,     &CPU::sbc_a_c,    &CPU::sbc_a_d,   &CPU::sbc_a_e,
			&CPU::sbc_a_h,     &CPU::sbc_a_l,    &CPU::sbc_a_hl,  &CPU::sbc_a_a,
			&CPU::and_b,       &CPU::and_c,      &CPU::and_d,     &CPU::and_e,
			&CPU::and_h,       &CPU::and_l,      &CPU::and_hl,    &CPU::and_a,
			&CPU::xor_b,       &CPU::xor_c,      &CPU::xor_d,     &CPU::xor_e,
			&CPU::xor_h,       &CPU::xor_l,      &CPU::xor_hl,    &CPU::xor_a,
			&CPU::or_b,        &CPU::or_c,       &CPU::or_d,      &CPU::or_e,
			&CPU::or_h,        &CPU::or_l,       &CPU::or_hl,     &CPU::or_a,
			&CPU::cp_b,        &CPU::cp_c,       &CPU::cp_d,      &CPU::cp_e,
			&CPU::cp_h,        &CPU::cp_l,       &CPU::cp_hl,     &CPU::cp_a,
			&CPU::ret_nz,      &CPU::pop_bc,     &CPU::jp_nz_a16, &CPU::jp_a16,
			&CPU::call_nz_a16, &CPU::push_bc,    &CPU::add_a_d8,  &CPU::rst_0,
			&CPU::ret_z,       &CPU::ret,        &CPU::jp_z_a16,  &CPU::invalid,
			&CPU::call_z_a16,  &CPU::call_a16,   &CPU::adc_a_d8,  &CPU::rst_1,
			&CPU::ret_nc,      &CPU::pop_de,     &CPU::jp_nc_a16, &CPU::invalid,
			&CPU::call_nc_a16, &CPU::push_de,    &CPU::sub_d8,    &CPU::rst_2,
			&CPU::ret_c,       &CPU::reti,       &CPU::jp_c_a16,  &CPU::invalid,
			&CPU::call_c_a16,  &CPU::invalid,    &CPU::sbc_a_d8,  &CPU::rst_3,
			&CPU::ld_a8_a,     &CPU::pop_hl,     &CPU::ld_c_mem_a,&CPU::invalid,
			&CPU::invalid,     &CPU::push_hl,    &CPU::and_d8,    &CPU::rst_4,
			&CPU::add_sp_s8,   &CPU::jp_hl,      &CPU::ld_a16_a,  &CPU::invalid,
			&CPU::invalid,     &CPU::invalid,    &CPU::xor_d8,    &CPU::rst_5,
			&CPU::ld_a_a8,     &CPU::pop_af,     &CPU::ld_a_c_mem,&CPU::di,
			&CPU::invalid,     &CPU::push_af,    &CPU::or_d8,     &CPU::rst_6,
			&CPU::ld_hl_sp_s8, &CPU::ld_sp_hl,   &CPU::ld_a_a16,  &CPU::ei,
			&CPU::invalid,     &CPU::invalid,    &CPU::cp_d8,     &CPU::rst_7};

	opcodeLUTCB = std::vector<int (CPU::*)(void)>{
		&CPU::rlc_b,   &CPU::rlc_c,   &CPU::rlc_d,    &CPU::rlc_e,  &CPU::rlc_h,   &CPU::rlc_l,   &CPU::rlc_hl,   &CPU::rlc_a,  &CPU::rrc_b,   &CPU::rrc_c,   &CPU::rrc_d,    &CPU::rrc_e,  &CPU::rrc_h,   &CPU::rrc_l,   &CPU::rrc_hl,   &CPU::rrc_a,
			&CPU::rl_b,    &CPU::rl_c,    &CPU::rl_d,     &CPU::rl_e,   &CPU::rl_h,    &CPU::rl_l,    &CPU::rl_hl,    &CPU::rl_a,   &CPU::rr_b,    &CPU::rr_c,    &CPU::rr_d,     &CPU::rr_e,   &CPU::rr_h,    &CPU::rr_l,    &CPU::rr_hl,    &CPU::rr_a,
			&CPU::sla_b,   &CPU::sla_c,   &CPU::sla_d,    &CPU::sla_e,  &CPU::sla_h,   &CPU::sla_l,   &CPU::sla_hl,   &CPU::sla_a,  &CPU::sra_b,   &CPU::sra_c,   &CPU::sra_d,    &CPU::sra_e,  &CPU::sra_h,   &CPU::sra_l,   &CPU::sra_hl,   &CPU::sra_a,
			&CPU::swap_b,  &CPU::swap_c,  &CPU::swap_d,   &CPU::swap_e, &CPU::swap_h,  &CPU::swap_l,  &CPU::swap_hl,  &CPU::swap_a, &CPU::srl_b,   &CPU::srl_c,   &CPU::srl_d,    &CPU::srl_e,  &CPU::srl_h,   &CPU::srl_l,   &CPU::srl_hl,	&CPU::srl_a,
			&CPU::bit_0_b, &CPU::bit_0_c, &CPU::bit_0_f,  &CPU::bit_0_e,&CPU::bit_0_h, &CPU::bit_0_l, &CPU::bit_0_hl, &CPU::bit_0_a,&CPU::bit_1_b, &CPU::bit_1_c, &CPU::bit_1_f,  &CPU::bit_1_e,&CPU::bit_1_h, &CPU::bit_1_l, &CPU::bit_1_hl, &CPU::bit_1_a,
			&CPU::bit_2_b, &CPU::bit_2_c, &CPU::bit_2_f,  &CPU::bit_2_e,&CPU::bit_2_h, &CPU::bit_2_l, &CPU::bit_2_hl, &CPU::bit_2_a,&CPU::bit_3_b, &CPU::bit_3_c, &CPU::bit_3_f,  &CPU::bit_3_e,&CPU::bit_3_h, &CPU::bit_3_l, &CPU::bit_3_hl, &CPU::bit_3_a,
			&CPU::bit_4_b, &CPU::bit_4_c, &CPU::bit_4_f,  &CPU::bit_4_e,&CPU::bit_4_h, &CPU::bit_4_l, &CPU::bit_4_hl, &CPU::bit_4_a,&CPU::bit_5_b, &CPU::bit_5_c, &CPU::bit_5_f,  &CPU::bit_5_e,&CPU::bit_5_h, &CPU::bit_5_l, &CPU::bit_5_hl, &CPU::bit_5_a,
			&CPU::bit_6_b, &CPU::bit_6_c, &CPU::bit_6_f,  &CPU::bit_6_e,&CPU::bit_6_h, &CPU::bit_6_l, &CPU::bit_6_hl, &CPU::bit_6_a,&CPU::bit_7_b, &CPU::bit_7_c, &CPU::bit_7_f,  &CPU::bit_7_e,&CPU::bit_7_h, &CPU::bit_7_l, &CPU::bit_7_hl, &CPU::bit_7_a,
			&CPU::res_0_b, &CPU::res_0_c, &CPU::res_0_f,  &CPU::res_0_e,&CPU::res_0_h, &CPU::res_0_l, &CPU::res_0_hl, &CPU::res_0_a,&CPU::res_1_b, &CPU::res_1_c, &CPU::res_1_f,  &CPU::res_1_e,&CPU::res_1_h, &CPU::res_1_l, &CPU::res_1_hl, &CPU::res_1_a,
			&CPU::res_2_b, &CPU::res_2_c, &CPU::res_2_f,  &CPU::res_2_e,&CPU::res_2_h, &CPU::res_2_l, &CPU::res_2_hl, &CPU::res_2_a,&CPU::res_3_b, &CPU::res_3_c, &CPU::res_3_f,  &CPU::res_3_e,&CPU::res_3_h, &CPU::res_3_l, &CPU::res_3_hl, &CPU::res_3_a,
			&CPU::res_4_b, &CPU::res_4_c, &CPU::res_4_f,  &CPU::res_4_e,&CPU::res_4_h, &CPU::res_4_l, &CPU::res_4_hl, &CPU::res_4_a,&CPU::res_5_b, &CPU::res_5_c, &CPU::res_5_f,  &CPU::res_5_e,&CPU::res_5_h, &CPU::res_5_l, &CPU::res_5_hl, &CPU::res_5_a,
			&CPU::res_6_b, &CPU::res_6_c, &CPU::res_6_f,  &CPU::res_6_e,&CPU::res_6_h, &CPU::res_6_l, &CPU::res_6_hl, &CPU::res_6_a,&CPU::res_7_b, &CPU::res_7_c, &CPU::res_7_f,  &CPU::res_7_e,&CPU::res_7_h, &CPU::res_7_l, &CPU::res_7_hl, &CPU::res_7_a,
			&CPU::set_0_b, &CPU::set_0_c, &CPU::set_0_f,  &CPU::set_0_e,&CPU::set_0_h, &CPU::set_0_l, &CPU::set_0_hl, &CPU::set_0_a,&CPU::set_1_b, &CPU::set_1_c, &CPU::set_1_f,  &CPU::set_1_e,&CPU::set_1_h, &CPU::set_1_l, &CPU::set_1_hl, &CPU::set_1_a,
			&CPU::set_2_b, &CPU::set_2_c, &CPU::set_2_f,  &CPU::set_2_e,&CPU::set_2_h, &CPU::set_2_l, &CPU::set_2_hl, &CPU::set_2_a,&CPU::set_3_b, &CPU::set_3_c, &CPU::set_3_f,  &CPU::set_3_e,&CPU::set_3_h, &CPU::set_3_l, &CPU::set_3_hl, &CPU::set_3_a,
			&CPU::set_4_b, &CPU::set_4_c, &CPU::set_4_f,  &CPU::set_4_e,&CPU::set_4_h, &CPU::set_4_l, &CPU::set_4_hl, &CPU::set_4_a,&CPU::set_5_b, &CPU::set_5_c, &CPU::set_5_f,  &CPU::set_5_e,&CPU::set_5_h, &CPU::set_5_l, &CPU::set_5_hl, &CPU::set_5_a,
			&CPU::set_6_b, &CPU::set_6_c, &CPU::set_6_f,  &CPU::set_6_e,&CPU::set_6_h, &CPU::set_6_l, &CPU::set_6_hl, &CPU::set_6_a,&CPU::set_7_b, &CPU::set_7_c, &CPU::set_7_f,  &CPU::set_7_e,&CPU::set_7_h, &CPU::set_7_l, &CPU::set_7_hl, &CPU::set_7_a
	};
}

void CPU::fetch() {
	instruction = ram.read(PC);
	PC++;

	// There are 2 types of instructions in the game boy: "regular" and 0xCB
	// offset instructions 0xCB offset instructions are executed as such: Start:
	// 0xCB 		 0xF6 0xCB is read, indicating that the next instruction
	// is from the 0xCB offset table not the regular opcode table note that 0xCB
	// can be an
	// instruction in and of itself ie: Start: 0xCB 		 0xCB is a valid
	// program

	// This is important because otherwise the emulator will enter an infinite
	// loop of setting the flag Hence, if instruction is 0xCB and flag is not set,
	// set flag and treat next instruction as normal

	if (instruction == 0xCB && flagCB == 0) {
		flagCB = 1;
		instruction = ram.read(PC);
		PC++;
	}
}

void CPU::execute() { // dont forget interrupts

	while(cycles>=0){	//if cylces is non-zero, previous instruction is still executing.
		if(cycles==0){	//hence, decrement by one and try loop until its 0 for the next insturction to be executed
						// use correct lookup table
			if (flagCB == 0) {
				(this->*opcodeLUT[instruction])();
				break;
			} 
			else {
				(this->*opcodeLUTCB[instruction])();
				flagCB=0;
			}
		}
		else{
			cycles--;
		}		
	}
}


void CPU::write(uint16_t addr, uint8_t data){
	ram.write(addr,data);

}
void CPU::read(uint16_t addr){
	printf("%X : %X\n", addr, ram.read(addr));
}

void CPU::getRegs(){
	printf("AF: %X\n",regs.AF);
	printf("BC: %X\n",regs.BC);
	printf("DE: %X\n",regs.DE);
	printf("HL: %X\n",regs.HL);

}

/******implementation of core instructions below******/

/*=============Table 1 below=============*/
int CPU::ld_reg_addr(uint8_t &reg1, uint8_t data) {
	cycles = 2;
	reg1 = data;
	return 0;
}

int CPU::ld_reg_reg(uint8_t &reg1, uint8_t reg2) {
	cycles = 1;
	reg1 = reg2;
	return 0;
}

int CPU::ld_reg_d8(uint8_t &reg) {
	cycles = 2;
	uint8_t imm = ram.read(PC);
	PC++;
	reg = imm;
	return 0;
}

int CPU::ld_reg_d16(uint16_t &reg1) {
	cycles = 3;
	uint16_t data = ram.read(PC);
	PC++;
	data |= ram.read(PC) << 8;
	PC++;
	reg1 = data;
	return 0;
}

int CPU::inc_reg(uint8_t &reg) {
	cycles = 1;
	regs.halfCarry = ((reg & 0x0F) == 0x0F);
	reg++;
	regs.zero = (reg == 0x00);
	regs.negative = 0;
	return 0;
}


int CPU::inc_reg(uint16_t &reg) {
	cycles = 2;
	reg++;
	return 0;
}

int CPU::dec_reg(uint8_t &reg) {
	cycles = 1;
	regs.halfCarry = ((reg & 0x0F) == 0x00);
	reg--;
	regs.zero = (reg == 0x00);
	regs.negative = 1;
	return 0;
}

int CPU::dec_reg(uint16_t &reg) {
	cycles = 2;
	regs.BC--;
	return 0;
}

int CPU::ld_mem_a(uint16_t &addr) {
	cycles = 2;
	ram.write(addr, regs.A);
	return 0;
}

int CPU::jr(bool condition) {
	if (condition) {
		cycles = 3;
		int8_t s8 = ram.read(PC);
		PC++;
		PC = PC + s8;
	} else {
		cycles = 2;
		PC++;
	}
	return 0;
}


int CPU::add(uint8_t &reg1, uint8_t &reg2) {
	cycles = 1;
	uint16_t result = reg1 + reg2;
	regs.halfCarry = (reg1 & 0x0F) + (reg2 & 0x0F);
	regs.carry = result > 0xff;
	regs.negative = 0;
	return 0;
}

int CPU::add(uint16_t &reg1, uint16_t &reg2) {
	cycles = 2;
	regs.negative = 0;
	uint32_t result = reg1 + reg2;
	regs.carry = result > 0xffff;
	regs.halfCarry = ((reg1 & 0x0fff) + (reg2 & 0x0fff) > 0x0fff);
	regs.negative = 0;
	return 0;
}

int CPU::adc(uint8_t &reg1, uint8_t &reg2) {

	int carry = (regs.carry == 1);
	uint16_t result = reg1 + reg2 + carry;
	regs.zero = (result == 0);
	regs.carry = (result > 0xff);
	regs.halfCarry = (((reg1 & 0x0F) + (reg2 & 0x0F) + carry) > 0x0F);
	regs.negative = 0;
	reg1 = (int8_t)result & 0xff;
	return 0;
}
int CPU::sub(uint8_t &reg1, uint8_t &reg2) {
	uint16_t result = reg1 - reg2;
	regs.carry = (reg2 > reg1);
	regs.halfCarry = ((reg2 & 0x0f) > (reg1 & 0x0f));
	reg1 = reg1 - reg2;
	regs.zero = (reg1 == 0);
	regs.negative = 1;
	return 0;
}
int CPU::sub(uint16_t &reg1, uint16_t &reg2) {return 0;}
int CPU::sbc(uint16_t &reg1, uint16_t &reg2) {return 0;}
int CPU::sbc(uint8_t &reg1, uint8_t &reg2) {
	int carry = regs.carry;

	regs.carry = ((reg2 + carry) > reg1);
	regs.halfCarry = (((reg2 & 0x0F) + carry) > (reg1 & 0x0F));

	reg1 = reg1 - carry - reg2;
	regs.zero = (reg1 == 0);
	regs.negative = 1;
	return 0;
}
int CPU::_and(
		uint16_t &reg1,
		uint16_t &reg2) { // many of these functions cam be written as templates

	reg1 = reg1 & reg2;
	regs.carry = 0;
	regs.halfCarry = 1;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_and(uint8_t &reg1, uint8_t &reg2) {
	reg1 = reg1 & reg2;
	regs.carry = 0;
	regs.halfCarry = 1;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_xor(uint16_t &reg1, uint16_t &reg2) {
	reg1 ^= reg2;
	regs.carry = 0;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_xor(uint8_t &reg1, uint8_t &reg2) {
	reg1 ^= reg2;
	regs.carry = 0;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_or(uint16_t &reg1, uint16_t &reg2) {
	reg1 |= reg2;
	regs.carry = 0;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_or(uint8_t &reg1, uint8_t &reg2) {
	reg1 |= reg2;
	regs.carry = 0;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::cp(uint16_t &reg1, uint16_t &reg2) {

	regs.negative = 1;
	regs.zero = (reg1 == reg2);
	regs.carry = (reg2 > reg1);
	regs.halfCarry = ((reg2 & 0x0FF) > (reg1 & 0x0FF));
	return 0;
}
int CPU::cp(
		uint8_t &reg1,
		uint8_t &reg2) { // doesnt actually store result of subtraction anywhere

	regs.negative = 1;
	regs.zero = (reg1 == reg2);
	regs.carry = (reg2 > reg1);
	regs.halfCarry = ((reg2 & 0x0F) > (reg1 & 0x0F));
	return 0;
}
int CPU::pop(uint16_t &reg1) {
	cycles=3;
	reg1 = ram.read(SP);
	SP++;
	reg1 |= ram.read(SP) << 8;
	SP++;
	return 0;
}

int CPU::push(uint16_t &reg1) {
	SP--;
	ram.write(SP, (reg1 & 0xFF00) >> 8);
	SP--;
	ram.write(SP, (reg1 & 0x00FF) >> 8);
	return 0;
}
int CPU::retZC(bool condition) {
	if (condition) {
		cycles = 5;
		uint16_t _PC = 0x00;
		_PC = ram.read(SP);
		SP++;
		_PC |= ram.read(SP) << 8;
		SP++;
		PC = _PC;
	} else {
		cycles = 2;
	}
	return 0;
}

int CPU::rst(int index) {
	cycles = 4;
	SP--;
	ram.write(SP, (PC & 0xFF00) >> 8);
	SP--;
	ram.write(SP, PC & 0x00FF);
	PC = ram.read(0x00) << 8;
	PC |= ram.read(0x8 * index);
	return 0;
}

int CPU::_ret(bool condition) {

	if (condition) {
		cycles = 5;
		PC = ram.read(SP);
		SP++;
		PC |= ram.read(SP) << 8;
		SP++;
	} else {
		cycles = 2;
	}
	return 0;
}

int CPU::jp_a16(bool condition){

	if(condition){
		cycles=4;
		uint16_t a16=ram.read(PC);
		PC++;
		a16|=ram.read(PC)<<8;
		PC++;
		PC=a16;

	}
	else{
		cycles=3;
		PC++;
		PC++;
	}

	return 0;
}


int CPU::call(bool condition){	
	if(condition){
		cycles=6;
		//get call address
		int _PC=ram.read(PC);
		PC++;
		_PC |= ram.read(PC) << 8;
		PC++;

		//push pc to stack then jump
		ram.write(SP,(PC&0xFF00)>>8);
		SP--;
		ram.write(SP,(PC&0x00FF));
		SP--;
		PC=_PC;

	}
	else{
		cycles=3;
	}
	return 0;
}


/*=============Table 1 above=============*/
///////////////////////////////////////////
/*=============Table 2 below=============*/

int CPU::rlc(){}
int CPU::rrc(){}
int CPU::rl(){}
int CPU::rr(){}

int CPU::sla(){}
int CPU::sra(){}

int CPU::swap(){}
int CPU::srl(){}

int CPU::bit(){
	
	return 0;
}
int CPU::res(uint8_t & reg, int pos){	//reset bit at offset
	int offsetBin=1<<pos;
	offsetBin=~offsetBin;
	reg=reg&offsetBin;	//and reg with the inverse of its "offset"
	return 0;
}
int CPU::set(uint8_t & reg, int pos){ //set bit at offset
	int offsetBin=1<<pos;
	reg=reg|offsetBin;	//and reg with the inverse of its "offset"
	return 0;
}


/*=============Table 2 above=============*/
/******implementation of core instructions above******/

int CPU::nop() { // 0x00
	cycles = 1;
	return 0;
}
int CPU::invalid() { // 0x00
	cycles = 1;
	return 0;
}

int CPU::ld_bc_d16() { // 0x01
	ld_reg_d16(regs.BC);
	return 0;
}

int CPU::ld_bc_a() { // 0x02
	ld_mem_a(regs.BC);
	return 0;
}

int CPU::inc_bc() { // 0x03
	inc_reg(regs.BC);
	return 0;
}

int CPU::inc_b() { // 0x04
	inc_reg(regs.B);
	return 0;
}

int CPU::dec_b() { // 0x05
	dec_reg(regs.B);
	return 0;
}

int CPU::ld_b_d8() { // 0x06
	ld_reg_d8(regs.B);
	return 0;
}

int CPU::rlca() { // 0x07
	cycles = 1;
	regs.zero = 0;
	regs.negative = 0;
	regs.halfCarry = 0;

	regs.carry = (regs.A & 0x80) >> 8;
	regs.A = regs.A << 1;
	regs.A |= regs.carry; // to rotate, use the same value that was stored in the
						  // carry bit
	return 0;
}

int CPU::ld_a16_sp() { // 0x08
	int cycles = 5;
	uint16_t a16 = ram.read(PC);
	PC++;
	a16 |= ram.read(PC) << 8;
	PC++;
	ram.write(a16, SP & 0x00FF);
	ram.write(a16 + 1, (SP & 0xFF00) >> 8);
	return 0;
}

int CPU::add_hl_bc() { // 0x09
	add(regs.HL, regs.BC);
	return 0;
}

int CPU::ld_a_bc() { // 0x0A
	ld_reg_addr(regs.A, ram.read(regs.BC));
	return 0;
}

int CPU::dec_bc() { // 0x0B
	dec_reg(regs.BC);
	return 0;
}

int CPU::inc_c() { // 0x0C
	inc_reg(regs.C);
	return 0;
}

int CPU::dec_c() { // 0x0D
	dec_reg(regs.C);
	return 0;
}

int CPU::ld_c_d8() { // 0x0E
	ld_reg_d8(regs.C);
	return 0;
}

int CPU::rrca() { // 0x0F
	cycles = 1;
	regs.zero = 0;
	regs.negative = 0;
	regs.halfCarry = 0;
	regs.carry = regs.A & 0x01;
	regs.A = regs.A >> 1;
	regs.A |= regs.carry;
	return 0;
}

int CPU::stop() { // 0x10
	return 0;
}

int CPU::ld_de_d16() { // 0x11
	ld_reg_d16(regs.DE);
	return 0;
}

int CPU::ld_de_a() { // 0x12
	ld_mem_a(regs.DE);
	return 0;
}

int CPU::inc_de() { // 0x13
	inc_reg(regs.DE);
	return 0;
}

int CPU::inc_d() { // 0x14
	inc_reg(regs.D);
	return 0;
}

int CPU::dec_d() { // 0x15
	dec_reg(regs.B);
	return 0;
}

int CPU::ld_d_d8() { // 0x16
	ld_reg_d8(regs.D);
	return 0;
}

int CPU::rla() { // 0x17
				 // possible error
	regs.zero = 0;
	regs.negative = 0;
	regs.halfCarry = 0;

	int MSB = (regs.A & 0x80) >> 8;
	int carry = regs.carry;
	regs.carry = MSB;
	regs.A = regs.A << 1;
	regs.A |= carry;
	return 0;
}

int CPU::jr_s8() { // 0x18
				   // possible error here (signed or unsigned number?)
	jr(1);
	return 0;
}

int CPU::add_hl_de() { // 0x19
	add(regs.HL, regs.DE);
	return 0;
}

int CPU::ld_a_de() { // 0x1A
	ld_reg_addr(regs.A, ram.read(regs.DE));
	return 0;
}

int CPU::dec_de() { // 0x1B
	dec_reg(regs.DE);
	return 0;
}

int CPU::inc_e() { // 0x1C
	inc_reg(regs.E);
	return 0;
}

int CPU::dec_e() { // 0x1D
	dec_reg(regs.C);
	return 0;
}

int CPU::ld_e_d8() { // 0x1E
	ld_reg_d8(regs.C);
	return 0;
}

int CPU::rra() { // 0x1F
	int temp = regs.A & 0x01;
	regs.A = regs.A >> 1;
	regs.A |= regs.carry << 8;
	regs.carry = temp;
	return 0;
}

int CPU::jr_nz_s8() { // 0x20
	jr(regs.zero != 1);
	return 0;
}

int CPU::ld_hl_d16() { // 0x21
	ld_reg_d16(regs.HL);
	return 0;
}

int CPU::ld_hlp_a() { // 0x22
	ld_mem_a(regs.HL);
	regs.HL++;
	return 0;
}

int CPU::inc_hl() { // 0x23
	inc_reg(regs.HL);
	return 0;
}

int CPU::inc_h() { // 0x24
	inc_reg(regs.H);
	return 0;
}

int CPU::dec_h() { // 0x25
	dec_reg(regs.B);
	return 0;
}

int CPU::ld_h_d8() { // 0x26
	ld_reg_d8(regs.H);
	return 0;
}

int CPU::daa() { // 0x27
	return 0;
}

int CPU::jr_z_s8() { // 0x28
	jr(regs.zero == 1);
	return 0;
}

int CPU::add_hl_hl() { // 0x29
	add(regs.HL, regs.HL);
	return 0;
}

int CPU::ld_a_hlp() { // 0x2A
	ld_reg_addr(regs.A, ram.read(regs.HL));
	regs.HL++;
	return 0;
}

int CPU::dec_hl() { // 0x2B
	dec_reg(regs.HL);
	return 0;
}

int CPU::inc_l() { // 0x2C
	inc_reg(regs.L);
	return 0;
}

int CPU::dec_l() { // 0x2d
	dec_reg(regs.L);
	return 0;
}

int CPU::ld_l_d8() { // 0x2E
	ld_reg_d8(regs.L);
	return 0;
}

int CPU::cpl() { // 0x2F
	cycles = 1;
	regs.A = ~regs.A;
	regs.negative = 1;
	regs.halfCarry = 1;
	return 0;
}

int CPU::jr_nc_s8() { // 0x30

	jr(regs.carry != 1);
	return 0;
}

int CPU::ld_sp_d16() { // 0x31
	ld_reg_d16(SP);
	return 0;
}

int CPU::ld_hlm_a() { // 0x32
	ld_mem_a(regs.HL);
	regs.HL--;
	return 0;
}

int CPU::inc_sp() { // 0x33
	inc_reg(SP);
	return 0;
}

int CPU::inc_hl_mem() { // 0x34
	uint8_t contents = ram.read(regs.HL);
	cycles = 1;
	regs.halfCarry = ((contents & 0x0F) == 0x0F);
	regs.negative = 0;
	regs.zero = (contents == 0xFF);
	ram.write(regs.HL, contents++);
	return 0;
}

int CPU::dec_hl_mem() { // 0x35
	uint8_t contents = ram.read(regs.HL);
	regs.halfCarry = ((contents & 0x0F) == 0x00);
	regs.negative = 1;
	regs.zero = (contents == 0x01);
	ram.write(regs.HL, contents--);
	return 0;
}

int CPU::ld_hl_d8() { // 0x36
	cycles = 2;
	uint8_t imm = ram.read(PC);
	PC++;
	ram.write(regs.HL, imm);
	return 0;
}

int CPU::scf() { // 0x37
	cycles = 1;

	regs.carry = 1;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = 0;
	return 0;
}

int CPU::jr_c_s8() { // 0x38

	jr(regs.carry == 1);
	return 0;
}

int CPU::add_hl_sp() { // 0x39
	add(regs.HL, SP);
	return 0;
}

int CPU::ld_a_hlm() { // 0x3A
	ld_reg_addr(regs.A, ram.read(regs.HL));
	regs.HL--;
	return 0;
}

int CPU::dec_sp() { // 0x3B
	dec_reg(regs.BC);
	return 0;
}

int CPU::inc_a() { // 0x3C
	inc_reg(regs.A);
	return 0;
}

int CPU::dec_a() { // 0x3D
	dec_reg(regs.C);
	return 0;
}

int CPU::ld_a_d8() { // 0x3E
	ld_reg_d8(regs.A);
	return 0;
}

int CPU::ccf() { // 0x3F
	cycles = 1;
	regs.negative = 0;
	regs.halfCarry = 0;
	regs.carry = ~regs.carry;
	return 0;
}

int CPU::ld_b_b() { // 0x40
	ld_reg_reg(regs.B, regs.B);
	return 0;
}
int CPU::ld_b_c() { // 0x41
	ld_reg_reg(regs.B, regs.C);
	return 0;
}
int CPU::ld_b_d() { // 0x42
	ld_reg_reg(regs.B, regs.D);
	return 0;
}
int CPU::ld_b_e() { // 0x43
	ld_reg_reg(regs.B, regs.E);
	return 0;
}
int CPU::ld_b_h() { // 0x44
	ld_reg_reg(regs.B, regs.H);
	return 0;
}
int CPU::ld_b_l() { // 0x45
	ld_reg_reg(regs.B, regs.L);
	return 0;
}
int CPU::ld_b_hl() { // 0x46
	cycles = 2;
	uint8_t operand = ram.read(regs.HL);
	regs.B = operand;
	return 0;
}
int CPU::ld_b_a() { // 0x47
	ld_reg_reg(regs.B, regs.A);
	return 0;
}
int CPU::ld_c_b() { // 0x48
	ld_reg_reg(regs.C, regs.B);
	return 0;
}
int CPU::ld_c_c() { // 0x49
	ld_reg_reg(regs.C, regs.C);
	return 0;
}
int CPU::ld_c_d() { // 0x4A
	ld_reg_reg(regs.C, regs.D);
	return 0;
}
int CPU::ld_c_e() { // 0x4B
	ld_reg_reg(regs.C, regs.E);
	return 0;
}
int CPU::ld_c_h() { // 0x4C
	ld_reg_reg(regs.C, regs.H);
	return 0;
}
int CPU::ld_c_l() { // 0x4D
	ld_reg_reg(regs.C, regs.L);
	return 0;
}
int CPU::ld_c_hl() { // 0x4E
	cycles = 2;
	uint8_t operand = ram.read(regs.HL);
	regs.C = operand;
	return 0;
}
int CPU::ld_c_a() { // 0x4F
	ld_reg_reg(regs.C, regs.A);
	return 0;
}

/*0x50 instructions*/
int CPU::ld_d_b() { // 0x50
	ld_reg_reg(regs.D, regs.B);
	return 0;
}
int CPU::ld_d_c() { // 0x51
	ld_reg_reg(regs.D, regs.C);
	return 0;
}
int CPU::ld_d_d() { // 0x52
	ld_reg_reg(regs.D, regs.D);
	return 0;
}
int CPU::ld_d_e() { // 0x53
	ld_reg_reg(regs.D, regs.E);
	return 0;
}
int CPU::ld_d_h() { // 0x54
	ld_reg_reg(regs.D, regs.H);
	return 0;
}
int CPU::ld_d_l() { // 0x55
	ld_reg_reg(regs.D, regs.L);
	return 0;
}
int CPU::ld_d_hl() { // 0x56
	cycles = 2;
	uint8_t operand = ram.read(regs.HL);
	regs.D = operand;
	return 0;
}
int CPU::ld_d_a() { // 0x57
	ld_reg_reg(regs.D, regs.A);
	return 0;
}
int CPU::ld_e_b() { // 0x58
	ld_reg_reg(regs.E, regs.B);
	return 0;
}
int CPU::ld_e_c() { // 0x59
	ld_reg_reg(regs.E, regs.C);
	return 0;
}
int CPU::ld_e_d() { // 0x5A
	ld_reg_reg(regs.E, regs.D);
	return 0;
}
int CPU::ld_e_e() { // 0x5B
	ld_reg_reg(regs.E, regs.E);
	return 0;
}
int CPU::ld_e_h() { // 0x5C
	ld_reg_reg(regs.E, regs.H);
	return 0;
}
int CPU::ld_e_l() { // 0x5D
	ld_reg_reg(regs.E, regs.L);
	return 0;
}
int CPU::ld_e_hl() { // 0x5E
	cycles = 2;
	uint8_t operand = ram.read(regs.HL);
	regs.E = operand;
	return 0;
}
int CPU::ld_e_a() { // 0x5F
	ld_reg_reg(regs.E, regs.A);
	return 0;
}

/*0x60 instructions*/
int CPU::ld_h_b() { // 0x60
	ld_reg_reg(regs.H, regs.B);
	return 0;
}
int CPU::ld_h_c() { // 0x61
	ld_reg_reg(regs.H, regs.C);
	return 0;
}
int CPU::ld_h_d() { // 0x62
	ld_reg_reg(regs.H, regs.D);
	return 0;
}
int CPU::ld_h_e() { // 0x63
	ld_reg_reg(regs.H, regs.E);
	return 0;
}
int CPU::ld_h_h() { // 0x64
	ld_reg_reg(regs.H, regs.H);
	return 0;
}
int CPU::ld_h_l() { // 0x65
	ld_reg_reg(regs.H, regs.L);
	return 0;
}
int CPU::ld_h_hl() { // 0x66
	cycles = 2;
	uint8_t operand = ram.read(regs.HL);
	regs.H = operand;
	return 0;
}
int CPU::ld_h_a() { // 0x67
	ld_reg_reg(regs.H, regs.A);
	return 0;
}
int CPU::ld_l_b() { // 0x68
	ld_reg_reg(regs.L, regs.B);
	return 0;
}
int CPU::ld_l_c() { // 0x69
	ld_reg_reg(regs.L, regs.C);
	return 0;
}
int CPU::ld_l_d() { // 0x6A
	ld_reg_reg(regs.L, regs.D);
	return 0;
}
int CPU::ld_l_e() { // 0x6B
	ld_reg_reg(regs.L, regs.E);
	return 0;
}
int CPU::ld_l_h() { // 0x6C
	ld_reg_reg(regs.L, regs.H);
	return 0;
}
int CPU::ld_l_l() { // 0x6D
	ld_reg_reg(regs.L, regs.L);
	return 0;
}
int CPU::ld_l_hl() { // 0x6E
	cycles = 2;
	uint8_t operand = ram.read(regs.HL);
	regs.L = operand;
	return 0;
}
int CPU::ld_l_a() { // 0x6F
	ld_reg_reg(regs.L, regs.A);
	return 0;
}

/*0x70 instructions*/
int CPU::ld_hl_b() {
	cycles = 2;
	ram.write(regs.HL, regs.B);
	return 0;
}
int CPU::ld_hl_c() {
	cycles = 2;
	ram.write(regs.HL, regs.C);
	return 0;
}
int CPU::ld_hl_d() {
	cycles = 2;
	ram.write(regs.HL, regs.D);
	return 0;
}
int CPU::ld_hl_e() {
	cycles = 2;
	ram.write(regs.HL, regs.E);
	return 0;
}
int CPU::ld_hl_h() {
	cycles = 2;
	ram.write(regs.HL, regs.H);
	return 0;
}
int CPU::ld_hl_l() {
	cycles = 2;
	ram.write(regs.HL, regs.L);
	return 0;
}
int CPU::HALT() {return 0;}
int CPU::ld_hl_a() {
	cycles = 2;
	ram.write(regs.HL, regs.A);
	return 0;
}
int CPU::ld_a_b() {
	ld_reg_reg(regs.A, regs.B);
	return 0;
}
int CPU::ld_a_c() {
	ld_reg_reg(regs.A, regs.C);
	return 0;
}
int CPU::ld_a_d() {
	ld_reg_reg(regs.A, regs.D);
	return 0;
}
int CPU::ld_a_e() {
	ld_reg_reg(regs.A, regs.E);
	return 0;
}
int CPU::ld_a_h() {
	ld_reg_reg(regs.A, regs.H);
	return 0;
}
int CPU::ld_a_l() {
	ld_reg_reg(regs.A, regs.L);
	return 0;
}
int CPU::ld_a_hl() {
	cycles = 2;
	uint8_t operand = ram.read(regs.HL);
	regs.A = operand;
	return 0;
}
int CPU::ld_a_a() {
	ld_reg_reg(regs.A, regs.A);
	return 0;
};

/*0x80 instructions*/
int CPU::add_a_b() { // 0x80
	add(regs.A, regs.B);
	return 0;
}
int CPU::add_a_c() { // 0x81
	add(regs.A, regs.C);
	return 0;
}
int CPU::add_a_d() { // 0x82
	add(regs.A, regs.D);
	return 0;
}
int CPU::add_a_e() { // 0x83
	add(regs.A, regs.E);
	return 0;
}
int CPU::add_a_h() { // 0x84
	add(regs.A, regs.H);
	return 0;
}
int CPU::add_a_l() { // 0x85
	add(regs.A, regs.L);
	return 0;
}
int CPU::add_a_hl() { // 0x86
	cycles = 1;
	uint16_t result = regs.A + ram.read(regs.HL);
	regs.halfCarry = (regs.A & 0x0F) + (ram.read(regs.HL) & 0x0F);
	regs.carry = result > 0xff;
	regs.negative = 0;
	return 0;
}
int CPU::add_a_a() { // 0x87
	add(regs.A, regs.A);
	return 0;
}
int CPU::adc_a_b() { // 0x88
	adc(regs.A, regs.A);
	return 0;
}
int CPU::adc_a_c() { // 0x89
	adc(regs.A, regs.C);
	return 0;
}
int CPU::adc_a_d() { // 0x8A
	adc(regs.A, regs.D);
	return 0;
}
int CPU::adc_a_e() { // 0x8B
	adc(regs.A, regs.E);
	return 0;
}
int CPU::adc_a_h() { // 0x8C
	adc(regs.A, regs.H);
	return 0;
}
int CPU::adc_a_l() { // 0x8D
	adc(regs.A, regs.L);
	return 0;
}
int CPU::adc_a_hl() { // 0x8E
	return 0;
}
int CPU::adc_a_a() { // 0x8F
	adc(regs.A, regs.A);
	return 0;
}

/*0x90 instructions*/
int CPU::sub_b() { // 0x90
	sub(regs.A, regs.B);
	return 0;
}
int CPU::sub_c() { // 0x91
	sub(regs.A, regs.C);
	return 0;
}
int CPU::sub_d() { // 0x92
	sub(regs.A, regs.D);
	return 0;
}
int CPU::sub_e() { // 0x93
	sub(regs.A, regs.E);
	return 0;
}
int CPU::sub_h() { // 0x94
	sub(regs.A, regs.H);
	return 0;
}
int CPU::sub_l() { // 0x95
	sub(regs.A, regs.L);
	return 0;
}
int CPU::sub_hl() { // 0x96
	return 0;
}
int CPU::sub_a() { // 0x97
	sub(regs.A, regs.A);
	return 0;
}
int CPU::sbc_a_b() { // 0x98
	sbc(regs.A, regs.B);
	return 0;
}
int CPU::sbc_a_c() { // 0x99
	sbc(regs.A, regs.C);
	return 0;
}
int CPU::sbc_a_d() { // 0x9A
	sbc(regs.A, regs.D);
	return 0;
}
int CPU::sbc_a_e() { // 0x9B
	sbc(regs.A, regs.E);
	return 0;
}
int CPU::sbc_a_h() { // 0x9C
	sbc(regs.A, regs.H);
	return 0;
}
int CPU::sbc_a_l() { // 0x9D
	sbc(regs.A, regs.L);
	return 0;
}
int CPU::sbc_a_hl() { // 0x9E
	return 0;
}
int CPU::sbc_a_a() { // 0x9F
	sbc(regs.A, regs.A);
	return 0;
}

/*0xA0 instructions*/
int CPU::and_b() { // 0xA0
	_and(regs.A, regs.B);
	return 0;
}
int CPU::and_c() { // 0xA1
	_and(regs.A, regs.C);
	return 0;
}
int CPU::and_d() { // 0xA2
	_and(regs.A, regs.D);
	return 0;
}
int CPU::and_e() { // 0xA3
	_and(regs.A, regs.E);
	return 0;
}
int CPU::and_h() { // 0xA4
	_and(regs.A, regs.H);
	return 0;
}
int CPU::and_l() { // 0xA5
	_and(regs.A, regs.L);
	return 0;
}
int CPU::and_hl() { // 0xA6
	return 0;
}
int CPU::and_a() { // 0xA7
	_and(regs.A, regs.A);
	return 0;
}
int CPU::xor_b() { // 0xA8
	_xor(regs.A, regs.B);
	return 0;
}
int CPU::xor_c() { // 0xA9
	_xor(regs.A, regs.B);
	return 0;
}
int CPU::xor_d() { // 0xAA
	_xor(regs.A, regs.B);
	return 0;
}
int CPU::xor_e() { // 0xAB
	_xor(regs.A, regs.B);
	return 0;
}
int CPU::xor_h() { // 0xAC
	_xor(regs.A, regs.B);
	return 0;
}
int CPU::xor_l() { // 0xAD
	_xor(regs.A, regs.B);
	return 0;
}
int CPU::xor_hl() { // 0xAE
	_xor(regs.A, regs.B);
	return 0;
}
int CPU::xor_a() { // 0xAF
	_xor(regs.A, regs.B);
	return 0;
}

/*0xB0 instructions*/
int CPU::or_b() { // 0xB0
	_or(regs.A, regs.B);
	return 0;
}
int CPU::or_c() { // 0xB1
	_or(regs.A, regs.C);
	return 0;
}
int CPU::or_d() { // 0xB2
	_or(regs.A, regs.D);
	return 0;
}
int CPU::or_e() { // 0xB3
	_or(regs.A, regs.E);
	return 0;
}
int CPU::or_h() { // 0xB0
	_or(regs.A, regs.H);
	return 0;
}
int CPU::or_l() { // 0xB0
	_or(regs.A, regs.L);
	return 0;
}
int CPU::or_hl() { // 0xB0
	return 0;
}
int CPU::or_a() { // 0xB0
	_or(regs.A, regs.A);
	return 0;
}
int CPU::cp_b() { // 0xB0
	cp(regs.A, regs.B);
	return 0;
}
int CPU::cp_c() { // 0xB0
	cp(regs.A, regs.C);
	return 0;
}
int CPU::cp_d() { // 0xB0
	cp(regs.A, regs.D);
	return 0;
}
int CPU::cp_e() { // 0xB0
	cp(regs.A, regs.E);
	return 0;
}
int CPU::cp_h() { // 0xB0
	cp(regs.A, regs.H);
	return 0;
}
int CPU::cp_l() { // 0xB0
	cp(regs.A, regs.L);
	return 0;
}
int CPU::cp_hl() { // 0xB0
	return 0;
}
int CPU::cp_a() { // 0xB0
	cp(regs.A, regs.A);
	return 0;
}

/*0xC0*/
int CPU::ret_nz() {
	retZC(regs.zero == 0);
	return 0;
}
int CPU::pop_bc() {
	pop(regs.BC);
	return 0;
}
int CPU::jp_nz_a16() {
	jp_a16(regs.zero==0);
	return 0;
}
int CPU::jp_a16() {
	jp_a16(1);
	return 0;
}

int CPU::call_nz_a16() {
	call(regs.zero!=0);
	return 0;
}
int CPU::push_bc() {
	push(regs.BC);
	return 0;
}
int CPU::add_a_d8() {return 0;}
int CPU::rst_0() {
	rst(0);
	return 0;
}
int CPU::ret_z() {
	_ret(regs.zero == 1);
	return 1;
}
int CPU::ret() {
	_ret(1);
	return 1;
}
int CPU::jp_z_a16() {
	jp_a16(regs.zero==1);
	return 0;
}
int CPU::call_z_a16() {
	call(regs.zero==0);
	return 0;
}
int CPU::call_a16() {	
	call(1);
	return 0;
}
int CPU::adc_a_d8() {return 0;}
int CPU::rst_1() {
	rst(1);
	return 0;
}

/*0xD0*/
int CPU::ret_nc() { // 0xD0
	retZC(regs.carry == 0);
	return 0;
}
int CPU::pop_de() { // 0xD1
	pop(regs.AF);
	return 0;
}
int CPU::jp_nc_a16() { // 0xD2
	jp_a16(regs.carry==0);
	return 0;
}
int CPU::call_nc_a16() { // 0xD4
	call(regs.carry!=0);
	return 0;
}
int CPU::push_de() { // 0xD4
	push(regs.DE);
	return 0;
}
int CPU::sub_d8() { // 0xD5
	return 0;
}
int CPU::rst_2() { // 0xD6
	rst(2);
	return 0;
}
int CPU::ret_c() { // 0xD7
	_ret(regs.carry == 1);
	return 1;
}
int CPU::reti() { // 0xD8
	return 0;
}
int CPU::jp_c_a16() { // 0xDA
	return 0;
}
int CPU::call_c_a16() { // 0xDC
	call(regs.carry==0);
	return 0;

}
int CPU::sbc_a_d8() { // 0xDE
	return 0;
}
int CPU::rst_3() { // 0xDF
	rst(3);
	return 0;
}

/*0xE0*/
int CPU::ld_a8_a() {	//0xE0
	cycles=3;
	uint8_t a8=ram.read(PC);
	uint16_t addr=(a8|0xFF00);
	PC++;
	ram.write(addr,regs.A);
	return 0;
}
int CPU::pop_hl() {	//0xE1
	pop(regs.HL);
	return 0;
}
int CPU::ld_c_mem_a() {	//0xE2
	cycles=2;
	uint16_t addr=(regs.C|0xFF00);
	PC++;
	ld_mem_a(addr);
	return 0;
}
int CPU::push_hl() {	//0xE3
	push(regs.HL);
	return 0;
}
int CPU::and_d8() {return 0;}
int CPU::rst_4() {
	rst(4);
	return 0;
}
int CPU::add_sp_s8() {return 0;}
int CPU::jp_hl() {
	cycles=1;
	PC=regs.HL;
	return 0;
}
int CPU::ld_a16_a() {return 0;}
int CPU::xor_d8() {return 0;}
int CPU::rst_5() {
	rst(5);
	return 0;
}

/*0xF0*/
int CPU::ld_a_a8() {	//0xF0
	cycles=3;
	uint16_t addr=ram.read(PC);
	PC++;
	addr=addr|0xFF00;
	regs.A=ram.read(addr);
	return 0;
}
int CPU::pop_af() {	//0xF1
	pop(regs.AF);
	return 0;
}
int CPU::ld_a_c_mem() {	//0xF2
	cycles=2;
	uint16_t addr=regs.C|0xFF00;
	regs.A=ram.read(addr);
	return 0;
}
int CPU::di() {return 0;}
int CPU::push_af() {
	push(regs.AF);
	return 0;
}
int CPU::or_d8() {return 0;}
int CPU::rst_6() {
	rst(6);
	return 0;
}
int CPU::ld_hl_sp_s8() {return 0;}
int CPU::ld_sp_hl() {return 0;}
int CPU::ld_a_a16() {return 0;}
int CPU::ei() {return 0;}
int CPU::cp_d8() {return 0;}
int CPU::rst_7() {
	rst(7);
	return 0;
}

/***********16 BIT INSTRUCTIONS BELOW******************/
/*0x00*/
int CPU::rlc_b() {return 0;}
int CPU::rlc_c() {return 0;}
int CPU::rlc_d() {return 0;}
int CPU::rlc_e() {return 0;}
int CPU::rlc_h() {return 0;}
int CPU::rlc_l() {return 0;}
int CPU::rlc_hl() {return 0;}
int CPU::rlc_a() {return 0;}
int CPU::rrc_b() {return 0;}
int CPU::rrc_c() {return 0;}
int CPU::rrc_d() {return 0;}
int CPU::rrc_e() {return 0;}
int CPU::rrc_h() {return 0;}
int CPU::rrc_l() {return 0;}
int CPU::rrc_hl() {return 0;}
int CPU::rrc_a() {return 0;}

/*0x01*/
int CPU::rl_b() {return 0;}
int CPU::rl_c() {return 0;}
int CPU::rl_d() {return 0;}
int CPU::rl_e() {return 0;}
int CPU::rl_h() {return 0;}
int CPU::rl_l() {return 0;}
int CPU::rl_hl() {return 0;}
int CPU::rl_a() {return 0;}
int CPU::rr_b() {return 0;}
int CPU::rr_c() {return 0;}
int CPU::rr_d() {return 0;}
int CPU::rr_e() {return 0;}
int CPU::rr_h() {return 0;}
int CPU::rr_l() {return 0;}
int CPU::rr_hl() {return 0;}
int CPU::rr_a() {return 0;}

/*0x02*/
int CPU::sla_b() {return 0;}
int CPU::sla_c() {return 0;}
int CPU::sla_d() {return 0;}
int CPU::sla_e() {return 0;}
int CPU::sla_h() {return 0;}
int CPU::sla_l() {return 0;}
int CPU::sla_hl() {return 0;}
int CPU::sla_a() {return 0;}
int CPU::sra_b() {return 0;}
int CPU::sra_c() {return 0;}
int CPU::sra_d() {return 0;}
int CPU::sra_e() {return 0;}
int CPU::sra_h() {return 0;}
int CPU::sra_l() {return 0;}
int CPU::sra_hl() {return 0;}
int CPU::sra_a() {return 0;}

/*0x03*/
int CPU::swap_b() {return 0;}
int CPU::swap_c() {return 0;}
int CPU::swap_d() {return 0;}
int CPU::swap_e() {return 0;}
int CPU::swap_h() {return 0;}
int CPU::swap_l() {return 0;}
int CPU::swap_hl() {return 0;}
int CPU::swap_a() {return 0;}
int CPU::srl_b() {return 0;}
int CPU::srl_c() {return 0;}
int CPU::srl_d() {return 0;}
int CPU::srl_e() {return 0;}
int CPU::srl_h() {return 0;}
int CPU::srl_l() {return 0;}
int CPU::srl_hl() {return 0;}
int CPU::srl_a() {return 0;}

/*0x04*/
int CPU::bit_0_b() {return 0;}
int CPU::bit_0_c() {return 0;}
int CPU::bit_0_f() {return 0;}
int CPU::bit_0_e() {return 0;}
int CPU::bit_0_h() {return 0;}
int CPU::bit_0_l() {return 0;}
int CPU::bit_0_hl() {return 0;}
int CPU::bit_0_a() {return 0;}
int CPU::bit_1_b() {return 0;}
int CPU::bit_1_c() {return 0;}
int CPU::bit_1_f() {return 0;}
int CPU::bit_1_e() {return 0;}
int CPU::bit_1_h() {return 0;}
int CPU::bit_1_l() {return 0;}
int CPU::bit_1_hl() {return 0;}
int CPU::bit_1_a() {return 0;}

/*0x05*/
int CPU::bit_2_b() {return 0;}
int CPU::bit_2_c() {return 0;}
int CPU::bit_2_f() {return 0;}
int CPU::bit_2_e() {return 0;}
int CPU::bit_2_h() {return 0;}
int CPU::bit_2_l() {return 0;}
int CPU::bit_2_hl() {return 0;}
int CPU::bit_2_a() {return 0;}
int CPU::bit_3_b() {return 0;}
int CPU::bit_3_c() {return 0;}
int CPU::bit_3_f() {return 0;}
int CPU::bit_3_e() {return 0;}
int CPU::bit_3_h() {return 0;}
int CPU::bit_3_l() {return 0;}
int CPU::bit_3_hl() {return 0;}
int CPU::bit_3_a() {return 0;}

/*0x06*/
int CPU::bit_4_b() {return 0;}
int CPU::bit_4_c() {return 0;}
int CPU::bit_4_f() {return 0;}
int CPU::bit_4_e() {return 0;}
int CPU::bit_4_h() {return 0;}
int CPU::bit_4_l() {return 0;}
int CPU::bit_4_hl() {return 0;}
int CPU::bit_4_a() {return 0;}
int CPU::bit_5_b() {return 0;}
int CPU::bit_5_c() {return 0;}
int CPU::bit_5_f() {return 0;}
int CPU::bit_5_e() {return 0;}
int CPU::bit_5_h() {return 0;}
int CPU::bit_5_l() {return 0;}
int CPU::bit_5_hl() {return 0;}
int CPU::bit_5_a() {return 0;}

/*0x07*/
int CPU::bit_6_b() {return 0;}
int CPU::bit_6_c() {return 0;}
int CPU::bit_6_f() {return 0;}
int CPU::bit_6_e() {return 0;}
int CPU::bit_6_h() {return 0;}
int CPU::bit_6_l() {return 0;}
int CPU::bit_6_hl() {return 0;}
int CPU::bit_6_a() {return 0;}
int CPU::bit_7_b() {return 0;}
int CPU::bit_7_c() {return 0;}
int CPU::bit_7_f() {return 0;}
int CPU::bit_7_e() {return 0;}
int CPU::bit_7_h() {return 0;}
int CPU::bit_7_l() {return 0;}
int CPU::bit_7_hl() {return 0;}
int CPU::bit_7_a() {return 0;}

/*0x08*/
int CPU::res_0_b() {
	res(regs.B,0);
	return 0;
}
int CPU::res_0_c() {
	res(regs.C,0);
	return 0;
}
int CPU::res_0_f() {
	res(regs.F,0);
	return 0;
}
int CPU::res_0_e() {
	res(regs.E,0);
	return 0;
}
int CPU::res_0_h() {
	res(regs.H,0);
	return 0;
}
int CPU::res_0_l() {
	res(regs.L,0);
	return 0;
}
int CPU::res_0_hl() {
}
int CPU::res_0_a() {
	res(regs.A,0);
	return 0;
}
int CPU::res_1_b() {
	res(regs.B,1);
	return 0;
}
int CPU::res_1_c() {
	res(regs.C,1);
	return 0;
}
int CPU::res_1_f() {
	res(regs.F,1);
	return 0;
}
int CPU::res_1_e() {
	res(regs.E,1);
	return 0;
}
int CPU::res_1_h() {
	res(regs.H,1);
	return 0;
}
int CPU::res_1_l() {
	res(regs.L,1);
	return 0;
}
int CPU::res_1_hl() {return 0;}
int CPU::res_1_a() {
	res(regs.A,1);
	return 0;
}

/*0x09*/
int CPU::res_2_b() {
	res(regs.B,2);
	return 0;
}
int CPU::res_2_c() {
	res(regs.C,2);
	return 0;
}
int CPU::res_2_f() {
	res(regs.F,2);
	return 0;
}
int CPU::res_2_e() {
	res(regs.E,2);
	return 0;
}
int CPU::res_2_h() {
	res(regs.H,2);
	return 0;
}
int CPU::res_2_l() {
	res(regs.L,2);
	return 0;
}
int CPU::res_2_hl() {
	return 0;
}
int CPU::res_2_a() {
	res(regs.A,2);
	return 0;
}
int CPU::res_3_b() {
	res(regs.B,3);
	return 0;
}
int CPU::res_3_c() {
	res(regs.C,3);
	return 0;
}
int CPU::res_3_f() {
	res(regs.F,3);
	return 0;
}
int CPU::res_3_e() {
	res(regs.E,3);
	return 0;
}
int CPU::res_3_h() {
	res(regs.H,3);
	return 0;
}
int CPU::res_3_l() {
	res(regs.L,3);
	return 0;
}
int CPU::res_3_hl() {
	return 0;
}
int CPU::res_3_a() {
	res(regs.A,3);
	return 0;
}

/*0x0A*/
int CPU::res_4_b() {
	res(regs.B,4);
	return 0;
}
int CPU::res_4_c() {
	res(regs.C,4);
	return 0;
}
int CPU::res_4_f() {
	res(regs.F,4);
	return 0;
}
int CPU::res_4_e() {
	res(regs.E,4);
	return 0;
}
int CPU::res_4_h() {
	res(regs.H,4);
	return 0;
}
int CPU::res_4_l() {
	res(regs.L,4);
	return 0;
}
int CPU::res_4_hl() {
	return 0;
}
int CPU::res_4_a() {
	res(regs.A,4);
	return 0;
}
int CPU::res_5_b() {
	res(regs.B,5);
	return 0;
}
int CPU::res_5_c() {
	res(regs.C,5);
	return 0;
}
int CPU::res_5_f() {
	res(regs.F,5);
	return 0;
}
int CPU::res_5_e() {
	res(regs.E,5);
	return 0;
}
int CPU::res_5_h() {
	res(regs.H,5);
	return 0;
}
int CPU::res_5_l() {
	res(regs.L,5);
	return 0;
}
int CPU::res_5_hl() {
	return 0;
}
int CPU::res_5_a() {
	res(regs.A,5);
	return 0;
}

/*0x0B*/
int CPU::res_6_b() {
	res(regs.B,6);
	return 0;
}
int CPU::res_6_c() {
	res(regs.C,6);
	return 0;
}
int CPU::res_6_f() {
	res(regs.F,6);
	return 0;
}
int CPU::res_6_e() {
	res(regs.E,6);
	return 0;
}
int CPU::res_6_h() {
	res(regs.H,6);
	return 0;
}
int CPU::res_6_l() {
	res(regs.L,6);
	return 0;
}
int CPU::res_6_hl() {
	return 0;
}
int CPU::res_6_a() {
	res(regs.A,6);
	return 0;
}
int CPU::res_7_b() {
	res(regs.B,7);
	return 0;
}
int CPU::res_7_c() {
	res(regs.C,7);
	return 0;
}
int CPU::res_7_f() {
	res(regs.F,7);
	return 0;
}
int CPU::res_7_e() {
	res(regs.E,7);
	return 0;
}
int CPU::res_7_h() {
	res(regs.H,7);
	return 0;
}
int CPU::res_7_l() {
	res(regs.L,7);
	return 0;
}
int CPU::res_7_hl() {
	return 0;
}
int CPU::res_7_a() {
	res(regs.A,7);
	return 0;
}
/*0x0C*/
int CPU::set_0_b() {
	set(regs.B,0);
	return 0;
}
int CPU::set_0_c() {
	set(regs.C,0);
	return 0;
}
int CPU::set_0_f() {
	set(regs.F,0);
	return 0;
}
int CPU::set_0_e() {
	set(regs.E,0);
	return 0;
}
int CPU::set_0_h() {
	set(regs.H,0);
	return 0;
}
int CPU::set_0_l() {
	set(regs.L,0);
	return 0;
}
int CPU::set_0_hl() {
	return 0;
}
int CPU::set_0_a() {
	set(regs.A,0);
	return 0;
}
int CPU::set_1_b() {
	set(regs.B,1);
	return 0;
}
int CPU::set_1_c() {
	set(regs.C,1);
	return 0;
}
int CPU::set_1_f() {
	set(regs.F,1);
	return 0;
}
int CPU::set_1_e() {
	set(regs.E,1);
	return 0;
}
int CPU::set_1_h() {
	set(regs.H,1);
	return 0;
}
int CPU::set_1_l() {
	set(regs.L,1);
	return 0;
}
int CPU::set_1_hl() {
	return 0;
}
int CPU::set_1_a() {
	set(regs.A,1);
	return 0;
}

/*0x0D*/
int CPU::set_2_b() {
	set(regs.B,1);
	return 0;
}
int CPU::set_2_c() {
	set(regs.C,1);
	return 0;
}
int CPU::set_2_f() {
	set(regs.F,1);
	return 0;
}
int CPU::set_2_e() {
	set(regs.E,1);
	return 0;
}
int CPU::set_2_h() {
	set(regs.H,1);
	return 0;
}
int CPU::set_2_l() {
	set(regs.L,2);
	return 0;
}int CPU::set_2_hl() {
	return 0;
}
int CPU::set_2_a() {
	set(regs.A,2);
	return 0;
}
int CPU::set_3_b() {
	set(regs.B,3);
	return 0;
}
int CPU::set_3_c() {
	set(regs.C,3);
	return 0;
}
int CPU::set_3_f() {
	set(regs.F,3);
	return 0;
}
int CPU::set_3_e() {
	set(regs.E,3);
	return 0;
}
int CPU::set_3_h() {
	set(regs.H,3);
	return 0;
}
int CPU::set_3_l() {
	set(regs.L,3);
	return 0;
}
int CPU::set_3_hl() {

	return 0;
}
int CPU::set_3_a() {
	set(regs.A,3);
	return 0;
}
/*0x0E*/
int CPU::set_4_b() {
	set(regs.B,4);
	return 0;
}
int CPU::set_4_c() {
	set(regs.C,4);
	return 0;
}
int CPU::set_4_f() {
	set(regs.F,4);
	return 0;
}
int CPU::set_4_e() {
	set(regs.E,4);
	return 0;
}
int CPU::set_4_h() {
	set(regs.H,4);
	return 0;
}
int CPU::set_4_l() {
	set(regs.L,4);
	return 0;
}
int CPU::set_4_hl() {
	return 0;
}
int CPU::set_4_a() {
	set(regs.A,5);
	return 0;
}
int CPU::set_5_b() {
	set(regs.B,5);
	return 0;
}
int CPU::set_5_c() {
	set(regs.C,5);
	return 0;
}
int CPU::set_5_f() {
	set(regs.F,5);
	return 0;
}
int CPU::set_5_e() {
	set(regs.E,5);
	return 0;
}
int CPU::set_5_h() {
	set(regs.H,5);
	return 0;
}
int CPU::set_5_l() {
	set(regs.L,5);
	return 0;
}
int CPU::set_5_hl() {
	return 0;
}
int CPU::set_5_a() {
	set(regs.A,5);
	return 0;
}
/*0x0F*/
int CPU::set_6_b() {
	set(regs.B,6);
	return 0;
}
int CPU::set_6_c() {
	set(regs.C,6);
	return 0;
}
int CPU::set_6_f() {
	set(regs.F,6);
	return 0;
}
int CPU::set_6_e() {
	set(regs.E,6);
	return 0;
}
int CPU::set_6_h() {
	set(regs.H,6);
	return 0;
}
int CPU::set_6_l() {
	set(regs.L,6);
	return 0;
}
int CPU::set_6_hl() {
	return 0;
}
int CPU::set_6_a() {
	set(regs.A,6);
	return 0;
}
int CPU::set_7_b() {
	set(regs.B,7);
	return 0;
}
int CPU::set_7_c() {
	set(regs.C,7);
	return 0;
}
int CPU::set_7_f() {
	set(regs.F,7);
	return 0;
}
int CPU::set_7_e() {
	set(regs.E,7);
	return 0;
}
int CPU::set_7_h() {
	set(regs.H,7);
	return 0;
}
int CPU::set_7_l() {
	set(regs.L,7);
	return 0;
}
int CPU::set_7_hl() {
	return 0;
}
int CPU::set_7_a() {
	set(regs.A,7);
	return 0;
}
