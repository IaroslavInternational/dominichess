#include "Graphics.h"
#include "dxerr.h"
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>
#include <array>
#include "GraphicsThrowMacros.h"

#include "EngineFunctions.hpp"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

namespace FramebufferShaders
{
	#include "FramebufferPS.shh"
	#include "FramebufferVS.shh"
}

Graphics::Graphics(HWND hWnd, int width, int height)
	:
	width( width ),
	height( height )
{	
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;
	using Microsoft::WRL::ComPtr;
	GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext
	) );

	ComPtr<ID3D11Resource> pBackBuffer;
	if (FAILED(hr = pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer)))
	{
		// error
	}

	if (FAILED(hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView)))
	{
		// error
	}

	pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);

	D3D11_VIEWPORT vp;
	vp.Width    = (float)width;
	vp.Height   = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1u,&vp );
	
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = width;
	sysTexDesc.Height = height;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	
	if (FAILED(hr = pDevice->CreateTexture2D(&sysTexDesc, nullptr, &pSysBufferTexture)))
	{
		GFX_THROW_INFO(hr, L"Creating sysbuffer texture");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	if (FAILED(hr = pDevice->CreateShaderResourceView(pSysBufferTexture.Get(),
		&srvDesc, &pSysBufferTextureView)))
	{
		GFX_THROW_INFO(hr, L"Creating view on sysBuffer texture");
	}

	if (FAILED(hr = pDevice->CreatePixelShader(
		FramebufferShaders::g_FramebufferPS,
		sizeof(FramebufferShaders::g_FramebufferPS),
		nullptr,
		&pPixelShader)))
	{
		GFX_THROW_INFO(hr, L"Creating pixel shader");
	}

	if (FAILED(hr = pDevice->CreateVertexShader(
		FramebufferShaders::g_FramebufferVS,
		sizeof(FramebufferShaders::g_FramebufferVS),
		nullptr,
		&pVertexShader)))
	{
		GFX_THROW_INFO(hr, L"Creating vertex shader");
	}

	const FSQVertex vertices[] =
	{
		{ -1.0f,  1.0f,  0.5f,  0.0f,  0.0f },
		{  1.0f,  1.0f,  0.5f,  1.0f,  0.0f },
		{  1.0f, -1.0f,  0.5f,  1.0f,  1.0f },
		{ -1.0f,  1.0f,  0.5f,  0.0f,  0.0f },
		{  1.0f, -1.0f,  0.5f,  1.0f,  1.0f },
		{ -1.0f, -1.0f,  0.5f,  0.0f,  1.0f },
	};

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(FSQVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if (FAILED(hr = pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer)))
	{
		GFX_THROW_INFO(hr, L"Creating vertex buffer");
	}

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	if (FAILED(hr = pDevice->CreateInputLayout(ied, 2,
		FramebufferShaders::g_FramebufferVS,
		sizeof(FramebufferShaders::g_FramebufferVS),
		&pInputLayout)))
	{
		GFX_THROW_INFO(hr, L"Creating input layout");
	}

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if (FAILED(hr = pDevice->CreateSamplerState(&sampDesc, &pSamplerState)))
	{
		GFX_THROW_INFO(hr, L"Creating sampler state");
	}

	pSysBuffer = reinterpret_cast<Color*>(_aligned_malloc(sizeof(Color) * width * height, 16u));
}

Graphics::~Graphics()
{
	if (pSysBuffer)
	{
		_aligned_free(pSysBuffer);
		pSysBuffer = nullptr;
	}

	if (pImmediateContext) pImmediateContext->ClearState();
}

void Graphics::BeginFrame() noexcept
{
	memset(pSysBuffer, 0u, sizeof(Color) * height * width);
}

void Graphics::EndFrame()
{
	HRESULT hr;
	
	if (FAILED(hr = pImmediateContext->Map(pSysBufferTexture.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture)))
	{
		GFX_THROW_INFO(hr, L"Mapping sysbuffer");
	}

	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData);
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof(Color);
	const size_t srcPitch = width;
	const size_t rowBytes = srcPitch * sizeof(Color);

	for (size_t y = 0u; y < height; y++)
	{
		memcpy(&pDst[y * dstPitch], &pSysBuffer[y * srcPitch], rowBytes);
	}

	pImmediateContext->Unmap(pSysBufferTexture.Get(), 0u);

	pImmediateContext->IASetInputLayout(pInputLayout.Get());
	pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	const UINT stride = sizeof(FSQVertex);
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	pImmediateContext->PSSetShaderResources(0u, 1u, pSysBufferTextureView.GetAddressOf());
	pImmediateContext->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf());

	
	if (FAILED(hr = pSwapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			GFX_THROW_INFO(pDevice->GetDeviceRemovedReason(), L"Presenting back buffer [device removed]");
		}
		else
		{
			GFX_THROW_INFO(hr, L"Presenting back buffer");
		}
	}

	pImmediateContext->Draw(6u, 0u);
}

UINT Graphics::GetWidth() const noexcept
{
	return width;
}

UINT Graphics::GetHeight() const noexcept
{
	return height;
}

RectI Graphics::GetScreenRect()
{
	return{ 0, static_cast<int>(width),0,static_cast<int>(height) };
}

Color Graphics::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < int(Graphics::width));
	assert(y >= 0);
	assert(y < int(Graphics::height));
	return pSysBuffer[Graphics::width * y + x];
}

void Graphics::PutPixel(int x, int y, int r, int g, int b)
{
	PutPixel(x, y, { unsigned char(r),unsigned char(g),unsigned char(b) });
}

void Graphics::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < int(Graphics::width));
	assert(y >= 0);
	assert(y < int(Graphics::height));
	pSysBuffer[Graphics::width * y + x] = c;
}

void Graphics::SetViewPort(D3D11_VIEWPORT& vp)
{
	pImmediateContext->RSSetViewports(1u, &vp);
}

void Graphics::DrawBorder(int xs, int ys, int xe, int ye, Color c)
{
	for (size_t i = xs; i < xe; i++)
	{
		PutPixel(i, ys, c);
	}

	for (size_t i = ys; i < ye; i++)
	{
		PutPixel(xs, i, c);
	}

	for (size_t i = xs; i < xe; i++)
	{
		PutPixel(i, ye, c);
	}

	for (size_t i = ys; i < ye; i++)
	{
		PutPixel(xe, i, c);
	}
}

void Graphics::DrawRectangle(int xs, int ys, int xe, int ye, Color c)
{
	for (size_t i = xs; i < xe; i++)
	{
		for (size_t j = ys; j < ye; j++)
		{
			PutPixel(i, j, c);
		}
	}
}

void Graphics::DrawGhostRectangle(int xs, int ys, int xe, int ye, Color c, float deep)
{
	for (size_t i = xs; i < xe; i++)
	{
		for (size_t j = ys; j < ye; j++)
		{
			const Color dstPixel = GetPixel(i, j);

			const Color blendedPixel = {
				unsigned char((dstPixel.GetR()) / deep),
				unsigned char((dstPixel.GetG()) / deep),
				unsigned char((dstPixel.GetB()) / deep)
			};

			PutPixel(i, j, blendedPixel);
		}
	}
}

void Graphics::DrawSpriteNonChroma(int x, int y, const Surface2D& s)
{
	DrawSpriteNonChroma(x, y, s.GetRect(), s);
}

void Graphics::DrawSpriteNonChroma(int x, int y, const RectI& srcRect, const Surface2D& s)
{
	DrawSpriteNonChroma(x, y, srcRect, GetScreenRect(), s);
}

void Graphics::DrawSpriteNonChroma(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s)
{
	assert(srcRect.left >= 0);
	assert(srcRect.right <= s.GetWidth());
	assert(srcRect.top >= 0);
	assert(srcRect.bottom <= s.GetHeight());
	if (x < clip.left)
	{
		srcRect.left += clip.left - x;
		x = clip.left;
	}
	if (y < clip.top)
	{
		srcRect.top += clip.top - y;
		y = clip.top;
	}
	if (x + srcRect.GetWidth() > clip.right)
	{
		srcRect.right -= x + srcRect.GetWidth() - clip.right;
	}
	if (y + srcRect.GetHeight() > clip.bottom)
	{
		srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
	}
	for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
	{
		for (int sx = srcRect.left; sx < srcRect.right; sx++)
		{
			if(s.GetPixel(sx, sy).GetA() != 0)
				PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, s.GetPixel(sx, sy));
		}
	}
}

void Graphics::DrawSprite(int x, int y, const Surface2D& s, Color chroma)
{
	DrawSprite(x, y, s.GetRect(), s, chroma);
}

void Graphics::DrawSprite(int x, int y, const RectI& srcRect, const Surface2D& s, Color chroma)
{
	DrawSprite(x, y, srcRect, GetScreenRect(), s, chroma);
}

void Graphics::DrawSprite(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s, Color chroma)
{
	assert(srcRect.left >= 0);
	assert(srcRect.right <= s.GetWidth());
	assert(srcRect.top >= 0);
	assert(srcRect.bottom <= s.GetHeight());
	if (x < clip.left)
	{
		srcRect.left += clip.left - x;
		x = clip.left;
	}
	if (y < clip.top)
	{
		srcRect.top += clip.top - y;
		y = clip.top;
	}
	if (x + srcRect.GetWidth() > clip.right)
	{
		srcRect.right -= x + srcRect.GetWidth() - clip.right;
	}
	if (y + srcRect.GetHeight() > clip.bottom)
	{
		srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
	}
	for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
	{
		for (int sx = srcRect.left; sx < srcRect.right; sx++)
		{
			const Color srcPixel = s.GetPixel(sx, sy);
			if (srcPixel != chroma)
			{
				PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, srcPixel);
			}
		}
	}
}

void Graphics::DrawSpriteSubstitute(int x, int y, Color substitute, const Surface2D& s, Color chroma)
{
	DrawSpriteSubstitute(x, y, substitute, s.GetRect(), s, chroma);
}

void Graphics::DrawSpriteSubstitute(int x, int y, Color substitute, const RectI& srcRect, const Surface2D& s, Color chroma)
{
	DrawSpriteSubstitute(x, y, substitute, srcRect, GetScreenRect(), s, chroma);
}

void Graphics::DrawSpriteSubstitute(int x, int y, Color substitute, RectI srcRect, const RectI& clip, const Surface2D& s, Color chroma)
{
	assert(srcRect.left >= 0);
	assert(srcRect.right <= s.GetWidth());
	assert(srcRect.top >= 0);
	assert(srcRect.bottom <= s.GetHeight());
	if (x < clip.left)
	{
		srcRect.left += clip.left - x;
		x = clip.left;
	}
	if (y < clip.top)
	{
		srcRect.top += clip.top - y;
		y = clip.top;
	}
	if (x + srcRect.GetWidth() > clip.right)
	{
		srcRect.right -= x + srcRect.GetWidth() - clip.right;
	}
	if (y + srcRect.GetHeight() > clip.bottom)
	{
		srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
	}
	for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
	{
		for (int sx = srcRect.left; sx < srcRect.right; sx++)
		{
			const Color srcPixel = s.GetPixel(sx, sy);
			if (srcPixel != chroma)
			{
				// use substitute color instead of color from the Surface2D (if not chroma)
				PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, substitute);
			}
		}
	}
}

void Graphics::DrawSpriteGhost(int x, int y, const Surface2D& s, float deep, Color chroma)
{
	DrawSpriteGhost(x, y, s.GetRect(), s, deep, chroma);
}

void Graphics::DrawSpriteGhost(int x, int y, const RectI& srcRect, const Surface2D& s, float deep, Color chroma)
{
	DrawSpriteGhost(x, y, srcRect, GetScreenRect(), s, deep, chroma);
}

void Graphics::DrawSpriteGhost(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s, float deep, Color chroma)
{
	assert(srcRect.left >= 0);
	assert(srcRect.right <= s.GetWidth());
	assert(srcRect.top >= 0);
	assert(srcRect.bottom <= s.GetHeight());
	if (x < clip.left)
	{
		srcRect.left += clip.left - x;
		x = clip.left;
	}
	if (y < clip.top)
	{
		srcRect.top += clip.top - y;
		y = clip.top;
	}
	if (x + srcRect.GetWidth() > clip.right)
	{
		srcRect.right -= x + srcRect.GetWidth() - clip.right;
	}
	if (y + srcRect.GetHeight() > clip.bottom)
	{
		srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
	}
	for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
	{
		for (int sx = srcRect.left; sx < srcRect.right; sx++)
		{
			const Color srcPixel = s.GetPixel(sx, sy);
			if (srcPixel != chroma)
			{
				const int xDest = x + sx - srcRect.left;
				const int yDest = y + sy - srcRect.top;
				const Color dstPixel = GetPixel(xDest, yDest);

				const Color blendedPixel = {
					unsigned char((dstPixel.GetR() + srcPixel.GetR()) / deep),
					unsigned char((dstPixel.GetG() + srcPixel.GetG()) / deep),
					unsigned char((dstPixel.GetB() + srcPixel.GetB()) / deep)
				};
				PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, blendedPixel);
			}
		}
	}
}

void Graphics::DrawSpriteGhostNonChroma(int x, int y, const Surface2D& s, float deep)
{
	DrawSpriteGhostNonChroma(x, y, s.GetRect(), s, deep);
}

void Graphics::DrawSpriteGhostNonChroma(int x, int y, const RectI& srcRect, const Surface2D& s, float deep)
{
	DrawSpriteGhostNonChroma(x, y, srcRect, GetScreenRect(), s, deep);
}

void Graphics::DrawSpriteGhostNonChroma(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s, float deep)
{
	assert(srcRect.left >= 0);
	assert(srcRect.right <= s.GetWidth());
	assert(srcRect.top >= 0);
	assert(srcRect.bottom <= s.GetHeight());
	if (x < clip.left)
	{
		srcRect.left += clip.left - x;
		x = clip.left;
	}
	if (y < clip.top)
	{
		srcRect.top += clip.top - y;
		y = clip.top;
	}
	if (x + srcRect.GetWidth() > clip.right)
	{
		srcRect.right -= x + srcRect.GetWidth() - clip.right;
	}
	if (y + srcRect.GetHeight() > clip.bottom)
	{
		srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
	}
	for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
	{
		for (int sx = srcRect.left; sx < srcRect.right; sx++)
		{
			const Color srcPixel = s.GetPixel(sx, sy);
			const int xDest = x + sx - srcRect.left;
			const int yDest = y + sy - srcRect.top;
			const Color dstPixel = GetPixel(xDest, yDest);

			const Color blendedPixel = {
				unsigned char((dstPixel.GetR() + srcPixel.GetR()) / deep),
				unsigned char((dstPixel.GetG() + srcPixel.GetG()) / deep),
				unsigned char((dstPixel.GetB() + srcPixel.GetB()) / deep)
			};
			PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, blendedPixel);
		}
	}
}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file),
	hr(hr)
{
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}

	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Код ошибки] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Строка ошибки] " << GetErrorString() << std::endl
		<< "[Описание] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Информация]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Engine Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Engine Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}

	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Информация]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Engine Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}