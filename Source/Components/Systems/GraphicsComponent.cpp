#include <Precompiled.h>
#include "GraphicsComponent.h"

DefineType(GraphicsComponent, SinningZilch)
{
    BindComponent(GraphicsComponent)
}

void GraphicsComponent::Serialize(DataNode* node)
{
    AssignDependancy(graphics, Space->GraphicsSpace);
    VShader = ResourceManager::FindResourceType<VertexShader>("Default.vert");    // the vertex shader
    PShader = ResourceManager::FindResourceType<PixelShader>("Default.pix");     // the pixel shader
    
}
void GraphicsComponent::Create()
{
    // create the vertex buffer
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));

    vbd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    vbd.ByteWidth = sizeof(Vertex) * Vertices.size();             // size is the VERTEX struct * 3
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));

    ibd.Usage = D3D11_USAGE_DYNAMIC;
    ibd.ByteWidth = sizeof(Unsigned3) * Indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ibd.MiscFlags = 0;

    graphics->AddGraphicsComponent(this, vbd, ibd);
}
void GraphicsComponent::Initialize()
{
    
}

void GraphicsComponent::UpdateVertexBuffer()
{
    if (!VertexBuffer)
    {
        return;
    }
    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    GameSession->GraphicsSystem->GetDeviceContext()->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, Vertices.data(), sizeof(Vertex) * Vertices.size());                            // copy the data
    GameSession->GraphicsSystem->GetDeviceContext()->Unmap(VertexBuffer, NULL);                                      // unmap the buffer
}

void GraphicsComponent::UpdateIndexBuffer()
{
    if (!IndexBuffer)
    {
        return;
    }
    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    GameSession->GraphicsSystem->GetDeviceContext()->Map(IndexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, Indices.data(), sizeof(Unsigned3) * Indices.size());                            // copy the data
    GameSession->GraphicsSystem->GetDeviceContext()->Unmap(IndexBuffer, NULL);                                      // unmap the buffer
}

void GraphicsComponent::Uninitialize()
{

}
void GraphicsComponent::Destroy()
{
    
}
