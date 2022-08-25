#include "headers/CPU.h"
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
 
#define INTERRUPT_ENABLE 0xFFFF
#define INTERRUPT_FLAGS 0xFF0F


/*System interface*/
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
			&CPU::ld_hl_b,     &CPU::ld_hl_c,    &CPU::ld_hl_d,   &CPU::ld_hl_e,&CPU::ld_hl_h,     &CPU::ld_hl_l,    &CPU::halt,      &CPU::ld_hl_a,&CPU::ld_a_b,      &CPU::ld_a_c,     &CPU::ld_a_d,    &CPU::ld_a_e,	&CPU::ld_a_h,      &CPU::ld_a_l,     &CPU::ld_a_hl,   &CPU::ld_a_a,
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
int CPU::fetchExecute(){
	//TODO: halt means fetch execture isnt done
	if(HALT==false){
		fetch();
		execute();
	}
	return cycles;
}

/*For testing*/
void CPU::setPC(uint16_t _PC){
	PC=_PC;
}
uint16_t CPU::getPC(){
	return PC;
}
uint16_t CPU::getInstruction(){
	return instruction;
}
void CPU::getRegs(){
	std::cout<<"BC="<<std::uppercase<<std::hex<<regs.BC<<" "
	<<"DE="<<std::uppercase<<std::hex<<regs.DE<<" "
	<<"HL="<<std::uppercase<<std::hex<<regs.HL<<" "
	<<"AF="<<std::uppercase<<std::hex<<regs.AF<<" "
	<<"SP="<<std::uppercase<<std::hex<<SP<<" "
	<<"PC="<<std::uppercase<<std::hex<<PC<<" ";
}



/*Execution*/
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


/*Interrupts*/
void CPU::requestInterrupt(int interrupt){

}


/*Helpers*/
void CPU::pushWordToStack(uint16_t data){
	uint8_t hi = data >> 8 ;
	uint8_t lo = data & 0xFF;
	SP-- ;
	ram->write(SP, hi) ;
	SP-- ;
	ram->write(SP,lo) ;
}
uint16_t CPU::popWordOffStack(){
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
bool CPU::testBit(uint8_t data, int bit){
	return (data&(1<<bit));
}
uint8_t CPU::bitSet(uint8_t data, int bit){
	return (data|(1<<bit));
}
bool CPU::getHalt(){
	return HALT;
}
void CPU::setHalt(bool _HALT){
	HALT=_HALT;
}
bool CPU::getFlag(std::string flag){
	if(flag=="carry"){
		return regs.carry==1;
	}
	else if(flag=="halfCarry"){
		return regs.halfCarry==1;
	}
	else if(flag=="negative"){
		return regs.negative==1;
	}
	else if(flag=="zero"){
		return regs.zero==1;
	}
}

/*===========================================*/
/***********Instruction "templates"***********/
/*===========================================*/
int CPU::ld_reg_addr(uint8_t &reg1, uint8_t data) {	//TODO: misleading name
	//printf("==LOAD REG FROM MEM==\n");
	cycles = 8;
	reg1 = data;
	return 0;
}
int CPU::ld_reg_reg(uint8_t &reg1, uint8_t reg2) {
	//printf("==LOAD FROM REG==\n");
	//printf("Before: Reg1: %x, Reg2: %x\n", reg1,reg2);
	cycles = 4;
	reg1 = reg2;
	//printf("After: Reg1: %x, Reg2: %x\n",reg1,reg2);
	return 0;
}
int CPU::ld_reg_d8(uint8_t &reg) {
	//printf("==LOAD FROM MEM==\n");
	//printf("Before: Reg1: %X\n",reg);
	cycles = 8;
	uint8_t imm = ram->read(PC);
	PC++;
	reg = imm;
	//printf("After: Reg1: %X\n",reg);
	return 0;
}
int CPU::ld_reg_d16(uint16_t &reg1) {
	//printf("==LOAD FROM MEM==\n");
	//printf("Before: Reg1: %X\n",reg1);
	cycles = 12;
	uint16_t data = readWord();
	PC+=2;
	reg1 = data;
	//printf("After: Reg1: %X\n",reg1);
	return 0;
}

int CPU::inc_reg(uint8_t &reg) {
	//printf("==INCREMENT REG==\n");
	uint8_t before = reg ;
	reg++;
	regs.zero=(reg==0);
	regs.negative=0;
	if ((before & 0xF) == 0xF)
		regs.halfCarry=1;
	else
		regs.halfCarry=0;
	//printf("After Reg1: %X\n",reg);
}


int CPU::inc_reg(uint16_t &reg) {
	//printf("==INCREMENT REG==\n");
	cycles = 8;
	reg++;
	return 0;
}

int CPU::dec_reg(uint8_t &reg) {
	cycles = 4;
	//printf("==DECREMENT REG==\n");
	//printf("Before: reg: %X\n", reg);
	uint8_t before = reg ;
	reg-- ;
	regs.zero=(reg==0);
	regs.negative=1;
	if ((before & 0x0F) == 0)
		regs.halfCarry=1;
	else
		regs.halfCarry=0;
	//printf("After: reg: %X\n", reg);
}
int CPU::dec_reg(uint16_t &reg) {
	//printf("==DECREMENT REG==\n");
	cycles = 8;
	reg--;
	return 0;
}
int CPU::ld_mem_a(uint16_t &addr) {
	//printf("==LOAD A TO MEM==\n");
	cycles = 8;
	ram->write(addr, regs.A);
	return 0;
}
int CPU::jr(bool condition) {
	//printf("==Jump PC==\n");
	//printf("Before: PC:%X \n",PC);
	if (condition) {
		cycles = 12;
		int8_t s8 = ram->read(PC);
		PC++;
		PC = PC + s8;
	} else {
		cycles = 8;
		PC++;
	}
	//printf("After: PC:%X \n",PC);
	return 0;
}
int CPU::add(uint8_t &reg1, uint8_t &reg2) {
	//printf("==ADD==\n");
	cycles = 4;
	uint16_t result = reg1 + reg2;
	
	regs.zero=((result&0xff)==0);
	regs.halfCarry = (((reg1 & 0x0F) + (reg2 & 0x0F))>0x0f);
	regs.carry = result > 0xff;
	regs.negative = 0;
	reg1=reg1 + reg2;
	return 0;
}
int CPU::add(uint16_t &reg1, uint16_t &reg2){
	//printf("==ADD==\n");
	cycles = 8;
	regs.negative = 0;
	uint32_t result = reg1 + reg2;
	regs.carry = result > 0xffff;
	regs.halfCarry = ((reg1 & 0x0fff) + (reg2 & 0x0fff) > 0x0fff);
	regs.negative = 0;
	return 0;
}
int CPU::adc(uint8_t &reg1, uint8_t &reg2) {
	//printf("==ADC==\n");
	cycles=4;
	int carry = (regs.carry == 1);
	uint16_t result = reg1 + reg2 + carry;
	regs.zero = ((result&0xFF) == 0);
	regs.carry = (result > 0xff);
	regs.halfCarry = (((reg1 & 0x0F) + (reg2 & 0x0F) + carry) > 0x0F);
	regs.negative = 0;
	reg1 = (int8_t)result & 0xff;
	return 0;
}
int CPU::sub(uint8_t &reg1, uint8_t &reg2) {
	//printf("==SUB==\n");
	uint16_t result = reg1 - reg2;
	regs.carry = (reg2 > reg1);
	regs.halfCarry = ((reg2 & 0x0f) > (reg1 & 0x0f));
	reg1 = reg1 - reg2;
	regs.zero = (reg1 == 0);
	regs.negative = 1;
	return 0;
}
int CPU::sbc(uint8_t &reg1, uint8_t &reg2) {
	//printf("==SBC==\n");
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
	//printf("==AND==\n");
	reg1 = reg1 & reg2;
	regs.carry = 0;
	regs.halfCarry = 1;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_and(uint8_t &reg1, uint8_t &reg2) {
	//printf("==AND==\n");
	cycles=4;
	reg1 = reg1 & reg2;
	regs.carry = 0;
	regs.halfCarry = 1;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}

int CPU::_xor(uint8_t &reg1, uint8_t &reg2) {
	//printf("==XOR==\n");
	cycles=4;
	reg1 ^= reg2;
	regs.carry = 0;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
int CPU::_or(uint8_t &reg1, uint8_t &reg2) {
	//printf("==OR==\n");
	cycles=4;
	reg1 |= reg2;
	regs.carry = 0;
	regs.halfCarry = 0;
	regs.negative = 0;
	regs.zero = (reg1 == 0);
	return 0;
}
//stolen from code slinger
int CPU::cp(uint8_t reg1, uint8_t reg2) { // doesnt actually store result of subtraction anywhere
	//printf("==CP==\n");
	cycles=4;
	uint8_t before = reg1 ;
	uint8_t toSubtract = reg2 ;
	reg1 -= toSubtract ;
	//regs.zero=(reg1==0);
	if (reg1 == 0)
		regs.zero=1;
	
	//else{
	//	regs.zero=0;	//TODO: This is behaving very weird in blargg tests. Pretty sure this is correct though
	//}

	regs.negative=1;
	// set if no borrow
	if (before < toSubtract)
		regs.carry=1;
	else{
		regs.carry=0;
	}
	signed short htest = before & 0xF ;
	htest -= (toSubtract & 0xF) ;

	if (htest < 0)
		regs.halfCarry=1;
	else{
		regs.halfCarry=0;
	}
	return 0;
}
int CPU::BIT(int bit, uint8_t & reg){	
	//printf("==BIT==\n");
	uint8_t bitResult=reg&(1<<bit);	//bitResult will store the and of the result and the 
									//bit in question
	regs.zero=((bitResult==0));	//instead of complementing, just set if reset, and reset if set...
	regs.negative=0;
	regs.halfCarry=1;
	return 0;
}
int CPU::pop(uint16_t &reg1) {
	cycles=12;
	//printf("==POP REG FROM STACK==\n");
	uint16_t data=popWordOffStack();
	reg1 =data;
	//printf("TOS is %X\n",data);
	return 0;
}
int CPU::push(uint16_t &reg1) {
	//printf("==PUSH REG TO STACK==\n");
	cycles=16;
	//push pc to stack then jump
	pushWordToStack(reg1);
	return 0;
}
int CPU::rst(int index) {
	//printf("==RST==\n");
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
	//printf("==RETURNING==\n");
	if(condition){
		PC = popWordOffStack() ;
		//printf("Return address=%X\n",PC);
		return 0;
	}
		//printf("Condition false\n");
	return 0;
}
int CPU::jp_a16(bool condition){
	//printf("==Jump==\n");
	//printf("PC before: %X\n",PC);
	uint16_t a16=readWord();
	PC+=2;
	if(condition){
		cycles=16;
		PC=a16;
	}
	else{
		cycles=12;
	}
	//printf("PC after: %X\n",PC);
	return 0;
}
int CPU::call(bool condition){	
	//printf("==CALL==\n");
	cycles+=12 ;
	uint16_t nn = readWord() ;
	PC += 2;
	//printf("Jump to %X from %X\n",nn,PC);

	if(condition){
		pushWordToStack(PC) ;
		PC = nn ;
	}
	return 0;
}
int CPU::rlc(uint8_t & reg){
	//printf("==rlc==\n");

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
	//printf("==rrc==\n");
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
	//printf("==rl==\n");
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
	//printf("==rr==\n");
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
	//printf("==SLA==\n");
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
	//printf("==SRA==\n");
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
	//printf("==swap==\n");
	//printf("==swap==\n");
	cycles=8;
	reg = (((reg & 0xF0) >> 4) | ((reg & 0x0F) << 4));

	if (reg == 0){
		regs.zero=1;
	}
	else{
		regs.zero=0;
	}
}
int CPU::srl(uint8_t & reg){
	//WHEN EDITING THIS FUNCTION ALSO EDIT CPU_SRL_MEMORY
	//printf("==SRL==\n");
	bool isLSBSet = testBit(reg,0) ;
	regs.F = 0 ;
	reg >>= 1;
	if (isLSBSet)
		regs.carry=1 ;
	if (reg == 0)
		regs.F = (regs.zero==1) ;
}


int CPU::res(uint8_t & reg, int pos){	//reset bit at offset
	//printf("==RES==\n");
	cycles=8;
	int offsetBin=1<<pos;
	offsetBin=~offsetBin;
	reg=reg&offsetBin;	//and reg with the inverse of its "offset"
	return 0;
}
int CPU::set(uint8_t & reg, int pos){ //set bit at offset
	//printf("==SET==\n");
	cycles=8;
	int offsetBin=1<<pos;
	reg=reg|offsetBin;	//and reg with the inverse of its "offset"
	return 0;
}


