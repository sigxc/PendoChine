#include "machine.h"
#include <stdio.h>

#ifdef VERBOSE
char *regs_name_lookup[] = {
  [PIP] = "pip",
  [PDV] = "pdv",
  [PAX] = "pax",
  [PBX] = "pbx",
  [PCX] = "pcx",
  [PDX] = "pdx",
};
#endif

void nop(void) {

#ifdef VERBOSE
  printf("%X:\t%02x\t\t" ANSI_OPC "nop" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode);
#endif

  machine.regs[PIP]++;
  return;
}

void load(void) {

#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "load " ANSI_NUM "%d" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    first_operand,
    regs_name_lookup[second_operand]);
#endif

  machine.regs[second_operand] = first_operand;
  machine.regs[PIP] += 3;
  return;
}

void loadmem(void) {

#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "loadmem\t" ANSI_NUM "%d" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    first_operand,
    regs_name_lookup[second_operand]);
#endif

  machine.regs[second_operand] = machine.mem[first_operand];
  machine.regs[PIP] += 3;
  return;
}

void store(void) {

#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "store\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_NUM "%d" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    regs_name_lookup[first_operand],
    second_operand);
#endif

  machine.mem[second_operand] = machine.regs[first_operand];
  machine.regs[PIP] += 3;

  /*
    if (machine.mem[second_operand] <= VGA_BUFFER_SIZE) {
      print_vga();
    }
  */
  return;
}

void mov(void) {

#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "mov\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    regs_name_lookup[first_operand],
    regs_name_lookup[second_operand]);
#endif

  machine.regs[second_operand] = machine.regs[first_operand];
  machine.regs[PIP] += 3;
  return;
}

void movmem(void) {

#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "movmem\t" ANSI_NUM "%d" ANSI_RESET ", " ANSI_NUM "%d" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    first_operand,
    second_operand);
#endif

  machine.mem[second_operand] = machine.mem[first_operand];
  machine.regs[PIP] += 3;
  /*
  if (machine.mem[second_operand] <= VGA_BUFFER_SIZE) {
      print_vga();
    }
  */
  return;
}

void add(void) {

#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "add\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n", machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    regs_name_lookup[first_operand],
    regs_name_lookup[second_operand]);
#endif

  uint32_t result;

  SET_ARBITRARY_BIT(
    machine.flags,
    __builtin_add_overflow(
      machine.regs[first_operand],
      machine.regs[second_operand],
      &result),
    OVERFLOW_FLAG_POS);

  SET_ARBITRARY_BIT(machine.flags, result == 0, ZERO_FLAG_POS);
  SET_ARBITRARY_BIT(machine.flags, result < 0, SIGN_FLAG_POS);
  SET_ARBITRARY_BIT(machine.flags, result % 2 == 0, PARITY_FLAG_POS);

  machine.regs[first_operand] = result;

  machine.regs[PIP] += 3;
  return;
}

void sub(void) {

#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "sub\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    regs_name_lookup[first_operand],
    regs_name_lookup[second_operand]);
#endif

  uint32_t result;

  SET_ARBITRARY_BIT(
    machine.flags,
    __builtin_sub_overflow(
      machine.regs[first_operand],
      machine.regs[second_operand],
      &result),
    OVERFLOW_FLAG_POS);

  SET_ARBITRARY_BIT(machine.flags, result == 0, ZERO_FLAG_POS);
  SET_ARBITRARY_BIT(machine.flags, result < 0, SIGN_FLAG_POS);
  SET_ARBITRARY_BIT(machine.flags, result % 2 == 0, PARITY_FLAG_POS);

  machine.regs[first_operand] = result;

  machine.regs[PIP] += 3;
  return;
}

void mul(void) {

#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "mul\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    regs_name_lookup[first_operand],
    regs_name_lookup[second_operand]);
#endif

  uint32_t result;

  SET_ARBITRARY_BIT(
    machine.flags,
    __builtin_mul_overflow(
      machine.regs[first_operand],
      machine.regs[second_operand],
      &result),
    OVERFLOW_FLAG_POS);

  SET_ARBITRARY_BIT(machine.flags, result == 0, ZERO_FLAG_POS);
  SET_ARBITRARY_BIT(machine.flags, result < 0, SIGN_FLAG_POS);
  SET_ARBITRARY_BIT(machine.flags, result % 2 == 0, PARITY_FLAG_POS);

  machine.regs[first_operand] = result;

  machine.regs[PIP] += 3;
  return;
}

void divide(void) {

#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "div\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    regs_name_lookup[first_operand],
    regs_name_lookup[second_operand]);
#endif

  if (machine.regs[second_operand] == 0) {
    printf(ANSI_ERROR "You can't divide by zero!" ANSI_RESET);
    machine.regs[PIP]++;
    machine.mem[machine.regs[PIP]] = HALT;
    return;
  }
  uint32_t temp = machine.regs[first_operand];
  machine.regs[first_operand] /= machine.regs[second_operand];
  machine.regs[PDV] = temp % machine.regs[second_operand];

  SET_ARBITRARY_BIT(machine.flags, machine.regs[first_operand] < 0, SIGN_FLAG_POS);

  SET_ARBITRARY_BIT(machine.flags, machine.regs[PDV] != 0, REMAINDER_FLAG_POS);
  machine.regs[PIP] += 3;
  return;
}

void power(void) {

#ifdef VERBOSE
  printf("%X:\t%02x %02x %02x\t" ANSI_OPC "pow\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    regs_name_lookup[first_operand],
    regs_name_lookup[second_operand]);
#endif
  uint32_t temp = machine.regs[first_operand];
  for (int i = 1; i < machine.regs[second_operand]; i++) {
    machine.regs[first_operand] *= temp;
  }
  machine.regs[PIP] += 3;
  return;
}
