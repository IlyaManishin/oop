# Lab 2: Sound Processor & Python-like DSL Interpreter

A WAV audio processing suite with a custom, handwritten interpreter for a Python-style scripting language.

Instead of writing a rigid command-line filter, this project implements a full compilation pipeline — a lexical tokenizer in C that tracks indentation, a recursive descent AST parser in C++, a tree-walk interpreter with dynamic typing, and a 16-bit PCM audio engine.

---

## The Big Idea: Handwritten Python-like DSL

The core highlight of this project is its scripting language. You can automate audio processing tasks using a clean, Python-like syntax with significant indentation:

```python
# Check source, create destination, and slice audio
if exists("tests/tests_data/wav_lib/media/Smoke.wav"):
    src = open_wav("tests/tests_data/wav_lib/media/Smoke.wav")
    dest = create_wav("output.wav")

    # Time conversion: H:M:S format into seconds
    t_start = time("0:0:10")
    t_end   = time("0:0:25")

    # Extract slice, apply audio filter, and mix
    clip = src.get_interval(t_start, t_end)
    clip.set_effect("bass")

    dest.write_interval(clip, t_start, False)
    print("Track processed successfully!")
else:
    print("Source audio file not found.")
```

---

## How the Engine Works

```
Script Text (.dsl)
       │
       ▼
[ C Tokenizer ]         ──> Tracks 4-space indentation; emits INDENT, DEDENT, NEWLINE
       │
       ▼
[ C++ AST Parser ]      ──> Recursive descent; builds Abstract Syntax Tree
       │
       ▼
[ Tree Interpreter ]    ──> Scopes, variable storage, dynamic type system (ExObj)
       │
       ▼
[ WAV Audio Core ]      ──> 16-bit PCM reader/writer, sample manipulation & filters
```

### 1. Lexer / Tokenizer (`src/libs/file_parser/tokenizer/`)
Written in C. It implements an indentation stack that tracks leading whitespace (treating tabs as 4 spaces) and automatically synthesizes `INDENT` and `DEDENT` tokens just like Python's lexer. It also handles string literals (single, double, and backtick quotes), numeric literals, identifiers, and symbols.

### 2. AST Parser (`src/libs/file_parser/`)
A recursive descent parser in C++. It parses statements into an Abstract Syntax Tree representing:
- Assignments (`ident = expr`)
- Function calls (`open_wav(...)`, `time(...)`, `print(...)`)
- Method invocations (`file.get_interval(...)`, `interval.set_effect(...)`)
- Conditionals (`if <predicate>: ... else: ...`)

### 3. Tree-Walk Interpreter (`src/interpreter/tree_interpreter/`)
Executes the AST directly using a runtime object system (`ExObj`):
- **Dynamic types**: `WavFileType`, `WavIntervalType`, `FloatType`, `StringType`, `BoolType`
- **Variable environments**: Scoped symbol tables for storing and resolving variables
- **Predicates**: `exists(path)` checks file availability for branching logic
- **Built-in functions**: `open_wav`, `create_wav`, `time("H:M:S")`, `print`

### 4. WAV Audio Engine (`src/libs/wav/`)
Manipulates 16-bit PCM stereo/mono WAV containers:
- Reads and validates standard RIFF/WAV headers
- Slices arbitrary temporal intervals
- Mixes and inserts audio segments with automatic zero-padding
- DSP filters:
  - `normal` — clean pass-through
  - `bass` — low-frequency amplification
  - `hach_lada` — aggressive low-end saturation
  - `raise_high` — high-frequency boost
  - `distortion` — signal clipping and drive

---

## Building and Testing

Prerequisites: CMake (>= 3.10), C++23 / C11 compiler, GoogleTest.

```bash
# Build the project
cmake -B build -S .
cmake --build build

# Run unit tests (CLI parser, tokenizer, AST parser, WAV reader, effects)
ctest --test-dir build --output-on-failure
```

---

## CLI Interface

In addition to DSL scripts, the program can be driven directly via terminal commands:

| Command | Usage | Description |
|---|---|---|
| `help` | `sound_processor help` | Print help message |
| `info` | `sound_processor info <file.wav>` | Display WAV header info (sample rate, channels, duration) |
| `create` | `sound_processor create <file.wav>` | Create a blank WAV container |
| `mix` | `sound_processor mix <out> <outSec> <in> <inStart> <inEnd>` | Copy a time interval from one file into another |
| `effect` | `sound_processor effect <effect> <file.wav> [start] [end]` | Apply audio filter (`bass`, `distortion`, `raise_high`, etc.) |
| `config` | `sound_processor config <script.dsl> [log.txt]` | Execute a Python-like DSL script |

---

## Examples

Runnable example scripts and a bash demo are in [**`examples/`**](examples/):

```bash
# Run a DSL mixing script
./build/bin/sound_processor config examples/mix_audio.dsl

# Run an effects demonstration
./build/bin/sound_processor config examples/apply_effects.dsl

# Run the complete automated workflow demo
./examples/workflow_demo.sh
```
