#pragma once
#ifndef OVERLAY
#define OVERLAY


#include "../../entity.hpp"

#include <dxgi.h>
#include <d3d11.h>
#include "hijack.h"
#include "../../../../framework/imgui/Font.h"


ID3D11Device* g_pd3dDevice;
ID3D11DeviceContext* g_pd3dDeviceContext;
IDXGISwapChain* g_pSwapChain;
ID3D11RenderTargetView* g_mainRenderTargetView;
ImFont* m_pFont;
HWND window;
bool clickable = false;
ImFont* Main_Font = nullptr;
ImFont* Font1 = nullptr;
ImFont* ico = nullptr;
ImFont* icons = nullptr;



void change_click(bool canclick)
{
	long style = ::GetWindowLong(window, GWL_EXSTYLE);
	if (canclick)
	{
		style &= ~WS_EX_LAYERED;
		SetWindowLong(window, GWL_EXSTYLE, style);
		SetFocus(window);
		clickable = true;
	}
	else
	{
		style |= WS_EX_LAYERED;
		SetWindowLong(window, GWL_EXSTYLE, style);
		SetFocus(window);
		clickable = false;
	}
}


namespace DirectX
{
	class DirectX11
	{
	public:

		
		
		__forceinline auto hijack() -> bool
		{
			while (!window) {
				window = SetUp::HiJackNotepadWindow();
				Sleep(10);
			}
			MARGINS margin = { -1 };
			DwmExtendFrameIntoClientArea(window, &margin);
			SetMenu(window, NULL);
			SetWindowLongPtr(window, GWL_STYLE, WS_VISIBLE);
			SetWindowLongPtr(window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);

			ShowWindow(window, SW_SHOW);
			UpdateWindow(window);

			SetWindowLong(window, GWL_EXSTYLE, GetWindowLong(window, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);

			return true;


			/*window = FindWindowA(_("MedalOverlayClass"), _("MedalOverlay"));
			if (!window)
			{
				MessageBoxA(NULL, "Medal Not Found", "Overlay", ALERT_SYSTEM_WARNING);
				return false;
			}
			MARGINS Margin = { -1 };
			DwmExtendFrameIntoClientArea(window, &Margin);
			SetWindowPos(window, 0, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
			ShowWindow(window, SW_SHOW);
			UpdateWindow(window);
			return true;*/
		}
		

		
		
		__forceinline void createrender()
		{
			ID3D11Texture2D* pBackBuffer;
			g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
			g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
			pBackBuffer->Release();
		}

		__forceinline void cleanrender()
		{
			if (g_mainRenderTargetView)
			{
				g_mainRenderTargetView->Release();
				g_mainRenderTargetView = NULL;
			}
		}

		void cleand3d()
		{
			cleanrender();
			if (g_pSwapChain)
			{
				g_pSwapChain->Release(); g_pSwapChain = NULL;
			}
			if (g_pd3dDeviceContext)
			{
				g_pd3dDeviceContext->Release();
				g_pd3dDeviceContext = NULL;
			}
			if (g_pd3dDevice)
			{
				g_pd3dDevice->Release();
				g_pd3dDevice = NULL;
			}
		}

		HRESULT DirectXInit()
		{
			DXGI_SWAP_CHAIN_DESC SwapChainDesc;
			ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
			SwapChainDesc.Windowed = TRUE;
			SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			SwapChainDesc.BufferDesc.Height = 0;
			SwapChainDesc.BufferDesc.Width = 0;
			SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SwapChainDesc.BufferCount = 1;
			SwapChainDesc.OutputWindow = window;
			SwapChainDesc.SampleDesc.Count = 8;
			SwapChainDesc.SampleDesc.Quality = 0;


			UINT createDeviceFlags = 0;
			D3D_FEATURE_LEVEL featureLevel;
			const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
			if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &SwapChainDesc, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
				return false;

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;

			auto& style = ImGui::GetStyle();

			ImGui::SetNextWindowSize(io.DisplaySize);
			io.IniFilename = NULL;


			Main_Font = io.Fonts->AddFontFromMemoryTTF(&MainFont, sizeof MainFont, 21.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

			Font1 = io.Fonts->AddFontFromMemoryTTF(&Font, sizeof Font, 20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

			ico = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 30.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

			icons = io.Fonts->AddFontFromMemoryTTF(&Icon, sizeof Icon, 20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

	

			ImGui::StyleColorsDark();

			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

			ID3D11Texture2D* pBackBuffer;
			g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
			g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

			pBackBuffer->Release();

			SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		}
		//int i = 0;
		__forceinline auto init() -> void
		{
			LPDIRECT3D9              g_pD3D = NULL;
			LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
			D3DPRESENT_PARAMETERS    g_d3dpp = {};

			auto& io = ImGui::GetIO();
			auto& style = ImGui::GetStyle();

			io.MouseDrawCursor = false;

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			
			drawer = new v::draw();
			visualiser = new v::vis();



			if (GetAsyncKeyState(VK_INSERT) & 1)
				globals->show_menu = !globals->show_menu;
			if (globals->show_menu)
			{
				if (clickable != true)
					change_click(true);

				drawer->menu();

			}
			else
			{
				if (clickable != false)
					change_click(false);
			}

			
			entity->loop();

			ImGui::Render();
			ImVec4 clear_color = ImVec4(0., 0., 0., 0.);
			g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
			g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
			g_d3dpp.FullScreen_RefreshRateInHz = true;

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			g_pSwapChain->Present(1, 0);
		}

		__forceinline auto draw() -> bool
		{
			static RECT old_rc;
			MSG Message = { NULL };
			ZeroMemory(&Message, sizeof(MSG));
			while (Message.message != WM_QUIT)
			{
				if (PeekMessage(&Message, window, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&Message);
					DispatchMessage(&Message);
				}
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				POINT p;
				POINT xy;
				GetCursorPos(&p);
				io.MousePos.x = p.x;
				io.MousePos.y = p.y;
				if (GetAsyncKeyState(VK_LBUTTON)) {
					io.MouseDown[0] = true;
					io.MouseClicked[0] = true;
					io.MouseClickedPos[0].x = io.MousePos.x;
					io.MouseClickedPos[0].x = io.MousePos.y;
				}
				else
					io.MouseDown[0] = false;

				init();
			}

			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
			cleand3d();
			DestroyWindow(window);
			return Message.wParam;
		}	
	};
}
static DirectX::DirectX11* render = new DirectX::DirectX11();


#endif