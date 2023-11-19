#include <stdio.h>
#include <stdlib.h>
#include "Rinex2Obs.h"

int rinex2obs(const char* filePath, struct ObsData *obsData) {
    FILE* file;
    char line[MAX_LINE_LENGTH];
    int cnt = 1;

    // Read file
    file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Skip header lines until "END OF HEADER" is encountered
    while (fgets(line, sizeof(line), file) != NULL) {
        double data1, data2, data3, data4;
        double data5, data6, data7, data8, data9, data10;
        int data11;
        if (sscanf(line, "%lf%lf%lf%lf", &data1, &data2, &data3, &data4) == 4) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", data1, data2, data3, data4);
            cnt++;
        } else if (strstr(line, "END OF HEADER") != NULL) {
            break;
        }
        else {
            // Handle the case where the line does not match the expected format
            printf("Error reading data from line: %s", line);
        }
    }
    // Close the file
    fclose(file);
    return 0;
}