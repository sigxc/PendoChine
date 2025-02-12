#include "machine.h"

int main(int argc, char *argv[]) {
  Machine machine;
  init(&machine);
  machine.eax = 1;
  machine.ebx = 2;
  machine.mem[0] = ADD;
  machine.mem[1] = EAX;
  machine.mem[2] = EBX;
  machine.mem[3] = HALT;

  execute(&machine);
  regs_dump(&machine);
  mem_dump(&machine);

  return 0;
}
