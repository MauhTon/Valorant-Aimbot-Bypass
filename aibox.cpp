#include "Drawing.h"


void Drawing::DrawBorderBox( int x, int y, int w, int h, int thickness, D3DCOLOR Colour, IDirect3DDevice9 *pDevice)
{
	//Top horiz line
	DrawFilledRect( x, y, w, thickness,  Colour, pDevice );
	//Left vertical line
	DrawFilledRect( x, y, thickness, h, Colour, pDevice );
	//right vertical line
	DrawFilledRect( (x + w), y, thickness, h, Colour, pDevice );
	//bottom horiz line
	DrawFilledRect( x, y + h, w+thickness, thickness, Colour, pDevice );
}


//We receive the 2-D Coordinates the color and the device we want to use to draw those colors with
void Drawing::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
{
	//We create our rectangle to draw on screen
	D3DRECT BarRect = { x, y, x + w, y + h }; 
	//We clear that portion of the screen and display our rectangle
	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}


void Drawing::Draw_Text(LPCSTR TextToDraw, int x, int y, D3DCOLOR Colour, LPD3DXFONT m_font)
{
	// Create a rectangle to indicate where on the screen it should be drawn
	RECT rct = {x- 120, y, x+ 120, y + 15};

	// Draw some text 
	m_font->DrawText(NULL, TextToDraw, -1, &rct, DT_NOCLIP, Colour );
}


