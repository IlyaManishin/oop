# Lab 1: Big Integer (BigLong)

A C++ library for arithmetic with large integers (`BigLong`).

Numbers are stored as vectors of 30-bit digits, allowing calculations beyond standard integer types.

---

## Features

- Construct from `int`, `long long`, `double`, or string
- Basic arithmetic: `+`, `-`, `*`, `/`
- Compound assignments: `+=`, `-=`, `*=`, `/=`
- Comparisons: `<`, `>`, `==`

---

## How to Build

```bash
cmake -B build -S .
cmake --build build

ctest --test-dir build --output-on-failure
```

---

## Code Example

```cpp
#include "big_long/big_long.h"
#include <iostream>

using bigLong::BigLong;

int main() {
    BigLong a("123456789012");
    BigLong b = 42;

    BigLong sum = a + b;
    BigLong prod = a * 1000;

    std::cout << "Sum: " << sum.convertToString() << "\n";
    std::cout << "Product: " << prod.convertToString() << "\n";
    return 0;
}
```

To run the included demo:

```bash
cmake --build build --target big_integer_examples
./build/bin/big_integer_examples
```
