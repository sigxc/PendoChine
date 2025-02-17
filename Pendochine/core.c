#include "machine.h"

Machine machine; // Needed to avoid multiple declarations

int main(int argc, char *argv[]) {
  init();
  machine.regs[PAX] = 2;
  mem(0) = STORE;
  mem(1) = PAX;
  mem(2) = 0xFF;
  mem(3) = LOADMEM;
  mem(4) = 127;
  mem(5) = PBX;
  mem(6) = ADD;
  mem(7) = PAX;
  mem(8) = PBX;
  mem(9) = NOP;
  mem(10) = MOV;
  mem(11) = PAX;
  mem(12) = PBX;
  mem(13) = MOVMEM;
  mem(14) = 127;
  mem(15) = 126;
  machine.regs[PCX] = 5;
  machine.regs[PDX] = 0;
  mem(16) = DIV;
  mem(17) = PCX;
  mem(18) = PDX;
  mem(19) = LOAD;
  mem(20) = 8;
  mem(21) = PDX;
  mem(22) = POW;
  mem(23) = PCX;
  mem(24) = PDX;
  mem(25) = HALT;

  execute();
  regs_dump();
  mem_dump();

  return 0;
}
