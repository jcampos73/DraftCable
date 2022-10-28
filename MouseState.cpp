#include "stdafx.h"
#include "MouseState.h"
#include "MouseDragState.h"

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

void MouseState::MouseUp()
{

}
