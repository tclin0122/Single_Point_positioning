#ifndef SPP_H_
#define SPP_H_

#define DEG_2_RADI 0.0174532925
#define Re         6371000
#define f          1575.42e6
#define hi         350000

#define P          1013.0      // [mBar,hPa] 
#define T          273.15      // [Kelvin]
#define RH         0.5

//constant from GPS ICD
#define c              299792458   //speed of light
#define pi             3.1415926535898; //Pi
#define mu             3.986005 * pow(10,14)
#define Omeg_dot_earth 7.2921151467 * pow(10,-5)
#define F              -4.442807633 * pow(10,-10)

#define ERR_TOLLERANCE 1e-5

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Rinex2Obs.h"
#include "Rinex2Nav.h"

void Cart2Ecip (double X, double Y, double Z, double *lat, double *lon, double *height);
void relative_position (double X, double X_a, double Y, double Y_a, double Z, double Z_a, double lat, double lon, double *zij);
void iono (double Zenth, double TECU, double *d_iono);
void trop (double Zenth, double height, double *d_trop);
int position_correction (double trop_delay[], double iono_delay[], double pseudorange[], double GPS_clk_correction[], double X_r, double Y_r, double Z_r, double X_s[], double Y_s[], double Z_s[], double *RX_x, double *RX_y, double *RX_z);


int spp(struct DataGPS *navData, struct ObsData *obsData, struct ObsHeaderInfo *obsHead, struct ObsSat *satlist);


#endif /* SPP_H_ */