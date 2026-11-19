#!/usr/bin/env bash
# ==============================================================================
# Sound Processor - Workflow Demonstration Script
# Demonstrates CLI operations (help, info, create, mix, effect) and DSL scripts.
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

cd "${PROJECT_ROOT}"

# Locate the executable
BIN="./build/bin/sound_processor"
if [[ ! -x "${BIN}" ]]; then
    echo "Executable not found at ${BIN}."
    echo "Please build the project first:"
    echo "  cmake -B build -S . && cmake --build build"
    exit 1
fi

MEDIA_SMOKE="tests/tests_data/wav_lib/media/Smoke.wav"
MEDIA_ALIVE="tests/tests_data/wav_lib/media/Alive.wav"

if [[ ! -f "${MEDIA_SMOKE}" || ! -f "${MEDIA_ALIVE}" ]]; then
    echo "Sample media files not found under tests/tests_data/wav_lib/media/."
    exit 1
fi

echo "=================================================="
echo " 1. CLI Command: help"
echo "=================================================="
"${BIN}" help

echo ""
echo "=================================================="
echo " 2. CLI Command: info"
echo "=================================================="
"${BIN}" info "${MEDIA_SMOKE}"

echo ""
echo "=================================================="
echo " 3. CLI Command: create"
echo "=================================================="
DEMO_WAV="demo_track.wav"
"${BIN}" create "${DEMO_WAV}"
echo "Created empty WAV: ${DEMO_WAV}"
"${BIN}" info "${DEMO_WAV}"

echo ""
echo "=================================================="
echo " 4. CLI Command: mix"
echo "=================================================="
echo "Mixing 5.0 seconds from Smoke.wav at position 0.0s..."
"${BIN}" mix "${DEMO_WAV}" 0.0 "${MEDIA_SMOKE}" 0.0 5.0

echo "Mixing 5.0 seconds from Alive.wav at position 5.0s..."
"${BIN}" mix "${DEMO_WAV}" 5.0 "${MEDIA_ALIVE}" 0.0 5.0

"${BIN}" info "${DEMO_WAV}"

echo ""
echo "=================================================="
echo " 5. CLI Command: effect"
echo "=================================================="
echo "Applying 'bass' effect from 0.0s to 5.0s..."
"${BIN}" effect bass "${DEMO_WAV}" 0.0 5.0

echo "Applying 'distortion' effect from 5.0s to 10.0s..."
"${BIN}" effect distortion "${DEMO_WAV}" 5.0 10.0
echo "Effects applied successfully."

echo ""
echo "=================================================="
echo " 6. DSL Engine: config (mix_audio.dsl)"
echo "=================================================="
"${BIN}" config "examples/mix_audio.dsl" "examples/mix_audio.log"
if [[ -f "mixed_output.wav" ]]; then
    "${BIN}" info "mixed_output.wav"
fi

echo ""
echo "=================================================="
echo " 7. DSL Engine: config (apply_effects.dsl)"
echo "=================================================="
"${BIN}" config "examples/apply_effects.dsl"
if [[ -f "processed_effects.wav" ]]; then
    "${BIN}" info "processed_effects.wav"
fi

echo ""
echo "=================================================="
echo " Cleanup temporary demo artifacts"
echo "=================================================="
rm -f "${DEMO_WAV}" "mixed_output.wav" "processed_effects.wav" "examples/mix_audio.log"
echo "Demo finished successfully and cleaned up generated files."
