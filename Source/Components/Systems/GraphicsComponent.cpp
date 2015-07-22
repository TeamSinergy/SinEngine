#include <Precompiled.h>
#include "GraphicsComponent.h"

DefineType(GraphicsComponent, SinningZilch)
{
    BindComponent(GraphicsComponent)
}

void GraphicsComponent::Serialize(DataNode* node)
{
    AssignDependancy(graphics, Space->GraphicsSpace);

    
    
}
void GraphicsComponent::Create()
{
    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(Vertex) * Vertices.size();             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    graphics->AddGraphicsComponent(this, bd);
}
void GraphicsComponent::Initialize()
{
    
}

void GraphicsComponent::UpdateVertexBuffer()
{
    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    GameSession->GraphicsSystem->GetDeviceContext()->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, Vertices.data(), sizeof(Vertex) * Vertices.size());                            // copy the data
    GameSession->GraphicsSystem->GetDeviceContext()->Unmap(VertexBuffer, NULL);                                      // unmap the buffer
}

void GraphicsComponent::Uninitialize()
{

}
void GraphicsComponent::Destroy()
{
    
}
