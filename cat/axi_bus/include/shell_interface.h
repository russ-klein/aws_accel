#ifndef SHELL_INTERFACE_H
#define SHELL_INTERFACE_H
/*****************************************************************************

  Copyright (c) 2000-2014, David Nguyen Van Mau
  All rights reserved.


  Amethyst Technologies
  845 Edgewood Rd.
  Redwood City, CA 94062


******************************************************************************/
/*****************************************************************************

  File: shell_interface.h

  Overview: Break dependencies with Amethyst includes
******************************************************************************/

/*****************************************************************************
  Class: ShellInterface

  Overview: Handles the API calls to an Amethyst shell.  Initializes TCL,
	runs commands and cleans up
	
  Memory Owned:
	The entire DbDb

  Attributes:
*****************************************************************************/

#ifdef WIN32
  #ifdef WIN32_NO_DLL
    #define AMETH_DLLEXPORT
    #define AMETH_DLLIMPORT
  #else
    #define AMETH_DLLEXPORT __declspec(dllexport)
    #define AMETH_DLLIMPORT __declspec(dllimport)
  #endif
#else  /* defined(WIN32) */
#define AMETH_DLLIMPORT
#endif /* defined(WIN32) */

#ifndef AMETH_DLL
#define AMETH_DLL AMETH_DLLIMPORT
#endif

class AMETH_DLL ShellInterface
{
public:
  ShellInterface(int argc, const char *argv[], int persist=1) ;
  ~ShellInterface();
 
  // These routines do most of the work
  int           RunCmd(const char* cmd, int resetBuffer=1);
  const char *	CmdOut();	
  const char *	TclOut();
  const char *	ErrorOut();
  const char *	WarnOut();
  int           Error();

  // Library manipulations
  int load_library(const char* lib_path);
  int get_cap_input_smallest_ff(double & value) ;
  int get_trans_output_smallest_ff(double capa, double & value);

private:

  class ShellInterfaceImpl* d_impl;			

};



#endif
