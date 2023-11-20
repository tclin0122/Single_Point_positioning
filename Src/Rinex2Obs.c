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

int rinex2obs(const char* filePath, struct ObsData *obsData, struct ObsHeaderInfo *obsHead, struct ObsSat *satlist) {
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
            //check rinex file       
            if(obsHead->rinexType > 3 && obsHead->rinexType < 2) {
                fclose(file);
                printf("Incorrect Rinex version %lf\n", obsHead->rinexType);
                return 1;
            }
        } else {
            // Handle the case where the line does not match the expected format
            printf("Error reading data from line: %s", line);
        }
        if (strstr(line, "RINEX") != NULL) {
            break;
        }
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
    // Observed satellite
    if (fgets(line, sizeof(line), file) != NULL) {
        double data1, data2, data3, data4, data5, data6, data7; 
        int data8;
        int data9[12];
        removeG(line);
        if (sscanf(line, "%lf%lf%lf%lf%lf%lf%lf%d%d%d%d%d%d%d%d%d%d%d%d%d", &data1, &data2, &data3, &data4, &data5, &data6, &data7, &data8,
            &data9[0], &data9[1], &data9[2], &data9[3], &data9[4], &data9[5], &data9[6], &data9[7], &data9[8], &data9[9], &data9[10], &data9[11]) == 20) {
            // Process the extracted data
            printf("print line = %s\n",line);
            // Assign value
            satlist->GPS_num = data8;
            printf("Avaliable Satellite %d\n",satlist->GPS_num);

            printf("Data: %d %d %d\n", data9[9], data9[10], data9[11]);
            for(int i = 0; i < satlist->GPS_num; i++ ) {
                satlist->PRN_list[i] = data9[i];
                //printf("show %d\n",satlist->PRN_list[ i ]);
            }
        } else {
            // Handle the case where the line does not match the expected format
            printf("Error reading data from line: %s", line);
        }
        
    }
    
    

    // Close the file
    fclose(file);
    return 0;
}