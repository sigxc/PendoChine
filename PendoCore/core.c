#include "core.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printlog(const char *str) {
  if (loglevel == VERBOSE) {
    fputs(str, stdout);
  }
}

void read_binary(const char *filename) {
  FILE *stream = fopen(filename, "rb");
  if (stream == NULL) {
    perror("Failed to read binary");
    exit(1);
  }
  fread(&machine.mem[VGA_BUFFER_SIZE], sizeof(int32_t), MEM_SIZE, stream);
}

void regs_dump() {
  printf("PAX: %08X PBX: %08X\n", machine.regs[PAX], machine.regs[PBX]);
  printf("PCX: %08X PDX: %08X\n", machine.regs[PCX], machine.regs[PDX]);
  printf("PIP: %08X PDV: %08X\n", machine.regs[PIP], machine.regs[PDV]);
  printf("FLAGS: %08b\n", machine.flags);
}

void init() {

  printlog("Initializing machine...\n");
  printlog("\tZeroing general purpose registers\n");

  for (int i = 1; i < NUM_REGS; i++) {
    machine.regs[i] = 0;
  }

  printlog("\tSetting PIP at the beginning of the code section\n");
  machine.regs[PIP] = VGA_BUFFER_SIZE + 1;

  printlog("\tZeroing flags\n");
  machine.flags = 0;

  printlog("\tZeroing machine memory\n");
  memset(machine.mem, '\0', MEM_SIZE * sizeof(uint8_t));
}

void mem_dump() {
  printlog("[    ] Creating dump file");
  fflush(stdout);

  FILE *fd = fopen("mem_dump.bin", "w");
  if (fd == NULL) {
    printlog("\r[ " ANSI_ERROR "ERROR" ANSI_RESET " ] Creating dump file\n");
    perror("\r" ANSI_INFO "fopen" ANSI_RESET "\n");
    return;
  }

  printlog("\r[ " ANSI_SUCCESS "OK" ANSI_RESET " ] Creating dump file\n");
  printlog("[    ] Writing dump");
  fflush(stdout);

  if (fwrite(machine.mem, sizeof(uint8_t), MEM_SIZE, fd) == 0) {
    printlog("\r[ " ANSI_ERROR "ERROR" ANSI_RESET " ] Writing dump\n");
    perror("\r" ANSI_INFO "fwrite" ANSI_RESET "\n");
  };

  printlog("\r[ " ANSI_SUCCESS "OK" ANSI_RESET " ] Writing dump\n");

  fclose(fd);
}

void execute() {

  void (*opcodes_lookup[])() = {
    nop, load, loadmem, store, mov,
    movmem, add, sub, mul, divide,
    power, cmp, /*jmp, jz, jnz,
    js, jns, jo, jno, jp,
    jnp, inc, dec, call, setz,
    setc, sets, seto,
    setp, sete, setb, setl,
    clz, clc, cls, clo, clp, cle,
    clb, cll, pvb*/
  };

  printlog("Executing...\n");
  while (1) {
    if (machine.mem[machine.regs[PIP]] == HALT) {
      printlog("\x1b[80C\x1b[25B\n");
      printlog("Reached " ANSI_OPC "HALT" ANSI_RESET " opcode. Stopping execution\n");
      return;
    }
    opcodes_lookup[opcode]();
  }
}
