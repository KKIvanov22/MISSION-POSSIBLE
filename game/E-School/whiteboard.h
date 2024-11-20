#pragma once
#include "pch.h"
#include "raymath.h"
namespace whiteboard {
	void DrawTexturedFrontFace(Texture2D texture, Vector3 position, float width, float height, float depth);
	Texture2D whiteboard();
}