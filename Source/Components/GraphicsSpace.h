#pragma once
#include "SinEntity.h"
//struct GraphicsObject
//{
//    GraphicsObject(const DXBuffer*const& vertexBuffer, const DXBuffer *const& indexBuffer) :
//        VertexBuffer(vertexBuffer),
//        IndexBuffer(indexBuffer) {}
//    const DXBuffer*const& VertexBuffer;
//    const DXBuffer*const& IndexBuffer;
//};

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

    void AddGraphicsComponent(GraphicsComponent* comp, const DXBufferDescription& vertexBuffer, const DXBufferDescription& indexBuffer);
    
    void RemoveGraphicsComponent(GraphicsComponent* comp);

    DXBuffer* CreateBuffer(const DXBufferDescription& desc);
    void DestroyBuffer(DXBuffer* buffer);

//private:

    const GraphicsSystem* Graphics;



    Array<GraphicsComponent*> GraphicsComponents; //For all the graphics components we are drawing. Maybe array of arrays?
    Array<DXBuffer*> VertexBuffers; //For all the stored vertices.
    Array<DXBuffer*> IndexBuffers; //For all the stored indices

    DXBuffer* VSConstantBuffer; //For all the user-defined variables in the VertexShader.
    DXBuffer* PSConstantBuffer; //For all the user-defined variables in the PixelShader.

    
	ID3D11BlendState* BlendState;
    //Array<GraphicsObject> GraphicsObjects; //For all the GraphicsObjects;

    DXInputLayout* InputLayout; //This holds the layout of our Vertex struct.
};