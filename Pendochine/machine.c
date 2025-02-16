#include "machine.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void regs_dump() {
  printf("PAX: %08X PBX: %08X\n", machine.regs[PAX],
         machine.regs[PBX]);
  printf("PCX: %08X PDX: %08X\n", machine.regs[PCX],
         machine.regs[PDX]);
  printf("PIP: %08X PDV: %08X\n", machine.regs[PIP],
         machine.regs[PDV]);
  printf("FLAGS: %b\n", machine.flags);
}

#ifndef VERBOSE
void init() {
  for (int i = 1; i < NUM_REGS; i++) {
    machine.regs[i] = 0;
  }
  machine.regs[PIP] = VGA_BUFFER_SIZE + 1;
  machine.flags = 0;
  memset(machine.mem, '\0', MEM_SIZE * sizeof(uint32_t));
}

void mem_dump() {
  FILE *fd = fopen("mem_dump.bin", "w");

  if (fd == NULL) {
    perror("fopen");
    return;
  }

  if (fwrite(machine.mem, sizeof(uint32_t), MEM_SIZE, fd) == 0) {
    perror("fwrite");
  }

  fclose(fd);
}

void execute() {

  // clang-format off
  void (*funcs_lookup[])() = {
    nop, load, loadmem, store, mov,
    movmem, add, sub, mul, divide,
    power, /*cmp, jmp, jz, jnz,
    js, jns, jo, jno, jp,
    jnp, inc, dec, call, clz,
    clc, cls, clo, clp, cle,
    clb, cll*/
  };
  // clang-format on

  while (1) {
    if (machine.mem[machine.regs[PIP]] == HALT) {
      return;
    }

    funcs_lookup[opcode]();
  }
}
#endif

#ifdef VERBOSE
void init() {

  printf("Initializing machine...\n");
  printf("\tZeroing general purpose registers\n");

  for (int i = 1; i < NUM_REGS; i++) {
    machine.regs[i] = 0;
  }

  printf("Setting PIP at the beginning of the code section\n");
  machine.regs[PIP] = VGA_BUFFER_SIZE + 1;

  printf("\tZeroing flags\n");
  machine.flags = 0;

  printf("\tZeroing machine memory\n");
  memset(machine.mem, '\0', MEM_SIZE * sizeof(uint32_t));
}

void mem_dump() {
  printf("[    ] Creating dump file");
  fflush(stdout);

  FILE *fd = fopen("mem_dump.bin", "w");
  if (fd == NULL) {
    printf("\r[ " ANSI_ERROR "ERROR" ANSI_RESET
           " ] Creating dump file\n");
    perror("\r" ANSI_INFO "fopen" ANSI_RESET "\n");
    return;
  }

  printf("\r[ " ANSI_SUCCESS "OK" ANSI_RESET
         " ] Creating dump file\n");
  printf("[    ] Writing dump");
  fflush(stdout);

  if (fwrite(machine.mem, sizeof(uint32_t), MEM_SIZE, fd) == 0) {
    printf("\r[ " ANSI_ERROR "ERROR" ANSI_RESET
           " ] Writing dump\n");
    perror("\r" ANSI_INFO "fwrite" ANSI_RESET "\n");
  };

  printf("\r[ " ANSI_SUCCESS "OK" ANSI_RESET
         " ] Writing dump\n");

  fclose(fd);
}

void execute() {

  // clang-format off
  void (*opcodes_lookup[])() = {
    nop, load, loadmem, store, mov,
    movmem, add, sub, mul, divide,
    power, /*cmp, jmp, jz, jnz,
    js, jns, jo, jno, jp,
    jnp, inc, dec, call, clz,
    clc, cls, clo, clp, cle,
    clb, cll*/
  };
  // clang-format on
  printf("Executing...\n");
  while (1) {
    if (machine.mem[machine.regs[PIP]] == HALT) {
      printf("Reached " ANSI_OPC "HALT" ANSI_RESET
             " opcode. Stopping execution\n");
      return;
    }
    opcodes_lookup[opcode]();
  }
}
#endif
