#ifndef GUIPARAMS_H
#define GUIPARAMS_H

#include <windows.h>
#include "global.h"

typedef struct guiparams GUIPARAMS;

GUIPARAMS *GParamsCreate(void);

// GUIPARAMS setters
UINT SetGParamsUpdate(BOOL updateScreen, GUIPARAMS *gp);

UINT SetGParamsBHeight(UINT height, GUIPARAMS *gp);

UINT SetGParamsBWidth(UINT width, GUIPARAMS *gp);

UINT SetGParamsBPP(UINT bpp, GUIPARAMS *gp);

UINT SetGParamsRPP(UINT rpp, GUIPARAMS *gp);

UINT SetGParamsBPR(UINT bpr, GUIPARAMS *gp);

UINT SetGParamsCHeight(UINT height, GUIPARAMS *gp);

UINT SetGParamsCWidth(UINT width, GUIPARAMS *gp);

UINT SetGParamsAlignH(UINT align, GUIPARAMS *gp);

UINT SetGParamsAlignV(UINT align, GUIPARAMS *gp);

// GUIPARAMS getters
SCROLLINFO *GetGParamsScroll(GUIPARAMS *gp, UINT *errcode);

BOOL GetGParamsUpdate(GUIPARAMS *gp, UINT *errcode);

UINT GetGParamsBHeight(GUIPARAMS *gp, UINT *errcode);

UINT GetGParamsBWidth(GUIPARAMS *gp, UINT *errcode);

UINT GetGParamsBPP(GUIPARAMS *gp, UINT *errcode);

UINT GetGParamsRPP(GUIPARAMS *gp, UINT *errcode);

UINT GetGParamsBPR(GUIPARAMS *gp, UINT *errcode);

UINT GetGParamsCHeight(GUIPARAMS *gp, UINT *errcode);

UINT GetGParamsCWidth(GUIPARAMS *gp, UINT *errcode);

UINT GetGParamsAlignH(GUIPARAMS *gp, UINT *errcode);

UINT GetGParamsAlignV(GUIPARAMS *gp, UINT *errcode);

#endif