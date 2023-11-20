#include <stdio.h>
#include <stdlib.h>
#include "Rinex2Obs.h"

void removeG(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == 'G') {
            str[i] = ' ';
        }
    }
}

int rinex2obs(const char* filePath, struct ObsData *obsData, struct ObsHeaderInfo *obsHead) {
    FILE* file;
    char line[MAX_LINE_LENGTH];
    int cnt = 1;

    // Read file
    file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Check Rinex
    while (fgets(line, sizeof(line), file) != NULL) {
        double data1;
        if (sscanf(line, "%lf", &data1) <= 1) {
            // Process the extracted data
            printf("print line = %s\n",line);
            // Assign value
            obsHead->rinexType = data1;
            printf("Data: %lf\n", obsHead->rinexType);
        } else {
            // Handle the case where the line does not match the expected format
            printf("Error reading data from line: %s", line);
        }
        if (strstr(line, "RINEX") != NULL) {
            break;
        }
    }

    if(obsHead->rinexType > 3 && obsHead->rinexType < 2) {
        printf("Incorrect Rinex version %lf\n", obsHead->rinexType);
        return 1;
    }
    // Get approximate position
    while (fgets(line, sizeof(line), file) != NULL) {
        double data1, data2, data3;
        if (sscanf(line, "%lf%lf%lf", &data1, &data2, &data3) == 3) {
            printf("print line = %s\n",line);
            // Assign value
            obsHead -> approxPosX = data1;
            obsHead -> approxPosY = data2;
            obsHead -> approxPosZ = data3;
            printf("Data: %.4lf %.4lf %.4lf\n", data1, data2, data3);
        } else {
            // Handle the case where the line does not match the expected format
            printf("Error reading data from line: %s", line);
        } 
        if (strstr(line, "APPROX POSITION XYZ") != NULL) {
            break;
        }
    }
    // Skip header lines until "END OF HEADER" is encountered
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "END OF HEADER") != NULL) {
            break;
        }
    }

    
    while (fgets(line, sizeof(line), file) != NULL) {
        double data1, data2, data3, data4;
        double data5, data6, data7, data8, data9, data10;
        int data11;
        removeG(line);
        if (sscanf(line, "%lf%lf%lf", &data1, &data2, &data3) == 3) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            printf("Data: %.12lf %.12lf %.12lf\n", data1, data2, data3);
            cnt++;
        } else {
            // Handle the case where the line does not match the expected format
            printf("Error reading data from line: %s", line);
        }
        if (strstr(line, "0.0000000  0") != NULL) {
            break;
        }
        
    }
    

    // Close the file
    fclose(file);
    return 0;
}