#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>

#define MEM_SIZE 1024

typedef struct {
  uint8_t mem[MEM_SIZE];
} MACHINE;

// Так как регистры 32-х битные то наименование будет соответствующее

typedef struct {
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
  // Я перенёс указатель на инструкцию в регистры паматушта так во всех процах делают
  uint32_t eip; // Это на инструкцию
  uint32_t esp; // Это на стек
} REGISTERS;

enum {
  EAX = 0x00,
  EBX = 0x01,
  ECX = 0x02,
  EDX = 0x03,
  EIP = 0x04,
  ESP = 0x05,
};

typedef enum {
  NOP = 0x00,
  HALT = 0x01,
  LOAD = 0x02,
  ADD = 0x03,
  DIV = 0x04,
  MUL = 0x05,
  SUB = 0x06,
  INC = 0x07,
  DEC = 0x08,
  JMP = 0x09,
  CALL = 0x10,
} Instruction;

void init(MACHINE *machine, REGISTERS *regs);
void dump_regs(REGISTERS *regs);
void dump_mem(MACHINE *machine);
void exec_instruction(MACHINE *machine, REGISTERS *regs);

#endif
