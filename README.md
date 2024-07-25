# Bytecomp
## Efficient Byte-Level File Comparison Utility
   
<br/>

**bytecomp** is a command-line utility that performs a byte-level comparison of two files and provides detailed statistics about their differences, including:

- **Difference Percentage:** The percentage of bytes that differ between the two files.
- **Average Value Divergence:** The average amount (as a percentage) by which the differing byte values diverge.
- **Entropy (Optional):** The entropy of each file, providing a measure of their randomness or information content.

**Key Features:**

- **Memory Efficiency:** Uses memory mapping (`mmap`) to avoid loading entire files into memory, making it suitable for comparing large files.
- **Chunk-Based Processing:** Processes files in chunks to ensure smooth progress and prevent slowdowns with larger files.
- **Real-Time Progress:** Displays a progress bar and real-time statistics during comparison.
- **Optional Entropy Calculation:** Entropy calculation can be enabled using the `-e` flag.

## Usage
```
bytecomp [-e] file1.bin file2.bin
```
**Options:**

- `-e, --entropy`: Calculate and print the entropy of each file.

**Example:**
```
bytecomp -e file1.bin file2.bin
```
**Output:**
```
50% [##################### ] Diff: 2.45%, Div: 17.91%
100% [########################################] Diff: 2.45%, Div: 17.91%

Difference percentage: 2.45%
Average value divergence: 17.91%
Entropy of file 1: 7.85
Entropy of file 2: 7.92
```

## Use Cases

- **Data Integrity Verification:** Compare backups or copies of files to ensure they are identical.
- **Patch Analysis:** Analyze the differences between two versions of a file to understand the changes made.
- **File Corruption Detection:** Identify potential corruption in files by comparing them to known good copies.
- **Data Compression Evaluation:** Compare the entropy of files before and after compression to assess compression effectiveness.
- **Security Analysis:** Compare files for suspicious modifications or hidden data.

## Building bytecomp

1. Clone the repository: `git clone https://github.com/your-username/bytecomp.git`
2. Compile the code: `gcc bytecomp.c -o bytecomp -lm` (the `-lm` flag is needed for the `log2()` function)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

