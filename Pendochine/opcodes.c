#include "machine.h"

#ifdef VERBOSE
#include <stdio.h>

// clang-format off
char *regs_name_lookup[] = {
  [PIP] = "pip",
  [PDV] = "pdv",
  [PAX] = "pax",
  [PBX] = "pbx",
  [PCX] = "pcx",
  [PDX] = "pdx",
};
// clang-format on
#endif

void nop(void) {
#ifdef VERBOSE
  printf("%X:\t%02x\t\t" ANSI_OPC "nop" ANSI_RESET "\n", machine.regs[PIP], opcode);
#endif
  machine.regs[PIP]++;
  return;
}

void load(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "load " ANSI_NUM "%d" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n", machine.regs[PIP], opcode,
         first_operand, second_operand, first_operand, regs_name_lookup[second_operand]);
#endif
  machine.regs[second_operand] = first_operand;
  machine.regs[PIP] += 3;
  return;
}

void loadmem(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "loadmem\t" ANSI_NUM "%d" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n", machine.regs[PIP], opcode,
         first_operand, second_operand, first_operand, regs_name_lookup[second_operand]);
#endif
  machine.regs[second_operand] = machine.mem[first_operand];
  machine.regs[PIP] += 3;
  return;
}

void store(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "store\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_NUM "%d" ANSI_RESET "\n", machine.regs[PIP], opcode,
         first_operand, second_operand, regs_name_lookup[first_operand], second_operand);
#endif
  machine.mem[second_operand] = machine.regs[first_operand];
  machine.regs[PIP] += 3;
  return;
}

void mov(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "mov\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n", machine.regs[PIP], opcode,
         first_operand, second_operand, regs_name_lookup[first_operand], regs_name_lookup[second_operand]);
#endif
  machine.regs[second_operand] = machine.regs[first_operand];
  machine.regs[PIP] += 3;
  return;
}

void movmem(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "movmem\t" ANSI_NUM "%d" ANSI_RESET ", " ANSI_NUM "%d" ANSI_RESET "\n", machine.regs[PIP], opcode,
         first_operand, second_operand, first_operand, second_operand);
#endif
  machine.mem[second_operand] = machine.mem[first_operand];
  machine.regs[PIP] += 3;
  return;
}

void add(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "add\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n", machine.regs[PIP], opcode,
         first_operand, second_operand, regs_name_lookup[first_operand], regs_name_lookup[second_operand]);
#endif
  machine.regs[first_operand] += machine.regs[second_operand];
  machine.regs[PIP] += 3;
  return;
}
