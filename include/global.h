#ifndef GLOBAL_H
#define GLOBAL_H

#include <windows.h>
#include "error.h"

// Indeces for GDataSet function
#define GUI 0
#define BUF 1

// Custom format for avoiding copy of data
// from clipboard history (buffer)
UINT CF_IGNORE;

typedef struct gdata GDATA;

// Initialize global data object and CF_IGNORE format
GDATA *GlobalDataInit(void *lp, void *rp);

// Get gparams or buffer from global data
void *GDataGet(UINT index, GDATA *gdata, UINT *errcode);

// Set buffer to the gdata
UINT GDataSet(UINT index, void *ptr, GDATA *gdata);

#endif