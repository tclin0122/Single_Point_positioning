#ifndef SPP_H_
#define SPP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Rinex2Obs.h"
#include "Rinex2Nav.h"


int spp(struct DataGPS *navData, struct ObsData *obsData, struct ObsHeaderInfo *obsHead, struct ObsSat *satlist);

#endif /* SPP_H_ */