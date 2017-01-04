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

	/*==================================*/
	// Z-buffer
	/*==================================*/
	D3D11_TEXTURE2D_DESC textD = { 0 };

	textD.Width = (UINT)Window->Bounds.Width;
	textD.Height = (UINT)Window->Bounds.Height;
	textD.ArraySize = 1;
	textD.MipLevels = 1;
	textD.SampleDesc.Count = 1;
	textD.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textD.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ComPtr<ID3D11Texture2D> zBufferTexture;
	device->CreateTexture2D(
							&textD, 
							nullptr, 
							&zBufferTexture
	);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));

	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(zBufferTexture.Get(), &dsvd, &zBuffer);
	
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

	time = 0.0f;
}

// performs updates to the game state
void CGame::Update()
{
	time += 0.05f;
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

		//clear the depth buffer
		deviceContext->ClearDepthStencilView(zBuffer.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		// set the vertex buffer
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// set the primitive topology
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw 3 vertices, starting from vertex 0
		deviceContext->Draw(36, 0);


		/*==================================*/
		// Transformations
		/*==================================*/
		//claculate world transformation
		XMMATRIX matRotate = XMMatrixRotationY(time);

		//Calculate the view transformation
		XMVECTOR camPosition = XMVectorSet(1.5f, 9.5f,15.0f, 0.0f);
		XMVECTOR camLookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR camUp =     XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX matView =   XMMatrixLookAtLH(camPosition, camLookAt, camUp);

		//calculate the projection transformation
		CoreWindow ^Window = CoreWindow::GetForCurrentThread();
		XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
								XMConvertToRadians(45.0f), 
								(FLOAT)Window->Bounds.Width / (FLOAT)Window->Bounds.Height, 
								1.0f, 
								100.0f
		);

		//calculate the final matrix
		XMMATRIX matFinal = matRotate * matView * matProjection;

		// load the data into constant buffer
		deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &matFinal, 0, 0);
		deviceContext->DrawIndexed(18, 0, 0);

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
		//base
		{ -1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f },  //0
		{ 1.0f,  -1.0f, 1.0f,  0.0f, 0.0f, 1.0f },  //1
 		{ -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f },  //2
		{ 1.0f,  -1.0f, -1.0f, 0.0f, 1.0f, 1.0f },  //3

		//top
		{ 0.0f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f },  //4
	};

	// create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(VERTEX) * ARRAYSIZE(OurVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { OurVertices, 0, 0 };

	device->CreateBuffer(&bd, &srd, &vertexBuffer);

	short OurIndices[]
	{
		0,2,1, // base
		1,2,3,
		0,1,4, //sides
		1,3,4,
		3,2,4,
		2,0,4,
	};

	//create the index buffer
	D3D11_BUFFER_DESC indexDesc = {0};
	indexDesc.ByteWidth = sizeof(short) * ARRAYSIZE(OurIndices);
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResourceDataIndices = {OurIndices, 0, 0};
	device->CreateBuffer(&indexDesc, &subResourceDataIndices, &indexBuffer);
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

	//Create a constant buffer
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = 64;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
	deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}
