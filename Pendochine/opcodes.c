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
  printf("%X:\t%02x\t\t\x1b[34mnop\x1b[0m\n", machine.regs[PIP], opcode);
#endif
  machine.regs[PIP]++;
  return;
}

void load(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t\x1b[34mload \x1b[35m%d\x1b[0m, \x1b[35m%s\x1b[0m\n", machine.regs[PIP], opcode, first_operand, second_operand,
         first_operand, regs_name_lookup[second_operand]);
#endif
  machine.regs[second_operand] = first_operand;
  machine.regs[PIP] += 3;
  return;
}

void loadmem(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t\x1b[34mloadmem\t\x1b[33m%d\x1b[0m, \x1b[35m%s\x1b[0m\n", machine.regs[PIP], opcode, first_operand, second_operand,
         first_operand, regs_name_lookup[second_operand]);
#endif
  machine.regs[second_operand] = machine.mem[first_operand];
  machine.regs[PIP] += 3;
  return;
}

void store(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t\x1b[34mstore\t\x1b[35m%s\x1b[0m, \x1b[33m%d\x1b[0m\n", machine.regs[PIP], opcode, first_operand, second_operand,
         regs_name_lookup[first_operand], second_operand);
#endif
  machine.mem[second_operand] = machine.regs[first_operand];
  machine.regs[PIP] += 3;
  return;
}

void mov(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t\x1b[34mmov\t\x1b[35m%s\x1b[0m, \x1b[35m%s\x1b[0m\n", machine.regs[PIP], opcode, first_operand, second_operand,
         regs_name_lookup[first_operand], regs_name_lookup[second_operand]);
#endif
  machine.regs[second_operand] = machine.regs[first_operand];
  machine.regs[PIP] += 3;
  return;
}

void movmem(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t\x1b[34mmovmem\t\x1b[33m%d\x1b[0m, \x1b[33m%d\x1b[0m\n", machine.regs[PIP], opcode, first_operand, second_operand,
         first_operand, second_operand);
#endif
  machine.mem[second_operand] = machine.mem[first_operand];
  machine.regs[PIP] += 3;
  return;
}

void add(void) {
#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t\x1b[34madd\t\x1b[35m%s\x1b[0m,\x1b[35m %s\x1b[0m\n", machine.regs[PIP], opcode, first_operand, second_operand,
         regs_name_lookup[first_operand], regs_name_lookup[second_operand]);
#endif
  machine.regs[first_operand] += machine.regs[second_operand];
  machine.regs[PIP] += 3;
  return;
}
