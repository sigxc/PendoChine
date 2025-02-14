#include "machine.h"

Machine machine; // Needed to avoid multiple declarations

int main(int argc, char *argv[]) {
  init();
  machine.regs[PAX] = 2;
  machine.mem[0] = STORE;
  machine.mem[1] = PAX;
  machine.mem[2] = 0xFF;
  machine.mem[3] = LOADMEM;
  machine.mem[4] = 255;
  machine.mem[5] = PBX;
  machine.mem[6] = ADD;
  machine.mem[7] = PAX;
  machine.mem[8] = PBX;
  machine.mem[9] = NOP;
  machine.mem[10] = MOV;
  machine.mem[11] = PAX;
  machine.mem[12] = PBX;
  machine.mem[13] = MOVMEM;
  machine.mem[14] = 255;
  machine.mem[15] = 254;
  machine.mem[16] = HALT;

  execute();
  regs_dump();
  mem_dump();

  return 0;
}
