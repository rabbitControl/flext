/* 

flext - C++ layer for Max/MSP and pd (pure data) externals

Copyright (c) 2001,2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file flatom-pr.cpp
    \brief Definitions for printing and scanning the t_atom type.
*/
 
#include "flext.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void flext::PrintAtom(const t_atom &a,char *buf)
{
	switch(a.a_type) {
	case A_FLOAT:
#ifdef PD
		if(a.a_w.w_float == (int)a.a_w.w_float)
			sprintf(buf,"%i",(int)a.a_w.w_float);
		else
#endif
		sprintf(buf,"%f",(float)a.a_w.w_float);
		break;
#ifdef MAXMSP
	case A_LONG:
		sprintf(buf,"%i",(int)a.a_w.w_long);
		break;
#endif
#ifdef PD
	case A_POINTER:
		sprintf(buf,"%x",a.a_w.w_gpointer);
		break;
#endif
	case A_SYMBOL:
		strcpy(buf,flext::GetString(a.a_w.w_symbol));
		break;
	}
}

bool flext::ScanAtom(t_atom &a,const char *buf)
{
	// skip whitespace
	while(*buf && isspace(*buf)) ++buf;
	if(!*buf) return false;

	char tmp[1024];
	strcpy(tmp,buf);
	char *c = tmp;

	// check for word type (s = 0,1,2 ... int,float,symbol)
	int s = 0;
	for(; *c && !isspace(*c); ++c) {
		if(!isdigit(*c)) 
			s = (*c != '.' || s == 1)?2:1;
	}

	switch(s) {
	case 0: // integer
#ifdef MAXMSP
		a.a_type = A_LONG;
		a.a_w.w_long = atol(tmp);
		break;
#endif
	case 1: // float
		a.a_type = A_FLOAT;
		a.a_w.w_float = (float)atof(tmp);
		break;
	default: { // anything else is a symbol
		char t = *c; *c = 0;
		a.a_type = A_SYMBOL;
		a.a_w.w_symbol = (t_symbol *)flext::MakeSymbol(tmp);
		*c = t;
		break;
	}
	}

	return true;
}


