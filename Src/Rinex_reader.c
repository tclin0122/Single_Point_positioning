#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Include the stdlib.h header for atoi

#define MAX_LINE_LENGTH 80

struct NavData {
    int PRN;
    double toc;
    double af0, af1, af2;
    // Add more fields as needed
};

int main() {
    FILE* file;
    char line[MAX_LINE_LENGTH];
    struct NavData navData;
    int numSats = 0;

    // Replace with the path to your RINEX navigation file
    const char* filePath = "0lov033b.04n";

    file = fopen(filePath, "r");

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Process the file line by line
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Check if the line contains navigation data
        if (strstr(line, "G02") != NULL) {  // Replace "G01" with the PRN you are interested in
            navData.PRN = atoi(line + 1);
            fgets(line, MAX_LINE_LENGTH, file); // Line 2
            // Read and store relevant navigation data here
            // For example:
            fgets(line, MAX_LINE_LENGTH, file);
            sscanf(line + 3, "%lf", &navData.toc);
            fgets(line, MAX_LINE_LENGTH, file);
            sscanf(line + 3, "%lf%lf%lf", &navData.af0, &navData.af1, &navData.af2);
            break; // We found the data for PRN 1; exit the loop
        }
    }

    // Close the file
    fclose(file);

    // Print the collected navigation data
    printf("PRN: %d\n", navData.PRN);
    printf("ToC: %.2lf\n", navData.toc);
    printf("Af0: %.2lf, Af1: %.2lf, Af2: %.2lf\n", navData.af0, navData.af1, navData.af2);

    return 0;
}
