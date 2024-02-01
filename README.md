# e125

Это примитивный 8-битный(1 ячейка памяти - 8 бит или 1 байт) процессор с памятью в 1 килобайт.

## Методы

`byte` = `unsigned char`

`LoadToMemory`: `byte Байт` `byte Позиция` (невозможно поместить в первые три байта)  
`GetFromMemory`: `byte Позиция` (возвращает `byte`)

`Run`: `byte Позиция`



Пример главного файла:
```cpp
#include <iostream>
#include "main.hpp"

using byte = unsigned char;

int main()
{
	// add 10 15 5
	LoadToMemory(32, 4);
	LoadToMemory(10, 5);
	LoadToMemory(15, 6);
	LoadTomemory(5, 7);
	LoadToMemory(70, 8); // !!! Обязательно !!! Это значит конец выполнения
	Run(4);

	std::cout << (int)(GetFromMemory(10)) << std::endl;

	return 0;
}
```

## Сделано

* `Move` Копирует данные
* `MoveA`(10) `adr <- value`
* `MoveB`(11) `adr <- adr`

* `Add` Складывает
* `AddA`(30) `adr <- adr1 + adr2`
* `AddB`(31) `adr <- adr1 + value`
* `AddC`(32) `adr <- value + value`

* `Substract` Вычитает
* `SubstractA`(40) `adr <- adr1 - adr2`
* `SubstractB`(41) `adr <- adr1 - value`
* `SubstractC`(42) `adr <- value - adr2`
* `SubstractD`(43) `adr <- value - value`
* Ассемблер

## Запланировано

* `Multiply`/`Division` Арифметические операции
* `Jump` Переходит к указанному адресу
* `Compare` Сверяет первый и второй аргумент, результат записывается в первую ячейку
* `Jump-If-Equals`/`Jump-If-Not-Equals`/`...` Переходит к указанному адресу, если результат сверения равен ...
* Компилятор какого-нибудь языка
* Интерпретатор какого-нибудь языка
* Компилятор Brainfuck