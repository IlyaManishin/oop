# Examples

Sample scripts and workflow for `sound_processor`.

---

## Files

- [**`mix_audio.dsl`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/examples/mix_audio.dsl) — opens files, extracts an interval, and mixes it into a new WAV file.
- [**`apply_effects.dsl`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/examples/apply_effects.dsl) — applies audio effects (`bass`, `raise_high`, `distortion`).
- [**`workflow_demo.sh`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/examples/workflow_demo.sh) — demo bash script running basic CLI commands and DSL scripts.

---

## How to Run

Build the project first:

```bash
cmake -B build -S .. && cmake --build build
```

Run a DSL script:

```bash
./build/bin/sound_processor config mix_audio.dsl
```

Or run the full demo script:

```bash
./workflow_demo.sh
```
