/* @(#)widget.h 1.11 04/07/29
    Copyright 1996-2004 by Concept Engineering GmbH
    Author: Lothar Linhard
    ============================================================================
*/

#ifndef tk_widget_h
#define tk_widget_h

#ifdef __cplusplus
extern "C" {
#endif


struct Tcl_Interp;

#if (defined(WIN32) || defined(WIN64)) && !defined(NO_DLLEXPORT)
__declspec( dllexport )
#endif
int Nlview_Init(struct Tcl_Interp* interp);


#ifdef __cplusplus
}
#endif

#endif
