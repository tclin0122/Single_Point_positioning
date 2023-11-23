#ifndef SPP_H_
#define SPP_H_

#define DEG_2_RADI 0.0174532925
#define Re         6371000
#define f          1575.42e6
#define hi         350000

#define P          1013.0      // [mBar,hPa] 
#define T          273.15      // [Kelvin]
#define RH         0.5

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Rinex2Obs.h"
#include "Rinex2Nav.h"


int spp(struct DataGPS *navData, struct ObsData *obsData, struct ObsHeaderInfo *obsHead, struct ObsSat *satlist);

#endif /* SPP_H_ */