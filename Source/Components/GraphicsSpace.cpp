#include <Precompiled.h>
#include "GraphicsSpace.h"
#include "GraphicsComponent.h"


DefineType(GraphicsSpace, SinningZilch)
{
    BindComponent(GraphicsSpace);
}

void GraphicsSpace::Serialize(DataNode* node)
{
    Owner->GraphicsSpace = this;
    DataComponent* data = static_cast<DataComponent*>(node);
    AssignDependancy(Graphics, GameSession->GraphicsSystem);
}
void GraphicsSpace::Create()
{
    InitializePipeline();
    //EventConnect(GameSession, Events::EngineUpdate, &ForwardRenderer::RenderSpace, this);
    
}
void GraphicsSpace::Initialize()
{
    
}

//This extra step is inelegant but be very useful for optimization in the future.
void GraphicsSpace::AddGraphicsComponent(GraphicsComponent* comp, const DXBufferDescription& vertexBuffer, const DXBufferDescription& indexBuffer)
{
    GraphicsComponents.push_back(comp);
    if (vertexBuffer.ByteWidth != 0)
    {
        comp->SetVertexBuffer(CreateBuffer(vertexBuffer));
    }
    if (indexBuffer.ByteWidth != 0)
    {
        comp->SetIndexBuffer(CreateBuffer(indexBuffer));
    }
    
}
void GraphicsSpace::RemoveGraphicsComponent(GraphicsComponent* comp)
{
    GraphicsComponents.erase_value(comp);
    DestroyBuffer(comp->GetVertexBuffer());
    DestroyBuffer(comp->GetIndexBuffer());
}

DXBuffer* GraphicsSpace::CreateBuffer(const DXBufferDescription& desc)
{
    DXBuffer* buffer;
    Graphics->GetDevice()->CreateBuffer(&desc, NULL, &buffer);       // create the buffer
    switch (desc.BindFlags)
    {
    case D3D11_BIND_VERTEX_BUFFER:
    {
        VertexBuffers.push_back(buffer);
    }break;
    case D3D11_BIND_INDEX_BUFFER:
    {
        IndexBuffers.push_back(buffer);
    }break;
    }
    return buffer;
}

void GraphicsSpace::DestroyBuffer(DXBuffer* buffer)
{
    if (!buffer)
    {
        return;
    }
    DXBufferDescription desc;
    buffer->GetDesc(&desc);

    switch (desc.BindFlags)
    {
    case D3D11_BIND_VERTEX_BUFFER:
    {
        VertexBuffers.erase_value(buffer);
    }break;
    case D3D11_BIND_INDEX_BUFFER:
    {
        IndexBuffers.erase_value(buffer);
    }break;
    }

    ReleaseCOM(buffer);
}

void GraphicsSpace::InitializePipeline()
{

    //Should initialize them all;
    //VertexShader* VS = ResourceManager::FindResourceType<VertexShader>("VertexTest.vert");
    //PixelShader* PS = ResourceManager::FindResourceType<PixelShader>("PixelTest.pix");

    DXDeviceInterface* DeviceInterface = Graphics->GetDevice();
    DXDeviceContext* DeviceContext = Graphics->GetDeviceContext();

    

    


	//D3D11_SAMPLER_DESC
	DeviceContext->PSSetSamplers(0, 0, nullptr);


    //Perhaps I can add this to the GraphicsSystemClass
    // this can be part of the Init_Pipeline() function
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VSBufferDefault) + sizeof(VSBufferDefault) % 16; //Must always be a multiple of 16
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    DeviceInterface->CreateBuffer(&bd, NULL, &VSConstantBuffer);
    DeviceContext->VSSetConstantBuffers(0, 1, &VSConstantBuffer);

	D3D11_BLEND_DESC blendDesc;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.AlphaToCoverageEnable = true;

	DeviceInterface->CreateBlendState(&blendDesc, &BlendState);


	//float BlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	DeviceContext->OMSetBlendState(BlendState, 0, 0xffffffff);
    //INDEX BUFFER
    // Fill in a buffer description.
    //D3D11_BUFFER_DESC bufferDesc;
    //ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    //bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    //bufferDesc.ByteWidth = sizeof(unsigned int) * 3 * 2;
    //bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    //bufferDesc.CPUAccessFlags = 0;
    //bufferDesc.MiscFlags = 0;

    //// Create indices.
    ////unsigned int indices[] = { 0, 1, 2, 0, 2, 3 }; //Triangle list
    //unsigned int indices[] = { 0, 1, 2, 3, 2, 0}; //Triangle strip

    ////Make it so that each object has their own index buffer.

    //// Define the resource data.
    //D3D11_SUBRESOURCE_DATA InitData;
    //InitData.pSysMem = indices;
    //InitData.SysMemPitch = 0;
    //InitData.SysMemSlicePitch = 0;

    // Create the buffer with the device.
    //DeviceInterface->CreateBuffer(&bufferDesc, &InitData, &IndexBuffer);
    // Set the buffer.
    //DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

}

void GraphicsSpace::DrawDebugTriangle()
{
    DXDeviceContext* DeviceContext = Graphics->GetDeviceContext();
    //WILL CAUSE A MEMORY LEAK!
    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(Vertex) * 3;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
    DXBuffer* buffer = CreateBuffer(bd);

    Vertex OurVertices[] =
    {
        { Real3(0.0f, 0.5f, 1.0f), Real4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Real3(0.45f, -0.5, 1.0f), Real4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Real3(-0.45f, -0.5f, 1.0f), Real4(0.0f, 0.0f, 1.0f, 1.0f) },
    };

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    DeviceContext->Map(buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                      // copy the data
    DeviceContext->Unmap(buffer, NULL);                                      // unmap the buffer



}

void GraphicsSpace::Uninitialize()
{

}
void GraphicsSpace::Destroy()
{
    ReleaseCOM(BlendState);
    ReleaseCOM(InputLayout);
    //ReleaseCOM(IndexBuffer);
    ReleaseCOM(VSConstantBuffer);
    ReleaseCOM(PSConstantBuffer);
    VertexBuffers.clear();
    //EventDisconnect(GameSession, this, Events::EngineUpdate, this);
}