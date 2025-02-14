#include "core.h"
#include <stdio.h>

int main() {
  MACHINE   machine;
  REGISTERS regs;
  init(&machine, &regs);

  // Давай ка скажу, что здесь происходит
  machine.mem[0] = LOAD; // Это короче чтобы загрузать...
  machine.mem[1] = pip;  // в регистр 0...
  machine.mem[2] = 42;   // вот это число.

  machine.mem[6] = LOAD; // Тут тоже самое, только загружаем мы 8 в регистр 1
  machine.mem[7] = EBX;
  machine.mem[8] = 8;

  machine.mem[12] = DIV; // Здесь используется инструкция для сложения
  machine.mem[13] = pip; // Вот этого регистра, в котором 42
  machine.mem[14] = EBX; // И вот этого, в котором 8

  machine.mem[18] = CALL;
  machine.mem[19] = 39;

  machine.mem[39] = HALT; // А тут выход с 1

  dump_mem(&machine);

  while (1) {
    if (regs.eip >= MEM_SIZE) {
      printf("ERROR: Out of mem\n");
      return 1;
    }
    exec_instruction(&machine, &regs);
  }

  return 0;
}
