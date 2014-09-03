#pragma once

class Clipboard
{
public:
	Clipboard(void);
	~Clipboard(void);

	void CopyToClipboard(HWND h);
};
