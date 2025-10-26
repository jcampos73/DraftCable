#pragma once
#include "DraftDrawDoc.h"
#include "MouseState.h"
class MouseDragState : public MouseState {

public:
	MouseDragState();
	~MouseDragState();
	DECLARE_DYNCREATE(MouseDragState)

	virtual MouseState* MouseDown();

	virtual MouseState* MouseUp();
};