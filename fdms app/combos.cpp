
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