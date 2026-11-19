# Lab 2: Sound Processor

A WAV file processor written in C++ and C.

It can cut audio intervals, mix multiple tracks, and apply simple sound effects. It can be controlled from the command line or through a configuration script with Python-like indentation.

---

## Features

- **WAV processing**: reads and writes 16-bit PCM WAV files
- **Audio effects**: `bass`, `hach_lada`, `raise_high`, `distortion`
- **Script interpreter**: custom tokenizer and parser for configuration scripts
- **CLI commands**: create, mix, view info, apply effects

---

## How to Build

```bash
cmake -B build -S .
cmake --build build

ctest --test-dir build --output-on-failure
```

---

## CLI Usage

| Command | Example | What it does |
|---|---|---|
| `help` | `sound_processor help` | Show help message |
| `info` | `sound_processor info file.wav` | Print WAV header information |
| `create` | `sound_processor create out.wav` | Create an empty WAV file |
| `mix` | `sound_processor mix out.wav 0 in.wav 5 15` | Insert interval 5s–15s from `in.wav` into `out.wav` at 0s |
| `effect` | `sound_processor effect bass file.wav 0 10` | Apply effect to interval |
| `config` | `sound_processor config script.dsl` | Run commands from a script |

---

## Script Example

Scripts use 4-space indentation:

```python
src = open_wav("input.wav")
out = create_wav("output.wav")

t0 = time("0:0:0")
t10 = time("0:0:10")

clip = src.get_interval(t0, t10)
clip.set_effect("bass")

out.write_interval(clip, t0, False)
```

Runnable scripts are located in [**`examples/`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/examples).
