#include "machine.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init(Machine *machine) {
  machine->eip = 0;
  machine->eax = 0;
  machine->ebx = 0;
  machine->ecx = 0;
  machine->edx = 0;
  machine->flags = 0;
  memset(machine->mem, '\0', mem_SIZE);
}

void regs_dump(Machine *machine) {
  printf("EAX: %08X EBX: %08X", machine->eax, machine->ebx);
  printf("ECX: %08X EDX: %08X", machine->ecx, machine->edx);
  printf("EIP: %08X FLAGS: %b", machine->eip, machine->flags);
}

void mem_dump(Machine *machine) {
  FILE *fd = fopen("mem_dump.txt", "w");
  if (fd == NULL) {
    perror("fopen");
    return;
  }
  fwrite(machine->mem, sizeof(unsigned char), mem_SIZE, fd);
  fclose(fd);
}

// Здесь ОЧЕНЬ МНОГО плейсхолдеров
void execute(Machine *machine) {
  // clang-format off
  uint32_t *regs_lookup[] = {
    [EAX] = &machine->eax,
    [EBX] = &machine->ebx,
    [ECX] = &machine->ecx,
    [EDX] = &machine->edx};
  // clang-format on

  while (1) {
    uint8_t opcode = machine->mem[machine->eip];
    uint8_t operand1 = machine->mem[machine->eip + 1];
    uint8_t operand2 = machine->mem[machine->eip + 2];

    switch (opcode) {

    case NOP: {
      machine->eip++;
      break;
    }

    case LOAD: {
      break;
    }

    case LOADMEM: {
      break;
    }

    case STORE: {
      break;
    }

    case STOREMEM: {
      break;
    }

    case ADD: {
      *regs_lookup[operand1] = *regs_lookup[operand1] + *regs_lookup[operand2];
      break;
    }

    case SUB: {
      break;
    }

    case MUL: {
      break;
    }

    case DIV: {
      break;
    }

    case POW: {
      break;
    }

    case SQRT: {
      break;
    }

    case JMP: {
      machine->eip = operand1;
      break;
    }

    case JZ: {
      break;
    }
    case JNZ: {
      break;
    }

    case HALT: {
      return;
    };

    default:
      printf("Unknown opcode: %02X\n", opcode);
      return;
    }
  }
}
