# WAV Sound Processor & DSL Scripting Engine

A high-performance audio processing suite and domain-specific language (DSL) interpreter for 16-bit PCM WAV audio manipulation, built in modern C++23 and C.

## Key Features

- **PCM 16-bit WAV Processing**: Fast chunk-aligned reading, slicing, zero-padding extension, and lossless sample writing.
- **Modular Sound Filters**: Real-time DSP effects including low-frequency boost (`bass`), aggressive bass (`hach_lada`), high-frequency enhancement (`raise_high`), and saturation (`distortion`).
- **Custom DSL Engine**: Python-style indentation-aware grammar (`INDENT`/`DEDENT`), standalone C lexical tokenizer, recursive descent AST parser, and runtime tree interpreter.
- **Robust CLI**: Unified subcommands for immediate file operations, batch automation, and script execution.

## Architecture

```
CLI Parser & Dispatcher  ──>  File Executor  ──>  AST Interpreter  ──>  WAV DSP Engine
 (src/command_executor)       (src/libs/file_parser)   (src/interpreter)    (src/libs/wav)
```

- [**`src/libs/wav/`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/src/libs/wav): Core RIFF/WAV parser, stream allocator, and sample transformation filters.
- [**`src/libs/file_parser/`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/src/libs/file_parser): Low-level C tokenizer managing indentation state and C++ AST builder.
- [**`src/interpreter/`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/src/interpreter): Tree interpreter executing variables, predicates (`exists`), time conversions, and object methods.
- [**`src/command_executor/`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/src/command_executor): CLI interface mapping shell commands to audio operations.

## Build & Test

Prerequisites: CMake (>= 3.10), GCC/Clang with C++23 support, and GoogleTest.

```bash
cmake -B build -S .
cmake --build build
ctest --test-dir build --output-on-failure
```

Run demo workflow:
```bash
./examples/workflow_demo.sh
```

## CLI Reference

| Command | Usage | Description |
|---|---|---|
| `help` | `sound_processor help` | Print usage summary |
| `info` | `sound_processor info <file.wav>` | Display header metadata and duration |
| `create` | `sound_processor create <output.wav>` | Create a new PCM WAV file container |
| `mix` | `sound_processor mix <out> <outSec> <in> <inStart> <inEnd>` | Slice interval and mix into output |
| `effect` | `sound_processor effect <effect> <file> [start] [end]` | Apply filter (`bass`, `hach_lada`, `raise_high`, `distortion`) |
| `config` | `sound_processor config <script.dsl> [logfile]` | Execute batch DSL automation script |

## DSL Scripting Example

Scripts use Python-like 4-space indentation and built-in audio functions:

```python
if exists("tests/tests_data/wav_lib/media/Smoke.wav"):
    src = open_wav("tests/tests_data/wav_lib/media/Smoke.wav")
    out = create_wav("mixed.wav")
    t0 = time("0:0:0")
    t5 = time("0:0:5")
    intro = src.get_interval(t0, t5)
    intro.set_effect("bass")
    out.write_interval(intro, t0, False)
    print("Processed interval successfully")
```
See runnable scripts in [**`examples/`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/examples).
