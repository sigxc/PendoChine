# PendoAsm

## Syntax:
```assembly
msg: .string "Hello sPASM\n"

start:
	load pax, msg
	hlt
```
***load, loadmem и подобные на данный момент не засунуты в mov. Возможно в будущем изменится, но это не точно.***
