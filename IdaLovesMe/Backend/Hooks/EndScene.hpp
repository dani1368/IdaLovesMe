#pragma once
#include "../Globalincludes.h"
#include "../Config/Settings.h"
#include "../../Frontend/Menu/Menu.h"
#include "../../Frontend/Renderer/Renderer.h"
#include "../Features/Visuals/EventLogger.h"

#include <intrin.h>

typedef HRESULT(__stdcall* fnPresent2)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
fnPresent2 oPresent;

typedef HRESULT(__stdcall* fnReset2)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
fnReset2 oReset;

namespace Hooked {
	static HRESULT __stdcall present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion) noexcept
	{
		if (Settings->UnloadCheat)
			return oPresent(device, src, dest, windowOverride, dirtyRegion);

		IDirect3DStateBlock9* PixelState = NULL;
		IDirect3DVertexDeclaration9* vertDec;
		IDirect3DVertexShader9* vertShader;

		device->CreateStateBlock(D3DSBT_PIXELSTATE, &PixelState);
		device->GetVertexDeclaration(&vertDec);
		device->GetVertexShader(&vertShader);

		static auto wanted_ret_address = _ReturnAddress();
		if (_ReturnAddress() == wanted_ret_address)
		{
			Render::Draw->Init(device);
			Render::Draw->Reset();

			CMenu::get()->Initialize();
			CMenu::get()->Draw();

			Features::EventLogger->Draw();
		}

		PixelState->Apply();
		PixelState->Release();

		device->SetVertexDeclaration(vertDec);
		device->SetVertexShader(vertShader);

		if (GetAsyncKeyState(VK_DELETE) && 1)
			Settings->UnloadCheat = true;

		return oPresent(device, src, dest, windowOverride, dirtyRegion);
	}

	static HRESULT D3DAPI reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
	{
		if (Settings->UnloadCheat) {
			Render::Draw->ReleaseObjects();
			return oReset(device, params);
		}

		return oReset(device, params);
	}
}