#include <stdio.h>
#include <unistd.h>
#include "core.h"

int main(int argc, char *argv[]) {
  loglevel = DEFAULT;
  int arg = 0;

  while ((arg = getopt(argc, argv, "v")) != -1) {
    switch(arg) {
      case 'v': {
        loglevel = VERBOSE;
        break;
      }
      default: {
        puts("Unknown argument!");
        break;
      }
    }
  }

  init();
  read_binary("executable.pbin");
  execute();
  regs_dump();
  mem_dump();

  return 0;
}
