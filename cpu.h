#include <stdint.h>
#include <map>

#define u8 uint8_t // Byte 8-bit
#define u16 uint16_t // Word 16-bit

#define s8 int8_t
#define s16 int16_t

#define byte_size 16

// flag defs 
#define NEG 0x80 // Negative
#define OVF 0x40 // Overflow
#define SPB 0x20 //For Stack Pointer Usually Set to One
#define BREAK 0x10 // Break
#define DECM 0x08 // Decimal Mode
#define INTR 0x04 // Interrupt
#define ZERO 0x02 // Zero
#define CARRY 0x01 // Carry





class CPU{
    public:
    u8 A,X,Y; // Accumulator and Storage Registers
    u8 P; // Flag Register
    u8 SP; //  Stack Pointer
    u16 PC; // Program Counter

    int cycle; // Cycle Counter (Probs Gonna Be Global Later On)

    // opcode shit...
    // EVIL NIGGA OPCODE LOOK UP TABLE
    // THIS SHIT HARDCODED MAKE YAKUB PROUD
    
    

    // CPU init functions
    void init_pc(); // Init Program Counter at 0xFFFC
    void init_sp(); // Init Stack Pointer at 0xFD
    void init_reg(); // Init Registers to Zero

    // CPU Memory Access
    void write_to_mem(u16 addr,u8 val); // Write Byte to Memory
    u8 read_from_mem(u16 addr); // Read Byte From Memory
    u16 create_addr(u8 start_addr);
    u16 create_addr(u16* start_addr);

    // CPU Flag Functions
    void set_neg(bool state);
    void set_ovf(bool state);
    void set_spb(bool state);
    void set_break(bool state);
    void set_decm(bool state);
    void set_zero(bool state);
    void set_carry(bool state);

    void check_neg(u8 val);
    void check_ovf(u8 val);
    void check_zero(u8 val);
    void check_carry(u8 val);

    // CPU Functions
    void step(); // Main Loop


    //opcode helpers
    u8 rotate(u8 val);
        
    // Opcode function
    void BRK();              // 0x00
    void ORA_X_Indirect();   // 0x01
    void ORA_ZeroPage();     // 0x05
    void ASL_ZeroPage();     // 0x06
    void PHP();              // 0x08
    void ORA_Immediate();    // 0x09
    void ASL_Accumulator();  // 0x0A
    void ORA_Absolute();     // 0x0D
    void ASL_Absolute();     // 0x0E
    void BPL();              // 0x10
    void ORA_Indirect_Y();   // 0x11
    void ORA_ZeroPage_X();   // 0x15
    void ASL_ZeroPage_X();   // 0x16
    void CLC();              // 0x18
    void ORA_Absolute_X();   // 0x1D
    void ASL_Absolute_X();   // 0x1E
    void JSR();              // 0x20
    void AND_X_Indirect();   // 0x21
    void BIT_ZeroPage();     // 0x24
    void AND_ZeroPage();     // 0x25
    void ROL_ZeroPage();     // 0x26
    void PLP();              // 0x28
    void AND_Immediate();    // 0x29
    void ROL_Accumulator();  // 0x2A
    void BIT_Absolute();     // 0x2C
    void AND_Absolute();     // 0x2D
    void ROL_Absolute();     // 0x2E
    void BMI();              // 0x30
    void AND_Indirect_Y();   // 0x31
    void AND_ZeroPage_X();   // 0x35
    void ROL_ZeroPage_X();   // 0x36
    void SEC();              // 0x38
    void AND_Absolute_X();   // 0x3D
    void ROL_Absolute_X();   // 0x3E
    void RTI();              // 0x40
    void EOR_X_Indirect();   // 0x41
    void EOR_ZeroPage();     // 0x45
    void LSR_ZeroPage();     // 0x46
    void PHA();              // 0x48
    void EOR_Immediate();    // 0x49
    void LSR_Accumulator();  // 0x4A
    void JMP_Absolute();     // 0x4C
    void EOR_Absolute();     // 0x4D
    void LSR_Absolute();     // 0x4E
    void BVC();              // 0x50
    void EOR_Indirect_Y();   // 0x51
    void EOR_ZeroPage_X();   // 0x55
    void LSR_ZeroPage_X();   // 0x56
    void CLI();              // 0x58
    void EOR_Absolute_X();   // 0x5D
    void LSR_Absolute_X();   // 0x5E
    void RTS();              // 0x60
    void ADC_X_Indirect();   // 0x61
    void ADC_ZeroPage();     // 0x65
    void ROR_ZeroPage();     // 0x66
    void PLA();              // 0x68
    void ADC_Immediate();    // 0x69
    void ROR_Accumulator();  // 0x6A
    void JMP_Indirect();     // 0x6C
    void ADC_Absolute();     // 0x6D
    void ROR_Absolute();     // 0x6E
    void BVS();              // 0x70
    void ADC_Indirect_Y();   // 0x71
    void ADC_ZeroPage_X();   // 0x75
    void ROR_ZeroPage_X();   // 0x76
    void SEI();              // 0x78
    void ADC_Absolute_X();   // 0x7D
    void ROR_Absolute_X();   // 0x7E
    void STA_X_Indirect();   // 0x81
    void STY_ZeroPage();     // 0x84
    void STA_ZeroPage();     // 0x85
    void STX_ZeroPage();     // 0x86
    void DEY();              // 0x88
    void TXA();              // 0x8A
    void STY_Absolute();     // 0x8C
    void STA_Absolute();     // 0x8D
    void STX_Absolute();     // 0x8E
    void BCC();              // 0x90
    void STA_Indirect_Y();   // 0x91
    void STY_ZeroPage_X();   // 0x94
    void STA_ZeroPage_X();   // 0x95
    void STX_ZeroPage_Y();   // 0x96
    void TYA();              // 0x98
    void TXS();              // 0x9A
    void STA_Absolute_X();   // 0x9D
    void LDY_Immediate();    // 0xA0
    void LDA_X_Indirect();   // 0xA1
    void LDX_Immediate();    // 0xA2
    void LDY_ZeroPage();     // 0xA4
    void LDA_ZeroPage();     // 0xA5
    void LDX_ZeroPage();     // 0xA6
    void TAY();              // 0xA8
    void LDA_Immediate();    // 0xA9
    void TAX();              // 0xAA
    void LDY_Absolute();     // 0xAC
    void LDA_Absolute();     // 0xAD
    void LDX_Absolute();     // 0xAE
    void BCS();              // 0xB0
    void LDA_Indirect_Y();   // 0xB1
    void LDY_ZeroPage_X();   // 0xB4
    void LDA_ZeroPage_X();   // 0xB5
    void LDX_ZeroPage_Y();   // 0xB6
    void CLV();              // 0xB8
    void TSX();              // 0xBA
    void LDY_Absolute_X();   // 0xBC
    void LDA_Absolute_X();   // 0xBD
    void LDX_Absolute_Y();   // 0xBE
    void CPY_Immediate();    // 0xC0
    void CMP_X_Indirect();   // 0xC1
    void CPY_ZeroPage();     // 0xC4
    void CMP_ZeroPage();     // 0xC5
    void DEC_ZeroPage();     // 0xC6
    void INY();              // 0xC8
    void CMP_Immediate();    // 0xC9
    void DEX();              // 0xCA
    void CPY_Absolute();     // 0xCC
    void CMP_Absolute();     // 0xCD
    void DEC_Absolute();     // 0xCE
    void BNE();              // 0xD0
    void CMP_Indirect_Y();   // 0xD1
    void CMP_ZeroPage_X();   // 0xD5
    void DEC_ZeroPage_X();   // 0xD6
    void CLD();              // 0xD8
    void CMP_Absolute_X();   // 0xDD
    void DEC_Absolute_X();   // 0xDE
    void CPX_Immediate();    // 0xE0
    void SBC_X_Indirect();   // 0xE1
    void CPX_ZeroPage();     // 0xE4
    void SBC_ZeroPage();     // 0xE5
    void INC_ZeroPage();     // 0xE6
    void INX();              // 0xE8
    void SBC_Immediate();    // 0xE9
    void NOP();              // 0xEA
    void CPX_Absolute();     // 0xEC
    void SBC_Absolute();     // 0xED
    void INC_Absolute();     // 0xEE
    void BEQ();              // 0xF0
    void SBC_Indirect_Y();   // 0xF1
    void SBC_ZeroPage_X();   // 0xF5
    void INC_ZeroPage_X();   // 0xF6
    void SED();              // 0xF8
    void SBC_Absolute_X();   // 0xFD
    void INC_Absolute_X();   // 0xFE
};