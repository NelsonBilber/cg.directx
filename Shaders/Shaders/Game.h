#pragma once

#include <wrl.h>
#include <windows.graphics.directx.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace DirectX;

// a struct to represent a single vertex
struct VERTEX
{
	float X, Y, Z;    // vertex position
	float R, G, B;    // color values
};


class CGame
{
public:

	/*---------------------------------------------------------------------------------------------------
	Direct3D COM interface.
	The device1 refrets to Direct3D version 11 and 11.2
	----------------------------------------------------------------------------------------------------*/
	ComPtr<ID3D11Device1> device; // the device interface (virtual representation of the video adapter
	ComPtr<ID3D11DeviceContext1> deviceContext; //The control panel for the GPU
	ComPtr<IDXGISwapChain1> swapChain; //the swap chain interface
	ComPtr<ID3D11RenderTargetView> renderTarget; // the render target interface
	ComPtr<ID3D11Buffer> vertexBuffer;              // the vertex buffer interface
	ComPtr<ID3D11VertexShader> vertexShader;        // the vertex shader interface
	ComPtr<ID3D11PixelShader> pixelShader;          // the pixel shader interface
	ComPtr<ID3D11InputLayout> inputLayout;          // the input layout interface

	void Initialize(); // starting up code
	void Update(); // code tha manipulates the game sucj as timer an input
	void Render(); // draw graphics

	void InitGraphics();
	void InitPipeline();
};
