#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>

#define MEM_SIZE 65536
#define NUM_REGS 6
#define VGA_BUFFER_SIZE (80 * 25 * sizeof(uint8_t))

typedef struct {
  int8_t   mem[MEM_SIZE];
  int32_t  regs[NUM_REGS];
  uint32_t flags : 8;
} Machine;

// Needed to avoid multiple declarations
extern Machine machine;
extern int loglevel;

#define DEFAULT 0
#define VERBOSE 1

#define BIT(x) (1 << x)
#define SET_FLAG(bitfield, flag) (bitfield |= flag)
#define RESET_FLAG(bitfield, flag) (bitfield &= ~flag)
#define SWITCH_FLAG(bitfield, flag) (bitfield ^= flag)
#define CHECK_FLAG(bitfield, flag) (bitfield & flag)
#define SET_ARBITRARY_BIT(bitfield, expr, place) (bitfield |= ((expr) << place))

#define ZERO_FLAG BIT(0)
#define SIGN_FLAG BIT(1)
#define OVERFLOW_FLAG BIT(2)
#define PARITY_FLAG BIT(3)
#define REMAINDER_FLAG BIT(4)
#define EQUAL_FLAG BIT(5)
#define BIGGER_FLAG BIT(6)
#define SMALLER_FLAG BIT(7)

#define ZERO_FLAG_POS 0
#define SIGN_FLAG_POS 1
#define OVERFLOW_FLAG_POS 2
#define PARITY_FLAG_POS 3
#define REMAINDER_FLAG_POS 4
#define EQUAL_FLAG_POS 5
#define BIGGER_FLAG_POS 6
#define SMALLER_FLAG_POS 7

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
  DEC, CALL, SETZ, SETC,
  SETS, SETO, SETP, SETE,
  SETB, SETL, CLZ, CLC, CLS,
  CLO, CLP, CLE, CLB, CLL,
  HALT = -1,
};

enum {
  PIP, PDV,
  PAX, PBX,
  PCX, PDX,
};

void init();
void regs_dump();
void mem_dump();
void execute();

void nop();
void load();
void loadmem();
void store();
void mov();
void movmem();
void add();
void sub();
void mul();
void divide();
void power();
void cmp();
void jmp();
void jz();
void jnz();
void js();
void jns();
void jc();
void jnc();
void jo();
void jno();
void jp();
void jnp();
void inc();
void dec();
void call();
void clz();
void clc();
void cls();
void clo();
void clp();
void cle();
void clb();
void cll();

void pvb();
void printlog(const char *str);

#endif
