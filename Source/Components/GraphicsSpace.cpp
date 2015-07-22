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

void GraphicsSpace::AddGraphicsComponent(GraphicsComponent* comp, const DXBufferDescription& bufferDesc)
{
    GraphicsComponents.push_back(comp);
    comp->SetVertexBuffer(CreateBuffer(bufferDesc));
}
void GraphicsSpace::RemoveGraphicsComponent(GraphicsComponent* comp)
{
    GraphicsComponents.erase_value(comp);
    DestroyBuffer(comp->GetVertexBuffer());
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
    }
    return buffer;
}

void GraphicsSpace::DestroyBuffer(DXBuffer* buffer)
{
    DXBufferDescription desc;
    buffer->GetDesc(&desc);

    switch (desc.BindFlags)
    {
    case D3D11_BIND_VERTEX_BUFFER:
    {
        VertexBuffers.erase_value(buffer);
    }break;
    }

    ReleaseCOM(buffer);
}

void GraphicsSpace::InitializePipeline()
{

    //Should initialize them all;
    VertexShader* VS = ResourceManager::FindResourceType<VertexShader>("VertexTest.vert");
    PixelShader* PS = ResourceManager::FindResourceType<PixelShader>("PixelTest.pix");

    DXDeviceInterface* DeviceInterface = Graphics->GetDevice();
    DXDeviceContext* DeviceContext = Graphics->GetDeviceContext();

    DeviceInterface->CreateVertexShader(VS->StoredShader->GetBufferPointer(), VS->StoredShader->GetBufferSize(), NULL, &pVS);
    DeviceInterface->CreatePixelShader(PS->StoredShader->GetBufferPointer(), PS->StoredShader->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    DeviceContext->VSSetShader(pVS, nullptr, 0);
    DeviceContext->PSSetShader(pPS, nullptr, 0);


    //The element description
    D3D11_INPUT_ELEMENT_DESC inputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    DeviceInterface->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout), VS->StoredShader->GetBufferPointer(), VS->StoredShader->GetBufferSize(), &InputLayout);
    DeviceContext->IASetInputLayout(InputLayout);


    //Perhaps I can add this to the GraphicsSystemClass
    // this can be part of the Init_Pipeline() function
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VSBufferDefault) + sizeof(VSBufferDefault) % 16; //Must always be a multiple of 16
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    DeviceInterface->CreateBuffer(&bd, NULL, &VSConstantBuffer);
    DeviceContext->VSSetConstantBuffers(0, 1, &VSConstantBuffer);

    //INDEX BUFFER
    // Fill in a buffer description.
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(unsigned int) * 3 * 2;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    // Create indices.
    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
    // Define the resource data.
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = indices;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    // Create the buffer with the device.
    DeviceInterface->CreateBuffer(&bufferDesc, &InitData, &IndexBuffer);
    // Set the buffer.
    DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

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
    ReleaseCOM(pVS);
    ReleaseCOM(pPS) //TO BE MADE HASMAPS
    ReleaseCOM(InputLayout);
    ReleaseCOM(IndexBuffer);
    ReleaseCOM(VSConstantBuffer);
    ReleaseCOM(PSConstantBuffer);
    VertexBuffers.clear();
    //EventDisconnect(GameSession, this, Events::EngineUpdate, this);
}