#pragma once
#include <Precompiled.h>
#include "Component.h"
#include "GraphicsComponent.h"
#include "ResourceManager.h"

class GraphicsComponent : public Component
{
    ZilchDeclareDerivedType(GraphicsComponent, Component);

    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;

    void UpdateVertexBuffer();
    void UpdateIndexBuffer();
    
    void SetVertexBuffer(DXBuffer* buffer) { VertexBuffer = buffer; }
    DXBuffer* GetVertexBuffer() const { return VertexBuffer; }
    void SetIndexBuffer(DXBuffer* buffer) { IndexBuffer = buffer; }
    DXBuffer* GetIndexBuffer() const { return IndexBuffer; }

    unsigned GetVertexCount() const { return Vertices.size(); }
    unsigned GetIndexCount() const { return Indices.size(); }

    unsigned GetPrimitiveTopology() const { return PrimitiveTopology; }
    unsigned& GetPrimitiveTopology() { return PrimitiveTopology; }
    void SetPrimitiveTopology(unsigned topologyMode) { PrimitiveTopology = topologyMode; }

	const Texture* GetSpriteSource() const { return SpriteSource; }
	Texture* GetSpriteSource() { return SpriteSource; }

	const VertexShader* GetVertexShader() const { return VShader; }
	VertexShader* GetVertexShader() { return VShader; }

	const PixelShader* GetPixelShader() const { return PShader; }
	PixelShader* GetPixelShader() { return PShader; }
protected:
	
	Real3 Offset = Real3();

    GraphicsSpace* graphics;

    Array<Vertex> Vertices;
    Array<Unsigned3> Indices;
    DXBuffer* VertexBuffer = nullptr;
    DXBuffer* IndexBuffer = nullptr;
	Texture* SpriteSource = nullptr;

    VertexShader* VShader;
    PixelShader* PShader;

    unsigned PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};