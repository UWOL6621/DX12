#include "pch.h"
#include "Engine.h"

void Engine::Init(const WindowInfo& info)
{
	_window = info;
	

	// 그려질 화면 크기를 설정
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_cb->Init(sizeof(Transform), 256);
	_tableDescHeap->Init(256);
	_depthStencilBuffer->Init(_window);

	_input->Init(info.hwnd);
	_timer->Init();

	ResizeWindow(info.width, info.height);
//resize함수가 호출되고 device가 생성되기전에 한번더 호출된다면, resize함수내의 _depthStencilBufffer를 만드는 과정에서 에러가 뜰것이기에 맨밑으로 위치를 바꿈
}

void Engine::Render()
{
	RenderBegin();

	// TODO : 나머지 물체들 그려준다

	RenderEnd();
}

void Engine::Update()
{
	_input->Update();
	_timer->Update();

	ShowFps();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	_window.width = width;
	_window.height = height;

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);

	_depthStencilBuffer->Init(_window);
	//resizedwindow 함수는 화면크기를 조정하는 함수이기 때문에 혹시라도 화면크기가 다시 호출되면, DSV의 초기화하는 부분도 같이 이뤄줘야함
}

void Engine::ShowFps()
{
	uint32 fps = _timer->GetFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_window.hwnd, text);
}

