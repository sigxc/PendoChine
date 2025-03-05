#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define MAX_LABELS 100
#define MAX_LINE_LEN 256
#define MEM_SIZE 65536
#define MAX_REGS 6

#define ERROR "\x1b[31mERROR\x1b[0m"

typedef enum {
  NONE,
  REG,
  IMM,
  ARG_LABEL,
  MEM
} ArgType;

typedef struct {
  const char* mnemonic;
  int opcode;
  int arg_count;
  ArgType arg_types[2];
} Instruction;

const Instruction instructions[] = {
  {"NOP",       0,    0, {NONE, NONE}},
  {"LOAD",      1,    2, {IMM, REG}},
  {"LOADMEM",   2,    2, {REG, MEM}},
  {"STORE",     3,    2, {MEM, REG}},
  {"MOV",       4,    2, {REG, REG}},
  {"MOVMEM",    5,    2, {MEM, MEM}},
  {"ADD",       6,    2, {REG, REG}},
  {"SUB",       7,    2, {REG, REG}},
  {"MUL",       8,    2, {REG, REG}},
  {"DIV",       9,    2, {REG, REG}},
  {"POW",       0x0A, 2, {REG, REG}},
  {"CMP", 	0x0B, 2, {REG, REG}},
  {"JMP", 	0x0C, 1, {MEM}},
  {"JZ" ,       0x0D, 1, {MEM}},
  {"HLT",       -1, 0, {NONE, NONE}},
};

typedef struct {
  char name[50];
  uint16_t address;
} Label;

#endif
