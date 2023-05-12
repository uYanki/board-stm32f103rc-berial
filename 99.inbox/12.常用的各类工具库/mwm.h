/*
** $Id: mwm.h,v 1.1 2003/10/20 15:03:42 chrisgreen Exp $
**
**  mwm.h
**
** Copyright (C) 2002 Sourcefire,Inc
** Marc Norton
**
** Modifed Wu-Manber style Multi-Pattern Matcher
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**
**
*/

#ifndef MWM_H
#define MWM_H

#ifdef __cplusplus
extern "C" {
#endif

void* mwmNew(int bmPatternNum);
void  mwmFree(void* pv);

int mwmAddPatternEx(void* pv, uint8_t* P, int m, uint32_t noCase, uint32_t offset, uint32_t depth, void* ID, int IID);

void mwmLargeShifts(void* pv, int flag);
int  mwmPrepPatterns(void* pv);

#ifdef BITOP_TEST
void mwmSetRuleMask(void* pv, BITOP* rm);
#endif

int mwmSearch(void* pv, uint8_t* T, int n, int (*action)(void* id, int index, void* data), void* data);

/* Not so useful, but not ready to be dumped  */
int  mwmAddPattern(void* pv, uint8_t* P, int m, uint32_t id);
int  mwmGetNumPatterns(void* pv);
void mwmFeatures(void);

#ifdef __cplusplus
}
#endif
#endif
