#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Function to parse a line of RINEX navigation data and extract double values
int parseRINEXNavLine(const char *line, double *values, int numValues) {
    int i = 0;
    const char *token = strtok((char *)line, " ");
    
    while (token != NULL && i < numValues) {
        values[i] = atof(token);
        token = strtok(NULL, " ");
        i++;
    }
    
    return i;  // Return the number of values extracted
}
void replaceDWithE(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == 'D' || str[i] == 'd') {
            str[i] = 'e';
        }
    }
}

int main() {
    FILE *navFile = fopen("Data/0lov033b.04n", "r");
    if (navFile == NULL) {
        fprintf(stderr, "Error: Unable to open the navigation data file.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    double values[40];  // Assuming there are 40 double values in a navigation record
    int numValues;
    while (fgets(line, MAX_LINE_LENGTH, navFile)) {
        // Check if it's a navigation data record (e.g., RINEX version 3.03)
        if (line[0] == ' ') {
            replaceDWithE(line);
            numValues = parseRINEXNavLine(line, values, 40);

            // Now, values[] contains the double values from the line.
            // You can use them as needed.
            for (int i = 0; i < numValues; i++) {
                printf("Value %d: %lf\n", i + 1, values[i]);
            }
        }
    }

    fclose(navFile);
    return 0;
}
