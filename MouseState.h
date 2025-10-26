#pragma once
#include "DraftDrawDoc.h"
class MouseState : public CObject{

public:
	MouseState();
	~MouseState();
	DECLARE_DYNCREATE(MouseState)

	virtual MouseState* MouseDown();

	virtual MouseState* MouseUp();
};