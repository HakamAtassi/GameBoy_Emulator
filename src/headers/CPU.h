#ifndef CPU_H_
#define CPU_H_

#include "RAM.h"
#include "Registers.h"
#include "Cartridge.h"
#include <cstdint>
#include <vector>
#include <memory>

class CPU {


	public:
		CPU();
		CPU(RAM * _ram);
		
		void write(uint16_t addr, uint8_t data); 
		void read(uint16_t addr); 
		void getRegs();

		void loadRom(std::string rom);
		int fetchExecute();
		void updateTimers(int requiredClocks);
		void Handleinterrupts();



	private:

		std::unique_ptr<RAM> ram;
		// registers
		Registers regs;
		uint16_t PC = 0; // program counter
		uint16_t SP = 0; // stack pointer
		uint8_t instruction = 0;
		//LUTs for opcodes
		std::vector<int (CPU::*)(void)> opcodeLUT;
		std::vector<int (CPU::*)(void)> opcodeLUTCB; // functions for CB indext instructions

		int cycles=0;	//number of cycles to wait for current instruction
		bool flagCB = 0; // flag is set when 0xCB is read as an instruction
							// set, => use CB indexed instruction
							// unset
							// interrupts not enabled when this value is set

							// cycle count (CPU is multicycle. A new instruction is executed only when
							// this is 0)
		void fetch();
		void execute();
		//ROM header data
		uint8_t checksum=0;
		uint8_t cartridgeType=0;	//defines the cartridges mapper
									//0x00: ROM only
									//0x01 MBC1...
		uint8_t romSize=0;			//"This byte indicates how much ROM is present on the cartridge."
									//rom size in KB (1024 bits)
		
		uint8_t ramSize=0;			//amount of additional ram on cartridge
		std::string Title;
		uint8_t CGBFlag=0;
		uint8_t SGBFlag=0;

		
		void initCartHeader();	//rom containens important information from 0x0100 to 0x14F
								//initilize those values and set correct values (like checksum and flags)

		// base instructions
		// these instructions form the bases of (almost) all the opcodes.
		//table 1 instruction helpers below
		//  ld_destination_source  //
		int ld_reg_addr(uint8_t &reg1, uint8_t data);	//load data to reg1. Helper function
		int ld_reg_reg(uint8_t &reg1, uint8_t reg2); // load contents of reg2 into reg1
		int ld_reg_d8(uint8_t &reg);    // load 8 bit immediate into register
		int ld_reg_d16(uint16_t &reg1); // load 16 bit immediate into register
		int ld_mem_a(uint16_t &addr);   // load A into memory address

		int jr(bool condition);

		int inc_reg(uint8_t &reg);
		int inc_reg(uint16_t &reg);

		int dec_reg(uint8_t &reg);
		int dec_reg(uint16_t &reg);

		int add(uint8_t &reg1, uint8_t &reg2);
		int add(uint16_t &reg1, uint16_t &reg2);

		int adc(uint8_t &reg1, uint8_t &reg2);
		int adc(uint16_t &reg1, uint16_t &reg2);

		int sub(uint8_t &reg1, uint8_t &reg2);
		int sub(uint16_t &reg1, uint16_t &reg2);

		int sbc(uint16_t &reg1, uint16_t &reg2);
		int sbc(uint8_t &reg1, uint8_t &reg2);

		int _and(uint16_t &reg1, uint16_t &reg2);
		int _and(uint8_t &reg1, uint8_t &reg2);

		int _xor(uint16_t &reg1, uint16_t &reg2);
		int _xor(uint8_t &reg1, uint8_t &reg2);

		int _or(uint16_t &reg1, uint16_t &reg2);
		int _or(uint8_t &reg1, uint8_t &reg2);

		int cp(uint16_t &reg1, uint16_t &reg2);
		int cp(uint8_t &reg1, uint8_t &reg2);

		int pop(uint16_t &reg1);	//pop from stack
		int push(uint16_t &reg1);	//push to stack
		int retZC(bool condition);	//ret based on Z or C condition
		int rst(int index);	
		int _ret(); // there is already an opcode called RET.

		int jp_a16(bool condition);	//jump to immediate
		int call(bool condition);	//pushes next instruction to stack, jumps, then returns controll



		//table 2 instructiton helpers below
		int rlc();
		int rrc();
		int rl();
		int rr();

		int sla();
		int sra();
		
		int swap();
		int srl();

		int bit();

		int res(uint8_t & reg, int pos);
		int set(uint8_t & reg, int pos);

		// 8 bit opcodes
		/*0x00*/
		int nop();
		int ld_bc_d16();
		int ld_bc_a();
		int inc_bc();
		int inc_b();
		int dec_b();
		int ld_b_d8();
		int rlca();
		int ld_a16_sp();
		int add_hl_bc();
		int ld_a_bc();
		int dec_bc();
		int inc_c();
		int dec_c();
		int ld_c_d8();
		int rrca();

		/*0x10*/
		int stop();
		int ld_de_d16();
		int ld_de_a();
		int inc_de();
		int inc_d();
		int dec_d();
		int ld_d_d8();
		int rla();
		int jr_s8();
		int add_hl_de();
		int ld_a_de();
		int dec_de();
		int inc_e();
		int dec_e();
		int ld_e_d8();
		int rra();

		/*0x20*/
		int jr_nz_s8();
		int ld_hl_d16();
		int ld_hlp_a();
		int inc_hl();
		int inc_h();
		int dec_h();
		int ld_h_d8();
		int daa();
		int jr_z_s8();
		int add_hl_hl();
		int ld_a_hlp();
		int dec_hl();
		int inc_l();
		int dec_l();
		int ld_l_d8();
		int cpl();

		/*0x30*/
		int jr_nc_s8();
		int ld_sp_d16();
		int ld_hlm_a();
		int inc_sp();
		int inc_hl_mem();
		int dec_hl_mem();
		int ld_hl_d8();
		int scf();
		int jr_c_s8();
		int add_hl_sp();
		int ld_a_hlm();
		int dec_sp();
		int inc_a();
		int dec_a();
		int ld_a_d8();
		int ccf();

		/*0x40*/
		int ld_b_b();
		int ld_b_c();
		int ld_b_d();
		int ld_b_e();
		int ld_b_h();
		int ld_b_l();
		int ld_b_hl();
		int ld_b_a();
		int ld_c_b();
		int ld_c_c();
		int ld_c_d();
		int ld_c_e();
		int ld_c_h();
		int ld_c_l();
		int ld_c_hl();
		int ld_c_a();

		/*0x50*/
		int ld_d_b();
		int ld_d_c();
		int ld_d_d();
		int ld_d_e();
		int ld_d_h();
		int ld_d_l();
		int ld_d_hl();
		int ld_d_a();
		int ld_e_b();
		int ld_e_c();
		int ld_e_d();
		int ld_e_e();
		int ld_e_h();
		int ld_e_l();
		int ld_e_hl();
		int ld_e_a();

		/*0x60*/
		int ld_h_b();
		int ld_h_c();
		int ld_h_d();
		int ld_h_e();
		int ld_h_h();
		int ld_h_l();
		int ld_h_hl();
		int ld_h_a();
		int ld_l_b();
		int ld_l_c();
		int ld_l_d();
		int ld_l_e();
		int ld_l_h();
		int ld_l_l();
		int ld_l_hl();
		int ld_l_a();

		/*0x70*/
		int ld_hl_b();
		int ld_hl_c();
		int ld_hl_d();
		int ld_hl_e();
		int ld_hl_h();
		int ld_hl_l();
		int HALT();
		int ld_hl_a();
		int ld_a_b();
		int ld_a_c();
		int ld_a_d();
		int ld_a_e();
		int ld_a_h();
		int ld_a_l();
		int ld_a_hl();
		int ld_a_a();

		/*0x80*/
		int add_a_b();
		int add_a_c();
		int add_a_d();
		int add_a_e();
		int add_a_h();
		int add_a_l();
		int add_a_hl();
		int add_a_a();
		int adc_a_b();
		int adc_a_c();
		int adc_a_d();
		int adc_a_e();
		int adc_a_h();
		int adc_a_l();
		int adc_a_hl();
		int adc_a_a();

		/*0x90*/
		int sub_b();
		int sub_c();
		int sub_d();
		int sub_e();
		int sub_h();
		int sub_l();
		int sub_hl();
		int sub_a();
		int sbc_a_b();
		int sbc_a_c();
		int sbc_a_d();
		int sbc_a_e();
		int sbc_a_h();
		int sbc_a_l();
		int sbc_a_hl();
		int sbc_a_a();

		/*0xA0*/
		int and_b();
		int and_c();
		int and_d();
		int and_e();
		int and_h();
		int and_l();
		int and_hl();
		int and_a();
		int xor_b();
		int xor_c();
		int xor_d();
		int xor_e();
		int xor_h();
		int xor_l();
		int xor_hl();
		int xor_a();

		/*0xB0*/
		int or_b();
		int or_c();
		int or_d();
		int or_e();
		int or_h();
		int or_l();
		int or_hl();
		int or_a();
		int cp_b();
		int cp_c();
		int cp_d();
		int cp_e();
		int cp_h();
		int cp_l();
		int cp_hl();
		int cp_a();

		/*0xC0*/
		int ret_nz();
		int pop_bc();
		int jp_nz_a16();
		int jp_a16();
		int call_nz_a16();
		int push_bc();
		int add_a_d8();
		int rst_0();
		int ret_z();
		int ret();
		int jp_z_a16();
		int call_z_a16();
		int call_a16();
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
		int jp_c_a16();
		int call_c_a16();
		int sbc_a_d8();
		int rst_3();

		/*0xE0*/
		int ld_a8_a();
		int pop_hl();
		int ld_c_mem_a();
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
		int ld_a_c_mem();
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
		
		/*invalid opcode*/
		int invalid();

		// 16 bit opcodes
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
		int bit_5_c();
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
		int bit_7_c();
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
		int res_5_c();
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
		int res_7_c();
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
		int set_5_c();
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
		int set_7_c();
		int set_7_f();
		int set_7_e();
		int set_7_h();
		int set_7_l();
		int set_7_hl();
		int set_7_a();

};

#endif
