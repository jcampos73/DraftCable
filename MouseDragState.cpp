#include "stdafx.h"
#include "MouseDragState.h"

IMPLEMENT_DYNCREATE(MouseDragState, MouseState)

MouseDragState::MouseDragState()
{
}

MouseDragState::~MouseDragState()
{
}

MouseState* MouseDragState::MouseDown()
{
	return this;
}

MouseState* MouseDragState::MouseUp()
{
	return new MouseState();
}
