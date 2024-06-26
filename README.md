# e125

Это примитивный 8-битный(1 ячейка памяти - 8 бит или 1 байт) процессор с памятью в 255 байт.

## Сборка

### Под Linux

Установите компилятор Clang.  

Склонируйте репозеторий: `git clone https://github.com/etar125/e125.git`  

Соберите:  
```
cd e125
mkdir test
make
```

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

- `Multiply` Умножает
- - `MultiplyA`(50) `adr <- adr1 * adr2`
- - `MultiplyB`(51) `adr <- adr1 * value`
- - `MultiplyC`(52) `adr <- value * value`

- `Division` Делит
- - `DivisionA`(60) `adr <- adr1 / adr2`
- - `DivisionB`(61) `adr <- adr1 / value`
- - `DivisionC`(62) `adr <- value / adr2`
- - `DivisionD`(63) `adr <- value / value`

- Ассемблер

- `Jump` Переходит к указанному адресу
- - `JumpA`(20) `value`
- - `JumpB`(21) `adr`

- `Jump-If-Zero` Переходит к указанному адресу, если значение равно нулю
- - `Jump-If-ZeroA`(22) `adr value`
- - `Jump-If-ZeroB`(23) `adr adr`

- `Jump-If-Not-Zero` Переходит к указанному адресу, если значение не равно нулю
- - `Jump-If-Not-ZeroA`(24) `adr value`
- - `Jump-If-Not-ZeroB`(25) `adr adr`

- Поддержка расширений

## Запланировано

- `Compare` Сверяет первый и второй аргумент, результат записывается в первую ячейку
- `Jump-If-Equals`/`Jump-If-Not-Equals`/`...` Переходит к указанному адресу, если результат сверения равен ...
- Компилятор какого-нибудь языка
- Интерпретатор какого-нибудь языка
- Компилятор Brainfuck
- Стэк
- `Push` Ложит в стэк
- `Call`/`Return`
- Возможность транслировать байт-код в языки программирования C++, C#, Python, Visual Basic .NET, TinySS.