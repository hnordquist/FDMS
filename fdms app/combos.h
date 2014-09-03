#pragma once

class Combos : public CStringArray
{
public:
	Combos(void);
	~Combos(void);
	virtual void Build(CObList* plist) = 0;
	CMapStringToPtr _map;

	int GetIndexOf(LPCTSTR s);
};

class FacilityCombo : public Combos
{
public:
	~FacilityCombo(void);
	void Build(CObList* plist);

};

class DetectorCombo : public Combos
{
public:
	~DetectorCombo(void);
	void Build(CObList* plist);
};


class CycleCombo : public Combos
{
public:

	void Build(CObList* plist);

};
class MeasTypeCombo : public Combos
{

public:
	void Build(CObList* plist);
};
