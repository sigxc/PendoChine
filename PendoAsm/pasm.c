#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LABELS 1000
#define MAX_CODE_SIZE 65536
#define MAX_LINE_LEN 256
#define REGS_NUMBER 4

// GREEN - 32
#define ERROR "\x1b[31mERROR\x1b[0m"

typedef struct {
  char     name[32];
  uint32_t address;
} Label;

typedef struct {
  const char *name;
  int         opcode;
  int         num_operands;
  int         operand_types[2];
} OpcodeInfo;

Label labels[MAX_LABELS];
int   label_count = 0;

uint32_t output[MAX_CODE_SIZE];
int      output_pos = 0;

OpcodeInfo opcode_table[] = {
    {"nop", 0x00, 0, {0, 0}},
    {"load", 0x01, 2, {1, 2}},
    {"loadmem", 0x02, 2, {1, 3}},
    {"store", 0x03, 2, {3, 1}},
    {"mov", 0x04, 2, {1, 1}},
    {"movmem", 0x05, 2, {3, 3}},
    {"add", 0x06, 2, {1, 1}},
    {"sub", 0x07, 2, {1, 1}},
    {"mul", 0x08, 2, {1, 1}},
    {"div", 0x09, 2, {1, 1}},
    {"pow", 0x0A, 2, {1, 1}},
    {"cmp", 0x0B, 2, {1, 1}},
    {"jmp", 0x0C, 1, {3}},
    {"jz", 0x0D, 1, {3}},
    {"jnz", 0x0E, 1, {3}},
    {"js", 0x0F, 1, {3}},
    {"jns", 0x10, 1, {3}},
    {"jc", 0x11, 1, {3}},
    {"jnc", 0x12, 1, {3}},
    {"jo", 0x13, 1, {3}},
    {"jno", 0x14, 1, {3}},
    {"jp", 0x15, 1, {3}},
    {"jnp", 0x16, 1, {3}},
    {"inc", 0x17, 1, {1}},
    {"dec", 0x18, 1, {1}},
    {"call", 0x19, 1, {3}},
    {"clz", 0x1A, 0, {0}},
    {"clc", 0x1B, 0, {0}},
    {"cls", 0x1C, 0, {0}},
    {"clo", 0x1D, 0, {0}},
    {"clp", 0x1E, 0, {0}},
    {"cle", 0x1F, 0, {0}},
    {"clb", 0x20, 0, {0}},
    {"cll", 0x21, 0, {0}},
    {"hlt", -1, 0, {0}},
};

void error_exit(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

void add_label(const char *name, uint32_t address) {
  if (label_count >= MAX_LABELS) error_exit(ERROR ": Too many labels\n");
  for (int i = 0; i < label_count; i++) {
    if (strcasecmp(labels[i].name, name) == 0) error_exit(ERROR ": Duplicate label: %s\n", name);
  }

  strncpy(labels[label_count].name, name, 31);
  labels[label_count].address = address;
  label_count++;
}

uint32_t find_label(const char *name) {
  for (int i = 0; i < label_count; i++) {
    if (strcasecmp(labels[i].name, name) == 0) return labels[i].address;
  }
  error_exit(ERROR ": Undefined label: %s\n", name);
  return 0;
}

int label_exists(const char *name) {      /* Это функция чисто, чтобы проверить на точку входа */
  for (int i = 0; i < label_count; i++) { /* Хотя кстати, в целом, может когда-то ещё на какую-то свистоперделку проверю */
    if (strcasecmp(labels[i].name, name) == 0) return 1;
  }
  return 0;
}

int is_register(const char *token) {
  const char *regs[] = {"pax", "pbx", "pcx", "pdx"};
  for (int i = 0; i < REGS_NUMBER; i++) {
    if (strcasecmp(token, regs[i]) == 0) return i;
  }
  return -1;
}

int parse_immediate(const char *token, uint32_t *value) {
  char *endptr;
  if (token[0] == '0' && (token[1] == 'x' || token[1] == 'X'))
    *value = strtoul(token + 2, &endptr, 16);
  else
    *value = strtoul(token, &endptr, 10);

  return *endptr == '\0';
}

void process_line_pass1(char *line, uint32_t *address) {
  char *comment = strchr(line, ';');
  if (comment) *comment = '\0';

  char  *trimmed = line + strspn(line, " \t");
  size_t len = strlen(trimmed);
  while (len > 0 && isspace(trimmed[len - 1])) trimmed[--len] = '\0';
  if (!*trimmed) return;

  char *colon = strchr(trimmed, ':');
  if (colon) {
    *colon = '\0';
    char *label = trimmed;
    while (*label && isspace(*label)) label++;
    char *end = label + strlen(label) - 1;
    while (end > label && isspace(*end)) end--;
    *(end + 1) = '\0';
    add_label(label, *address);
    trimmed = colon + 1;
  }

  char *mnemonic = strtok(trimmed, " \t");
  if (!mnemonic) return;

  for (size_t i = 0; i < sizeof(opcode_table) / sizeof(OpcodeInfo); i++) {
    if (strcasecmp(mnemonic, opcode_table[i].name) == 0) {
      *address += 1 + opcode_table[i].num_operands;
      return;
    }
  }

  error_exit(ERROR ": Unknown instruction: %s\n", mnemonic);
}

void process_line_pass2(char *line, uint32_t *address) {
  char *comment = strchr(line, ';');
  if (comment) *comment = '\0';

  char  *trimmed = line + strspn(line, " \t");
  size_t len = strlen(trimmed);
  while (len > 0 && isspace(trimmed[len - 1])) trimmed[--len] = '\0';
  if (!*trimmed) return;

  char *colon = strchr(trimmed, ':');
  if (colon) trimmed = colon + 1;

  char *mnemonic = strtok(trimmed, " \t");
  if (!mnemonic) return;

  OpcodeInfo *op = NULL;
  for (size_t i = 0; i < sizeof(opcode_table) / sizeof(OpcodeInfo); i++) {
    if (strcasecmp(mnemonic, opcode_table[i].name) == 0) {
      op = &opcode_table[i];
      break;
    }
  }
  if (!op) error_exit(ERROR ": Unknown instruction: %s\n", mnemonic);

  output[output_pos++] = op->opcode;
  (*address)++;

  for (int i = 0; i < op->num_operands; i++) {
    char *token = strtok(NULL, " ,\t");
    if (!token) error_exit(ERROR ": Missing operand %d for %s\n", i + 1, op->name);

    uint32_t value;
    int      reg = is_register(token);

    switch (op->operand_types[i]) {
    case 1:
      if (reg == -1) error_exit(ERROR ": Expected register for operand %d of %s\n", i + 1, op->name);
      value = reg;
      break;

    case 2:
      if (!parse_immediate(token, &value)) error_exit(ERROR ": Invalid immediate value: %s\n", token);
      break;

    case 3:
      if (parse_immediate(token, &value)) break;
      if ((reg = is_register(token)) != -1) {
        value = reg;
        break;
      }
      value = find_label(token);
      break;

    default:
      error_exit(ERROR ": Invalid operand type\n");
    }

    output[output_pos++] = value;
    (*address)++;
  }
}

int main(int argc, char *argv[]) {
  time_t     rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input.asm> <output.pin>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *fp = fopen(argv[1], "r");
  if (!fp) error_exit(ERROR ": Could not open input file\n");

  char     line[MAX_LINE_LEN];
  uint32_t address = 0;
  while (fgets(line, MAX_LINE_LEN, fp)) process_line_pass1(line, &address);

  rewind(fp);
  address = 0;
  output_pos = 0;
  while (fgets(line, MAX_LINE_LEN, fp)) process_line_pass2(line, &address);
  fclose(fp);

  if (output_pos > MAX_CODE_SIZE) error_exit(ERROR ": Out of memory\n");

  FILE *out = fopen(argv[2], "wb");
  if (!out) error_exit(ERROR ": Could not open output file\n");
  fwrite(output, sizeof(uint32_t), output_pos, out);
  fclose(out);

  printf("Compilation \x1b[32mfinished\x1b[0m at %s", asctime(timeinfo));
  return 0;
}
