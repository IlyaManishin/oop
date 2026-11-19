# Lab 0: Word Count

A program that reads a text file, counts word occurrences, and saves the frequencies into a CSV file sorted in descending order.

---

## How to Build

```bash
cmake -B build -S .
cmake --build build

ctest --test-dir build --output-on-failure
```

---

## Usage

```bash
./build/bin/word_count <input_file> [-dest=<output.csv>] [-d=<delimiter>] [-h=<max_rows>]
```

### Options

- `<input_file>` — path to the input text file (required)
- `-dest=<file>` — output CSV file (default: `output.csv`)
- `-d=<char>` — delimiter character (default: `;`)
- `-h=<number>` — max rows to write (default: `100`)

---

## Example

```bash
./build/bin/word_count examples/sample.txt -dest=output.csv -d=, -h=5
```

Result in `output.csv`:

```csv
Word,Count,Frequency,
of,5,0.0521
the,5,0.0521
OOP,3,0.0312
objects,3,0.0312
programming,3,0.0312
```
