# BigLong — Arbitrary Precision Integer Library (Lab 1)

A C++23 library implementing signed arbitrary-precision integer arithmetic (`BigLong`). The library stores digits in custom bit-width limbs with dynamic vector allocation and provides a comprehensive set of arithmetic and comparison operators.

## Features

- **Multi-Type Constructors**: Construct numbers from standard integer types (`int`, `int64_t`), floating-point values, and decimal strings (`std::string`, `const char*`).
- **Full Operator Overloading**:
  - Arithmetic: `+`, `-`, `*`, `/`, unary `-`
  - In-place compound assignments: `+=`, `-=`, `*=`, `/=`
  - Relational comparisons: `==`, `<`, `>`
- **Mixed-Mode Operations**: Seamless interoperability between `BigLong` instances and primitive C++ numeric types via templated operators.
- **Configurable Digit Base**: Configurable radix and limb size defined in `big_long_config.h` (using 30-bit limbs inside `uint32_t` storage).
- **Comprehensive Test Suite**: 34 unit tests covering edge cases, large-digit operations, sign handling, and division logic with GoogleTest.

## Project Structure

```
lab1_big_integer/
├── big_long/         # Core library implementation and headers
│   ├── big_long.h           # Main BigLong class definition
│   ├── big_long_config.h    # Digit base and bit constants
│   ├── big_long_template.h  # Template operator implementations
│   └── big_long_core.cpp    # Vector normalization and digit ops
├── tests/            # GoogleTest test suites
├── examples/         # Usage examples and demonstrations
├── main.cpp          # Minimal runner
└── CMakeLists.txt    # Build configuration
```

## Build & Test

```bash
# Build library and tests
cmake -B build -S .
cmake --build build

# Run unit tests
ctest --test-dir build --output-on-failure
```

## Usage Example

```cpp
#include "big_long/big_long.h"
#include <iostream>

using bigLong::BigLong;

int main() {
    BigLong a("123456789012");
    BigLong b = 42;

    BigLong sum = a + b;
    BigLong product = a * 1000;

    std::cout << "Sum: " << sum.convertToString() << "\n";
    std::cout << "Product: " << product.convertToString() << "\n";
    return 0;
}
```

Run the included example executable:
```bash
cmake --build build --target big_integer_examples
./build/bin/big_integer_examples
```
