#include <Precompiled.h>
#include "ForwardRenderer.h"
#include "GraphicsComponent.h"

DefineType(ForwardRenderer, SinningZilch)
{
    BindComponent(ForwardRenderer);
}

void ForwardRenderer::Serialize(DataNode* node)
{
    Owner->ForwardRenderer = this;
    DataComponent* data = static_cast<DataComponent*>(node);
    AssignDependancy(Graphics, GameSession->GraphicsSystem);
    AssignDependancy(GraphicsSpace, Space->GraphicsSpace);

    SerializeValue(data, ClearColor);
    GraphicsManager::NormalizeColor(ClearColor, Graphics->GetColorMode());
    
}
void ForwardRenderer::Create()
{
    EventConnect(GameSession, Events::EngineUpdate, &ForwardRenderer::RenderSpace, this);
    
    SetRenderTargetView();
    //Needs to happen after
    CreateDepthBuffer();
    
}
void ForwardRenderer::Initialize()
{
    
}

void ForwardRenderer::SetRenderTargetView(DXTexture2D* target)
{
    HRESULT result;
    if (target == nullptr) //Set the render target to the back buffer
    {
        RenderTarget = Graphics->GetPrimaryRenderTarget();
        return;
    }
    DXDeviceInterface* DeviceInterface = Graphics->GetDevice();
    DXDeviceContext* DeviceContext = Graphics->GetDeviceContext();
    // use the buffer address to create the render target
    result = DeviceInterface->CreateRenderTargetView(target, NULL, &RenderTarget);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");

    // set the render target as the buffer
    //Number of render targets
    //Pointer to an array of render targets
    target->Release();
    // Initailze the depth stencil view.
    // Bind the render target view and depth stencil buffer to the output render pipeline.

    // set the render target as the back buffer
    DeviceContext->OMSetRenderTargets(1, &RenderTarget, NULL);
}

void ForwardRenderer::CreateDepthBuffer()
{
    //Create the texture for the Depth Buffer
    HRESULT result;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    DXTexture2D* DepthStencilBuffer;
    //Depth buffer
    // Initialize the description of the depth buffer.
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = GameSession->WindowSystem->GetDimensions().x;
    depthBufferDesc.Height = GameSession->WindowSystem->GetDimensions().y;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthBufferDesc.SampleDesc.Count = Graphics->GetSampleRate();
    depthBufferDesc.SampleDesc.Quality = Graphics->GetQualityLevel() - 1;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    /*depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;*/

    // Create the texture for the depth buffer using the filled out description.
    result = Graphics->GetDevice()->CreateTexture2D(&depthBufferDesc, NULL, &DepthStencilBuffer);
    ErrorIf(FAILED(result), "Failed to Initialize DirectX11");
    
    D3D11_DEPTH_STENCIL_VIEW_DESC zBuffer;
    ZeroMemory(&zBuffer, sizeof(zBuffer));

    // Set up the depth stencil view description.
    zBuffer.Format = DXGI_FORMAT_D32_FLOAT;
    if (depthBufferDesc.SampleDesc.Count > 1)
    {
        zBuffer.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    }
    else
    {
        zBuffer.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    }
    //zBuffer.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    result = Graphics->GetDevice()->CreateDepthStencilView(DepthStencilBuffer, &zBuffer, &DepthBuffer);
    ReleaseCOM(DepthStencilBuffer);

    Graphics->GetDeviceContext()->OMSetRenderTargets(1, &RenderTarget, DepthBuffer);
}

void ForwardRenderer::RenderSpace(UpdateEvent* event)
{
    // clear the back buffer to the clearcolor
    float color[4] = { ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w };
    Graphics->GetDeviceContext()->ClearRenderTargetView(RenderTarget, color);

    //clear the depth buffer
    Graphics->GetDeviceContext()->ClearDepthStencilView(DepthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

    DXDeviceInterface* DeviceInterface = Graphics->GetDevice();
    DXDeviceContext* DeviceContext = Graphics->GetDeviceContext();

    VSBufferDefault WorldViewProj;
	
    
    unsigned vertexCount = 0;
    for (unsigned i = 0; i < GraphicsSpace->GraphicsComponents.size(); ++i)
    {
        vertexCount += GraphicsSpace->GraphicsComponents[i]->GetVertexCount();
    }

    // set the new values for the constant buffer
    //DeviceContext->UpdateSubresource(GraphicsSpace->VSConstantBuffer, 0, 0, &WorldViewProj, 0, 0);
    //Prefer map/unmap ^


    // Clear the depth buffer.
    //DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // do 3D rendering on the back buffer here
    //This is the primary bottleneck
    unsigned stride[2] = { sizeof(Vertex), sizeof(Vertex)};
    unsigned offset[2] = { 0, 0 };
	//DeviceContext->IASetVertexBuffers(0, GraphicsSpace->VertexBuffers.size(), GraphicsSpace->VertexBuffers.data(), stride, offset);
    
	
    for (unsigned i = 0; i < GraphicsSpace->GraphicsComponents.size(); ++i)
    {
		
		GraphicsComponent* comp = GraphicsSpace->GraphicsComponents[i];
		DeviceContext->IASetVertexBuffers(0, 1, &GraphicsSpace->VertexBuffers[i], stride, offset);

		DeviceContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(comp->GetPrimitiveTopology()));
		WorldViewProj.WorldViewProjection = comp->Owner->Transform->GetWorldMatrix() * Graphics->GetMainCamera()->ViewProjectionMatrix();
        //MainCamera->ViewMatrix() * MainCamera->ProjectionMatrix();
        WorldViewProj.WorldViewProjection.Transpose();
		
		DeviceContext->VSSetShader(comp->GetVertexShader()->StoredShader, nullptr, 0);
		DeviceContext->PSSetShader(comp->GetPixelShader()->StoredShader, nullptr, 0);
		DeviceContext->IASetInputLayout(comp->GetVertexShader()->InputLayout);

		if (comp->GetSpriteSource())
		{
			DeviceContext->PSSetShaderResources(0, 1, &comp->GetSpriteSource()->StoredTexture);
		}
		// set the shader objects
		

        DeviceContext->UpdateSubresource(GraphicsSpace->VSConstantBuffer, 0, 0, &WorldViewProj, 0, 0);
		if (comp->GetIndexBuffer())
		{
			DeviceContext->IASetIndexBuffer(comp->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		}
		if (comp->GetIndexBuffer())
		{
			DeviceContext->DrawIndexed(comp->GetIndexCount() * 3, 0, 0);
		}
		else
		{
			DeviceContext->Draw(comp->GetVertexCount(), 0);
		}
        //3 because the indices array is made of unsigned 3's
		
        //
    }
    //DeviceContext->Draw(3, 0);

    //DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffers[1], stride, offset);
    //DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // Set the buffer.
    //DeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    

    //WorldViewProj.WorldViewProjection = GraphicsSpace->GraphicsComponents[1]->Owner->Transform->GetWorldMatrix() * Graphics->GetMainCamera()->ViewProjectionMatrix();
    ////MainCamera->ViewMatrix() * MainCamera->ProjectionMatrix();
    //WorldViewProj.WorldViewProjection.Transpose();
    //DeviceContext->UpdateSubresource(GraphicsSpace->VSConstantBuffer, 0, 0, &WorldViewProj, 0, 0);
    //DeviceContext->DrawIndexed(6, 0, 0);

    //DeviceContext->DrawIndexedInstanced(3, 2, 0, 0, 0);
    //DeviceContext->Draw(3, 3);
    /*HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;

    unsigned int bufferNumber;*/



    ////// Set the position of the constant buffer in the vertex shader.
    //bufferNumber = 0;

    ////// Finanly set the constant buffer in the vertex shader with the updated values.
    //DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &MatrixBuffer);

    ////// Set the vertex input layout.
    //DeviceContext->IASetInputLayout(InputLayout);


    //// Set the vertex and pixel shaders that will be used to render this triangle.

    //// Render the triangle.
    //DeviceContext->DrawIndexed(3, 0, 0);


    // Present the back buffer to the screen since rendering is complete.
    if (Graphics->IsVSyncEnabled())
    {
        // Lock to screen refresh rate.
        Graphics->GetSwapChain()->Present(1, 0);
    }
    else
    {
        // Present as fast as possible.
        Graphics->GetSwapChain()->Present(0, 0);
    }



    EventSend(Space, Events::FrameUpdate, event);
}

void ForwardRenderer::Uninitialize()
{

}
void ForwardRenderer::Destroy()
{
    ReleaseCOM(DepthBuffer);
    //If it is different then we need to:
    //ReleaseCOM(RenderTarget);
    EventDisconnect(GameSession, this, Events::EngineUpdate, this);
}
