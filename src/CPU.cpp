#include "headers/CPU.h"
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>

 
#define INTERRUPT_ENABLE 0xFFFF
#define INTERRUPT_FLAGS 0xFF0F


CPU::CPU(){};


CPU::CPU(RAM * _ram, bool * _IME):ram(_ram),IME(_IME){

	//start up values
	SP = 0xFFFE;
	regs.AF = 0x01B0;
	regs.BC = 0x0013;
	regs.DE = 0x00D8;
	regs.HL = 0x014D;
	PC = 0x0100;

/*
	regs.zero=1;
	regs.negative=0;

	regs.halfCarry=!(checksum==0);
	regs.carry=!(checksum==0);
*/


	opcodeLUT = std::vector<int (CPU::*)(void)>{
			&CPU::nop,         &CPU::ld_bc_d16,  &CPU::ld_bc_a,   &CPU::inc_bc, &CPU::inc_b,       &CPU::dec_b,      &CPU::ld_b_d8,   &CPU::rlca,	&CPU::ld_a16_sp,   &CPU::add_hl_bc,  &CPU::ld_a_bc,   &CPU::dec_bc, &CPU::inc_c,       &CPU::dec_c,      &CPU::ld_c_d8,   &CPU::rrca,
			&CPU::stop,        &CPU::ld_de_d16,  &CPU::ld_de_a,   &CPU::inc_de,	&CPU::inc_d,       &CPU::dec_d,      &CPU::ld_d_d8,   &CPU::rla,	&CPU::jr_s8,       &CPU::add_hl_de,  &CPU::ld_a_de,   &CPU::dec_de,	&CPU::inc_e,       &CPU::dec_e,      &CPU::ld_e_d8,   &CPU::rra,
			&CPU::jr_nz_s8,    &CPU::ld_hl_d16,  &CPU::ld_hlp_a,  &CPU::inc_hl,	&CPU::inc_h,       &CPU::dec_h,      &CPU::ld_h_d8,   &CPU::daa,	&CPU::jr_z_s8,     &CPU::add_hl_hl,  &CPU::ld_a_hlp,  &CPU::dec_hl,	&CPU::inc_l,       &CPU::dec_l,      &CPU::ld_l_d8,   &CPU::cpl,
			&CPU::jr_nc_s8,    &CPU::ld_sp_d16,  &CPU::ld_hlm_a,  &CPU::inc_sp,	&CPU::inc_hl_mem,  &CPU::dec_hl_mem, &CPU::ld_hl_d8,  &CPU::scf,	&CPU::jr_c_s8,     &CPU::add_hl_sp,  &CPU::ld_a_hlm,  &CPU::dec_sp,	&CPU::inc_a,       &CPU::dec_a,      &CPU::ld_a_d8,   &CPU::ccf,
			&CPU::ld_b_b,      &CPU::ld_b_c,     &CPU::ld_b_d,    &CPU::ld_b_e,	&CPU::ld_b_h,      &CPU::ld_b_l,     &CPU::ld_b_hl,   &CPU::ld_b_a,	&CPU::ld_c_b,      &CPU::ld_c_c,     &CPU::ld_c_d,    &CPU::ld_c_e,	&CPU::ld_c_h,      &CPU::ld_c_l,     &CPU::ld_c_hl,   &CPU::ld_c_a,
			&CPU::ld_d_b,      &CPU::ld_d_c,     &CPU::ld_d_d,    &CPU::ld_d_e,	&CPU::ld_d_h,      &CPU::ld_d_l,     &CPU::ld_d_hl,   &CPU::ld_d_a,	&CPU::ld_e_b,      &CPU::ld_e_c,     &CPU::ld_e_d,    &CPU::ld_e_e,	&CPU::ld_e_h,      &CPU::ld_e_l,     &CPU::ld_e_hl,   &CPU::ld_e_a,
			&CPU::ld_h_b,      &CPU::ld_h_c,     &CPU::ld_h_d,    &CPU::ld_h_e,	&CPU::ld_h_h,      &CPU::ld_h_l,     &CPU::ld_h_hl,   &CPU::ld_h_a,	&CPU::ld_l_b,      &CPU::ld_l_c,     &CPU::ld_l_d,    &CPU::ld_l_e,	&CPU::ld_l_h,      &CPU::ld_l_l,     &CPU::ld_l_hl,   &CPU::ld_l_a,
			&CPU::ld_hl_b,     &CPU::ld_hl_c,    &CPU::ld_hl_d,   &CPU::ld_hl_e,&CPU::ld_hl_h,     &CPU::ld_hl_l,    &CPU::HALT,      &CPU::ld_hl_a,&CPU::ld_a_b,      &CPU::ld_a_c,     &CPU::ld_a_d,    &CPU::ld_a_e,	&CPU::ld_a_h,      &CPU::ld_a_l,     &CPU::ld_a_hl,   &CPU::ld_a_a,
			&CPU::add_a_b,     &CPU::add_a_c,    &CPU::add_a_d,   &CPU::add_a_e,&CPU::add_a_h,     &CPU::add_a_l,    &CPU::add_a_hl,  &CPU::add_a_a,&CPU::adc_a_b,     &CPU::adc_a_c,    &CPU::adc_a_d,   &CPU::adc_a_e,&CPU::adc_a_h,     &CPU::adc_a_l,    &CPU::adc_a_hl,  &CPU::adc_a_a,
			&CPU::sub_b,       &CPU::sub_c,      &CPU::sub_d,     &CPU::sub_e,	&CPU::sub_h,       &CPU::sub_l,      &CPU::sub_hl,    &CPU::sub_a,	&CPU::sbc_a_b,     &CPU::sbc_a_c,    &CPU::sbc_a_d,   &CPU::sbc_a_e,&CPU::sbc_a_h,     &CPU::sbc_a_l,    &CPU::sbc_a_hl,  &CPU::sbc_a_a,
			&CPU::and_b,       &CPU::and_c,      &CPU::and_d,     &CPU::and_e,	&CPU::and_h,       &CPU::and_l,      &CPU::and_hl,    &CPU::and_a,	&CPU::xor_b,       &CPU::xor_c,      &CPU::xor_d,     &CPU::xor_e,	&CPU::xor_h,       &CPU::xor_l,      &CPU::xor_hl,    &CPU::xor_a,
			&CPU::or_b,        &CPU::or_c,       &CPU::or_d,      &CPU::or_e,	&CPU::or_h,        &CPU::or_l,       &CPU::or_hl,     &CPU::or_a,	&CPU::cp_b,        &CPU::cp_c,       &CPU::cp_d,      &CPU::cp_e,	&CPU::cp_h,        &CPU::cp_l,       &CPU::cp_hl,     &CPU::cp_a,
			&CPU::ret_nz,      &CPU::pop_bc,     &CPU::jp_nz_a16, &CPU::jp_a16,	&CPU::call_nz_a16, &CPU::push_bc,    &CPU::add_a_d8,  &CPU::rst_0,	&CPU::ret_z,       &CPU::ret,        &CPU::jp_z_a16,  &CPU::invalid,&CPU::call_z_a16,  &CPU::call_a16,   &CPU::adc_a_d8,  &CPU::rst_1,
			&CPU::ret_nc,      &CPU::pop_de,     &CPU::jp_nc_a16, &CPU::invalid,&CPU::call_nc_a16, &CPU::push_de,    &CPU::sub_d8,    &CPU::rst_2,	&CPU::ret_c,       &CPU::reti,       &CPU::jp_c_a16,  &CPU::invalid,&CPU::call_c_a16,  &CPU::invalid,    &CPU::sbc_a_d8,  &CPU::rst_3,
			&CPU::ld_a8_a,     &CPU::pop_hl,     &CPU::ld_c_mem_a,&CPU::invalid,&CPU::invalid,     &CPU::push_hl,    &CPU::and_d8,    &CPU::rst_4,	&CPU::add_sp_s8,   &CPU::jp_hl,      &CPU::ld_a16_a,  &CPU::invalid,&CPU::invalid,     &CPU::invalid,    &CPU::xor_d8,    &CPU::rst_5,
			&CPU::ld_a_a8,     &CPU::pop_af,     &CPU::ld_a_c_mem,&CPU::di,		&CPU::invalid,     &CPU::push_af,    &CPU::or_d8,     &CPU::rst_6,	&CPU::ld_hl_sp_s8, &CPU::ld_sp_hl,   &CPU::ld_a_a16,  &CPU::ei,		&CPU::invalid,     &CPU::invalid,    &CPU::cp_d8,     &CPU::rst_7
			};

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

bool CPU::testBit(uint8_t data, int bit){
	return (data&(1<<bit));
}

uint8_t CPU::bitSet(uint8_t data, int bit){
	return (data|(1<<bit));
}

void CPU::pushWordToStack(uint16_t data){

	uint8_t hi = data >> 8 ;
	uint8_t lo = data & 0xFF;
	SP-- ;
	ram->write(SP, hi) ;
	SP-- ;
	ram->write(SP,lo) ;
}

uint16_t CPU::PopWordOffStack(){
	uint16_t word = ram->read(SP+1) << 8 ;
	word |= ram->read(SP) ;
	SP+=2 ;
	return word ;
}
uint16_t CPU::readWord() const{
	uint16_t res = ram->read(PC+1) ;
	res = res << 8 ;
	res |= ram->read(PC) ;
	return res ;
}




void CPU::fetch() {
	instruction = ram->read(PC);
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
		instruction = ram->read(PC);
		PC++;
	}

}

void CPU::execute() { // dont forget interrupts
				// use correct lookup table
	if (flagCB == 0) {
		(this->*opcodeLUT[instruction])();

	} 
	else {
		(this->*opcodeLUTCB[instruction])();
		flagCB=0;
	}
}

int CPU::fetchExecute(){
	fetch();
	execute();
	return cycles;
}

uint16_t CPU::getInstruction(){
	return instruction;
}

uint16_t CPU::getPC(){
	return PC;
}

void CPU::setPC(uint16_t _PC){
	PC=_PC;
}

void CPU::write(uint16_t addr, uint8_t data){
	ram->write(addr,data);
}

void CPU::read(uint16_t addr){
	printf("%X: %X  ",addr,ram->read(addr));
}

void CPU::getRegs(){
	printf("AF: %X\n",regs.AF);
	printf("BC: %X\n",regs.BC);
	printf("DE: %X\n",regs.DE);
	printf("HL: %X\n",regs.HL);

}


/*===========================================*/
/*Implementation of helper instructions below*/
/*===========================================*/
int CPU::ld_reg_addr(uint8_t &reg1, uint8_t data) {
	printf("==LOAD REG FROM MEM==\n");
	cycles = 8;
	reg1 = data;
	return 0;
}

int CPU::ld_reg_reg(uint8_t &reg1, uint8_t reg2) {
	printf("==LOAD FROM REG==\n");
	printf("Before: Reg1: %x, Reg2: %x\n", reg1,reg2);
	cycles = 4;
	reg1 = reg2;
	printf("After: Reg1: %x, Reg2: %x\n",reg1,reg2);
	return 0;
}

int CPU::ld_reg_d8(uint8_t &reg) {
	printf("==LOAD FROM MEM==\n");
	printf("Before: Reg1: %X\n",reg);
	cycles = 8;
	uint8_t imm = ram->read(PC);
	PC++;
	reg = imm;
	printf("After: Reg1: %X\n",reg);

	return 0;
}

int CPU::ld_reg_d16(uint16_t &reg1) {
	printf("==LOAD FROM MEM==\n");
	printf("Before: Reg1: %X\n",reg1);

	cycles = 12;
	uint16_t data = readWord();
	PC+=2;
	reg1 = data;
	printf("After: Reg1: %X\n",reg1);
	return 0;
}

int CPU::inc_reg(uint8_t &reg) {
	/*
	printf("==INCREMENT REG==\n");
	printf("Before Reg1: %X\n",reg);
	if(reg==255){
		printf("INCREMENT OVERFLOW\n");
	}
	cycles = 4;
	uint8_t prev=reg;
	reg++;
	regs.zero = (reg == 0x00);
	regs.negative = 0;
	regs.halfCarry = ((prev & 0x0F) == 0x0F);
	

	printf("After Reg1: %X\n",reg);
	return 0;
*/
	printf("==INCREMENT REG==\n");


	uint8_t before = reg ;

	reg++;
	regs.zero=(reg==0);


	regs.negative=0;


	if ((before & 0xF) == 0xF)
		regs.halfCarry=1;
	else
		regs.halfCarry=0;

	printf("After Reg1: %X\n",reg);


}


int CPU::inc_reg(uint16_t &reg) {
	printf("==INCREMENT REG==\n");

	cycles = 8;
	reg++;
	return 0;
}

int CPU::dec_reg(uint8_t &reg) {
	cycles = 4;
	printf("==DECREMENT REG==\n");
	printf("Before: reg: %X\n", reg);
	uint8_t before = reg ;
	reg-- ;
	if (reg == 0)
		regs.zero=1;
	else
		regs.zero=0;
	regs.negative=1;
	if ((before & 0x0F) == 0)
		regs.halfCarry=1;
	else
		regs.halfCarry=0;

	printf("After: reg: %X\n", reg);

}

int CPU::dec_reg(uint16_t &reg) {
	printf("==DECREMENT REG==\n");

	cycles = 8;
	regs.BC--;
	return 0;
}

int CPU::ld_mem_a(uint16_t &addr) {
	printf("==LOAD A TO MEM==\n");

	cycles = 8;
	ram->write(addr, regs.A);
	return 0;
}

int CPU::jr(bool condition) {
	printf("==Jump PC==\n");
	printf("Before: PC:%X \n",PC);
	if (condition) {
		cycles = 12;
		char s8 = ram->read(PC);
		PC++;
		PC = PC + s8;
	} else {
		cycles = 8;
		PC++;
	}
	printf("After: PC:%X \n",PC);
	return 0;
}


int CPU::add(uint8_t &reg1, uint8_t &reg2) {
	printf("==ADD==\n");
	cycles = 4;
	uint16_t result = reg1 + reg2;
	regs.halfCarry = (reg1 & 0x0F) + (reg2 & 0x0F);
	regs.carry = result > 0xff;
	regs.negative = 0;
	return 0;
}

int CPU::add(uint16_t &reg1, uint16_t &reg2) {
	printf("==ADD==\n");
	cycles = 8;
	regs.negative = 0;
	uint32_t result = reg1 + reg2;
	regs.carry = result > 0xffff;
	regs.halfCarry = ((reg1 & 0x0fff) + (reg2 & 0x0fff) > 0x0fff);
	regs.negative = 0;
	return 0;
}

int CPU::adc(uint8_t &reg1, uint8_t &reg2) {
	printf("==ADC==\n");
	cycles=4;
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
	printf("==SUB==\n");
	uint16_t result = reg1 - reg2;
	regs.carry = (reg2 > reg1);
	regs.halfCarry = ((reg2 & 0x0f) > (reg1 & 0x0f));
	reg1 = reg1 - reg2;
	regs.zero = (reg1 == 0);
	regs.negative = 1;
	return 0;
}



int CPU::sbc(uint8_t &reg1, uint8_t &reg2) {
	printf("==SBC==\n");
	cycles=4;
	int carry = regs.carry;
	regs.carry = ((reg2 + carry) > reg1);
	regs.halfCarry = (((reg2 & 0x0F) + carry) > (reg1 & 0x0F));

	reg1 = reg1 - carry - reg2;
	regs.zero = (reg1 == 0);
	regs.negative = 1;
	return 0;
}
int CPU::_and(uint16_t &reg1,uint16_t &reg2) { // many of these functions cam be written as templates
	printf("==AND==\n");
	reg1 = reg1 & reg2;
	regs.carry = 0;
	regs.halfCarry = 1;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_and(uint8_t &reg1, uint8_t &reg2) {
	printf("==AND==\n");
	cycles=4;
	reg1 = reg1 & reg2;
	regs.carry = 0;
	regs.halfCarry = 1;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_xor(uint16_t &reg1, uint16_t &reg2) {
	printf("==XOR==\n");

	reg1 ^= reg2;
	regs.carry = 0;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_xor(uint8_t &reg1, uint8_t &reg2) {
	printf("==XOR==\n");
	cycles=4;
	reg1 ^= reg2;
	regs.carry = 0;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_or(uint16_t &reg1, uint16_t &reg2) {
	printf("==OR==\n");
	reg1 |= reg2;
	regs.carry = 0;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_or(uint8_t &reg1, uint8_t &reg2) {
	printf("==OR==\n");
	cycles=4;
	reg1 |= reg2;
	regs.carry = 0;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::cp(uint16_t reg1, uint16_t reg2) {
	printf("==CP==\n");
	regs.negative = 1;
	regs.zero = (reg1 == reg2);
	regs.carry = (reg2 > reg1);
	regs.halfCarry = ((reg2 & 0x0FF) > (reg1 & 0x0FF));
	return 0;
}

//stolen from code slinger
int CPU::cp(uint8_t reg1, uint8_t reg2) { // doesnt actually store result of subtraction anywhere
	printf("==CP==\n");
	cycles=4;
	uint8_t before = reg1 ;
	uint8_t toSubtract = reg2 ;
	reg1 -= toSubtract ;

	if (reg1 == 0)
		regs.zero=1;

	regs.negative=1;
	// set if no borrow
	if (before < toSubtract)
		regs.carry=1;
	signed short htest = before & 0xF ;
	htest -= (toSubtract & 0xF) ;

	if (htest < 0)
		regs.halfCarry=1;

	return 0;
}

int CPU::BIT(int bit, uint8_t & reg){	
	printf("==BIT==\n");
	uint8_t bitResult=reg&(1<<bit);	//bitResult will store the and of the result and the 
									//bit in question
	regs.zero=((bitResult==0));	//instead of complementing, just set if reset, and reset if set...
	regs.negative=0;
	regs.halfCarry=1;
	return 0;
}

int CPU::pop(uint16_t &reg1) {
	cycles=12;
	printf("==POP REG FROM STACK==\n");
	reg1=PopWordOffStack();
	return 0;
}

int CPU::push(uint16_t &reg1) {
	printf("==PUSH REG TO STACK==\n");

	cycles=16;
	//push pc to stack then jump
	pushWordToStack(reg1);
	return 0;
}



int CPU::rst(int index) {
	printf("==RST==\n");
	cycles = 16;
	SP--;
	ram->write(SP, (PC & 0xFF00) >> 8);
	SP--;
	ram->write(SP, PC & 0x00FF);
	PC = ram->read(0x00) << 8;
	PC |= ram->read(0x8 * index);
	return 0;
}

int CPU::_ret(bool condition) {
	cycles = 16;
	printf("==RETURNING==\n");
	if(condition){
		PC = PopWordOffStack() ;
		printf("Return address=%X\n",PC);
		return 0;
	}
		printf("Condition false\n");


	return 0;
}

int CPU::jp_a16(bool condition){
	printf("==Jump==\n");
	printf("PC before: %X\n",PC);
	uint16_t a16=readWord();
	PC+=2;
	if(condition){
		cycles=16;
		PC=a16;
	}
	else{
		cycles=12;
	}
	printf("PC after: %X\n",PC);
	return 0;
}


int CPU::call(bool condition){	
	printf("==CALL==\n");
	cycles+=12 ;
	uint16_t nn = readWord() ;
	PC += 2;
	printf("Jump to %X from %X\n",nn,PC);

	if(condition){
		pushWordToStack(PC) ;
		PC = nn ;
	}

	return 0;
}


int CPU::rlc(uint8_t & reg){
	uint8_t prev=reg;
	reg<<=1;
	regs.carry=((prev&0x80)==0x80);	//set carry if top bit was one
	reg=reg|regs.carry;	//put carry into bit 0 of result
	regs.zero=(reg==0);
	regs.negative=0;
	regs.halfCarry=0;
	return 0;
	
}
int CPU::rrc(uint8_t & reg){
	uint8_t prev=reg;
	reg>>=1;
	regs.carry=((prev&1)==1);	//set carry if lower bit was 1
	reg=reg|(regs.carry<<8);	//put carry into bit 0 of result
	regs.zero=(reg==0);
	regs.negative=0;
	regs.halfCarry=0;
	return 0;
}

//these next few instructions implementations are stolen from CodeSlinger. 

//TODO: add cycles to these instructions
int CPU::rl(uint8_t & reg){
	// WHEN EDITING THIS FUNCTION ALSO EDIT CPU_RL_MEMORY

	bool isCarrySet = (regs.carry==1) ;
	bool isMSBSet = testBit(reg, 7) ;

	regs.F = 0 ;

	reg <<= 1 ;

	if (isMSBSet)
		regs.carry=1 ;

	if (isCarrySet)
		reg = bitSet(reg, 0) ;

	if (reg == 0)
		regs.F = (regs.zero==1) ;
}
int CPU::rr(uint8_t & reg){
	// WHEN EDITING THIS ALSO EDIT CPU_RR_MEMORY

	bool isCarrySet = (regs.carry==1) ;
	bool isLSBSet = testBit(reg, 0) ;

	regs.F = 0 ;

	reg >>= 1 ;

	if (isLSBSet)
		regs.carry=1 ;

	if (isCarrySet)
		reg = bitSet(reg, 7) ;

	if (reg == 0)
		regs.F = (regs.zero==1) ;
}
int CPU::sla(uint8_t & reg){
	// WHEN EDITING THIS ALSO EDIT CPU_SLA_MEMORY


	bool isMSBSet = testBit(reg, 7);

	reg <<= 1;

	regs.F = 0 ;

	if (isMSBSet)
		regs.carry=1 ;

	if (reg == 0)
		regs.F = (regs.zero==1) ;
}
int CPU::sra(uint8_t & reg){
	// WHEN EDITING THIS FUNCTION ALSO EDIT CPU_SRA_MEMORY


	bool isLSBSet = testBit(reg,0) ;
	bool isMSBSet = testBit(reg,7) ;

	regs.F = 0 ;

	reg >>= 1;

	if (isMSBSet)
		reg = bitSet(reg,7) ;
	if (isLSBSet)
		regs.carry=1 ;

	if (reg == 0)
		regs.F = (regs.zero==1) ;
}
int CPU::swap(uint8_t & reg){

}
int CPU::srl(uint8_t & reg){
	//WHEN EDITING THIS FUNCTION ALSO EDIT CPU_SRL_MEMORY

	bool isLSBSet = testBit(reg,0) ;

	regs.F = 0 ;

	reg >>= 1;

	if (isLSBSet)
		regs.carry=1 ;

	if (reg == 0)
		regs.F = (regs.zero==1) ;
}


int CPU::res(uint8_t & reg, int pos){	//reset bit at offset
	cycles=8;
	int offsetBin=1<<pos;
	offsetBin=~offsetBin;
	reg=reg&offsetBin;	//and reg with the inverse of its "offset"
	return 0;
}
int CPU::set(uint8_t & reg, int pos){ //set bit at offset
	cycles=8;
	int offsetBin=1<<pos;
	reg=reg|offsetBin;	//and reg with the inverse of its "offset"
	return 0;
}

/*===========================================*/
/*Implementation of helper instructions above*/
/*===========================================*/
///////////////////////////////////////////////////
/*=========================================*/
/*Implementation table 1 instructions below*/
/*=========================================*/


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/*===============================LOAD INSTUCTIONS===============================*/
/*ld reg, d16*/
int CPU::ld_bc_d16() { // 0x01
	ld_reg_d16(regs.BC);
	return 0;
}
int CPU::ld_de_d16() { // 0x11
	ld_reg_d16(regs.DE);
	return 0;
}
int CPU::ld_hl_d16() { // 0x21
	ld_reg_d16(regs.HL);
	return 0;
}
int CPU::ld_sp_d16() { // 0x31
	ld_reg_d16(SP);
	return 0;
}
/*ld reg, (addr)*/
int CPU::ld_b_hl() { // 0x46
	printf("==LOAD from HL==\n");
	printf("Before: Reg1: %X, HL: %X",regs.D,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.B = operand;
	printf("After: Reg1: %X, HL: %X",regs.D,regs.HL);
	return 0;
}
int CPU::ld_d_hl() { // 0x56
	printf("==LOAD from HL==\n");
	printf("Before: Reg1: %X, HL: %X",regs.D,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.D = operand;
	printf("After: Reg1: %X, HL: %X",regs.D,regs.HL);
	return 0;
}
int CPU::ld_h_hl() { // 0x66
	printf("==LOAD from HL==\n");
	printf("Before: Reg1: %X, HL: %X",regs.D,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.H = operand;
	printf("After: Reg1: %X, HL: %X",regs.D,regs.HL);
	return 0;
}

int CPU::ld_c_hl() { // 0x4E
	printf("==LOAD from HL==\n");
	printf("Before: Reg1: %X, HL: %X",regs.C,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.C = operand;
	printf("After: Reg1: %X, HL: %X",regs.C,regs.HL);
	return 0;
}
int CPU::ld_e_hl() { // 0x5E
	printf("==LOAD from HL==\n");
	printf("Before: Reg1: %X, HL: %X",regs.E,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.E = operand;
	printf("After: Reg1: %X, HL: %X",regs.E,regs.HL);
	return 0;
}
int CPU::ld_l_hl() { // 0x6E
	printf("==LOAD from HL==\n");
	printf("Before: Reg1: %X, HL: %X",regs.L,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.L = operand;
	printf("After: Reg1: %X, HL: %X",regs.L,regs.HL);
	return 0;
}
int CPU::ld_a_hl() {	//0x7E
	printf("==LOAD from HL==\n");
	printf("Before: Reg1: %X, HL: %X",regs.A,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.A = operand;
	printf("After: Reg1: %X, HL: %X",regs.A,regs.HL);
	return 0;
}
int CPU::ld_a_hlp() { // 0x2A
	regs.A=ram->read(regs.HL);
	regs.HL++;
	return 0;
}
int CPU::ld_a_hlm() { // 0x3A
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.A = operand;
	regs.HL--;
	return 0;
}
int CPU::ld_a_bc() { // 0x0A
	cycles = 8;
	uint8_t operand = ram->read(regs.BC);
	regs.A = operand;
	return 0;
}
int CPU::ld_a_de() { // 0x1A
	cycles = 8;
	uint8_t operand = ram->read(regs.DE);
	regs.A = operand;
	return 0;
}
int CPU::ld_a_c_mem() {	//0xF2
	cycles=8;
	regs.A=ram->read(regs.C|0xFF00);
	return 0;
}
int CPU::ld_a_a8() {	//0xF0
	cycles=12;
	uint8_t addr=ram->read(PC);
	PC++;
	regs.A=ram->read(addr|0xFF00);
	return 0;
}
int CPU::ld_a_a16() {	//0xFA
	cycles=16;
	uint16_t a16=ram->read(PC);
	PC++;
	a16|=ram->read(PC)<<8;
	PC++;
	regs.A=ram->read(a16);
	return 0;
}

/*ld (addr), data*/
int CPU::ld_hl_d8() { // 0x36
	cycles = 8;
	uint8_t imm = ram->read(PC);
	PC++;
	ram->write(regs.HL, imm);
	return 0;
}
int CPU::ld_bc_a() { // 0x02
	ld_mem_a(regs.BC);
	return 0;
}
int CPU::ld_de_a() { // 0x12
	ld_mem_a(regs.DE);
	return 0;
}
int CPU::ld_hlp_a() { // 0x22
	ld_mem_a(regs.HL);
	regs.HL++;
	return 0;
}
int CPU::ld_hlm_a() { // 0x32
	ld_mem_a(regs.HL);
	regs.HL--;
	return 0;
}
int CPU::ld_a16_a() {	//0xEA
	cycles=16;
	printf("==LOAD A INTO MEM==\n");
	uint16_t addr=ram->read(PC);
	PC++;
	addr=ram->read(PC)<<8;
	PC++;
	printf("Before: %X\n",ram->read(addr));
	ram->write(addr,regs.A);
	printf("After: %X\n",ram->read(addr));
	return 0;
}
int CPU::ld_a16_sp() { // 0x08
	//TODO: double check
	int cycles = 20;
	printf("==LOAD SP INTO MEM==\n");
	uint16_t addr=ram->read(PC);
	PC++;
	addr=ram->read(PC)<<8;
	PC++;

	printf("Before: %X\n",ram->read(addr));
	ram->write(addr,SP);
	printf("After: %X\n",ram->read(addr));
	ram->write(addr+1,(SP&0xFF00)>>8);
	return 0;
}
int CPU::ld_a8_a() {	//0xE0
	cycles=12;
	printf("==LOAD A INTO MEM==\n");
	uint16_t addr=ram->read(PC)|0xFF00;
	PC++;
	printf("Before: %X\n",ram->read(addr));
	ram->write(addr,regs.A);
	printf("After: %X\n",ram->read(addr));
	return 0;
}
int CPU::ld_c_mem_a() {	//0xE2
	cycles=8;
	uint16_t addr=regs.C|0xFF00;
	ram->write(addr,regs.A);
	return 0;
}
int CPU::ld_b_d8() { // 0x06
	ld_reg_d8(regs.B);
	return 0;
}
int CPU::ld_d_d8() { // 0x16
	ld_reg_d8(regs.D);
	return 0;
}
int CPU::ld_h_d8() { // 0x26
	ld_reg_d8(regs.H);
	return 0;
}
int CPU::ld_c_d8() { // 0x0E
	ld_reg_d8(regs.C);
	return 0;
}
int CPU::ld_e_d8() { // 0x1E
	ld_reg_d8(regs.E);
	return 0;
}
int CPU::ld_l_d8() { // 0x2E
	ld_reg_d8(regs.L);
	return 0;
}
int CPU::ld_a_d8() { // 0x3E
	ld_reg_d8(regs.A);
	return 0;
}
int CPU::ld_hl_b() {	//0x70
	cycles = 8;
	ram->write(regs.HL, regs.B);
	return 0;
}
int CPU::ld_hl_c() {	//0x71
	cycles = 8;
	ram->write(regs.HL, regs.C);
	return 0;
}
int CPU::ld_hl_d() {	//0x72
	cycles = 8;
	ram->write(regs.HL, regs.D);
	return 0;
}
int CPU::ld_hl_e() {	//0x73
	cycles = 8;
	ram->write(regs.HL, regs.E);
	return 0;
}
int CPU::ld_hl_h() {	//0x74
	cycles = 8;
	ram->write(regs.HL, regs.H);
	return 0;
}
int CPU::ld_hl_l() {	//0x75
	cycles = 8;
	ram->write(regs.HL, regs.L);
	return 0;
}
int CPU::ld_hl_a() {	//0x77
	cycles = 8;
	ram->write(regs.HL, regs.A);
	return 0;
}
/*ld reg reg*/
int CPU::ld_b_c() { // 0x40
	ld_reg_reg(regs.B, regs.C);
	return 0;
}
int CPU::ld_b_b() { // 0x40
	ld_reg_reg(regs.B, regs.B);
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
int CPU::ld_c_a() { // 0x4F
	ld_reg_reg(regs.C, regs.A);
	return 0;
}
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
int CPU::ld_e_a() { // 0x5F
	ld_reg_reg(regs.E, regs.A);
	return 0;
}
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
int CPU::ld_l_a() { // 0x6F
	ld_reg_reg(regs.L, regs.A);
	return 0;
}
int CPU::HALT() {	//0x76
	return 0;
	while(1){

    };
    //TODO: set HALT bool and do something with it
}
int CPU::ld_a_b() {	//0x78
	ld_reg_reg(regs.A, regs.B);
	return 0;
}
int CPU::ld_a_c() {	//0x79
	ld_reg_reg(regs.A, regs.C);
	return 0;
}
int CPU::ld_a_d() {	//0x7A
	ld_reg_reg(regs.A, regs.D);
	return 0;
}
int CPU::ld_a_e() {	//0x7B
	ld_reg_reg(regs.A, regs.E);
	return 0;
}
int CPU::ld_a_h() {	//0x7C
	ld_reg_reg(regs.A, regs.H);
	return 0;
}
int CPU::ld_a_l() {	//0x7D
	ld_reg_reg(regs.A, regs.L);
	return 0;
}
int CPU::ld_a_a() {	//0x7F
	ld_reg_reg(regs.A, regs.A);
	return 0;
};
int CPU::ld_sp_hl() {	//0xF9
	return 0;
}
int CPU::ld_hl_sp_s8() {	//0xF8	
	//TODO: implement this
	PC++;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/*===============================JUMP INSTRUCTIONS===============================*/
int CPU::jr_s8() { // 0x18
				   // possible error here (signed or unsigned number?)
	jr(1);
	return 0;
}
int CPU::jr_nz_s8() { // 0x20
	jr(regs.zero == 0);
	return 0;
}
int CPU::jr_z_s8() { // 0x28
	jr(regs.zero == 1);
	return 0;
}
int CPU::jr_nc_s8() { // 0x30
	jr(regs.carry == 0);
	return 0;
}
int CPU::jr_c_s8() { // 0x38

	jr(regs.carry == 1);
	return 0;
}
/*jp instructions*/
int CPU::jp_nz_a16() {	//0xC2
	jp_a16(regs.zero==0);
	return 0;
}
int CPU::jp_a16() {	//0xC3
	jp_a16(1);
	return 0;
}
int CPU::jp_z_a16() {	//0xCA
	jp_a16(regs.zero==1);
	return 0;
}
int CPU::jp_nc_a16() { // 0xD2
	jp_a16(regs.carry==0);
	return 0;
}
int CPU::jp_c_a16() { // 0xDA
	jp_a16(regs.carry==1);
	return 0;
}
int CPU::jp_hl() {	//0xE9
	cycles=4;
	PC=regs.HL;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/*===============================CALL INSTRUCTIONS===============================*/
int CPU::call_nz_a16() {	//0xC4
	call(regs.zero==0);
	return 0;
}
int CPU::call_z_a16() {	//0xCC
	call(regs.zero==1);
	return 0;
}
int CPU::call_a16() {	//0xCD
	call(1);
	return 0;
}
int CPU::call_nc_a16() { // 0xD4
	call(regs.carry==0);
	return 0;
}
int CPU::call_c_a16() { // 0xDC
	call(regs.carry==1);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/*===============================RETURN INSTRUCTIONS===============================*/
int CPU::ret_nz() {	//0xC0
	_ret(regs.zero==0);
	return 0;
}
int CPU::ret_z() {	//0xC8
	_ret(regs.zero == 1);
	return 1;
}
int CPU::ret() {	//0xC9
	_ret(1);
	return 1;
}
int CPU::ret_nc() { // 0xD0
	_ret(regs.carry == 0);
	return 0;
}
int CPU::ret_c() { // 0xD7
	_ret(regs.carry == 1);
	return 1;
}
int CPU::reti() { // 0xD8

	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/*===============================INC INSTRUCTIONS===============================*/
int CPU::inc_bc() { // 0x03
	inc_reg(regs.BC);
	return 0;
}
int CPU::inc_b() { // 0x04
	inc_reg(regs.B);
	return 0;
}
int CPU::inc_c() { // 0x0C
	inc_reg(regs.C);
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
int CPU::inc_e() { // 0x1C
	inc_reg(regs.E);
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
int CPU::inc_l() { // 0x2C
	inc_reg(regs.L);
	return 0;
}
int CPU::inc_sp() { // 0x33
	inc_reg(SP);
	return 0;
}
int CPU::inc_hl_mem() { // 0x34
	uint8_t contents = ram->read(regs.HL);
	cycles = 4;
	regs.halfCarry = ((contents & 0x0F) == 0x0F);
	regs.negative = 0;
	regs.zero = (contents == 0xFF);
	ram->write(regs.HL, contents++);
	return 0;
}
int CPU::inc_a() { // 0x3C
	inc_reg(regs.A);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/*===============================DEC INSTRUCTIONS===============================*/
int CPU::dec_b() { // 0x05
	dec_reg(regs.B);
	return 0;
}
int CPU::dec_bc() { // 0x0B
	dec_reg(regs.BC);
	return 0;
}
int CPU::dec_c() { // 0x0D
	dec_reg(regs.C);
	return 0;
}
int CPU::dec_d() { // 0x15
	dec_reg(regs.D);
	return 0;
}
int CPU::dec_de() { // 0x1B
	dec_reg(regs.DE);
	return 0;
}
int CPU::dec_e() { // 0x1D
	dec_reg(regs.E);
	return 0;
}
int CPU::dec_h() { // 0x25
	dec_reg(regs.H);
	return 0;
}
int CPU::dec_hl() { // 0x2B
	dec_reg(regs.HL);
	return 0;
}
int CPU::dec_l() { // 0x2d
	dec_reg(regs.L);
	return 0;
}
int CPU::dec_hl_mem() { // 0x35
	uint8_t contents = ram->read(regs.HL);
	regs.halfCarry = ((contents & 0x0F) == 0x00);
	regs.negative = 1;
	regs.zero = (contents == 0x01);
	ram->write(regs.HL, contents--);
	return 0;
}
int CPU::dec_sp() { // 0x3B
	dec_reg(SP);
	return 0;
}
int CPU::dec_a() { // 0x3D
	dec_reg(regs.A);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/*===============================PUSH INSTRUCTIONS===============================*/
int CPU::push_bc() {	//0xC5
	push(regs.BC);
	return 0;
}
int CPU::push_de() { // 0xD4
	push(regs.DE);
	return 0;
}
int CPU::push_hl() {	//0xE5
	push(regs.HL);
	return 0;
}
int CPU::push_af() {	//0xF5
	push(regs.AF);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/*===============================POP INSTRUCTIONS===============================*/
int CPU::pop_bc() {	//0xC1
	pop(regs.BC);
	return 0;
}
int CPU::pop_de() { // 0xD1
	pop(regs.DE);
	return 0;
}
int CPU::pop_hl() {	//0xE1
	pop(regs.HL);
	return 0;
}
int CPU::pop_af() {	//0xF1
	pop(regs.AF);
	return 0;
}
/////////////////////////
int CPU::nop() { // 0x00
	printf("NOP\n");
	cycles = 4;
	return 0;
}
int CPU::invalid() {	//illegal opcodes. 
						//for now they do nothing. 
	cycles = 4;
	return 0;
}

int CPU::rlca() { // 0x07
	cycles = 4;
	regs.zero = 0;
	regs.negative = 0;
	regs.halfCarry = 0;

	regs.carry = (regs.A & 0x80) >> 8;
	regs.A = regs.A << 1;
	regs.A |= regs.carry; // to rotate, use the same value that was stored in the
						  // carry bit
	return 0;
}



int CPU::add_hl_bc() { // 0x09
	add(regs.HL, regs.BC);
	return 0;
}







int CPU::rrca() { // 0x0F
	cycles = 4;
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

int CPU::rla() { // 0x17
				 // possible error
	cycles=4;
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


int CPU::add_hl_de() { // 0x19
	add(regs.HL, regs.DE);
	return 0;
}

int CPU::rra() { // 0x1F
	cycles=4;
	printf("===RRA===\n");
	printf("Before: REG.A: %d\n",regs.A);

	bool isCarrySet = regs.carry;
	bool isLSBSet = testBit(regs.A, 0) ;

	regs.F = 0;
	regs.A >>= 1;

	if (isLSBSet)
		regs.carry=0;

	if (isCarrySet)
		regs.A = bitSet(regs.A, 7) ;

	if (regs.A == 0)
		regs.zero=0;

	printf("After: REG.A: %d\n",regs.A);


	return 0;
}

//Stolen from CodeSlinger who stole it from someone else
int CPU::daa() { // 0x27
	cycles=4;
	if(regs.negative)
	{
		if((regs.A &0x0F ) >0x09 || regs.F &0x20 )
		{
			regs.A -=0x06; //Half borrow: (0-1) = (0xF-0x6) = 9
			if((regs.A&0xF0)==0xF0) regs.F|=0x10; else regs.F&=~0x10;
		}

		if((regs.A&0xF0)>0x90 || regs.F&0x10) regs.A-=0x60;
	}
	else
	{
		if((regs.A&0x0F)>9 || regs.F&0x20)
		{
			regs.A+=0x06; //Half carry: (9+1) = (0xA+0x6) = 10
			if((regs.A&0xF0)==0) regs.F|=0x10; else regs.F&=~0x10;
		}

		if((regs.A&0xF0)>0x90 || regs.F&0x10) regs.A+=0x60;
	}

	if(regs.A==0) regs.F|=0x80; else regs.F&=~0x80;

	return 0;
}



int CPU::add_hl_hl() { // 0x29
	add(regs.HL, regs.HL);
	return 0;
}

int CPU::cpl() { // 0x2F
	cycles = 4;
	regs.A = ~regs.A;
	regs.negative = 1;
	regs.halfCarry = 1;
	return 0;
}

int CPU::scf() { // 0x37
	cycles = 4;

	regs.carry = 1;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = 0;
	return 0;
}

int CPU::add_hl_sp() { // 0x39
	add(regs.HL, SP);
	return 0;
}

int CPU::ccf() { // 0x3F
	cycles = 4;
	regs.negative = 0;
	regs.halfCarry = 0;
	regs.carry = ~regs.carry;
	return 0;
}

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
	cycles = 4;
	uint16_t result = regs.A + ram->read(regs.HL);
	regs.halfCarry = (regs.A & 0x0F) + (ram->read(regs.HL) & 0x0F);
	regs.carry = result > 0xff;
	regs.negative = 0;
	return 0;
}
int CPU::add_a_a() { // 0x87
	add(regs.A, regs.A);
	return 0;
}
int CPU::adc_a_b() { // 0x88
	adc(regs.A, regs.B);
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
	uint8_t data= ram->read(regs.HL);
	adc(regs.A,data);
	return 0;
}
int CPU::adc_a_a() { // 0x8F
	adc(regs.A, regs.A);
	return 0;
}
int CPU::adc_a_d8() {	//0xCE
	uint8_t data= ram->read(PC);
	PC++;
	adc(regs.A,data);
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
	uint8_t data= ram->read(regs.HL);
	sub(regs.A,data);
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
	uint8_t data= ram->read(regs.HL);
	sbc(regs.A,data);
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
	uint8_t data= ram->read(regs.HL);
	_and(regs.A,data);
	return 0;
}
int CPU::and_a() { // 0xA7
	_and(regs.A, regs.A);
	return 0;
}

int CPU::and_d8() {	//0xE6
	uint8_t data=ram->read(PC);
	PC++;
	_and(regs.A,data);
	return 0;
}

int CPU::xor_b() { // 0xA8
	_xor(regs.A, regs.B);
	return 0;
}
int CPU::xor_c() { // 0xA9
	_xor(regs.A, regs.C);
	return 0;
}
int CPU::xor_d() { // 0xAA
	_xor(regs.A, regs.D);
	return 0;
}
int CPU::xor_e() { // 0xAB
	_xor(regs.A, regs.E);
	return 0;
}
int CPU::xor_h() { // 0xAC
	_xor(regs.A, regs.H);
	return 0;
}
int CPU::xor_l() { // 0xAD
	_xor(regs.A, regs.L);
	return 0;
}
int CPU::xor_hl() { // 0xAE
	uint8_t data= ram->read(regs.HL);
	_xor(regs.A, data);
	return 0;
}
int CPU::xor_a() { // 0xAF
	_xor(regs.A, regs.A);
	return 0;
}
int CPU::xor_d8() {	//0xEE
	cycles=8;
	uint8_t data=ram->read(PC);
	PC++;
	_xor(regs.A,data);
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
	uint8_t data= ram->read(regs.HL);
	_or(regs.A,data);
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
	uint8_t data= ram->read(regs.HL);
	cp(regs.A,data);
	return 0;
}
int CPU::cp_a() { // 0xB0
	cp(regs.A, regs.A);
	return 0;
}

int CPU::add_a_d8() {	//0xC6
	uint8_t data=ram->read(PC);
	PC++;
	add(regs.A,data);
	cycles=8;	//TODO: dont override the cycles set by the helper function
	return 0;
}
int CPU::rst_0() {	//0xC7
	rst(0);
	return 0;
}

int CPU::rst_1() {	//0xCF
	rst(1);
	return 0;
}

int CPU::sub_d8() { // 0xD5
	uint8_t data=ram->read(PC);
	PC++;
	sub(regs.A,data);
	return 0;
}
int CPU::rst_2() { // 0xD6
	rst(2);
	return 0;
}

int CPU::sbc_a_d8() { // 0xDE
	uint8_t data=ram->read(PC);
	PC++;
	sbc(regs.A,data);
	return 0;
}
int CPU::rst_3() { // 0xDF
	rst(3);
	return 0;
}

int CPU::rst_4() {	//0xE5
	rst(4);
	return 0;
}
int CPU::add_sp_s8() {	//0xE6
	//TODO: IMPLEMENT THIS
	return 0;
}

int CPU::rst_5() {	//0xEF
	rst(5);
	return 0;
}

/*0xF0*/

int CPU::di() {	//0xF3
	cycles=4;
	//TODO: probably some waiting I need to do here since change doesnt affect next instruction
	*IME=false;
	return 0;
}

int CPU::or_d8() {	//0xF6
	uint8_t data=ram->read(PC);
	PC++;
	_or(regs.A,data);
	return 0;
}
int CPU::rst_6() {	//0xF7
	rst(6);
	return 0;
}

int CPU::ei() {	//0xFB
	cycles=4;
	*IME=true;
	return 0;
}
int CPU::cp_d8() {	//0xFE
	cycles=8;
	uint8_t data=ram->read(PC);
	PC++;
	cp(regs.A,data);
	return 0;
}
int CPU::rst_7() {	//0xFF
	rst(7);
	return 0;
}

/*===========================================*/
/*Implementation of table 1 instructions above*/
/*===========================================*/
///////////////////////////////////////////////////
/*=========================================*/
/*Implementation table 2 instructions below*/
/*=========================================*/

/*0x00*/
int CPU::rlc_b() {
	rlc(regs.B);
	return 0;
}
int CPU::rlc_c() {
	rlc(regs.C);
	return 0;
}
int CPU::rlc_d() {
	rlc(regs.D);
	return 0;
}
int CPU::rlc_e() {
	rlc(regs.E);
	return 0;
}
int CPU::rlc_h() {
	rlc(regs.H);
	return 0;
}
int CPU::rlc_l() {
	rlc(regs.L);
	return 0;
}
int CPU::rlc_hl() {
	return 0;
}
int CPU::rlc_a() {
	rlc(regs.A);
	return 0;
}
int CPU::rrc_b() {
	rrc(regs.B);
	return 0;
}
int CPU::rrc_c() {
 	rrc(regs.C);
	return 0;
}
int CPU::rrc_d() {
	rrc(regs.D);
	return 0;
}
int CPU::rrc_e() {
	rrc(regs.E);
	return 0;
}
int CPU::rrc_h() {
	rrc(regs.H);
	return 0;
}
int CPU::rrc_l() {
	rrc(regs.L);
	return 0;
} 
int CPU::rrc_hl() {
	uint8_t data= ram->read(regs.HL);
	rrc(data);
	return 0;
}
int CPU::rrc_a() {
	rrc(regs.A);
	return 0;
}

/*0x01*/
int CPU::rl_b() {
	rl(regs.B);
	return 0;
}
int CPU::rl_c() {
	rl(regs.C);
	return 0;
}
int CPU::rl_d() {
	rl(regs.D);
	return 0;
}
int CPU::rl_e() {
	rl(regs.E);
	return 0;
}
int CPU::rl_h() {
	rl(regs.H);
	return 0;
}
int CPU::rl_l() {
	rl(regs.L);
	return 0;
}
int CPU::rl_hl() {
	uint8_t data= ram->read(regs.HL);
	rl(data);
	return 0;
}
int CPU::rl_a() {
	rl(regs.A);
	return 0;
}
int CPU::rr_b() {
	rr(regs.B);
	return 0;
}
int CPU::rr_c() {
	rr(regs.C);
	return 0;
}
int CPU::rr_d() {
	rr(regs.D);
	return 0;
}
int CPU::rr_e() {
	rr(regs.E);
	return 0;
}
int CPU::rr_h() {
	rr(regs.H);
	return 0;
}
int CPU::rr_l() {
	rr(regs.L);
	return 0;
}
int CPU::rr_hl() {
	uint8_t data= ram->read(regs.HL);
	rr(data);
	return 0;
}
int CPU::rr_a() {
	rr(regs.A);
	return 0;
}

/*0x02*/
int CPU::sla_b() {
	sla(regs.B);
	return 0;
}
int CPU::sla_c() {
	sla(regs.C);
	return 0;
}
int CPU::sla_d() {
	sla(regs.D);
	return 0;
}
int CPU::sla_e() {
	sla(regs.E);
	return 0;
}
int CPU::sla_h() {
	sla(regs.H);
	return 0;
}
int CPU::sla_l() {
	sla(regs.L);
	return 0;
}
int CPU::sla_hl() {
	uint8_t data= ram->read(regs.HL);
	sla(data);
	return 0;
}
int CPU::sla_a() {
	sla(regs.A);
	return 0;
}
int CPU::sra_b() {
	sra(regs.B);
	return 0;
}
int CPU::sra_c() {
	sra(regs.C);
	return 0;
}
int CPU::sra_d() {
	sra(regs.D);
	return 0;
}
int CPU::sra_e() {
	sra(regs.E);
	return 0;
}
int CPU::sra_h() {
	sra(regs.H);
	return 0;
}
int CPU::sra_l() {
	sra(regs.L);
	return 0;
}
int CPU::sra_hl() {
	uint8_t data= ram->read(regs.HL);
	sra(data);
	return 0;
}
int CPU::sra_a() {
	sra(regs.A);
	return 0;
}

/*0x03*/
int CPU::swap_b() {
	swap(regs.B);
	return 0;
}
int CPU::swap_c() {
	swap(regs.C);
	return 0;
}
int CPU::swap_d() {
	swap(regs.D);
	return 0;
}
int CPU::swap_e() {
	swap(regs.E);
	return 0;
}
int CPU::swap_h() {
	swap(regs.H);
	return 0;
}
int CPU::swap_l() {
	swap(regs.L);
	return 0;
}
int CPU::swap_hl() {
	uint8_t data= ram->read(regs.HL);
	swap(data);
	return 0;
}
int CPU::swap_a() {
	swap(regs.A);
	return 0;
}
int CPU::srl_b() {
	srl(regs.B);
	return 0;
}
int CPU::srl_c() {
	srl(regs.C);
	return 0;
}
int CPU::srl_d() {
	srl(regs.D);
	return 0;
}
int CPU::srl_e() {
	srl(regs.E);
	return 0;
}
int CPU::srl_h() {
	srl(regs.H);
	return 0;
}
int CPU::srl_l() {
	srl(regs.L);
	return 0;
}
int CPU::srl_hl() {
	uint8_t data= ram->read(regs.HL);
	srl(data);
	return 0;
}
int CPU::srl_a() {
	srl(regs.A);
	return 0;
}

/*0x04*/
int CPU::bit_0_b() {
	BIT(0,regs.B);
	return 0;
}
int CPU::bit_0_c() {
	BIT(0,regs.C);
	return 0;
}
int CPU::bit_0_f() {
	BIT(0,regs.F);
	return 0;
}
int CPU::bit_0_e() {
	BIT(0,regs.E);
	return 0;
}
int CPU::bit_0_h() {
	BIT(0,regs.H);
	return 0;
}
int CPU::bit_0_l() {
	BIT(0,regs.L);
	return 0;
}
int CPU::bit_0_hl() {
	uint8_t data= ram->read(regs.HL);
	BIT(0,data);
	return 0;
}
int CPU::bit_0_a() {
	BIT(0,regs.A);
	return 0;
}
int CPU::bit_1_b() {
	BIT(1,regs.B);
	return 0;
}
int CPU::bit_1_c() {
	BIT(1,regs.C);
	return 0;
}
int CPU::bit_1_f() {
	BIT(1,regs.F);
	return 0;
}
int CPU::bit_1_e() {
	BIT(1,regs.E);
	return 0;
}
int CPU::bit_1_h() {
	BIT(1,regs.H);
	return 0;
}
int CPU::bit_1_l() {
	BIT(1,regs.L);
	return 0;
}
int CPU::bit_1_hl() {
	uint8_t data= ram->read(regs.HL);
	BIT(1,data);
	return 0;
}
int CPU::bit_1_a() {
	BIT(1,regs.A);
	return 0;
}

/*0x05*/
int CPU::bit_2_b() {
	BIT(2,regs.B);
	return 0;
}
int CPU::bit_2_c() {
	BIT(2,regs.C);
	return 0;
}
int CPU::bit_2_f() {
	BIT(2,regs.F);
	return 0;
}
int CPU::bit_2_e() {
	BIT(2,regs.E);
	return 0;
}
int CPU::bit_2_h() {
	BIT(2,regs.H);
	return 0;
}
int CPU::bit_2_l() {
	BIT(2,regs.L);
	return 0;
}
int CPU::bit_2_hl(){
	uint8_t data= ram->read(regs.HL);
	BIT(2,data);
	return 0;
}
int CPU::bit_2_a() {
	BIT(2,regs.A);
	return 0;
}
int CPU::bit_3_b() {
	BIT(3,regs.B);
	return 0;
}
int CPU::bit_3_c() {
	BIT(3,regs.C);
	return 0;
}
int CPU::bit_3_f() {
	BIT(3,regs.F);
	return 0;
}
int CPU::bit_3_e() {
	BIT(3,regs.E);
	return 0;
}
int CPU::bit_3_h() {
	BIT(3,regs.H);
	return 0;
}
int CPU::bit_3_l() {
	BIT(3,regs.L);
	return 0;
}
int CPU::bit_3_hl() {
	uint8_t data= ram->read(regs.HL);
	BIT(3,data);
	return 0;
}
int CPU::bit_3_a() {
	BIT(3,regs.A);
	return 0;
}

/*0x06*/
int CPU::bit_4_b() {
	BIT(4,regs.B);
	return 0;
}
int CPU::bit_4_c() {
	BIT(4,regs.C);
	return 0;
}
int CPU::bit_4_f() {
	BIT(4,regs.F);
	return 0;
}
int CPU::bit_4_e() {
	BIT(4,regs.E);
	return 0;
}
int CPU::bit_4_h() {
	BIT(4,regs.H);
	return 0;
}
int CPU::bit_4_l() {
	BIT(4,regs.L);
	return 0;
}
int CPU::bit_4_hl() {
	uint8_t data= ram->read(regs.HL);
	BIT(4,data);
	return 0;
}
int CPU::bit_4_a() {
	BIT(4,regs.A);
	return 0;
}
int CPU::bit_5_b() {
	BIT(5,regs.B);
	return 0;
}
int CPU::bit_5_c() {
	BIT(5,regs.C);
	return 0;
}
int CPU::bit_5_f() {
	BIT(5,regs.F);
	return 0;
}
int CPU::bit_5_e() {
	BIT(5,regs.E);
	return 0;
}
int CPU::bit_5_h() {
	BIT(5,regs.H);
	return 0;
}
int CPU::bit_5_l() {
	BIT(5,regs.L);
	return 0;
}
int CPU::bit_5_hl() {
	uint8_t data= ram->read(regs.HL);
	BIT(5,data);
	return 0;
}
int CPU::bit_5_a() {
	BIT(5,regs.A);
	return 0;
}

/*0x07*/
int CPU::bit_6_b() {
	BIT(6,regs.B);
	return 0;
}
int CPU::bit_6_c() {
	BIT(6,regs.C);
	return 0;
}
int CPU::bit_6_f() {
	BIT(6,regs.F);
	return 0;
}
int CPU::bit_6_e() {
	BIT(6,regs.E);
	return 0;
}
int CPU::bit_6_h() {
	BIT(6,regs.H);
	return 0;
}
int CPU::bit_6_l() {
	BIT(6,regs.L);
	return 0;
}
int CPU::bit_6_hl() {
	uint8_t data= ram->read(regs.HL);
	BIT(6,data);
	return 0;
}
int CPU::bit_6_a() {
	BIT(6,regs.A);
	return 0;
}
int CPU::bit_7_b() {
	BIT(7,regs.B);
	return 0;
}
int CPU::bit_7_c() {
	BIT(7,regs.C);
	return 0;
}
int CPU::bit_7_f() {
	BIT(7,regs.F);
	return 0;
}
int CPU::bit_7_e() {
	BIT(7,regs.E);
	return 0;
}
int CPU::bit_7_h() {
	BIT(7,regs.H);
	return 0;
}
int CPU::bit_7_l() {
	BIT(7,regs.L);
	return 0;
}
int CPU::bit_7_hl() {
	uint8_t data= ram->read(regs.HL);
	BIT(7,data);
	return 0;
}
int CPU::bit_7_a() {
	BIT(7,regs.A);
	return 0;
}

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
	uint8_t data= ram->read(regs.HL);
	res(data,0);
	return 0;
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
int CPU::res_1_hl() {
	uint8_t data= ram->read(regs.HL);
	res(data,1);
	return 0;
}
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
	uint8_t data= ram->read(regs.HL);
	res(data,2);
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
	uint8_t data= ram->read(regs.HL);
	res(data,3);
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
	uint8_t data= ram->read(regs.HL);
	res(data,4);
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
	uint8_t data= ram->read(regs.HL);
	res(data,5);
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
	uint8_t data= ram->read(regs.HL);
	set(data,0);
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
	uint8_t data= ram->read(regs.HL);
	set(data,3);
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
	uint8_t data= ram->read(regs.HL);
	set(data,4);
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
	uint8_t data= ram->read(regs.HL);
	set(data,5);
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
	uint8_t data= ram->read(regs.HL);
	set(data,6);
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
	uint8_t data= ram->read(regs.HL);
	set(data,7);
	return 0;
}
int CPU::set_7_a() {
	set(regs.A,7);
	return 0;
}
