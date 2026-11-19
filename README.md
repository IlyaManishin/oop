# OOP Labs (NSU)

Solutions for the Object-Oriented Programming course at Novosibirsk State University (FIT NSU).

Written in C++.

---

## Projects

- [**`lab0_word_count`**](file:///home/mashina/Documents/last_labs/oop/lab0_word_count)  
  Word counter. Reads a text file, calculates word frequencies, and saves them to a CSV table.

- [**`lab1_big_integer`**](file:///home/mashina/Documents/last_labs/oop/lab1_big_integer)  
  Big integer library (`BigLong`). Supports arithmetic (`+`, `-`, `*`, `/`) and comparisons for large numbers.

- [**`lab2_sound_processor`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor)  
  WAV audio processor. Cuts, mixes, and applies audio filters via CLI or a simple script.

---

## How to Build

All projects use CMake.

```bash
# Build all labs
cmake -B build -S .
cmake --build build

# Run all tests
ctest --test-dir build --output-on-failure
```

You can also build any lab separately from its own directory:

```bash
cd lab0_word_count
cmake -B build -S .
cmake --build build
```
