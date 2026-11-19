# Object-Oriented Programming (C++) — NSU Course Projects

Solutions for the Object-Oriented Programming course at Novosibirsk State University (FIT NSU).

The repository covers three distinct projects written in modern C++, progressing from stream-based text analysis to low-level arbitrary-precision arithmetic, and culminating in a custom programming language interpreter for audio processing.

---

## Projects Overview

| Project | Description | Key Topics |
|---|---|---|
| [**`lab0_word_count`**](lab0_word_count/) | **Word Frequency Analyzer**<br>Reads text streams, counts word occurrences, and exports sorted frequency tables to CSV format. | STL containers (`std::map`, `std::vector`), file streams, custom CLI flag parsing, CSV generation. |
| [**`lab1_big_integer`**](lab1_big_integer/) | **BigLong: Arbitrary Precision Arithmetic**<br>A big integer library supporting signed arithmetic with large numbers stored in custom 30-bit limbs. | Operator overloading, template metaprogramming for numeric types, memory management, GoogleTest. |
| [**`lab2_sound_processor`**](lab2_sound_processor/) | **WAV Sound Processor & DSL Interpreter**<br>16-bit PCM audio processor driven either via CLI or through a custom, handwritten Python-like scripting language. | Lexer with indentation tracking (C), recursive descent AST parser (C++), tree-walk interpreter, audio DSP filters. |

---

## Repository Structure

```
├── lab0_word_count/        # Lab 0: Text frequency analyzer
│   ├── cmd/                # Custom command-line argument reader
│   ├── tables/             # Word counting and CSV table export
│   └── examples/           # Sample input text and generated CSV
│
├── lab1_big_integer/       # Lab 1: Big integer library
│   ├── big_long/           # Core library (BigLong class, limbs, operators)
│   ├── tests/              # Comprehensive GoogleTest test suite (34 tests)
│   └── examples/           # Demo script showing calculations
│
├── lab2_sound_processor/   # Lab 2: WAV audio processor & DSL interpreter
│   ├── src/
│   │   ├── libs/file_parser/   # Handwritten C lexer & C++ AST parser
│   │   ├── interpreter/        # Tree-walk interpreter & dynamic types
│   │   ├── libs/wav/           # PCM WAV container parser & audio filters
│   │   └── command_executor/   # CLI dispatcher
│   ├── tests/              # Test suite (parsers, commands, audio filters)
│   └── examples/           # Runnable .dsl scripts and bash workflow demo
│
└── CMakeLists.txt          # Unified root build configuration
```

---

## Toolchain & Requirements

- **Build System**: CMake >= 3.20
- **Compiler**: GCC >= 13 or Clang >= 16 (with C++23 and C11 support)
- **Testing**: GoogleTest (automatically detected via `find_package(GTest)`)

---

## How to Build

### Build Everything at Once

The root `CMakeLists.txt` builds all three labs and connects all test targets:

```bash
# Configure and build all projects
cmake -B build -S .
cmake --build build

# Run the full test suite across all projects
ctest --test-dir build --output-on-failure
```

### Build a Single Project

Each lab can also be built and tested independently from its own directory:

```bash
# Example: Build and test only Lab 2 (Sound Processor)
cd lab2_sound_processor
cmake -B build -S .
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## Running the Examples

Each subproject includes ready-to-run examples in its `examples/` directory:

- **Lab 0**: Run word counter on sample text:
  ```bash
  ./build/bin/word_count lab0_word_count/examples/sample.txt -dest=output.csv -d=, -h=10
  ```

- **Lab 1**: Run the big integer demonstration binary:
  ```bash
  ./build/bin/big_integer_examples
  ```

- **Lab 2**: Run audio processing DSL scripts or the automated bash demo:
  ```bash
  ./build/bin/sound_processor config lab2_sound_processor/examples/mix_audio.dsl
  ./lab2_sound_processor/examples/workflow_demo.sh
  ```
