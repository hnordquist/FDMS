#pragma once

class CField	: public CObject
{
public:
	CField(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat);
	~CField(void);

	typedef enum {
		eUnknownType, eString, eDouble, eLong, eDate, eTime, eDateTime, eBoolean,
		eImageField, eJetsam, eFrick, eFrack
	} VType;

	typedef enum	{eInvalid, eOK, eFormat, eRange, eStatusTypes} EType;

	static const PTCHAR szStatusDesc[eStatusTypes+1];

	CString low, high;
	EType m_cstate;

	CString name;
	bool bMutable;

	CString format, inputFormat, tooltipTemplate;

	VType m_type;

	void Reset(LPCTSTR pLow, LPCTSTR pHigh);

	virtual bool bSatisfiesConstraints(LPCTSTR pVal) = 0;

	virtual CString szGetConstraintDescription(LPCTSTR pVal) = 0;

	typedef struct 
	{
		CField::VType _type;
		LPTSTR name;
		CString low;
		CString high;
		bool bMutable;
		CString format;
		CString inputFormat;
		int xwidth;
		int cfmt;
		CString tooltipTemplate;
		bool bContent;
	} FieldDesc;

protected:
	virtual void Convert() = 0;

};
