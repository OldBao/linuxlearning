/**
 * @file   ll_macro.h
 * @author zgxcassar@gmail.com
 * @date   Thu Jun  6 23:05:49 2013
 * 
 * @brief  util macros  
 * 
 * 
 */

#ifndef LL_MACRO_H_
#define LL_MACRO_H_

#include <limits.h>

#define LL_OK       0
#define LL_FAILURE -1

//calculate offset of a member in a struct
#define LL_OFFSET_OF(s, t) ( (size_t) &(((s *)0)->t ) )

//calculate base address of a pointer
#define LL_CONTAINER_OF(p, s, t) \
  ( (s *) ( (char *)(p) - LL_OFFSET_OF(s, t) ) )

//transfer current to string
#define __LL_V_LINE(x) #x

#define __LL_LINE(x)   __LL_V_LINE(x)

//linux path infos
#ifdef PATH_MAX
#define LL_PATH_MAX             PATH_MAX
#else
#define LL_PATH_MAX             1024
#endif



//////////////////////////////
// ANSI COLOR MACORS         /
//////////////////////////////
#define LL_ANSI_ESC "\033"
#define LL_CSI		LL_ANSI_ESC "["
#define LL_ANSI_BLACK 	LL_CSI "30m" 	    
#define LL_ANSI_RED	LL_CSI "31m"
#define LL_ANSI_GREEN 	LL_CSI "32m"
#define LL_ANSI_YELLOW 	LL_CSI "33m"
#define LL_ANSI_BLUE	LL_CSI "34m"
#define LL_ANSI_MAGINA	LL_CSI "35m"
#define LL_ANSI_CYAN	LL_CSI "36m"
#define LL_ANSI_WHITE	LL_CSI "37m"

#define LL_ANSI_FGRED 		LL_CSI "1;31m"
#define LL_ANSI_FGGREEN 	LL_CSI "1;32m"
#define LL_ANSI_FGYELLO 	LL_CSI "1;33m"
#define LL_ANSI_FGBLACK 	LL_CSI "1;34m"
#define LL_ANSI_FGMAGINA 	LL_CSI "1;35m"
#define LL_ANSI_FGCYAN 		LL_CSI "1;36m"
#define LL_ANSI_FGWHITE 	LL_CSI "1;37m"

#define LL_ANSI_BGRED 		LL_CSI "41;1m"
#define LL_ANSI_BGGREEN 	LL_CSI "42;1m"
#define LL_ANSI_BGYELLO 	LL_CSI "43;1m"
#define LL_ANSI_BGBLUE		LL_CSI "44;1m"
#define LL_ANSI_BGMAGINA 	LL_CSI "45;1m"
#define LL_ANSI_BGCYAN 		LL_CSI "46;1m"
#define LL_ANSI_BGWHITE 	LL_CSI "47;1m"

#define LL_ANSI_OBGBLACK 	LL_CSI "40m"
#define LL_ANSI_OBGRED 		LL_CSI "41m"
#define LL_ANSI_OBGREEN 	LL_CSI "42m"
#define LL_ANSI_OBYELLO 	LL_ACSI "43m"
#define LL_ANSI_OBGBLUE 	LL_CSI "44m"
#define LL_ANSI_OBGMAGINA 	LL_CSI "45m"
#define LL_ANSI_OBGCYAN 	LL_CSI "46m"

#define LL_ANSI_CLRALL 		LL_CSI "2;37;0m"


#endif
