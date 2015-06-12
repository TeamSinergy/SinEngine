#include <Precompiled.h>
#include "GraphicsComponent.h"

DefineType(GraphicsComponent, SinningZilch)
{
    BindComponent(GraphicsComponent)
}

void GraphicsComponent::Serialize(DataNode* node)
{
    
    
}
void GraphicsComponent::Create()
{
    
}
void GraphicsComponent::Initialize()
{
    
}

void GraphicsComponent::UpdateVertexBuffer()
{
    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    graphics->DeviceContext->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, Vertices.data(), sizeof(Vertex) * Vertices.size());                            // copy the data
    graphics->DeviceContext->Unmap(VertexBuffer, NULL);                                      // unmap the buffer
}

void GraphicsComponent::Uninitialize()
{

}
void GraphicsComponent::Destroy()
{
    
}
