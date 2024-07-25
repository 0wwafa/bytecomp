#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h> // For fabs() and log2()
#include <getopt.h> // For getopt_long()

// Function to calculate entropy
double calculate_entropy(const char *data, size_t size) {
    int byte_counts[256] = {0};
    for (size_t i = 0; i < size; i++) {
        byte_counts[(unsigned char)data[i]]++;
    }

    double entropy = 0.0;
    for (int i = 0; i < 256; i++) {
        if (byte_counts[i] > 0) {
            double probability = (double)byte_counts[i] / size;
            entropy -= probability * log2(probability); 
        }
    }
    return entropy;
}

// Function to print the progress, difference percentage, and value divergence
void print_progress(size_t current, size_t total, size_t diff_bytes, double total_divergence) {
    const int bar_width = 50;
    double progress = (double)current / total;
    int filled_width = (int)(progress * bar_width);
    double diff_percentage = (double)diff_bytes / current * 100;
    double divergence_percentage = diff_bytes > 0 ? total_divergence / 255.0 / diff_bytes * 100.0 : 0.0;
    printf("\r%3zu%% [", (size_t)(progress * 100));
    for (int i = 0; i < bar_width; i++) {
        printf("%c", i < filled_width ? '#' : ' ');
    }
    printf("] Diff: %.2f%%, Div: %.2f%%", diff_percentage, divergence_percentage);
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    int calculate_entropy_flag = 0; 

    // Command-line options parsing
    static struct option long_options[] = {
        {"entropy", no_argument, 0, 'e'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;
    while ((c = getopt_long(argc, argv, "e", long_options, &option_index)) != -1) {
        switch (c) {
            case 'e':
                calculate_entropy_flag = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-e] <file1> <file2>\n", argv[0]);
                return 1;
        }
    }

    if (optind + 2 != argc) {
        fprintf(stderr, "Usage: %s [-e] <file1> <file2>\n", argv[0]);
        return 1;
    }
    const char *file1_path = argv[optind];
    const char *file2_path = argv[optind + 1];

    int fd1 = open(file1_path, O_RDONLY);
    int fd2 = open(file2_path, O_RDONLY);
    if (fd1 == -1 || fd2 == -1) {
        perror("open");
        return 1;
    }

    struct stat sb1, sb2;
    if (fstat(fd1, &sb1) == -1 || fstat(fd2, &sb2) == -1) {
        perror("fstat");
        return 1;
    }

    size_t min_size = sb1.st_size < sb2.st_size ? sb1.st_size : sb2.st_size;

    char *data1 = mmap(NULL, sb1.st_size, PROT_READ, MAP_PRIVATE, fd1, 0);
    char *data2 = mmap(NULL, sb2.st_size, PROT_READ, MAP_PRIVATE, fd2, 0);
    if (data1 == MAP_FAILED || data2 == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    size_t diff_bytes = 0;        // Total differing bytes
    double total_divergence = 0.0; // Total accumulated divergence
    size_t chunk_size = 1024 * 1024; // 1 MB chunk size

    for (size_t i = 0; i < min_size; i += chunk_size) {
        size_t chunk_end = i + chunk_size < min_size ? i + chunk_size : min_size;

        for (size_t j = i; j < chunk_end; j++) {
            if (data1[j] != data2[j]) {
                diff_bytes++;
                total_divergence += fabs((double)data1[j] - (double)data2[j]);
            }
        }

        print_progress(chunk_end, min_size, diff_bytes, total_divergence);
    }

    printf("\n");

    double percentage = (double)diff_bytes / min_size * 100;
    printf("Difference percentage: %.2f%%\n", percentage);
    // Calculate average divergence only if there are differing bytes
    printf("Average value divergence: %.2f%%\n", diff_bytes > 0 ? total_divergence / 255.0 / diff_bytes * 100.0 : 0.0);

    if (calculate_entropy_flag) {
        double entropy1 = calculate_entropy(data1, min_size);
        printf("Entropy of file 1: %.2f\n", entropy1);
        double entropy2 = calculate_entropy(data2, min_size);
        printf("Entropy of file 2: %.2f\n", entropy2);
    }

    if (munmap(data1, sb1.st_size) == -1 || munmap(data2, sb2.st_size) == -1) {
        perror("munmap");
        return 1;
    }

    close(fd1);
    close(fd2);

    return 0;
}
