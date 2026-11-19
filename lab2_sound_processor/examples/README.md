# Examples & Workflow Guides

This directory contains executable DSL scripts and workflow demonstrations for `sound_processor`.

## Files

- [**`mix_audio.dsl`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/examples/mix_audio.dsl): Demonstrates audio file opening, interval extraction with `time()` timestamps, and multi-track audio mixing into a newly created WAV file.
- [**`apply_effects.dsl`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/examples/apply_effects.dsl): Demonstrates sequential DSP filter processing across temporal intervals using `bass`, `raise_high`, `distorsion`, and `hach_lada`.
- [**`workflow_demo.sh`**](file:///home/mashina/Documents/last_labs/oop/lab2_sound_processor/examples/workflow_demo.sh): End-to-end bash script running all CLI commands (`help`, `info`, `create`, `mix`, `effect`) and batch execution of DSL scripts.

## Running the Examples

Ensure the project is built:
```bash
cmake -B build -S . && cmake --build build
```

Run individual DSL scripts:
```bash
./build/bin/sound_processor config examples/mix_audio.dsl
./build/bin/sound_processor config examples/apply_effects.dsl
```

Run the complete automated workflow test:
```bash
./examples/workflow_demo.sh
```
