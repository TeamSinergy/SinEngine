#pragma once
#include "SinEntity.h"
class GraphicsSpace : public Component
{
    ZilchDeclareDerivedType(GraphicsSpace, Component);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

    void InitializePipeline();

    void DrawDebugTriangle();

    void AddGraphicsComponent(GraphicsComponent* comp, const DXBufferDescription& bufferDesc);
    void RemoveGraphicsComponent(GraphicsComponent* comp);

    DXBuffer* CreateBuffer(const DXBufferDescription& desc);
    void DestroyBuffer(DXBuffer* buffer);

//private:

    const GraphicsSystem* Graphics;



    Array<GraphicsComponent*> GraphicsComponents; //For all the graphics components we are drawing. Maybe array of arrays?
    Array<DXBuffer*> VertexBuffers; //For all the stored vertices.
    DXBuffer* VSConstantBuffer; //For all the user-defined variables in the VertexShader.
    DXBuffer* PSConstantBuffer; //For all the user-defined variables in the PixelShader.

    DXBuffer* IndexBuffer;

    DXInputLayout* InputLayout; //This holds the layout of our Vertex struct.

    //MAKE A HASHMAP
    ID3D11VertexShader *pVS;    // the vertex shader
    ID3D11PixelShader *pPS;     // the pixel shader
};