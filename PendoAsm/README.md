# PendoAsm

## Синтаксис:
```assembly
msg: .string "Hello sPASM\n"

start:
	mov pax, [msg] ;; Перемещаем строку в pax
	hlt
```

## Инструкции:
```
 - nop     : _
 - mov     : [reg/label], [reg/imm/label]

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
