#include "core.h"
#include <stdio.h>

char *regs_name_lookup[] = {
  [PIP] = "pip",
  [PDV] = "pdv",
  [PAX] = "pax",
  [PBX] = "pbx",
  [PCX] = "pcx",
  [PDX] = "pdx",
};

void nop() {

  if (loglevel == VERBOSE) {
  printf(
    "%08X:\t%02x\t\t" ANSI_OPC "nop" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode);
  }

  machine.regs[PIP]++;
}

void load() {

  if (loglevel == VERBOSE) {
  printf(
    "%08X:\t%02x %02x %02x\t" ANSI_OPC "load " ANSI_NUM "%d" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
    machine.regs[PIP],
    opcode,
    first_operand,
    second_operand,
    first_operand,
    regs_name_lookup[second_operand]);
  }

  machine.regs[second_operand] = first_operand;
  machine.regs[PIP] += 3;
}

void loadmem() {

  if (loglevel == VERBOSE) {
    printf(
      "%08X:\t%02x %02x %02x\t" ANSI_OPC "loadmem\t" ANSI_NUM "%d" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
      machine.regs[PIP],
      opcode,
      first_operand,
      second_operand,
      first_operand,
      regs_name_lookup[second_operand]);
  }

  machine.regs[second_operand] = machine.mem[first_operand];
  machine.regs[PIP] += 3;
}

void store() {

  if (loglevel == VERBOSE) {
    printf(
      "%08X:\t%02x %02x %02x\t" ANSI_OPC "store\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_NUM "%d" ANSI_RESET "\n",
      machine.regs[PIP],
      opcode,
      first_operand,
      second_operand,
      regs_name_lookup[first_operand],
      second_operand);
  }

  machine.mem[second_operand] = machine.regs[first_operand];
  machine.regs[PIP] += 3;
}

void mov() {

  if (loglevel == VERBOSE) {
    printf(
      "%08X:\t%02x %02x %02x\t" ANSI_OPC "mov\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
      machine.regs[PIP],
      opcode,
      first_operand,
      second_operand,
      regs_name_lookup[first_operand],
      regs_name_lookup[second_operand]);
  }

  machine.regs[second_operand] = machine.regs[first_operand];
  machine.regs[PIP] += 3;
}

void movmem() {

  if (loglevel == VERBOSE) {
    printf(
      "%08X:\t%02x %02x %02x\t" ANSI_OPC "movmem\t" ANSI_NUM "%d" ANSI_RESET ", " ANSI_NUM "%d" ANSI_RESET "\n",
      machine.regs[PIP],
      opcode,
      first_operand,
      second_operand,
      first_operand,
      second_operand);
  }

  machine.mem[second_operand] = machine.mem[first_operand];
  machine.regs[PIP] += 3;
}

void add() {

  if (loglevel == VERBOSE) {
    printf(
      "%08X:\t%02x %02x %02x\t" ANSI_OPC "add\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n", machine.regs[PIP],
      opcode,
      first_operand,
      second_operand,
      regs_name_lookup[first_operand],
      regs_name_lookup[second_operand]);
  }

  int32_t result;

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
}

void sub() {

  if (loglevel == VERBOSE) {
    printf(
      "%08X:\t%02x %02x %02x\t" ANSI_OPC "sub\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
      machine.regs[PIP],
      opcode,
      first_operand,
      second_operand,
      regs_name_lookup[first_operand],
      regs_name_lookup[second_operand]);
  }

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
}

void mul() {

  if (loglevel == VERBOSE) {
    printf(
      "%08X:\t%02x %02x %02x\t" ANSI_OPC "mul\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
      machine.regs[PIP],
      opcode,
      first_operand,
      second_operand,
      regs_name_lookup[first_operand],
      regs_name_lookup[second_operand]);
  }

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
}

void divide() {

  if (loglevel == VERBOSE) {
    printf(
      "%08X:\t%02x %02x %02x\t" ANSI_OPC "div\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
      machine.regs[PIP],
      opcode,
      first_operand,
      second_operand,
      regs_name_lookup[first_operand],
      regs_name_lookup[second_operand]);
  }

  if (machine.regs[second_operand] == 0) {
    printf(ANSI_ERROR "You can't divide by zero!" ANSI_RESET);
    machine.regs[PIP]++;
    machine.mem[machine.regs[PIP]] = HALT;
    return;
  }
  int32_t temp = machine.regs[first_operand];
  machine.regs[first_operand] /= machine.regs[second_operand];
  machine.regs[PDV] = temp % machine.regs[second_operand];

  SET_ARBITRARY_BIT(machine.flags, machine.regs[first_operand] < 0, SIGN_FLAG_POS);

  SET_ARBITRARY_BIT(machine.flags, machine.regs[PDV] != 0, REMAINDER_FLAG_POS);
  machine.regs[PIP] += 3;
}

void power() {

  if (loglevel == VERBOSE) {
    printf(
      "%08X:\t%02x %02x %02x\t" ANSI_OPC "pow\t" ANSI_REG "%s" ANSI_RESET ", " ANSI_REG "%s" ANSI_RESET "\n",
      machine.regs[PIP],
      opcode,
      first_operand,
      second_operand,
      regs_name_lookup[first_operand],
      regs_name_lookup[second_operand]);
  }
  int32_t temp = machine.regs[first_operand];

  for (int i = 1; i < machine.regs[second_operand]; i++) {
    SET_ARBITRARY_BIT(
      machine.flags,
      __builtin_mul_overflow(
        machine.regs[first_operand],
        temp,
        &machine.regs[first_operand]),
      OVERFLOW_FLAG_POS);
  }

  machine.regs[PIP] += 3;
}

void pvb() {
  for (int i = 0; i < 25; i++) {
    printf("%.80s\n", &machine.mem[80 * i]);
  }
  printf("\x1b[25A\r");
}
