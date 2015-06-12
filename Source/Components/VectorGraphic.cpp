#include <Precompiled.h>
#include "VectorGraphic.h"

DefineType(VectorGraphic, SinningZilch)
{
    BindComponent(VectorGraphic)
}

void VectorGraphic::Serialize(DataNode* node)
{
    Owner->VectorGraphic = this;
    DataComponent* data = static_cast<DataComponent*>(node);
    //Real4& Color = vertexMods.;
    //SerializeValue(data, Color);
    
    
    Vertices.push_back(Vertex(Real3(0.0f, 0.5f, 0), Real4(1.0f, 0.0f, 0.0f, 1.0f)));
    Vertices.push_back(Vertex(Real3(0.45f, -0.5, 0), Real4(0.0f, 1.0f, 0.0f, 1.0f)));
    Vertices.push_back(Vertex(Real3(-0.45f, -0.5f, 0), Real4(0.0f, 0.0f, 1.0f, 1.0f)));
    
}
void VectorGraphic::Create()
{
    Dependancy(Space->GraphicsSystem);
    graphics = Space->GraphicsSystem;

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(Vertex) * Vertices.size();             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    graphics->AddGraphicsComponent(this, bd);
}
void VectorGraphic::Initialize()
{
    UpdateVertexBuffer();
}
void VectorGraphic::Uninitialize()
{

}
void VectorGraphic::Destroy()
{
    graphics->RemoveGraphicsComponent(this);
}
