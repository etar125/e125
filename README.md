# e125

Это примитивный 8-битный(1 ячейка памяти - 8 бит или 1 байт) процессор с памятью в 255 байт.

## Методы

`byte` = `unsigned char`

`LoadToMemory`: `byte Байт` `byte Позиция` (невозможно поместить в первые три байта)  
`GetFromMemory`: `byte Позиция` (возвращает `byte`)

`Run`: `byte Позиция`



Чтобы запустить файл с кодом: `./main -w -o main.txt`

`main` - e125  
`-w` - записать память в файл `memory-РАЗМЕРФАЙЛА`  
`-o` - запускать всё в одном классе (короче говоря одна память на все файлы для запуска)  
`main.txt` - файл с кодом в **бинарном** виде.



Если не хотите писать код в программах по типу GHex и т.п., есть ассемблер.

## Ассемблер

Поддерживаются три инструкции - `add`, `sub` и `mov`.  
Сначала в строчке стоит инструкция, потом аргументы.  
Если в начале аргумента стоит `$`, он будет принят как число, а не как адрес.  

Пример файла с кодом:
```
; сложение 5 и 15 и вычитание 8, перемещение результата в какой-нибудь адрес
add 128 $5 $15
sub 129 128 $8
mov 127 129
```

Чтобы получить файл с чистым бинарным кодом, прописываем: `./asm` (это работает только в том случае, если файл с кодом называется main.asm)  
Если хотите узнать больше, пропишите `./asm -help`.  

## Сделано

- `Move` Копирует данные
- - `MoveA`(10) `adr <- value`
- - `MoveB`(11) `adr <- adr`

- `Add` Складывает
- - `AddA`(30) `adr <- adr1 + adr2`
- - `AddB`(31) `adr <- adr1 + value`
- - `AddC`(32) `adr <- value + value`

- `Substract` Вычитает
- - `SubstractA`(40) `adr <- adr1 - adr2`
- - `SubstractB`(41) `adr <- adr1 - value`
- - `SubstractC`(42) `adr <- value - adr2`
- - `SubstractD`(43) `adr <- value - value`
- Ассемблер

## Запланировано

- `Multiply`/`Division` Арифметические операции
- `Jump` Переходит к указанному адресу
- `Compare` Сверяет первый и второй аргумент, результат записывается в первую ячейку
- `Jump-If-Equals`/`Jump-If-Not-Equals`/`...` Переходит к указанному адресу, если результат сверения равен ...
- Компилятор какого-нибудь языка
- Интерпретатор какого-нибудь языка
- Компилятор Brainfuck