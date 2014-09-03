#if !defined(_F_CONFIG_H_INCLUDED_)
#define _F_CONFIG_H_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define GETPROFINT(a)			CIniFileProcessor::GetIntValue(a)
#define GETPROFSTR(a,p,l)		CIniFileProcessor::GetStringValue(a, p, l)
#define GETPROFBOOL(a,b)  		CIniFileProcessor::GetBooleanValue(a,b)
#define GETPROFSTRARRAY(a,ar,l) CIniFileProcessor::GetStringArrayValues(a, ar, l)
#define GETRANGEVALUES(a,l,h)	CIniFileProcessor::GetRangeValue(a, l, h)

#include "IniFileProcessor.h"


#endif