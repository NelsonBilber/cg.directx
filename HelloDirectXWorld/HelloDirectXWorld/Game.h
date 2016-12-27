#pragma once

#include <wrl.h>
#include <windows.graphics.directx.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;

class CGame
{
public:

	/*---------------------------------------------------------------------------------------------------
	Direct3D COM interface.
	The device1 refrets to Direct3D version 11 and 11.2
	----------------------------------------------------------------------------------------------------*/
	ComPtr<ID3D11Device1> device; // the device interface (virtual representation of the video adapter
	ComPtr<ID3D11DeviceContext1> deviceContext; //The control panel for the GPU

	void Initialize(); // starting up code
	void Update(); // code tha manipulates the game sucj as timer an input
	void Render(); // draw graphics

};
