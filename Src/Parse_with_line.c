#include <stdio.h>
#include <stdlib.h>
#include "Rinex_reader.h"

void replaceDWithE(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == 'D' || str[i] == 'd') {
            str[i] = 'e';
        }
    }
}

int main() {
    FILE* file;
    char line[MAX_LINE_LENGTH];
    struct DataGPS navData;
    int numSats = 0;
    double rinex_version;
    int cnt=1;

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
        double data1, data2, data3, data4;
        double data5, data6, data7, data8, data9, data10;
        // Extract data as needed
        replaceDWithE(line);
        if(cnt == 1){
            if (sscanf(line, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", &data1, &data2, &data3, &data4, &data5, &data6, &data7, &data8, &data9, &data10) == 10) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            navData.PRN = data1;
            navData.clockBias = data8;
            navData.clockDrift = data9;
            navData.clockDriftRate = data10;
            printf("Data: %d %.19lf %.19lf %.19lf\n", navData.PRN, data8, data9, data10);
            cnt++;
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
        }
        else if(cnt >1 && cnt < 8){
            if (sscanf(line, "%lf%lf%lf%lf", &data1, &data2, &data3, &data4) == 4) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", data1, data2, data3, data4);
            cnt++;
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
        }
        else if( cnt == 8){
            if (sscanf(line, "%lf", &data1) == 1) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            printf("Data: %.12lf\n", data1);
            cnt = 1;
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
        }
        
    }

    fclose(file);
    return 0;
}


