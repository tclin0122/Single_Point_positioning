#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Include the stdlib.h header for atoi
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

    // Process the file line by line
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        //check rinex version
        if (strstr(line, "0") != NULL) {  // Replace "G01" with the PRN you are interested in
            //rinex_version = atoi(line+5);
            sscanf(line, "%3lf", &rinex_version);
            //sscanf(line + 22, "%19lf%19lf%19lf", &navData.clockBias, &navData.clockDrift, &navData.clockDriftRate);
            //navData.af0 = atoi(line + 22);
            //break; // We found the data for PRN 1; exit the loop
        }
        // Check if the line contains navigation data
        if (strstr(line, "5") != NULL) {  // Replace "G01" with the PRN you are interested in
            navData.PRN = atoi(line + 1);
            //sscanf(line, "%2d", &navData.PRN);
            fgets(line, MAX_LINE_LENGTH, file); // Line 2
            // Read and store relevant navigation data here
            // For example:
            fgets(line, MAX_LINE_LENGTH, file);
            sscanf(line + 3, "%20c", &navData.epochInfo);
            fgets(line, MAX_LINE_LENGTH, file);

            sscanf(line + 22, "%19lf%19lf%19lf", &navData.clockBias, &navData.clockDrift, &navData.clockDriftRate);
            //navData.af0 = atoi(line + 22);
            break; // We found the data for PRN 1; exit the loop
        }
    }

    // Close the file
    fclose(file);

    // Print the collected navigation data
    printf("RINEX version %f\n", rinex_version);
    printf("PRN: %2d\n", navData.PRN);
    printf("ToC: %20c\n", navData.epochInfo);
    printf("Af0: %.19lf, Af1: %.19lf, Af2: %.19lf\n", navData.clockBias, navData.clockDrift, navData.clockDriftRate);
    //printf("Af0: %.18lf\n", navData.af0);
    return 0;
}