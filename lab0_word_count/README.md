# Word Frequency Analyzer (Lab 0)

A C++ utility that reads a text file, counts word occurrences, calculates relative word frequencies, and exports a sorted frequency table to CSV format.

## Features

- **Text Stream Tokenizer**: Parses arbitrary text files into words separated by whitespace and punctuation.
- **Statistical Aggregation**: Computes raw counts and relative frequencies for every unique word.
- **Sorted CSV Output**: Automatically sorts words by frequency descending and outputs standard delimited tabular data.
- **Flexible CLI Configuration**: Configurable destination path, custom delimiters (e.g. `,` or `;`), and top-N row limits.

## Project Structure

```
lab0_word_count/
├── cmd/            # Command-line argument and flag parser
├── tables/         # File stream reader and frequency table generator
├── tests/          # Unit tests and test data
├── examples/       # Sample input and output files
├── main.cpp        # Application entrypoint
└── CMakeLists.txt  # Build configuration
```

## Build & Test

```bash
# Build standalone
cmake -B build -S .
cmake --build build

# Run unit tests
ctest --test-dir build --output-on-failure
```

## Usage

```bash
./build/bin/word_count <input_file> [-dest=<output.csv>] [-d=<delimiter>] [-h=<max_rows>]
```

### Options

| Flag | Description | Default |
|---|---|---|
| `<input_file>` | Path to the source text file (required) | — |
| `-dest=<path>` | Destination path for the output CSV file | `output.csv` |
| `-d=<char>` | Column delimiter character | `;` |
| `-h=<int>` | Maximum number of rows to export | `100` |

### Example

```bash
# Analyze sample text and export top 20 words with comma delimiter
./build/bin/word_count examples/sample.txt -dest=examples/sample_output.csv -d=, -h=20
```

**Output preview (`examples/sample_output.csv`):**
```csv
Word,Count,Frequency,
of,5,0.0521
the,5,0.0521
OOP,3,0.0312
objects,3,0.0312
programming,3,0.0312
```
