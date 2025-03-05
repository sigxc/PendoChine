#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"

const char* registers[] = {"PAX", "PBX", "PCX", "PDX"};

Label labels[MAX_LABELS];
int label_count = 0;

uint8_t output[MEM_SIZE];
int out_pos = 0;

void add_label(const char* name, uint16_t addr) {
  if (label_count >= MAX_LABELS) {
    // fprintf(stderr, ERROR ": Too much labels. \x1b[34mMAX\x1b[0m: %d\n", MAX_LABELS);
    fprintf(stderr, ERROR ": Чё ты так много меток в меня впихнул \x1b[34mMAX\x1b[0m: %d\n", MAX_LABELS);
    exit(1);
  }
  strcpy(labels[label_count].name, name);
  labels[label_count].address = addr;
  label_count++;
}

int find_label(const char* name) {
  for (int i = 0; i < label_count; i++) {
    if (strcmp(labels[i].name, name) == 0) return labels[i].address;
  }
  return -1;
}

int parse_reg(const char* reg) {
  for (int i = 0; i < MAX_REGS; i++) {
    if (strcasecmp(reg, registers[i]) == 0) return i;
  }
  return -1;
}

int parse_imm(const char* val) {
  if (val[0] == '0' && val[1] == 'x') return (int)strtol(val, NULL, 16);
  return atoi(val);
}

void write_byte(uint8_t byte) {
  if (out_pos >= MEM_SIZE) {
    fprintf(stderr, ERROR ": Out of memory\n");
    exit(1);
  }
  output[out_pos++] = byte;
}

void remove_commas(char* str) {
  for (int i = 0; str[i]; i++) {
    if (str[i] == ',') {
      str[i] = ' ';
    }
  }
}

/* Вот эта переменная отвечает за стадию компиляции
   --------------------------------\/ */
void process_line(char* line, int stage) {
  char* comment = strchr(line, ';');
  if (comment) *comment = '\0';

  while (isspace(*line)) line++;
  char* end = line + strlen(line) - 1;
  while (end > line && isspace(*end)) end--;
  *(end + 1) = '\0';

  if (!*line) return;

  char* label_end = strchr(line, ':');
  if (label_end) {
    *label_end = '\0';
    if (stage == 1) add_label(line, out_pos);
    return;
  }

  char* token = strtok(line, " \t");
  if (!token) return;

  const Instruction* instr = NULL;
  for (size_t i = 0; i < sizeof(instructions) / sizeof(Instruction); i++) {
    if (strcasecmp(token, instructions[i].mnemonic) == 0) {
      instr = &instructions[i];
      break;
    }
  }
  if (!instr) {
    // fprintf(stderr, ERROR ": Unknown instruction '%s'\n", token);
    fprintf(stderr, ERROR ": Ну и чё значит '%s'\n", token);
    exit(1);
  }

  if (stage == 2) write_byte((uint8_t)instr->opcode);
  // printf("Writing opcode: %d\n", instr->opcode);

  char* args = strtok(NULL, "");
  if (!args && instr->arg_count > 0) {
    // fprintf(stderr, ERROR ": Expected %d arguments for '%s'\n", instr->arg_count, instr->mnemonic);
    fprintf(stderr, ERROR ": Алё! Тебе надо %d аргументов для '%s'\n", instr->arg_count, instr->mnemonic);
    exit(1);
  }

  if (args) {
    remove_commas(args);
    char arg1[50], arg2[50];
    int n = sscanf(args, "%49s %49s", arg1, arg2);

    if (n != instr->arg_count) {
      // fprintf(stderr, ERROR ": Invalid argument count for '%s'\n", instr->mnemonic);
      fprintf(stderr, ERROR ": Что - '%s'? А кто про %d аргумент должен помнить? Я что ли?\n", instr->mnemonic, instr->arg_count);
      exit(1);
    }

    if (stage == 2) {
      for (int i = 0; i < instr->arg_count; i++) {
	char* arg = (i == 0) ? arg1 : arg2;
	// printf("Parsing argument %d: %s\n", i + 1, arg);

	switch (instr->arg_types[i]) {

	case REG: {
	  int reg = parse_reg(arg);
	  if (reg == -1) {
	    // fprintf(stderr, ERROR ": Unknow register '%s'\nInstruction: %s\n", arg, line);
	    fprintf(stderr, ERROR ": Ну и для чего тебе '%s' в %s?\n", arg, line);
	    exit(1);
	  }
	  write_byte(reg);
	  break;
	}

	case IMM: {
	  int imm = parse_imm(arg);
	  write_byte(imm);
	  break;
	}

	case MEM: {
	  int addr = find_label(arg);
	  if (addr == -1) addr = parse_imm(arg);
	  write_byte(addr >> 8);
	  write_byte(addr & 0xFF);
	  break;
	}

	default:
	  break;
	}
      }
    }
  }
}

void assemble(FILE* src) {
  char line[MAX_LINE_LEN];
  while (fgets(line, MAX_LINE_LEN, src)) process_line(line, 1);
  rewind(src);
  out_pos = 0;
  while (fgets(line, MAX_LINE_LEN, src)) process_line(line, 2);
}

int main(int argc, char* argv[]) {
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  if (argc != 3) {
    printf("Usage: %s <input.asm> <output.bin>\n", argv[0]);
    return 1;
  }

  FILE* fin = fopen(argv[1], "r");
  if (!fin) {
    perror(ERROR ": Failed to open input file");
    return 1;
  }

  assemble(fin);
  fclose(fin);

  FILE* fout = fopen(argv[2], "wb");
  if (!fout) {
    perror(ERROR ": Failed to open output file");
    return 1;
  }

  fwrite(output, 1, out_pos, fout);
  fclose(fout);

  printf("Compilation \x1b[32mfinished\x1b[0m at %s", asctime(timeinfo));
  printf("Pinary file: \x1b[32m%d\x1b[0m bytes\n", out_pos);
  return 0;
}
