#include "stdafx.h"
#include "MouseState.h"
#include "MouseDragState.h"

IMPLEMENT_DYNCREATE(MouseState, CObject)

MouseState::MouseState()
{
}

MouseState::~MouseState()
{
}

MouseState* MouseState::MouseDown()
{
	return new MouseDragState();
}

MouseState* MouseState::MouseUp()
{
	return this;
}
