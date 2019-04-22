#ifndef GEN_CVDATA_MOBSEL
#define GEN_CVDATA_MOBSEL

#include <gd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "settings.h"
#include "pixel.h"
#include "objects_params.h"
#include "link_areas.h"
#include "queue_pixel.h"
#include "equivalence_tree.h"
#include "find_real_objects.h"
#include "cvdata.h"

static byte flagInit = VARS_NOT_INITIALIZED;

CVData* genCVData(char *phName, CVData *data);
void initDataForCV(CVData *data, coord_t sx, coord_t sy, off_t sizeImg);
void freeDataForCV(CVData *data);
#endif
