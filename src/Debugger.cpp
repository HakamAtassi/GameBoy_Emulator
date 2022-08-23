#include "headers/Debugger.h"
#include <iostream>
#include <string>

#define SB 0xFF01
#define SC 0xFF02

using namespace std;

Debugger::Debugger(GameBoy * gameboy):gameboy(gameboy)
{
    gameboy->printTitle();

    printf("\nAvailable commands (case sensitive):step, nw/nws (next word (signed)). \n");

    printf("Press Enter to debug.\n");

    opcodeLUT = std::vector<std::string>{
			"nop",         "ld_bc_d16",  "ld_bc_a",   "inc_bc", "inc_b",       "dec_b",      "ld_b_d8",   "rlca",	"ld_a16_sp",   "add_hl_bc",  "ld_a_bc",   "dec_bc", "inc_c",       "dec_c",      "ld_c_d8",   "rrca",
			"stop",        "ld_de_d16",  "ld_de_a",   "inc_de",	"inc_d",       "dec_d",      "ld_d_d8",   "rla",	"jr_s8",       "add_hl_de",  "ld_a_de",   "dec_de",	"inc_e",       "dec_e",      "ld_e_d8",   "rra",
			"jr_nz_s8",    "ld_hl_d16",  "ld_hlp_a",  "inc_hl",	"inc_h",       "dec_h",      "ld_h_d8",   "daa",	"jr_z_s8",     "add_hl_hl",  "ld_a_hlp",  "dec_hl",	"inc_l",       "dec_l",      "ld_l_d8",   "cpl",
			"jr_nc_s8",    "ld_sp_d16",  "ld_hlm_a",  "inc_sp",	"inc_hl_mem",  "dec_hl_mem", "ld_hl_d8",  "scf",	"jr_c_s8",     "add_hl_sp",  "ld_a_hlm",  "dec_sp",	"inc_a",       "dec_a",      "ld_a_d8",   "ccf",
			"ld_b_b",      "ld_b_c",     "ld_b_d",    "ld_b_e",	"ld_b_h",      "ld_b_l",     "ld_b_hl",   "ld_b_a",	"ld_c_b",      "ld_c_c",     "ld_c_d",    "ld_c_e",	"ld_c_h",      "ld_c_l",     "ld_c_hl",   "ld_c_a",
			"ld_d_b",      "ld_d_c",     "ld_d_d",    "ld_d_e",	"ld_d_h",      "ld_d_l",     "ld_d_hl",   "ld_d_a",	"ld_e_b",      "ld_e_c",     "ld_e_d",    "ld_e_e",	"ld_e_h",      "ld_e_l",     "ld_e_hl",   "ld_e_a",
			"ld_h_b",      "ld_h_c",     "ld_h_d",    "ld_h_e",	"ld_h_h",      "ld_h_l",     "ld_h_hl",   "ld_h_a",	"ld_l_b",      "ld_l_c",     "ld_l_d",    "ld_l_e",	"ld_l_h",      "ld_l_l",     "ld_l_hl",   "ld_l_a",
			"ld_hl_b",     "ld_hl_c",    "ld_hl_d",   "ld_hl_e","ld_hl_h",     "ld_hl_l",    "halt",      "ld_hl_a","ld_a_b",      "ld_a_c",     "ld_a_d",    "ld_a_e",	"ld_a_h",      "ld_a_l",     "ld_a_hl",   "ld_a_a",
			"add_a_b",     "add_a_c",    "add_a_d",   "add_a_e","add_a_h",     "add_a_l",    "add_a_hl",  "add_a_a","adc_a_b",     "adc_a_c",    "adc_a_d",   "adc_a_e","adc_a_h",     "adc_a_l",    "adc_a_hl",  "adc_a_a",
			"sub_b",       "sub_c",      "sub_d",     "sub_e",	"sub_h",       "sub_l",      "sub_hl",    "sub_a",	"sbc_a_b",     "sbc_a_c",    "sbc_a_d",   "sbc_a_e","sbc_a_h",     "sbc_a_l",    "sbc_a_hl",  "sbc_a_a",
			"and_b",       "and_c",      "and_d",     "and_e",	"and_h",       "and_l",      "and_hl",    "and_a",	"xor_b",       "xor_c",      "xor_d",     "xor_e",	"xor_h",       "xor_l",      "xor_hl",    "xor_a",
			"or_b",        "or_c",       "or_d",      "or_e",	"or_h",        "or_l",       "or_hl",     "or_a",	"cp_b",        "cp_c",       "cp_d",      "cp_e",	"cp_h",        "cp_l",       "cp_hl",     "cp_a",
			"ret_nz",      "pop_bc",     "jp_nz_a16", "jp_a16",	"call_nz_a16", "push_bc",    "add_a_d8",  "rst_0",	"ret_z",       "ret",        "jp_z_a16",  "invalid","call_z_a16",  "call_a16",   "adc_a_d8",  "rst_1",
			"ret_nc",      "pop_de",     "jp_nc_a16", "invalid","call_nc_a16", "push_de",    "sub_d8",    "rst_2",	"ret_c",       "reti",       "jp_c_a16",  "invalid","call_c_a16",  "invalid",    "sbc_a_d8",  "rst_3",
			"ld_a8_a",     "pop_hl",     "ld_c_mem_a","invalid","invalid",     "push_hl",    "and_d8",    "rst_4",	"add_sp_s8",   "jp_hl",      "ld_a16_a",  "invalid","invalid",     "invalid",    "xor_d8",    "rst_5",
			"ld_a_a8",     "pop_af",     "ld_a_c_mem","di",		"invalid",     "push_af",    "or_d8",     "rst_6",	"ld_hl_sp_s8", "ld_sp_hl",   "ld_a_a16",  "ei",		"invalid",     "invalid",    "cp_d8",     "rst_7"
			};

	opcodeLUTCB = std::vector<std::string>{
			"rlc_b",   "rlc_c",   "rlc_d",    "rlc_e",  "rlc_h",   "rlc_l",   "rlc_hl",   "rlc_a",  "rrc_b",   "rrc_c",   "rrc_d",    "rrc_e",  "rrc_h",   "rrc_l",   "rrc_hl",   "rrc_a",
			"rl_b",    "rl_c",    "rl_d",     "rl_e",   "rl_h",    "rl_l",    "rl_hl",    "rl_a",   "rr_b",    "rr_c",    "rr_d",     "rr_e",   "rr_h",    "rr_l",    "rr_hl",    "rr_a",
			"sla_b",   "sla_c",   "sla_d",    "sla_e",  "sla_h",   "sla_l",   "sla_hl",   "sla_a",  "sra_b",   "sra_c",   "sra_d",    "sra_e",  "sra_h",   "sra_l",   "sra_hl",   "sra_a",
			"swap_b",  "swap_c",  "swap_d",   "swap_e", "swap_h",  "swap_l",  "swap_hl",  "swap_a", "srl_b",   "srl_c",   "srl_d",    "srl_e",  "srl_h",   "srl_l",   "srl_hl",	"srl_a",
			"bit_0_b", "bit_0_c", "bit_0_f",  "bit_0_e","bit_0_h", "bit_0_l", "bit_0_hl", "bit_0_a","bit_1_b", "bit_1_c", "bit_1_f",  "bit_1_e","bit_1_h", "bit_1_l", "bit_1_hl", "bit_1_a",
			"bit_2_b", "bit_2_c", "bit_2_f",  "bit_2_e","bit_2_h", "bit_2_l", "bit_2_hl", "bit_2_a","bit_3_b", "bit_3_c", "bit_3_f",  "bit_3_e","bit_3_h", "bit_3_l", "bit_3_hl", "bit_3_a",
			"bit_4_b", "bit_4_c", "bit_4_f",  "bit_4_e","bit_4_h", "bit_4_l", "bit_4_hl", "bit_4_a","bit_5_b", "bit_5_c", "bit_5_f",  "bit_5_e","bit_5_h", "bit_5_l", "bit_5_hl", "bit_5_a",
			"bit_6_b", "bit_6_c", "bit_6_f",  "bit_6_e","bit_6_h", "bit_6_l", "bit_6_hl", "bit_6_a","bit_7_b", "bit_7_c", "bit_7_f",  "bit_7_e","bit_7_h", "bit_7_l", "bit_7_hl", "bit_7_a",
			"res_0_b", "res_0_c", "res_0_f",  "res_0_e","res_0_h", "res_0_l", "res_0_hl", "res_0_a","res_1_b", "res_1_c", "res_1_f",  "res_1_e","res_1_h", "res_1_l", "res_1_hl", "res_1_a",
			"res_2_b", "res_2_c", "res_2_f",  "res_2_e","res_2_h", "res_2_l", "res_2_hl", "res_2_a","res_3_b", "res_3_c", "res_3_f",  "res_3_e","res_3_h", "res_3_l", "res_3_hl", "res_3_a",
			"res_4_b", "res_4_c", "res_4_f",  "res_4_e","res_4_h", "res_4_l", "res_4_hl", "res_4_a","res_5_b", "res_5_c", "res_5_f",  "res_5_e","res_5_h", "res_5_l", "res_5_hl", "res_5_a",
			"res_6_b", "res_6_c", "res_6_f",  "res_6_e","res_6_h", "res_6_l", "res_6_hl", "res_6_a","res_7_b", "res_7_c", "res_7_f",  "res_7_e","res_7_h", "res_7_l", "res_7_hl", "res_7_a",
			"set_0_b", "set_0_c", "set_0_f",  "set_0_e","set_0_h", "set_0_l", "set_0_hl", "set_0_a","set_1_b", "set_1_c", "set_1_f",  "set_1_e","set_1_h", "set_1_l", "set_1_hl", "set_1_a",
			"set_2_b", "set_2_c", "set_2_f",  "set_2_e","set_2_h", "set_2_l", "set_2_hl", "set_2_a","set_3_b", "set_3_c", "set_3_f",  "set_3_e","set_3_h", "set_3_l", "set_3_hl", "set_3_a",
			"set_4_b", "set_4_c", "set_4_f",  "set_4_e","set_4_h", "set_4_l", "set_4_hl", "set_4_a","set_5_b", "set_5_c", "set_5_f",  "set_5_e","set_5_h", "set_5_l", "set_5_hl", "set_5_a",
			"set_6_b", "set_6_c", "set_6_f",  "set_6_e","set_6_h", "set_6_l", "set_6_hl", "set_6_a","set_7_b", "set_7_c", "set_7_f",  "set_7_e","set_7_h", "set_7_l", "set_7_hl", "set_7_a"
	};

}

Debugger::~Debugger()
{
    delete gameboy;
}


void Debugger::printRegs(){
    gameboy->getRegs();
    printf("\n");

}

void Debugger::printPC(){
    printf("\nPC: 0x%X => 0x%X\n",prevPC, PC);
}
void Debugger::printInterrupts(){
    //print flags
    //print requests
    //print ime
}

void Debugger::printInstruction(){
    if(CBFlag==0){
        printf("\nOpcode: 0x%X => %s",opcode,opcodeLUT[opcode].c_str());

    }
    else{
        printf("\nOpcode 0x%X => %s",opcode,opcodeLUTCB[opcode].c_str());
    }


}
void Debugger::printMemory(){
}

void Debugger::printResult(){
    printf("%s\n",testResult.c_str());
}

void Debugger::printNextWord(){
    
    uint8_t b1=gameboy->read(PC+1);
    uint8_t b2=gameboy->read(PC+2);

    printf("Byte 1: 0x%X, Byte 2: 0x%X\n",b1,b2);
}


void Debugger::printFlags(){
    std::printf("carry flag: %d\n",gameboy->getFlag("carry"));
    std::printf("halfCarry flag:%d\n",gameboy->getFlag("halfCarry"));
    std::printf("negative flag: %d\n",gameboy->getFlag("negative"));
    std::printf("zero flag: %d\n",gameboy->getFlag("zero"));
}

void Debugger::debug(){
    PC=gameboy->getPC();    //update PC after instruction is executed but before printing
    opcode=gameboy->read(PC);

    printResult();
    printPC();
    printInstruction();
    printf("\n");
    printRegs();
    printFlags();
}


void Debugger::printNextWordSigned(){
    int8_t b1=gameboy->read(PC+1);
    int8_t b2=gameboy->read(PC+2);

    printf("Byte 1: %d, Byte 2: %d\n",b1,b2);
}


void Debugger::checkSerialOut(){
    if(gameboy->read(SC)==0x81){
        char resChar=gameboy->read(SB);
        gameboy->write(SC,0);
        testResult.push_back(resChar);
    }
}

void Debugger::run(){
    //TODO: update CB flag here

    prevPC=PC;
    //instruction;  use  __func__
    string input="initial";
    getline(std::cin,input);

    if(input==""){
        debug();
        printf("\n");
        gameboy->update();
    }
    else if(input=="step"){
        int stepSize=0;
        std::cout<<"print step number:\n";
        std::cin>>stepSize;
        for(int i=0;i<stepSize;i++){
            gameboy->update();
            checkSerialOut();
        }
        debug();
    }
    else if(input=="nw"){
        printNextWord();
    }
    else if(input=="nws"){
        printNextWordSigned();
    }
    else if(input=="PC"){
        printf("PC: 0x%X\n",PC);
    }
    else if(input=="read"){
        uint16_t addr=0;
        std::cout<<"address:\n";
        std::cin>>addr;
        printf("ram[0x%X]=0x%X\n",addr,gameboy->read(addr));
        getline(std::cin,input);

    }
    else if (input=="clear"){
        system("clear");
    }
    else{
        system("clear");
        debug();
    }
}