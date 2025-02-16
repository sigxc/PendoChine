// clang-format off
#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>

#define MEM_SIZE 65536
#define NUM_REGS 6

typedef struct {
  uint8_t mem[MEM_SIZE];
  uint32_t regs[NUM_REGS];
  uint32_t flags : 6;
} Machine;

#define VGA_BUFFER_SIZE (80 * 25 * sizeof(uint8_t))

// Needed to avoid multiple declarations
extern Machine machine;

#define BIT(x) (1 << x)
#define SET_FLAG(flags, flag) (flags |= flag)
#define RESET_FLAG(flags, flag) (flags &= ~flag)
#define SWITCH_FLAG(flags, flag) (flags ^= flag)
#define CHECK_FLAG(flags, flag) (flags & flag)
#define SET_ARBITRARY_BIT(bitfield, expr, place) (bitfield |= ((expr) << place))

#define ZERO_FLAG BIT(0)
#define SIGN_FLAG BIT(1)
#define OVERFLOW_FLAG BIT(2)
#define PARITY_FLAG BIT(3)
#define EQUAL_FLAG BIT(4)
#define BIGGER_FLAG BIT(5)
#define SMALLER_FLAG BIT(6)

#define ZERO_FLAG_POS 0
#define SIGN_FLAG_POS 1
#define OVERFLOW_FLAG_POS 2
#define PARITY_FLAG_POS 3
#define EQUAL_FLAG_POS 4
#define BIGGER_FLAG_POS 5
#define SMALLER_FLAG_POS 6

#define opcode machine.mem[machine.regs[PIP]]
#define first_operand machine.mem[machine.regs[PIP] + 1]
#define second_operand machine.mem[machine.regs[PIP] + 2]
#define mem(i) machine.mem[i + VGA_BUFFER_SIZE + 1]

#define ANSI_RESET "\x1b[0m"
#define ANSI_ERROR "\x1b[1;31m"
#define ANSI_SUCCESS "\x1b[1;32m"
#define ANSI_NUM "\x1b[33m"
#define ANSI_OPC "\x1b[34m"
#define ANSI_REG "\x1b[35m"
#define ANSI_INFO "\x1b[90m"

// LOAD загружает значение в регистр
// LOADMEM загружает значение из памяти в регистр
// STORE загружает значение из регистра в память
// MOVMEM перемещает значение в памяти
enum {
  NOP, LOAD, LOADMEM, STORE, MOV,
  MOVMEM, ADD, SUB, MUL, DIV,
  POW, CMP, JMP, JZ, JNZ,
  JS, JNS, JO, JNO,
  JP, JNP, JE, JNE,
  JB, JNB, JL, JNL, INC,
  DEC, CALL, CLZ, CLC, CLS,
  CLO, CLP, CLE, CLB, CLL,
  HALT = 0xFF,
};

enum {
  PIP, PDV,
  PAX, PBX,
  PCX, PDX,
};

void init(void);
void regs_dump(void);
void mem_dump(void);
void execute(void);

void nop(void);
void load(void);
void loadmem(void);
void store(void);
void mov(void);
void movmem(void);
void add(void);
void sub(void);
void mul(void);
void divide(void);
void power(void);
void cmp(void);
void jmp(void);
void jz(void);
void jnz(void);
void js(void);
void jns(void);
void jc(void);
void jnc(void);
void jo(void);
void jno(void);
void jp(void);
void jnp(void);
void inc(void);
void dec(void);
void call(void);
void clz(void);
void clc(void);
void cls(void);
void clo(void);
void clp(void);
void cle(void);
void clb(void);
void cll(void);

void print_vga(void);

#endif
// clang-format on
