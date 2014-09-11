/*
Copyright (c) 2014, Los Alamos National Security, LLC
All rights reserved.
Copyright 2014. Los Alamos National Security, LLC. This software was produced under U.S. Government contract 
DE-AC52-06NA25396 for Los Alamos National Laboratory (LANL), which is operated by Los Alamos National Security, 
LLC for the U.S. Department of Energy. The U.S. Government has rights to use, reproduce, and distribute this software.  
NEITHER THE GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, 
OR ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is modified to produce derivative works, 
such modified software should be clearly marked, so as not to confuse it with the version available from LANL.

Additionally, redistribution and use in source and binary forms, with or without modification, are permitted provided 
that the following conditions are met:
•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
•	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
nor the names of its contributors may be used to endorse or promote products derived from this software without specific 
prior written permission. 
THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "stdafx.h"
#include "fdms.h"
#include ".\Combos.h"
#include "MEvent.h"
#include "dconfig.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Combos::Combos() 
{

}

Combos::~Combos(void)
{
	POSITION pos = _map.GetStartPosition();
	while( pos != NULL )
	{
		char* p;
		CString key;
		// Gets key and value (p)
		_map.GetNextAssoc( pos, key, (void*&)p );
		if (p != NULL)
		{
			_map.RemoveKey(key);
			//delete p;
		}
	}
	_map.RemoveAll();
}

int Combos::GetIndexOf(LPCTSTR s)
{
	int res = -1;
	CString es;
	for (int i = 0; i < GetSize(); i++)
	{
		es = GetAt(i);
		if (es.Compare(s) == 0)
		{
			res = i;
			break;
		}
	}
	return res;
}


void FacilityCombo::Build(CObList* plist)
{

	for (int i = 0; i < g_iMPEntriesIn; i++)
	{
		if (g_szMPFacilityNames[i][0] == NULL)
			continue;
		if (strlen(g_szMPFacilityNames[i]) < 1)
			continue;
		void* pv;
		if (_map.Lookup(g_szMPFacilityNames[i], (void*&)pv))
		{

		}
		else
		{
			_map.SetAt(g_szMPFacilityNames[i], this);
			Add(g_szMPFacilityNames[i]);
		}

	}

	POSITION pos = plist->GetHeadPosition();
	while( pos != NULL )
	{
		void* pv;
		MEvent* p = (MEvent*)plist->GetNext( pos );
		if (! _map.Lookup(p->FacilityShortName , (void*&)pv))
		{
			_map.SetAt(p->FacilityShortName, this);
			Add(p->FacilityShortName);
		}
	}
}

FacilityCombo::~FacilityCombo(void)
{
}

DetectorCombo::~DetectorCombo(void)
{
}
void DetectorCombo::Build(CObList* plist)
{
	int len = 64;
	GETPROFSTRARRAY(eDETECTOR_ID_CHOICE, *this, len);
	void* pv;

	for (int j = 0; j < len; j++)
	{
		const CString& s = GetAt(j);
		if (s.IsEmpty())
			continue;
		if (!_map.Lookup(s, (void*&)pv))
			_map.SetAt(s, this);	

	}

	for (int i = 0; i < g_iMPEntriesIn; i++)
	{
		if (g_szMPDetectorIDs[i][0] == NULL)
			continue;
		if (strlen(g_szMPDetectorIDs[i]) < 1)
			continue;
		if (_map.Lookup(g_szMPDetectorIDs[i], (void*&)pv))
		{
		}
		else
		{
			_map.SetAt(g_szMPDetectorIDs[i], this);
			Add(g_szMPDetectorIDs[i]);
		}
	}

	POSITION pos = plist->GetHeadPosition();
	while( pos != NULL )
	{
		MEvent* p = (MEvent*)plist->GetNext( pos );
		if (! _map.Lookup(p->StationLongName , (void*&)pv))
		{
			_map.SetAt(p->StationLongName, this);
			Add(p->StationLongName);
		}
		if (! _map.Lookup(p->StationShortName , (void*&)pv))
		{
			_map.SetAt(p->StationShortName, this);
			Add(p->StationShortName);
		}
	}
}

void CycleCombo::Build(CObList* )
{

	for (int i = 0; i < 10; i++)
	{
		CString s;
		s.Format("%d", i+1);
		Add(s);
	}

}

extern PTCHAR tMeasurementTypeImage(tMeasurementType t);

void MeasTypeCombo::Build(CObList* )
{
	for (int i = 0; i < MeasurementTypeCount; i++)
	{
		CString t;
		switch (i)
		{
			case UnspecifiedVerification:
			case CycleVerification:
			case AssemblyVerification:
			case MeasurementTypeCount:
				t = tMeasurementTypeImage((tMeasurementType)i);
				break;
			default:
				break;
		}
		Add(t);
	}
}


void Trim(PTCHAR p)
{

}