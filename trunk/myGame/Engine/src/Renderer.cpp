#include "Renderer.h"
#include "Mathjavi.h"
#include "Camera.h"
//-----------------------------------------------
using namespace pGr;

D3DPRIMITIVETYPE primitivesMapping[pGr::PrimitiveCount] =
{
	D3DPT_TRIANGLELIST, 
	D3DPT_TRIANGLESTRIP,
	D3DPT_POINTLIST,
	D3DPT_LINELIST,     
	D3DPT_LINESTRIP,
	D3DPT_TRIANGLEFAN
};
D3DTRANSFORMSTATETYPE g_eMatrixTypeMapping [MatrixTypeCount] =
{
	D3DTS_VIEW,
	D3DTS_PROJECTION,
	D3DTS_WORLD
};
Renderer::Renderer():
	m_pkD3D(NULL),
	m_pkDevice(NULL),
	m_pkColorVB(NULL)
	{

	}

	//-----------------------------------------------
	Renderer::~Renderer(){
		if(m_pkColorVB){
			delete m_pkColorVB;
			m_pkColorVB = NULL;
		}
		if(m_pkTextureCoordVB){
			delete m_pkTextureCoordVB;
			m_pkTextureCoordVB=NULL;
		}
		if(m_pkD3D){
		m_pkD3D->Release();
		m_pkD3D=NULL;
		}
		if(m_pkDevice){
		m_pkDevice->Release();
		m_pkDevice = NULL;
		}
		//borrar texturas
		for(std::vector<Texture>::iterator it= m_akTextures.begin();it!=m_akTextures.end();it++)
		{
			(*it)->Release();
			(*it) = NULL;
		}
		m_akTextures.clear();
	}
	//-----------------------------------------------
	bool Renderer::init(HWND hWnd){
		m_pkD3D = Direct3DCreate9(D3D_SDK_VERSION);
		D3DPRESENT_PARAMETERS d;
		ZeroMemory(&d, sizeof(d));
		d.Windowed = TRUE;
		d.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d.hDeviceWindow = hWnd;

		
		
		if( m_pkD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, 
			hWnd, 
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
			&d, 
			&m_pkDevice) 
			== D3D_OK )
		{
			
			
			m_pkDevice->SetRenderState(D3DRS_LIGHTING,
								   FALSE);
			m_pkDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
			//m_pkDevice->SetRenderState(D3DRS_FILLMODE,
			//					   D3DFILL_WIREFRAME);

			//BLENDING
			m_pkDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
			m_pkDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
			m_pkDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			m_pkDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

			D3DVIEWPORT9 kViewport;
			m_pkDevice->GetViewport(&kViewport);


			float fViewportWidth= static_cast<float>(kViewport.Width);
			float fViewportHeight= static_cast<float>(kViewport.Height);


			D3DXMATRIX kProjectionMatrix;
			D3DXMatrixPerspectiveFovLH(&kProjectionMatrix,D3DXToRadian(90),fViewportWidth/fViewportHeight,1,3000);
			
			m_pkDevice->SetTransform(D3DTS_PROJECTION,&kProjectionMatrix);
			//m_pkColorVB = new pGr::VertexBuffer(m_rkRenderer,m_pkDevice,
			//m_pkColorVB = new pGr::VertexBuffer(m_pkDevice,sizeof(pGr::ColorVertex),pGr::ColorVertexType);
			//m_pkTextureCoordVB = new pGr::VertexBuffer(,m_pkDevice, 


			//m_pkTextureCoordVB = new pGr::VertexBuffer(m_pkDevice,sizeof(pGr::TextureCoordVertex),pGr::TextureCoordVertexType);
			
			m_pkCamera = new Camera(*m_pkDevice);
			return true;
		}
		return false;
	}
	void Renderer::setCameraPos(D3DXVECTOR3 Pos, D3DXVECTOR3 Look, D3DXVECTOR3 Up){
		D3DXMATRIX C_TransformMatrix;
		D3DXMatrixLookAtLH(&C_TransformMatrix,&Pos, &Look, &Up);
		m_pkDevice->SetTransform(D3DTS_VIEW,&C_TransformMatrix);
	}
	//-----------------------------------------------
	void Renderer::beginFrame(){
		// Limpia la escena
		if(r>=255)
			r=0;
		r+=3;
		if(g>=255)
			g=0;
		g+=12;
		if(b>=255)
			b=0;
		b+=25;
		m_pkDevice->Clear(0, 
						  NULL, 
						  D3DCLEAR_TARGET, 
						  D3DCOLOR_XRGB(0, 255, 0), 
						  1.0f, 
						  0);

		//codigo flashero
		
		//fin codigo flashero

		// Comienzo el render de una escena
		m_pkDevice->BeginScene();
	}
	//-----------------------------------------------
	void Renderer::endFrame(){
		// Termina 
		m_pkColorVB->flush();
		m_pkDevice->EndScene();
		m_pkDevice->Present(NULL, 
							NULL, 
							NULL, 
							NULL);

	}
	//-----------------------------------------------
	void Renderer::draw(ColorVertex* apkVertices,pGr::Primitive primitive,size_t vertexCount)
	{
		m_pkColorVB->bind();
		m_pkColorVB->draw(apkVertices,
						  primitivesMapping[primitive],
						  vertexCount);
	}
	void Renderer::draw(TextureCoordVertex* apkVertices,pGr::Primitive primitive,size_t vertexCount)
	{
		m_pkTextureCoordVB->bind();
		m_pkTextureCoordVB->draw(apkVertices,
						  primitivesMapping[primitive],
						  vertexCount);
	}

	void Renderer::setMatrix(MatrixType eMatrixType, const Matrix& rkMatrix)
	{
		m_pkDevice->SetTransform(g_eMatrixTypeMapping[eMatrixType],rkMatrix);
	}

	const Texture Renderer::loadTexture (const std::string& rkFilename,int iColorKey){
		IDirect3DTexture9* pkTexture = NULL;
		HRESULT hr= D3DXCreateTextureFromFileEx(
												m_pkDevice,
												rkFilename.c_str(),
												0,0,0,0,
												D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
												D3DX_FILTER_NONE, D3DX_FILTER_NONE,
												iColorKey,
												NULL,
												NULL,
												&pkTexture
		);
		
		if(hr!=D3D_OK){
			return NoTexture;
		}
		else{
			m_akTextures.push_back(pkTexture);
			return pkTexture;
		}
	
	}

	void Renderer::setCurrentTexture(const Texture& rkTexture){
		m_pkDevice->SetTexture(0, rkTexture);//el 0 es para especificar si es normalmap, difuse, specular, etc;
	}

	void Renderer::CreateVB(){


	}

	void Renderer::CreateIB(){


	}