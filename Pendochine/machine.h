#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>

#define MEM_SIZE 65536
#define NUM_REGISTERS 4

typedef struct {
} Registers;

typedef struct {
  uint8_t  mem[MEM_SIZE];
  uint32_t eip;
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
  uint32_t flags : 8;
} Machine;

#define BIT(x) (1 << x)
#define SET_FLAG(flags, flag) (flags |= flag)
#define RESET_FLAG(flags, flag) (flags &= ~flag)
#define SWITCH_FLAG(flags, flag) (flags ^= flag)
#define CHECK_FLAG(flags, flag) (flags & flag)

#define ZERO_FLAG BIT(0)
#define SIGN_FLAG BIT(1)
#define CARRY_FLAG BIT(2)
#define OVERFLOW_FLAG BIT(3)
#define PARITY_FLAG BIT(4)
#define EQUAL_FLAG BIT(5)
#define BIGGER_FLAG BIT(6)
#define LESSER_FLAG BIT(7)

typedef enum {
  NOP,
  LOAD,
  LOADMEM,
  STORE,
  STOREMEM,
  ADD,
  SUB,
  MUL,
  DIV,
  POW,
  SQRT,
  JMP,
  JZ,
  JNZ,
  JS,
  JNS,
  JC,
  JNC,
  JO,
  JNO,
  JP,
  JNP,
  INC,
  DEC,
  CALL,
  CLZ,
  CLC,
  CLS,
  CLO,
  CLP,
  CLE,
  CLB,
  CLL,
  HALT = 0xFF,
} Opcodes;

typedef enum { EAX, EBX, ECX, EDX } Register;

void init(Machine *machine);
void regs_dump(Machine *machine);
void mem_dump(Machine *machine);
void execute(Machine *machine);

#endif
