#include "core.h"
#include <stdio.h>
#include <stdlib.h>

void init(MACHINE *machine, REGISTERS *regs) {
  regs->eax = 0;
  regs->ebx = 0;
  regs->ecx = 0;
  regs->edx = 0;
  regs->eip = 0;
  regs->esp = MEM_SIZE;

  for (int i = 0; i < MEM_SIZE; i++) {
    machine->mem[i] = 0;
  }
}

void dump_regs(REGISTERS *regs) {
  printf("EAX: %u\n", regs->eax);
  printf("EBX: %u\n", regs->ebx);
  printf("ECX: %u\n", regs->ecx);
  printf("EDX: %u\n", regs->edx);
  printf("EIP: %u\n", regs->eip);
}

void dump_mem(MACHINE *machine) {
  FILE *file = fopen("mem_dump.txt", "w");
  if (file == NULL) {
    perror("ERROR: Failed to open file");
    return;
  }
  for (int i = 0; i < MEM_SIZE; i++) {
    fprintf(file, "mem %d: %u\n", i, machine->mem[i]);
  }
  fclose(file);
}

uint32_t get_register_value(REGISTERS *regs, uint8_t reg_index) {
  switch (reg_index) {
  case EAX:
    return regs->eax;
  case EBX:
    return regs->ebx;
  case ECX:
    return regs->ecx;
  case EDX:
    return regs->edx;
  default:
    printf("Invalid register index: %u\n", reg_index);
    exit(1);
  }
}

void set_register_value(REGISTERS *regs, uint8_t reg_index, uint32_t value) {
  switch (reg_index) {
  case EAX:
    regs->eax = value;
    break;
  case EBX:
    regs->ebx = value;
    break;
  case ECX:
    regs->ecx = value;
    break;
  case EDX:
    regs->edx = value;
    break;
  default:
    printf("Invalid register index: %u\n", reg_index);
    exit(1);
  }
}

void exec_instruction(MACHINE *machine, REGISTERS *regs) {
  Instruction instruction = (Instruction)machine->mem[regs->eip];

  printf("Instruction: 0x%02X at EIP: %u\n", instruction, regs->eip);
  printf("--------------------------------\n");

  uint8_t reg1_index, reg2_index;
  switch (instruction) {
  case NOP:
    regs->eip++;
    return;

  case HALT:
    printf("Halted\n");
    dump_regs(regs);
    exit(0);

  case LOAD:
    reg1_index = machine->mem[regs->eip + 1];
    uint32_t value = *((uint32_t *)&machine->mem[regs->eip + 2]);
    set_register_value(regs, reg1_index, value);
    regs->eip += 6;
    return;

  case ADD:
    reg1_index = machine->mem[regs->eip + 1];
    reg2_index = machine->mem[regs->eip + 2];
    set_register_value(regs, reg1_index, get_register_value(regs, reg1_index) + get_register_value(regs, reg2_index));
    regs->eip += 3;
    return;

  case DIV:
    reg1_index = machine->mem[regs->eip + 1];
    reg2_index = machine->mem[regs->eip + 2];
    uint32_t divisor = get_register_value(regs, reg2_index);
    if (divisor == 0) {
      printf("ERROR: Division by zero\n");
      exit(1);
    }
    set_register_value(regs, reg1_index, get_register_value(regs, reg1_index) / divisor);
    regs->eip += 3;
    return;

  case MUL:
    reg1_index = machine->mem[regs->eip + 1];
    reg2_index = machine->mem[regs->eip + 2];
    set_register_value(regs, reg1_index, get_register_value(regs, reg1_index) * get_register_value(regs, reg2_index));
    regs->eip += 3;
    return;

  case SUB:
    reg1_index = machine->mem[regs->eip + 1];
    reg2_index = machine->mem[regs->eip + 2];
    set_register_value(regs, reg1_index, get_register_value(regs, reg1_index) - get_register_value(regs, reg2_index));
    regs->eip += 3;
    return;

  case INC:
    reg1_index = machine->mem[regs->eip + 1];
    uint32_t inc_value = get_register_value(regs, reg1_index);
    set_register_value(regs, reg1_index, inc_value + 1);
    regs->eip += 2;
    return;

  case DEC:
    reg1_index = machine->mem[regs->eip + 1];
    uint32_t dec_value = get_register_value(regs, reg1_index);
    set_register_value(regs, reg1_index, dec_value - 1);
    regs->eip += 2;
    return;

  case JMP:
    uint32_t jump_address = *((uint32_t *)&machine->mem[regs->eip + 1]);
    regs->eip = jump_address;
    return;

  case CALL:
    uint32_t return_address = regs->eip + 5;
    machine->mem[regs->esp - 4] = return_address;
    regs->esp -= 4;

    uint32_t target_address = *((uint32_t *)&machine->mem[regs->eip + 1]);
    regs->eip = target_address;
    return;

  default:
    printf("Unknown instruction: 0x%02X\n", instruction);
    dump_regs(regs);
    exit(1);
  }
}
