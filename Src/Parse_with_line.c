#include <stdio.h>
#include <stdlib.h>
#include "Rinex_reader.h"


int main() {
    FILE* file;
    char line[MAX_LINE_LENGTH];
    struct DataGPS navData;
    int numSats = 0;
    double rinex_version;

    // Replace with the path to your RINEX navigation file
    const char* filePath = "Data/0lov033b.04n";

    // Read file
    file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Skip header lines until "END OF HEADER" is encountered
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "END OF HEADER") != NULL) {
            break;
        }
    }

    // Read data lines
    while (fgets(line, sizeof(line), file) != NULL) {
        // Extract data as needed
        double data1, data2, data3, data4;
        if (sscanf(line, "%lf%lf%lf%lf", &data1, &data2, &data3, &data4) == 4) {
            // Process the extracted data
            printf("Data: %lf %lf %lf %lf\n", data1, data2, data3, data4);
        } else {
            // Handle the case where the line does not match the expected format
            printf("Error reading data from line: %s", line);
        }
    }

    fclose(file);
    return 0;
}
