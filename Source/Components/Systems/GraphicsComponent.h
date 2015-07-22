#pragma once
#include "Component.h"
#include "GraphicsComponent.h"

class GraphicsComponent : public Component
{
    ZilchDeclareDerivedType(GraphicsComponent, Component);

    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

    void UpdateVertexBuffer();
    
    void SetVertexBuffer(DXBuffer* buffer) { VertexBuffer = buffer; }
    DXBuffer* GetVertexBuffer() const { return VertexBuffer; }

    unsigned GetVertexCount() const { return Vertices.size(); }
//protected:
    GraphicsSpace* graphics;

    Array<Vertex> Vertices;
    Array<Unsigned3> Indices;
    DXBuffer* VertexBuffer;
    DXBuffer* IndexBuffer;
    

    VertexShader* vertexShader = nullptr;    // the vertex shader
    PixelShader* pixelShader = nullptr;     // the pixel shader
};