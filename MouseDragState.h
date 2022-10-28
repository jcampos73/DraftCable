#pragma once
#include "DraftDrawDoc.h"
#include "MouseState.h"
class MouseDragState : public MouseState {

public:
	MouseDragState();
	~MouseDragState();

	virtual MouseState* MouseDown();

	virtual void MouseUp();
};