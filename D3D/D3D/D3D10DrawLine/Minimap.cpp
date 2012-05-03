#include "Minimap.h"

Minimap::Minimap()
{
	m_miniDV = NULL;
	m_miniTexSRV = NULL;
	m_height = 512;
	m_width = 512;
}

Minimap::~Minimap()
{
	SAFE_RELEASE(m_miniDV);
	SAFE_RELEASE(m_miniTexSRV);
	SAFE_RELEASE(m_minimap);
}

void Minimap::initialize(ID3D10Device* p_Device)
{
	m_Device = p_Device;
	
	vp.Height = m_height;
	vp.Width = m_width;
	vp.TopLeftY = 0;
	vp.TopLeftX = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
}

HRESULT Minimap::CreateTex()
{
	//Create texture desc
	D3D10_TEXTURE2D_DESC texDesc;
	ZeroMemory( &texDesc, sizeof(texDesc) );
	texDesc.Width = m_width;
	texDesc.Height = m_height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	if(FAILED( m_Device->CreateTexture2D( &texDesc, NULL, &m_minimap)))
	{
		return E_FAIL;
	}
	////Create the depth stencil view desc
	D3D10_RENDER_TARGET_VIEW_DESC descMap;
	descMap.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	descMap.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descMap.Texture2D.MipSlice = 0;

	if(FAILED( m_Device->CreateRenderTargetView( m_minimap, &descMap, &m_miniDV)))
		return E_FAIL;

	//Create sahder resource view desc
	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	if(FAILED( m_Device->CreateShaderResourceView( m_minimap, &srvDesc, &m_miniTexSRV))) 
		return E_FAIL;
	//SAFE_RELEASE(m_minimap);

	return S_OK;
}

ID3D10RenderTargetView* Minimap::getRTV()
{
	return m_miniDV;
}

ID3D10ShaderResourceView* Minimap::getSRV()
{
	return m_miniTexSRV;
}

const D3D10_VIEWPORT* Minimap::getVP()
{
	return &vp;
}