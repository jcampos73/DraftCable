#pragma once
#include "DraftDrawDoc.h"
class MouseState {

public:
	MouseState();
	~MouseState();

	virtual MouseState* MouseDown();

	virtual void MouseUp();
};