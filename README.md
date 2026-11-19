# Object-Oriented Programming (C++) — NSU Course Projects

A curated collection of modern C++ projects developed for the Object-Oriented Programming course at Novosibirsk State University (NSU). The repository adheres to industrial software engineering standards with unified CMake build orchestration, comprehensive unit testing, and modular architecture.

## Repository Projects

| Project | Description | Standard | Documentation |
|---|---|---|---|
| [**`lab0_word_count`**](file:///home/mashina/Documents/last_labs/oop/lab0_word_count) | **Word Frequency Analyzer**: Reads text streams, computes relative word frequency statistics, and exports sorted tabular CSV data. | C++20 | [README](file:///home/mashina/Documents/last_labs/oop/lab0_word_count/README.md) |
| [**`lab1_big_integer`**](file:///home/mashina/Documents/last_labs/oop/lab1_big_integer) | **BigLong (Arbitrary Precision Arithmetic)**: High-performance signed big integer arithmetic library with 30-bit limbs and full operator overloading. | C++23 | [README](file:///home/mashina/Documents/last_labs/oop/lab1_big_integer/README.md) |
| [**`lab2_sound_processor`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor) | **WAV Sound Processor & DSL**: 16-bit PCM audio processor featuring real-time DSP filters and a custom indentation-aware DSL scripting interpreter. | C++23 / C11 | [README](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/README.md) |

## Quick Start

### Prerequisites
- CMake >= 3.20
- GCC >= 13 or Clang >= 16 (with C++23 support)
- GoogleTest (for unit test suites)

### Build All Projects

```bash
# Configure and build all subprojects from repository root
cmake -B build -S .
cmake --build build

# Execute entire test suite (all subproject tests)
ctest --test-dir build --output-on-failure
```

### Build an Individual Project

Each subproject is self-contained and can be configured independently:

```bash
# Example: Build and test only lab1_big_integer
cmake -B lab1_big_integer/build -S lab1_big_integer
cmake --build lab1_big_integer/build
ctest --test-dir lab1_big_integer/build --output-on-failure
```

## Engineering Standards

- **Target-Centric CMake**: Unified root `CMakeLists.txt` managing subprojects without target collisions or global state pollution.
- **Out-of-Source Builds**: All build artifacts and binaries are isolated in `build/bin/` to keep the source tree pristine.
- **Automated Testing**: Integration with `CTest` and `GoogleTest` ensuring continuous verification across all components.
- **Practical Examples**: Each subproject includes runnable demonstration scripts and sample data in dedicated `examples/` folders.
