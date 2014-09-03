#pragma once


typedef CString RowText[CMeaCol::eColumnCount];


class ProcessMeasurement
{
public:
	ProcessMeasurement(void);
	~ProcessMeasurement(void);

	static void AddToGlobals(RowText& Row);
	static void UpdateGlobals(int iIndex, int iCID, PTCHAR p, CString& szDisplayableResult);
	static void CalcCoolingTime(int i);
	static void ApplyAdjustments(int i);
	static void SetDetectorID(int i);
	static void ComputeMeasurement(ChannelCounts& dAve);

	static void AddMeasurement(const ChannelCounts& dAve, int iIndex, COleDateTime odtMTime);


};
