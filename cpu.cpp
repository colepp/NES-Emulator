#include <iostream>


#include "cpu.h"

// Temp mem setup will create actual MEMORY object in the future
const int MEMORY_SIZE = 65536;
u8 MEMORY[MEMORY_SIZE];

typedef void(CPU::*operation_ptr)(); // typedef for a function pointer cuz pointers are retarded

operation_ptr opcode_table[256] = {
    &CPU::BRK,            // 0x00
    &CPU::ORA_X_Indirect, // 0x01
    nullptr,              // 0x02
    nullptr,              // 0x03
    nullptr,              // 0x04
    &CPU::ORA_ZeroPage,   // 0x05
    &CPU::ASL_ZeroPage,   // 0x06
    nullptr,              // 0x07
    &CPU::PHP,            // 0x08
    &CPU::ORA_Immediate,  // 0x09
    &CPU::ASL_Accumulator,// 0x0A (corrected from ASL_Absolute)
    nullptr,              // 0x0B
    nullptr,              // 0x0C
    &CPU::ORA_Absolute,   // 0x0D
    &CPU::ASL_Absolute,   // 0x0E
    nullptr,              // 0x0F
    &CPU::BPL,            // 0x10
    &CPU::ORA_Indirect_Y, // 0x11
    nullptr,              // 0x12
    nullptr,              // 0x13
    nullptr,              // 0x14
    &CPU::ORA_ZeroPage_X, // 0x15
    &CPU::ASL_ZeroPage_X, // 0x16
    nullptr,              // 0x17
    &CPU::CLC,            // 0x18
    nullptr,              // 0x19
    nullptr,              // 0x1A
    nullptr,              // 0x1B
    nullptr,              // 0x1C
    &CPU::ORA_Absolute_X, // 0x1D
    &CPU::ASL_Absolute_X, // 0x1E
    nullptr,              // 0x1F
    &CPU::JSR,            // 0x20
    &CPU::AND_X_Indirect, // 0x21
    nullptr,              // 0x22
    nullptr,              // 0x23
    &CPU::BIT_ZeroPage,   // 0x24
    &CPU::AND_ZeroPage,   // 0x25
    &CPU::ROL_ZeroPage,   // 0x26
    nullptr              // 0x27
    &CPU::PLP,            // 0x28
    &CPU::AND_Immediate,  // 0x29
    &CPU::ROL_Accumulator,// 0x2A
    nullptr,              // 0x2B
    &CPU::BIT_Absolute,   // 0x2C
    &CPU::AND_Absolute,   // 0x2D
    &CPU::ROL_Absolute,   // 0x2E
    nullptr,              // 0x2F
    &CPU::BMI,            // 0x30
    &CPU::AND_Indirect_Y, // 0x31
    nullptr,              // 0x32
    nullptr,              // 0x33
    nullptr,              // 0x34
    &CPU::AND_ZeroPage_X, // 0x35
    &CPU::ROL_ZeroPage_X, // 0x36
    nullptr,              // 0x37
    &CPU::SEC,            // 0x38
    nullptr,              // 0x39
    nullptr,              // 0x3A
    nullptr,              // 0x3B
    nullptr,              // 0x3C
    &CPU::AND_Absolute_X, // 0x3D
    &CPU::ROL_Absolute_X, // 0x3E
    nullptr,              // 0x3F
    &CPU::RTI,            // 0x40
    &CPU::EOR_X_Indirect, // 0x41
    nullptr,              // 0x42
    nullptr,              // 0x43
    nullptr,              // 0x44
    &CPU::EOR_ZeroPage,   // 0x45
    &CPU::LSR_ZeroPage,   // 0x46
    nullptr,              // 0x47
    &CPU::PHA,            // 0x48
    &CPU::EOR_Immediate,  // 0x49
    &CPU::LSR_Accumulator,// 0x4A
    nullptr,              // 0x4B
    &CPU::JMP_Absolute,   // 0x4C
    &CPU::EOR_Absolute,   // 0x4D
    &CPU::LSR_Absolute,   // 0x4E
    nullptr,              // 0x4F
    &CPU::BVC,            // 0x50
    &CPU::EOR_Indirect_Y, // 0x51
    nullptr,              // 0x52
    nullptr,              // 0x53
    nullptr,              // 0x54
    &CPU::EOR_ZeroPage_X, // 0x55
    &CPU::LSR_ZeroPage_X, // 0x56
    nullptr,              // 0x57
    &CPU::CLI,            // 0x58
    nullptr,              // 0x59
    nullptr,              // 0x5A
    nullptr,              // 0x5B
    nullptr,              // 0x5C
    &CPU::EOR_Absolute_X, // 0x5D
    &CPU::LSR_Absolute_X, // 0x5E
    nullptr,              // 0x5F
    &CPU::RTS,            // 0x60
    &CPU::ADC_X_Indirect, // 0x61
    nullptr,              // 0x62
    nullptr,              // 0x63
    nullptr,              // 0x64
    &CPU::ADC_ZeroPage,   // 0x65
    &CPU::ROR_ZeroPage,   // 0x66
    nullptr,              // 0x67
    &CPU::PLA,            // 0x68
    &CPU::ADC_Immediate,  // 0x69
    &CPU::ROR_Accumulator,// 0x6A
    nullptr,              // 0x6B
    &CPU::JMP_Indirect,   // 0x6C
    &CPU::ADC_Absolute,   // 0x6D
    &CPU::ROR_Absolute,   // 0x6E
    nullptr,              // 0x6F
    &CPU::BVS,            // 0x70
    &CPU::ADC_Indirect_Y, // 0x71
    nullptr,              // 0x72
    nullptr,              // 0x73
    nullptr,              // 0x74
    &CPU::ADC_ZeroPage_X, // 0x75
    &CPU::ROR_ZeroPage_X, // 0x76
    nullptr,              // 0x77
    &CPU::SEI,            // 0x78
    nullptr,              // 0x79
    nullptr,              // 0x7A
    nullptr,              // 0x7B
    nullptr,              // 0x7C
    &CPU::ADC_Absolute_X, // 0x7D
    &CPU::ROR_Absolute_X, // 0x7E
    nullptr,              // 0x7F
    nullptr,              // 0x80
    &CPU::STA_X_Indirect, // 0x81
    nullptr,              // 0x82
    nullptr,              // 0x83
    &CPU::STY_ZeroPage,   // 0x84
    &CPU::STA_ZeroPage,   // 0x85
    &CPU::STX_ZeroPage,   // 0x86
    nullptr,              // 0x87
    &CPU::DEY,            // 0x88
    nullptr,              // 0x89
    &CPU::TXA,            // 0x8A
    nullptr,              // 0x8B
    &CPU::STY_Absolute,   // 0x8C
    &CPU::STA_Absolute,   // 0x8D
    &CPU::STX_Absolute,   // 0x8E
    nullptr,              // 0x8F
    &CPU::BCC,            // 0x90
    &CPU::STA_Indirect_Y, // 0x91
    nullptr,              // 0x92
    nullptr,              // 0x93
    &CPU::STY_ZeroPage_X, // 0x94
    &CPU::STA_ZeroPage_X, // 0x95
    &CPU::STX_ZeroPage_Y, // 0x96
    nullptr,              // 0x97
    &CPU::TYA,            // 0x98
    nullptr,              // 0x99
    &CPU::TXS,            // 0x9A
    nullptr,              // 0x9B
    
    nullptr,              // 0x9C
    &CPU::STA_Absolute_X, // 0x9D
    nullptr,              // 0x9E
    nullptr,              // 0x9F
    &CPU::LDY_Immediate,  // 0xA0
    &CPU::LDA_X_Indirect, // 0xA1
    &CPU::LDX_Immediate,  // 0xA2
    nullptr,              // 0xA3
    &CPU::LDY_ZeroPage,   // 0xA4
    &CPU::LDA_ZeroPage,   // 0xA5
    &CPU::LDX_ZeroPage,   // 0xA6
    nullptr,              // 0xA7
    &CPU::TAY,            // 0xA8
    &CPU::LDA_Immediate,  // 0xA9
    &CPU::TAX,            // 0xAA
    nullptr,              // 0xAB
    &CPU::LDY_Absolute,   // 0xAC
    &CPU::LDA_Absolute,   // 0xAD
    &CPU::LDX_Absolute,   // 0xAE
    nullptr,              // 0xAF
    &CPU::BCS,            // 0xB0
    &CPU::LDA_Indirect_Y, // 0xB1
    nullptr,              // 0xB2
    nullptr,              // 0xB3
    &CPU::LDY_ZeroPage_X, // 0xB4
    &CPU::LDA_ZeroPage_X, // 0xB5
    &CPU::LDX_ZeroPage_Y, // 0xB6
    nullptr,              // 0xB7
    &CPU::CLV,            // 0xB8
    nullptr,              // 0xB9
    &CPU::TSX,            // 0xBA
    nullptr,              // 0xBB
    &CPU::LDY_Absolute_X, // 0xBC
    &CPU::LDA_Absolute_X, // 0xBD
    &CPU::LDX_Absolute_Y, // 0xBE
    nullptr,              // 0xBF
    &CPU::CPY_Immediate,  // 0xC0
    &CPU::CMP_X_Indirect, // 0xC1
    nullptr,              // 0xC2
    nullptr,              // 0xC3
    &CPU::CPY_ZeroPage,   // 0xC4
    &CPU::CMP_ZeroPage,   // 0xC5
    &CPU::DEC_ZeroPage,   // 0xC6
    nullptr,              // 0xC7
    &CPU::INY,            // 0xC8
    &CPU::CMP_Immediate,  // 0xC9
    &CPU::DEX,            // 0xCA
    nullptr,              // 0xCB
    &CPU::CPY_Absolute,   // 0xCC
    &CPU::CMP_Absolute,   // 0xCD
    &CPU::DEC_Absolute,   // 0xCE
    nullptr,              // 0xCF
    &CPU::BNE,            // 0xD0
    &CPU::CMP_Indirect_Y, // 0xD1
    nullptr,              // 0xD2
    nullptr,              // 0xD3
    nullptr,              // 0xD4
    &CPU::CMP_ZeroPage_X, // 0xD5
    &CPU::DEC_ZeroPage_X, // 0xD6
    nullptr,              // 0xD7
    &CPU::CLD,            // 0xD8
    nullptr,              // 0xD9
    nullptr,              // 0xDA
    nullptr,              // 0xDB
    nullptr,              // 0xDC
    &CPU::CMP_Absolute_X, // 0xDD
    &CPU::DEC_Absolute_X, // 0xDE
    nullptr,              // 0xDF
    &CPU::CPX_Immediate,  // 0xE0
    &CPU::SBC_X_Indirect, // 0xE1
    nullptr,              // 0xE2
    nullptr,              // 0xE3
    &CPU::CPX_ZeroPage,   // 0xE4
    &CPU::SBC_ZeroPage,   // 0xE5
    &CPU::INC_ZeroPage,   // 0xE6
    nullptr,              // 0xE7
    &CPU::INX,            // 0xE8
    &CPU::SBC_Immediate,  // 0xE9
    &CPU::NOP,            // 0xEA
    nullptr,              // 0xEB
    &CPU::CPX_Absolute,   // 0xEC
    &CPU::SBC_Absolute,   // 0xED
    &CPU::INC_Absolute,   // 0xEE
    nullptr,              // 0xEF
    &CPU::BEQ,            // 0xF0
    &CPU::SBC_Indirect_Y, // 0xF1
    nullptr,              // 0xF2
    nullptr,              // 0xF3
    nullptr,              // 0xF4
    &CPU::SBC_ZeroPage_X, // 0xF5
    &CPU::INC_ZeroPage_X, // 0xF6
    nullptr,              // 0xF7
    &CPU::SED,            // 0xF8
    nullptr,              // 0xF9
    nullptr,              // 0xFA
    nullptr,              // 0xFB
    nullptr,              // 0xFC
    &CPU::SBC_Absolute_X, // 0xFD
    &CPU::INC_Absolute_X, // 0xFE
    nullptr               // 0xFF
};





void CPU::init_pc(){this->PC = 0xFFFC;}
void CPU::init_sp(){this->SP = 0xFD;}
void CPU::init_reg(){
    this->A = this->X = this->Y = 0;
    this->P = 0x34;
}

void CPU::write_to_mem(u16 addr,u8 val){MEMORY[addr] = val;}
u8 CPU::read_from_mem(u16 addr){return MEMORY[addr];}

u16 CPU::create_addr(u8 start_addr){
    u8 low = read_from_mem(start_addr++);
    u8 high = read_from_mem(start_addr++);
    return (high << 8) | low;
}

u16 CPU::create_addr(u16* start_addr){
    u8 low = read_from_mem(*start_addr++);
    u8 high = read_from_mem(*start_addr++);
    return (high << 8) | low;
}

void CPU::set_zero(bool state){
    if(state){P |= ZERO;}
    else{P &= ~ZERO;}
}

void CPU::set_ovf(bool state){
    if(state){P|= OVF;}
    else{P &= ~OVF;}
}

void CPU::set_neg(bool state){
    if(state){P |= NEG;}
    else{P &= ~NEG;}
}

void CPU::set_break(bool state){
    if(state){P |= BREAK;}
    else{P &= ~BREAK;}

}

void CPU::set_carry(bool state){
    if(state){P |= CARRY;}
    else{P &= ~CARRY;}
}

void CPU::check_zero(u8 val){
    if(val == 0){set_zero(true);}
    else{set_zero(false);}
}

void CPU::check_neg(u8 val){
    if(val & 0x80){set_neg(true);}
    else{set_neg(false);}
}

void CPU::check_carry(u8 val){
    if(val & 0x80){set_carry(true);}
    else{set_carry(false);}
}

void CPU::check_ovf(u8 val){
    if(val & 40){set_ovf(true);}
    else{set_ovf(false);}
}

void CPU::step(){
    u8 next_instruction = read_from_mem(PC++);
    if(opcode_table[next_instruction]){
        (this->*opcode_table[next_instruction])();
    }else{
        NOP();
    }
    cycle += 1;

}

u8 CPU::rotate(u8 val){
    return ((val << 1) | (P & 0x01));
}


void CPU::AND_X_Indirect(){
    u8 zp_addr = read_from_mem(PC++) + X;
    u16 target_addr = create_addr(zp_addr);
    A &= read_from_mem(target_addr);
    check_zero(A);
    check_neg(A);
}

void CPU::BIT_ZeroPage(){
    u8 zp_addr = read_from_mem(PC++);
    u8 zp_target = read_from_mem(zp_addr);

    check_ovf(zp_target);
    check_neg(zp_target);
    check_zero((A & zp_target) == 0);
}

void CPU::AND_ZeroPage(){
    u8 zp_addr = read_from_mem(PC++);
    A |= read_from_mem(zp_addr);
    check_neg(A);
    check_zero(A);
}

void CPU::ORA_X_Indirect(){
    u8 zp_addr = read_from_mem(PC++) + X; // ZERO PAGE ADDRESS + X
    u16 target_addr = create_addr(zp_addr); // CREATE FULL ADDRESS
    A |= read_from_mem(target_addr); // A Ors with target_addrs
    check_neg(A);
    check_zero(A);
}

void CPU::ORA_Absolute_X(){
    u16 target_addr = create_addr(&PC) + X;
    A |= read_from_mem(target_addr);
    check_neg(A);
    check_zero(A);
}

void CPU::ORA_Indirect_Y(){
    u8 addr = read_from_mem(PC++);
    u16 target_addr = create_addr(addr) + Y;
    A |= read_from_mem(target_addr);
    check_neg(A);
    check_zero(A);
}

void CPU::ORA_ZeroPage_X(){
    u8 zp_addr = read_from_mem(PC++);
    u16 target_addr = create_addr(zp_addr) + X;
    A |= read_from_mem(target_addr);
    check_neg(A);
    check_zero(A);
}

void CPU::ORA_ZeroPage(){
    u8 zp_addr = read_from_mem(PC++);
    A |= read_from_mem((u16)zp_addr);
    check_neg(A);
    check_zero(A);
}

void CPU::ORA_Immediate(){
    u8 next_val = read_from_mem(PC++);
    A |= next_val;
    check_neg(A);
    check_zero(A);
}

void CPU::ORA_Absolute(){
    u16 abs_addr = create_addr(&PC);
    A |= read_from_mem(abs_addr);
    check_neg(A);
    check_zero(A);
}

void CPU::ASL_ZeroPage_X(){
    u8 zp_addr = read_from_mem(PC++);
    u16 target_addr = create_addr(zp_addr) + X;
    u8 target_val = read_from_mem(target_addr);
    check_carry(target_val);

    target_val  = (target_val << 1);
    write_to_mem(target_addr,target_val);
    check_zero(target_val);
    check_neg(target_val);
}


void CPU::ASL_Absolute(){
    u16 abs_addr = create_addr(&PC);
    u8 abs_val = read_from_mem(abs_addr);
    check_carry(abs_val);

    abs_val = (abs_val << 1);
    write_to_mem(abs_addr,abs_val);
    check_zero(abs_val);
    check_neg(abs_val);
}

void CPU::ASL_ZeroPage(){
    u8 zp_addr = read_from_mem(PC++);
    u8 target_val = read_from_mem(zp_addr);
    check_carry(target_val);

    // bit shift operation
    target_val = (target_val << 1);
    write_to_mem(zp_addr,target_val); // write back to mem
    check_neg(target_val);
    check_zero(target_val);
}

void CPU::ASL_Absolute_X(){
    u16 target_addr = create_addr(&PC) + X;
    u8 target_val = read_from_mem(target_addr);
    check_carry(target_val);

    target_val = (target_val << 1);
    write_to_mem(target_addr,target_val);
    check_neg(target_val);
    check_zero(target_val);
    
}

void CPU::ASL_Accumulator(){
    check_carry(A);
    A = (A << 1);
    check_zero(A);
    check_zero(A);
}

void CPU::ROL_ZeroPage(){
    u8 zp_addr = read_from_mem(PC++);
    u8 zp_target = read_from_mem(zp_addr);
    u8 old_carry = zp_target & 0x80;
    zp_target = rotate(zp_target);
    write_to_mem(zp_addr,zp_target);
    check_carry(old_carry);
    check_neg(zp_target);
    check_zero(zp_target);
    
}

void CPU::PHP(){
    u8 P_copy = P;
    P_copy |= BREAK;
    write_to_mem(0x0100 + SP,P_copy);
    SP--;
}

void CPU::BRK(){
    PC++;
    write_to_mem(0x0100 + SP--, (PC >> 8));
    write_to_mem(0x0100 + SP--, PC & 0xFF);
    write_to_mem(0x0100 + SP--, P | BREAK);
    P |= INTR;
    PC = (read_from_mem(0xFFFF) << 8) | read_from_mem(0xFFFE);
    cycle += 7;
}

void CPU::BPL(){
    s8 signed_offset = read_from_mem(PC++);
    if((P & NEG) == 0){
        PC += signed_offset;
    }
}

void CPU::CLC(){
    set_carry(false);
}

void CPU::JSR(){
    u16 target_addr = create_addr(&PC);
    write_to_mem(0x0100 + SP--,(u8)((PC - 1) >> 8));
    write_to_mem(0x0100 + SP--,((u8)(PC - 1)));
    
    PC = target_addr;
}


void CPU::NOP(){
    std::cout << "NO OPERATION!\n";
}

int main() {
    CPU cpu;

    // Initialize CPU
    cpu.init_pc();  // PC = 0xFFFC (will override later)
    cpu.init_sp();  // SP = 0xFD
    cpu.init_reg(); // A=0, X=0, Y=0, P=0x34

    // Load new test program at 0x8000 with subroutine at 0x8010
    u8 program[] = {
        // Main program at 0x8000
        0x18,              // CLC         - Clear Carry (C=0)
        0x24, 0x40,        // BIT $40     - Test bits in $40 (e.g., 0xC0)
        0x26, 0x40,        // ROL $40     - Rotate $40 left (C=0)
        0x20, 0x10, 0x80,  // JSR $8010   - Jump to subroutine at 0x8010
        0x08,              // PHP         - Push processor status
        0x00,              // BRK         - Break
        // Padding to align subroutine at 0x8010
        0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA,
        // Subroutine at 0x8010
        0x24, 0x41,        // BIT $41     - Test bits in $41 (e.g., 0x54)
        0x26, 0x41,        // ROL $41     - Rotate $41 left (uses C from main)
        0x18,              // CLC         - Clear Carry
        0x26, 0x41,        // ROL $41     - Rotate $41 again (C=0)
        0x60               // RTS         - Return to 0x8007
    };
    for (int i = 0; i < sizeof(program); i++) {
        cpu.write_to_mem(0x8000 + i, program[i]);
    }

    // Set test memory values
    cpu.write_to_mem(0x0040, 0xC0);  // Zero-page value for BIT and ROL in main
    cpu.write_to_mem(0x0041, 0x54);  // Zero-page value for BIT and ROL in subroutine

    // Override PC to start at program
    cpu.PC = 0x8000;

    // Run steps to cover main program and subroutine
    std::cout << "Initial: A=" << std::hex << std::hex << (int)cpu.A << std::hex << ", P=" << std::hex << (int)cpu.P << std::hex << ", SP=" << std::hex << (int)cpu.SP << ", PC=" << std::hex << cpu.PC << ", Cycles=" << std::hex <<cpu.cycle << "\n";
    for (int i = 0; i < 10; i++) {  // 10 steps to hit BRK and subroutine
        cpu.step();
        std::cout << "Step " << i + 1 << ": A=" << std::hex <<  (int)cpu.A << ", P=" << std::hex << (int)cpu.P << ", SP=" << std::hex << (int)cpu.SP << ", PC=" << std::hex << cpu.PC << ", Cycles=" << std::hex << cpu.cycle << "\n";
    }

    // Check stack values
    std::cout << "Stack at 0x01FC: " << (int)cpu.read_from_mem(0x01FC) << "\n";  // After PHP
    std::cout << "Stack at 0x01FB: " << (int)cpu.read_from_mem(0x01FB) << "\n";  // After JSR high
    std::cout << "Stack at 0x01FA: " << (int)cpu.read_from_mem(0x01FA) << "\n";  // After JSR low

    return 0;
}