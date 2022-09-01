#include "headers/CPU.h"

/**==CPU INSTRUCTION IMPLEMENTATIONS==**/

/*=========================================*/
/****************Instructions***************/
/*=========================================*/
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
	//printf("==LOAD from HL==\n");
	//printf("Before: Reg1: %X, HL: %X",regs.B,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.B = operand;
	//printf("After: Reg1: %X, HL: %X",regs.B,regs.HL);
	return 0;
}
int CPU::ld_d_hl() { // 0x56
	//printf("==LOAD from HL==\n");
	//printf("Before: Reg1: %X, HL: %X",regs.D,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.D = operand;
	//printf("After: Reg1: %X, HL: %X",regs.D,regs.HL);
	return 0;
}
int CPU::ld_h_hl() { // 0x66
	//printf("==LOAD from HL==\n");
	//printf("Before: Reg1: %X, HL: %X",regs.H,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.H = operand;
	//printf("After: Reg1: %X, HL: %X",regs.H,regs.HL);
	return 0;
}

int CPU::ld_c_hl() { // 0x4E
	//printf("==LOAD from HL==\n");
	//printf("Before: Reg1: %X, HL: %X",regs.C,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.C = operand;
	//printf("After: Reg1: %X, HL: %X",regs.C,regs.HL);
	return 0;
}
int CPU::ld_e_hl() { // 0x5E
	//printf("==LOAD from HL==\n");
	//printf("Before: Reg1: %X, HL: %X",regs.E,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.E = operand;
	//printf("After: Reg1: %X, HL: %X",regs.E,regs.HL);
	return 0;
}
int CPU::ld_l_hl() { // 0x6E
	//printf("==LOAD from HL==\n");
	//printf("Before: Reg1: %X, HL: %X",regs.L,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.L = operand;
	//printf("After: Reg1: %X, HL: %X",regs.L,regs.HL);
	return 0;
}
int CPU::ld_a_hl() {	//0x7E
	//printf("==LOAD from HL==\n");
	//printf("Before: Reg1: %X, HL: %X",regs.A,regs.HL);
	cycles = 8;
	uint8_t operand = ram->read(regs.HL);
	regs.A = operand;
	//printf("After: Reg1: %X, HL: %X",regs.A,regs.HL);
	return 0;
}
int CPU::ld_a_hlp() { // 0x2A
	regs.A=ram->read(regs.HL);
	regs.HL++;
	return 0;
}
int CPU::ld_a_hlm() { // 0x3A
	cycles = 8;
	regs.A = ram->read(regs.HL);
	regs.HL--;
	return 0;
}
int CPU::ld_a_bc() { // 0x0A
	cycles = 8;
	regs.A = ram->read(regs.BC);
	return 0;
}
int CPU::ld_a_de() { // 0x1A
	cycles = 8;
	regs.A = ram->read(regs.DE);
	return 0;
}
int CPU::ld_a_c_mem() {	//0xF2
	cycles=8;
	uint16_t addr=regs.C+0xFF00;
	regs.A=ram->read(addr);
	return 0;
}
int CPU::ld_a_a8() {	//0xF0
	cycles=12;
	uint16_t addr=ram->read(PC)+0xFF00;
	PC++;
	regs.A=ram->read(addr);
	return 0;
}
int CPU::ld_a_a16() {	//0xFA
	cycles=16;
	uint16_t a16=readWord();
	PC+=2;
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
	//printf("==LOAD A INTO MEM==\n");
	uint16_t addr=readWord();
	PC+=2;
	//printf("Before: %X\n",ram->read(addr));
	ram->write(addr,regs.A);
	//printf("After: %X\n",ram->read(addr));
	return 0;
}
int CPU::ld_a16_sp() { // 0x08
	//TODO: double check
	int cycles = 20;
	//printf("==LOAD SP INTO MEM==\n");
	uint16_t addr=readWord();
	PC+=2;

	ram->write(addr,SP&0x00FF);
	ram->write(addr+1,(SP&0xFF00)>>8);
	return 0;
}
int CPU::ld_a8_a() {	//0xE0
	cycles=12;
	//printf("==LOAD A INTO MEM==\n");
	uint16_t addr=ram->read(PC)+0xFF00;
	PC++;
	//printf("Before: %X\n",ram->read(addr));
	ram->write(addr,regs.A);
	//printf("After: %X\n",ram->read(addr));
	return 0;
}
int CPU::ld_c_mem_a() {	//0xE2
	cycles=8;
	uint16_t addr=regs.C+0xFF00;
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
int CPU::halt() {	//0x76
	HALT=true;
	return 0;
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
	SP=regs.HL;
	return 0;
}
int CPU::ld_hl_sp_s8() {	//0xF8
	int8_t s8=ram->read(PC);
	int result=SP+s8;

	regs.zero=0;
	regs.negative=0;
	regs.halfCarry=(((SP ^ s8 ^ (result & 0xFFFF)) & 0x10) == 0x10);
	regs.carry=(((SP ^ s8 ^ (result & 0xFFFF)) & 0x100) == 0x100);
	PC++;
	regs.HL=result;
}


/*===============================JUMP INSTRUCTIONS===============================*/
int CPU::jr_s8() { // 0x18
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
	//TODO: implement
	ei();
	_ret(1);
	return 0;
}


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
	contents++;

	regs.halfCarry = ((contents & 0x0F) == 0x00);
	regs.negative = 0;
	regs.zero = (contents==0);
	ram->write(regs.HL, contents);
	return 0;
}
int CPU::inc_a() { // 0x3C
	inc_reg(regs.A);
	return 0;
}

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
	contents--;

	regs.zero=(contents==0);
	regs.halfCarry = ((contents & 0x0F) == 0x0F);
	regs.negative = 1;

	ram->write(regs.HL, contents);

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
	cycles=16;
	uint16_t data = regs.AF&0xFFF0;
	push(data);
	return 0;
}


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


/*===============================MISC INSTRUCTIONS===============================*/

int CPU::nop() { // 0x00
	//printf("NOP\n");
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

	regs.carry = ((regs.A & 0x80)>0);
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
	regs.carry = ((regs.A & 0x01)>0);
	regs.A = regs.A >> 1;
	regs.A |= (regs.carry<<7);
	return 0;
}
int CPU::stop() { // 0x10
	return 0;
}
int CPU::rla() { // 0x17
	cycles=4;
	regs.zero = 0;
	regs.negative = 0;
	regs.halfCarry = 0;


	int carry = regs.carry;

	regs.carry = ((regs.A & 0x80)>0);

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
	//printf("===RRA===\n");
	//printf("Before: REG.A: %d\n",regs.A);

	bool isCarrySet = regs.carry;
	bool isLSBSet = (regs.A&0x01)>0;

	regs.A >>= 1;

	if (isLSBSet)
		regs.carry=1;
	else{
		regs.carry=0;
	}

	if (isCarrySet)
		regs.A = bitSet(regs.A, 7);

	regs.halfCarry=0;
	regs.negative=0;
	regs.zero=0;

	//printf("After: REG.A: %d\n",regs.A);

	return 0;
}

//adapted from jgilchrist on github
int CPU::daa() { // 0x27
	cycles=4;

	uint8_t reg = regs.A;

    uint16_t correction = regs.carry
        ? 0x60
        : 0x00;

    if (regs.halfCarry || (!regs.negative && ((reg & 0x0F) > 9))) {
        correction |= 0x06;
    }

    if (regs.carry || (!regs.negative && (reg > 0x99))) {
        correction |= 0x60;
    }

    if (regs.negative) {
        reg = (reg - correction);
    } else {
        reg = (reg + correction);
    }

    if (((correction << 2) & 0x100) != 0) {
        regs.carry=1;
    }

    regs.halfCarry=0;
    regs.zero=(reg==0);

    regs.A=reg;

	return 0;
}
int CPU::di() {	//0xF3
	cycles=4;
	//TODO: probably some waiting I need to do here since change doesnt affect next instruction
	*IME=false;
	return 0;
}
int CPU::ei() {	//0xFB
	cycles=4;
	*IME=true;
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
/*===============================ADD INSTRUCTIONS===============================*/
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
	uint8_t reg_hl_contents=ram->read(regs.HL);
	
	uint16_t result = regs.A + reg_hl_contents;
	regs.halfCarry=((reg_hl_contents&0x0f)+(regs.A&0x0f)>0xf);
	regs.carry = ((result & 0x100)!=0);
	regs.negative = 0;
	regs.A=result;
	regs.zero=(regs.A==0);
	return 0;
}
int CPU::add_a_a() { // 0x87
	add(regs.A, regs.A);
	return 0;
}
int CPU::add_a_d8() {	//0xC6
	uint8_t data=ram->read(PC);
	PC++;
	add(regs.A,data);
	cycles=8;	//TODO: dont override the cycles set by the helper function
	return 0;
}
int CPU::add_sp_s8() {	//0xE6

	int8_t s8=ram->read(PC);
	int result=SP+s8;

	regs.zero=0;
	regs.negative=0;
	regs.halfCarry=(((SP ^ s8 ^ (result & 0xFFFF)) & 0x10) == 0x10);
	regs.carry=(((SP ^ s8 ^ (result & 0xFFFF)) & 0x100) == 0x100);
	PC++;
	SP=result;
	return 0;
}
/*===============================ADC INSTRUCTIONS===============================*/
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

/*===============================SUB INSTRUCTIONS===============================*/
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


/*===============================SBC INSTRUCTIONS===============================*/
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
int CPU::sbc_a_d8() { // 0xDE
	uint8_t data=ram->read(PC);
	PC++;
	sbc(regs.A,data);
	return 0;
}
/*===============================AND INSTRUCTIONS===============================*/
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


/*===============================XOR INSTRUCTIONS===============================*/
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


/*===============================OR INSTRUCTIONS===============================*/
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
int CPU::or_d8() {	//0xF6
	uint8_t data=ram->read(PC);
	PC++;
	_or(regs.A,data);
	return 0;
}


/*===============================CP INSTRUCTIONS===============================*/
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
int CPU::cp_d8() {	//0xFE
	cycles=8;
	uint8_t data=ram->read(PC);
	PC++;
	cp(regs.A,data);
	return 0;
}


/*===============================RST INSTRUCTIONS===============================*/
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
int CPU::rst_3() { // 0xDF
	rst(3);
	return 0;
}
int CPU::rst_4() {	//0xE5
	rst(4);
	return 0;
}
int CPU::rst_5() {	//0xEF
	rst(5);
	return 0;
}
int CPU::rst_6() {	//0xF7
	rst(6);
	return 0;
}
int CPU::rst_7() {	//0xFF
	rst(7);
	return 0;
}


/*===============================RLC INSTRUCTIONS===============================*/
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
	uint8_t contents= ram->read(regs.HL);
	rlc(contents);
	ram->write(regs.HL,contents);
	return 0;
}
int CPU::rlc_a() {
	rlc(regs.A);
	return 0;
}


/*===============================RRC INSTRUCTIONS===============================*/
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
	ram->write(regs.HL,data);

	return 0;
}
int CPU::rrc_a() {
	rrc(regs.A);

	return 0;
}


/*===============================RL INSTRUCTIONS===============================*/
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
	ram->write(regs.HL,data);
	return 0;
}
int CPU::rl_a() {
	rl(regs.A);
	return 0;
}


/*===============================RR INSTRUCTIONS===============================*/
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
	ram->write(regs.HL,data);
	return 0;
}
int CPU::rr_a() {
	rr(regs.A);
	return 0;
}


/*===============================SLA INSTRUCTIONS===============================*/
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
	ram->write(regs.HL,data);
	return 0;
}
int CPU::sla_a() {
	sla(regs.A);
	return 0;
}


/*===============================SRA INSTRUCTIONS===============================*/
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
	ram->write(regs.HL,data);

	return 0;
}
int CPU::sra_a() {
	sra(regs.A);
	return 0;
}


/*===============================SWAP INSTRUCTIONS===============================*/
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
	ram->write(regs.HL,data);

	return 0;
}
int CPU::swap_a() {
	swap(regs.A);
	return 0;
}


/*===============================SRL INSTRUCTIONS===============================*/
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
	ram->write(regs.HL,data);

	return 0;
}
int CPU::srl_a() {
	srl(regs.A);
	return 0;
}


/*===============================BIT INSTRUCTIONS===============================*/
int CPU::bit_0_b() {
	BIT(0,regs.B);
	return 0;
}
int CPU::bit_0_c() {
	BIT(0,regs.C);
	return 0;
}
int CPU::bit_0_d() {
	BIT(0,regs.D);
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
	ram->write(regs.HL,data);
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
int CPU::bit_1_d() {
	BIT(1,regs.D);
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
	ram->write(regs.HL,data);
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
int CPU::bit_2_d() {
	BIT(2,regs.D);
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
	ram->write(regs.HL,data);

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
int CPU::bit_3_d() {
	BIT(3,regs.D);
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
	ram->write(regs.HL,data);

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
int CPU::bit_4_d() {
	BIT(4,regs.D);
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
	ram->write(regs.HL,data);

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
int CPU::bit_5_d() {
	BIT(5,regs.D);
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
	ram->write(regs.HL,data);

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
int CPU::bit_6_d() {
	BIT(6,regs.D);
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
	ram->write(regs.HL,data);

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
int CPU::bit_7_d() {
	BIT(7,regs.D);
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
	ram->write(regs.HL,data);

	return 0;
}
int CPU::bit_7_a() {
	BIT(7,regs.A);
	return 0;
}


/*===============================RES INSTRUCTIONS===============================*/
int CPU::res_0_b() {
	res(regs.B,0);
	return 0;
}
int CPU::res_0_c() {
	res(regs.C,0);
	return 0;
}
int CPU::res_0_d() {
	res(regs.D,0);
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
	ram->write(regs.HL,data);

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
int CPU::res_1_d() {
	res(regs.D,1);
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
	ram->write(regs.HL,data);

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
int CPU::res_2_d() {
	res(regs.D,2);
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
	ram->write(regs.HL,data);

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
int CPU::res_3_d() {
	res(regs.D,3);
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
	ram->write(regs.HL,data);

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
int CPU::res_4_d() {
	res(regs.D,4);
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
	ram->write(regs.HL,data);

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
int CPU::res_5_d() {
	res(regs.D,5);
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
	ram->write(regs.HL,data);

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

int CPU::res_6_d() {
	res(regs.D,6);
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
	uint8_t data= ram->read(regs.HL);
	res(data,6);
	ram->write(regs.HL,data);
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
int CPU::res_7_d() {
	res(regs.D,7);
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
	uint8_t data= ram->read(regs.HL);
	res(data,7);
	ram->write(regs.HL,data);

	return 0;
}
int CPU::res_7_a() {
	res(regs.A,7);
	return 0;
}


/*===============================SET INSTRUCTIONS===============================*/
/*0x0C*/
int CPU::set_0_b() {
	set(regs.B,0);
	return 0;
}
int CPU::set_0_c() {
	set(regs.C,0);
	return 0;
}
int CPU::set_0_d() {
	set(regs.D,0);
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
	ram->write(regs.HL,data);

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
int CPU::set_1_d() {
	set(regs.D,1);
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
	uint8_t data= ram->read(regs.HL);
	set(data,1);
	ram->write(regs.HL,data);
	return 0;
}
int CPU::set_1_a() {
	set(regs.A,1);
	return 0;
}
/*0x0D*/
int CPU::set_2_b() {
	set(regs.B,2);
	return 0;
}
int CPU::set_2_c() {
	set(regs.C,2);
	return 0;
}
int CPU::set_2_d() {
	set(regs.D,2);
	return 0;
}
int CPU::set_2_e() {
	set(regs.E,2);
	return 0;
}
int CPU::set_2_h() {
	set(regs.H,2);
	return 0;
}
int CPU::set_2_l() {
	set(regs.L,2);
	return 0;
}int CPU::set_2_hl() {
	uint8_t data= ram->read(regs.HL);
	set(data,2);
	ram->write(regs.HL,data);
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
int CPU::set_3_e() {
	set(regs.E,3);
	return 0;
}
int CPU::set_3_d() {
	set(regs.D,3);
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
	ram->write(regs.HL,data);

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
int CPU::set_4_d() {
	set(regs.D,4);
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
	ram->write(regs.HL,data);
	return 0;
}
int CPU::set_4_a() {
	set(regs.A,4);
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
int CPU::set_5_d() {
	set(regs.D,5);
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
	ram->write(regs.HL,data);

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
int CPU::set_6_d() {
	set(regs.D,6);
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
	ram->write(regs.HL,data);

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
int CPU::set_7_d() {
	set(regs.D,7);
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
	ram->write(regs.HL,data);

	return 0;
}
int CPU::set_7_a() {
	set(regs.A,7);
	return 0;
}
