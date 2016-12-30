#include "pch.h"
#include "Game.h"
#include <iostream>
#include <fstream>

using namespace std;

// this function loads a file into an Array^
Array<byte>^ LoadShaderFile(std::string File)
{
	Array<byte>^ FileData = nullptr;

	// open the file
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	// if open was successful
	if (VertexFile.is_open())
	{
		// find the length of the file
		int Length = (int)VertexFile.tellg();

		// collect the file data
		FileData = ref new Array<byte>(Length);
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(FileData->Data), Length);
		VertexFile.close();
	}

	return FileData;
}


// initalize and prepares 3D
void CGame::Initialize()
{
	/*==================================*/
	// Swap Chains - setup
	/*==================================*/

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
	if (!Window){
		cout <<"Can´t get window ..."<< endl;
	}

	if (!device) {
		cout << "Can´t get device ..." << endl;
	}

	dxgiFactory->CreateSwapChainForCoreWindow (	
		device.Get(),							//address of the device
		reinterpret_cast<IUnknown *>(Window),	//address if the window
		&swapChainDescription,					//address of the swap chain description
		nullptr,								//
		&swapChain								//
	);

	if (!swapChain) {
		cout << "Error creating swap chain ... " << endl;
	}



	/*==================================*/
	// Render frames
	/*==================================*/

	//Get a pointer to the back buffer
	ComPtr<ID3D11Texture2D> backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);

	//create a render target that points to our backbuffer
	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);

	// set the viewport
	D3D11_VIEWPORT viewport = { 0 };

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Window->Bounds.Width;
	viewport.Height = Window->Bounds.Height;

	deviceContext->RSSetViewports(1, &viewport);


	// initialize graphics and the pipeline
	InitGraphics();
	InitPipeline();

}

// performs updates to the game state
void CGame::Update()
{
}

// renders a simple frame of 3D graphics
void CGame::Render()
{
	if (swapChain) {
		//set new render target as the active render target
		deviceContext->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);

		//clear the back buffer to some colour
		float colour[4] = {0.2f, 0.5f, 0.3f, 1.0f};
		deviceContext->ClearRenderTargetView(renderTarget.Get(), colour);

		// set the vertex buffer
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

		// set the primitive topology
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw 3 vertices, starting from vertex 0
		deviceContext->Draw(3, 0);

		//switch the back buffer and the front buffer
		swapChain->Present(1, 0);
	} else {
		cout <<" No swap chain ..... "<< endl;
	}
}

// this function loads and initializes all graphics data
void CGame::InitGraphics()
{
	// create a triangle out of vertices
	VERTEX OurVertices[] =
	{
		{ 0.00f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f }, //full red
		{ 0.45f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f }, //full green
 		{ -0.45f,-0.5f, 0.0f,    0.0f, 0.0f, 1.0f }, //full blue
	};

	// create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(VERTEX) * ARRAYSIZE(OurVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { OurVertices, 0, 0 };

	device->CreateBuffer(&bd, &srd, &vertexBuffer);
}

// this function initializes the GPU settings and prepares it for rendering
void CGame::InitPipeline()
{
	// load the shader files
	Array<byte>^ VSFile = LoadShaderFile("VertexShader.cso");
	Array<byte>^ PSFile = LoadShaderFile("PixelShader.cso");

	// create the shader objects
	device->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexShader);
	device->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelShader);

	// set the shader objects as the active shaders
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	// initialize input layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// create and set the input layout
	device->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile->Data, VSFile->Length, &inputLayout);
	deviceContext->IASetInputLayout(inputLayout.Get());
}
