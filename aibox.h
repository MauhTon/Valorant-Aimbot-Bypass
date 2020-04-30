#pragma once

#include "d3d9.h"

static class Drawing
{
public:
	static void Draw_Text(LPCSTR TextToDraw, int x, int y, D3DCOLOR Colour, LPD3DXFONT m_font);
	static void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* d3dDevice);
	static void DrawBorderBox( int x, int y, int w, int h, int thickness, D3DCOLOR Colour, IDirect3DDevice9 *d3dDevice);
};

