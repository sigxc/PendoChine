# PendoAsm

## Синтаксис:
```assembly
msg: .string "Hello sPASM\n"

start:
	load pax, msg ;; Перемещаем строку в pax
	hlt
```

## Инструкции:
```
 - nop     : _
 - load    : reg, imm
 - loadmem : reg, mem
 - store   : mem, reg
 - mov     : reg, reg
 - movmem  : mem, mem
 
   add, sub \
 - mul, div  > reg, reg
   pow, cmp /
   
   jmp, jz    \
   js, jns     \
 - jc, jnc, jnz > mem, mem
   jo, jno     /
   jp, jnp    /
   
 - inc, dec  : reg
 - call      : mem
 
   clz, clc, cls \
 - clo, clp, cle  > _
   clb, cll      /
   
 - hlt       : _
```
## Структуры:

 - Метка:
   - Имя
   - Адресс
 
 - Инструкция:
   - Имя
   - Инструкция
   - Кол-во операндов
   - Тип операнда

***load, loadmem и подобные на данный момент не засунуты в mov. Возможно в будущем изменится, но это не точно.***
