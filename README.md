# GPT-Tokenator

Tokenator is a `C/C++` library for counting tokens for GPT-3 and GPT-4.

Using the provided library functions, you can calculate how many tokens your request to the OpenAI API takes up.

The library provides a `C` function:
```c
size_t tokenator_count(const char* data, size_t len);

```

Or `C++` functions:

```cpp
namespace tokenator {
    size_t count(const std::string& text) noexcept;
    size_t count(const char* data, size_t len) noexcept;
}
```

Source codes are located in the [src](src) directory.

To compile the library, install the dependencies
```bash
sudo apt-get install libicu-dev
```
And run the command

```bash
cd src
make
```
Compilation takes a long time. Therefore, for your convenience, I have added
precompiled files in the [libs](libs) directory:

`libtokenator.a`, `libtokenator_cpp.a`, `libtokenator_lto.a`, and `libtokenator_cpp_lto.a`

Header files are located in the [include](include) directory.

Here is an [example](test/tok_count.cpp) of using the C++ library
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

And here is an [example](test/tok_count_c.c) of using the C library
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

In the [test](test) directory, you can find examples of using the library for different languages:
- [C++](test/tok_count.cpp)
- [C](test/tok_count_c.c)
- [D](test/tok_count_d.d)
- [Go](test/tok_count_go.go)
- [C#](test/tok_count_cs/Program.cs)

## Source code
[https://github.com/valmat/gpt-tokenator](https://github.com/valmat/gpt-tokenator)

## License
[The MIT License](LICENSE)
