#ifndef CPU_H_
#define CPU_H_


#include "Registers.h"
#include "RAM.h"
#include <cstdint>


//Note: the CPU is little endian
namespace GameBoy{
	class CPU{

		public:
			CPU();

		private:
			//registers
			Registers regs;
			uint16_t PC=0;	//program counter
			uint16_t SP=0;	//stack pointer
			
			//ram module
			RAM ram;

			//cycle count (CPU is multicycle. A new instruction is executed only when this is 0)
			int cycles=0;
			uint8_t instruction=0;
			uint8_t data=0;


		private:
			//opcodes

			/*0x00 instructions*/
			int nop();			int ld_bc_d16();	int ld_bc_a();	int inc_bc();	int inc_b();	int dec_b();	int ld_b_d8();	int rlca();
			int ld_a16_sp();	int add_hl_bc();	int ld_a_bc();	int dec_bc();	int inc_c();	int dec_c();	int ld_c_d8();	int rrca();
			

			/*0x10 instructions*/
			int stop();			int ld_de_d16();	int ld_de_a();	int inc_de();	int inc_d();	int dec_d();	int ld_d_d8();	int rla();
			int	jr_s8();		int add_hl_de();	int ld_a_de();	int dec_de();	int inc_e();	int dec_e();	int ld_e_d8();	int rra();

			/*0x20 instructions*/
			int jr_nz_s8();			int ld_hl_d16();	int ld_hlp_a();	int inc_hl();	int inc_h();	int dec_h();	int ld_h_d8();	int daa();
			int	jr_z_s8();			int add_hl_hl();	int ld_a_hlp();	int dec_hl();	int inc_l();	int dec_l();	int ld_l_d8();	int cpl();


			/*0x30 instructions*/
			int jr_nc_s8();			int ld_sp_d16();	int ld_hlm_a();	int inc_sp();	int inc_hl_mem();	int dec_hl_mem();	int ld_hl_d8();	int scf();
			int	jr_c_s8();			int add_hl_sp();	int ld_a_hlm();	int dec_sp();	int inc_a();		int dec_a();		int ld_a_d8();	int ccf();


			/*0x40 instructions*/
			int ld_b_b();	int ld_b_c();	int ld_b_d();	int ld_b_e();	int ld_b_h();	int ld_b_l();	int ld_b_hl();	int ld_b_a();	
			int ld_c_b();	int ld_c_c();	int ld_c_d();	int ld_c_e();	int ld_c_h();	int ld_c_l(); 	int ld_c_hl();	int ld_c_a();
			
			/*0x50 instructions*/
			int ld_d_b();	int ld_d_c();	int ld_d_d();	int ld_d_e();	int ld_d_h();	int ld_d_l();	int ld_d_hl();	int ld_d_a();	
			int ld_e_b();	int ld_e_c();	int ld_e_d();	int ld_e_e();	int ld_e_h();	int ld_e_l(); 	int ld_e_hl();	int ld_e_a();

			/*0x60 instructions*/
			int ld_h_b();	int ld_h_c();	int ld_h_d();	int ld_h_e();	int ld_h_h();	int ld_h_l();	int ld_h_hl();	int ld_h_a();	
			int ld_l_b();	int ld_l_c();	int ld_l_d();	int ld_l_e();	int ld_l_h();	int ld_l_l(); 	int ld_l_hl();	int ld_l_a();

			/*0x70 instructions*/
			int ld_hl_b();	int ld_hl_c();	int ld_hl_d();	int ld_hl_e();	int ld_hl_h();	int ld_hl_l();	int HALT();		int ld_hl_a();	
			int ld_a_b();	int ld_a_c();	int ld_a_d();	int ld_a_e();	int ld_a_h();	int ld_a_l(); 	int ld_a_hl();	int ld_a_a();


			/*0x80 instructions*/
			int add_a_b();	int add_a_c();	int add_a_d();	int add_a_e();	int add_a_h();	int add_a_l();	int add_a_hl();	int add_a_a();
			int adc_a_b();	int adc_a_c();	int adc_a_d();	int adc_a_e();	int adc_a_h();	int adc_a_l();	int adc_a_hl();	int adc_a_a();
			
			/*0x90 instructions*/
			int sub_b();	int sub_c();	int sub_d();	int sub_e();	int sub_h();	int sub_l();	int sub_hl();	int sub_a();
			int sbc_a_b();	int sbc_a_c();	int sbc_a_d();	int sbc_a_e();	int sbc_a_h();	int sbc_a_l();	int sbc_a_hl();	int sbc_a_a();

			/*0xA0 instructions*/
			int add_b();	int add_c();	int add_d();	int add_e();	int add_h();	int add_l();	int add_hl();	int add_a();
			int xor_b();	int xor_c();	int xor_d();	int xor_e();	int xor_h();	int xor_l();	int xor_hl();	int xor_a();

			/*0xB0 instructions*/
			int or_b();	int or_c();	int or_d();	int or_e();	int or_h();	int or_l();	int or_hl();	int or_a();
			int cp_b();	int cp_c();	int cp_d();	int cp_e();	int cp_h();	int cp_l();	int cp_hl();	int cp_a();
			
			/*0xC0*/
			

			/*0xD0*/
			
			/*0xE0*/
			
			/*0xF0*/
	};
}





#endif 
