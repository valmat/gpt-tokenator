# GPT-Tokenator

Tokenator -- это `C/C++` библиотека для подсчёта токенов для GPT-3 и GPT-4

Спомощью предоставляемых библиотекой функций вы можете рассчитать сколкьо токенов занимает ваш запрос к API OpenAI

Библиотека предсотавляет `C` функцию:
```c
size_t tokenator_count(const char* data, size_t len);

```

Или `C++` функции:

```cpp
namespace tokenator {
    size_t count(const std::string& text) noexcept;
    size_t count(const char* data, size_t len) noexcept;
}
```

Исходные коды располагаются в катологе [src](src).

Для компиляции библиотеки установите зависимости
```bash
sudo apt-get install libicu-dev
```
И выполните команду

```bash
cd src
make
```
Компиляция занимает продолжительное время. Поэтому для вашего удобства я добавил
предкомпилированные файлы в каталоге [libs](libs):

`libtokenator.a`, `libtokenator_cpp.a`, `libtokenator_lto.a` и `libtokenator_cpp_lto.a`

Заголовочные файлы расположены в каталоге [include](include)

Вот [пример](test/tok_count.cpp) использования C++-библиотеки
```cpp
#include <iostream>
#include <string>
#include <cassert>
#include "tokenator.hpp"

int main() {
    std::string str{"This is a test, and it's working!"};
    size_t count = tokenator::count(str);
    std::cout << "phrase (" << count << " ):\t" << str << std::endl;
    assert(count == 10);

    return 0;
}
```

А вот [пример](test/tok_count_c.c) использования C-библиотеки
```c
#include "tokenator.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main() {
    const char* data = "This is a test, and it's working!";
    size_t count = tokenator_count(data, strlen(data));
    printf("phrase (%zu):\t%s\n", count, data);
    assert(count == 10);

    return 0;
}
```

В каталоге [test](test) вы можете найти примеры использования библиотеки для разных языков:
- [C++](test/tok_count.cpp)
- [C](test/tok_count_c.c)
- [D](test/tok_count_d.d)
- [Go](test/tok_count_go.go)
- [C#](test/tok_count_cs/Program.cs)

## The MIT License
[The MIT License](LICENSE)