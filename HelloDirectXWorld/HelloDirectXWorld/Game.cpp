#include "pch.h"
#include "Game.h"
#include <iostream>

using namespace std;

// initalize and prepares 3D
void CGame::Initialize()
{
	//define temp pointer to the game
	ComPtr<ID3D11Device> tempDevice;
	ComPtr<ID3D11DeviceContext> tempDeviceContext;

	//creating the device and the device context objects
	D3D11CreateDevice(
		nullptr, 
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&tempDevice,
		nullptr,
		&tempDeviceContext
	);

	//convert the pointers from Dx11 version to the DX 11.2 version
	tempDevice.As(&device);
	tempDeviceContext.As(&deviceContext);

	//1. convert our D3D11Device1 interface to an IDXGIDevice1
	ComPtr<IDXGIDevice1> dxgiDevice;
	device.As(&dxgiDevice);

	//2. Utilize the IDXGIDevice1 interface to get access to the adapter
	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	//3. Use the IDXGIAdapter interface to get acess to the parent
	ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

	//set up the swap chain description struct
	DXGI_SWAP_CHAIN_DESC1 swapChainDescription = { 0 };

	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //how the swap chain should be utilized
	swapChainDescription.BufferCount = 2;								//a front and back buffer
	swapChainDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;			// a common swap chain format // UNORM = unsingned normalized integer 8-bits for red, green and blue
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;		//the recomended flip mode
	swapChainDescription.SampleDesc.Count = 1;							//disables anti-alising

	//Create swap chain
	CoreWindow^ Window = CoreWindow::GetForCurrentThread(); //obtain a pointer to the window

	if (!Window)
	{
		cout <<"Can´t get window ..."<< endl;
	}

	if (!device)
	{
		cout << "Can´t get device ..." << endl;
	}

	dxgiFactory->CreateSwapChainForCoreWindow
	(	
		device.Get(),							//address of the device
		reinterpret_cast<IUnknown *>(Window),	//address if the window
		&swapChainDescription,					//address of the swap chain description
		nullptr,								//
		&swapChain								//
	);

	if (!swapChain)
	{
		cout << "Error creating swap chain ... " << endl;
	}

}

// performs updates to the game state
void CGame::Update()
{
}

// renders a simple frame of 3D graphics
void CGame::Render()
{
	if (swapChain)
	{
		//switch the back buffer and the front buffer
		swapChain->Present(1, 0);
	}
	else
	{
		cout <<" No swapChain ..... "<< endl;
	}
}