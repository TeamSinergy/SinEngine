#include <Precompiled.h>
#include "Sprite.h"

DefineType(Sprite, SinningZilch)
{
	BindComponent(Sprite)
	BindProperty(GetColor, SetColor, "Color");
}

void Sprite::Serialize(DataNode* node)
{
	DataComponent* data = static_cast<DataComponent*>(node);
    this->GraphicsComponent::Serialize(node);
	Owner->Sprite = this;
	SerializeValue(data, Color);
	GraphicsManager::NormalizeColor(Color);
	String texName = "UnitSquare.png";
	SerializeValueName(data, texName, "SpriteSource");
	SpriteSource = ResourceManager::FindResourceType<Texture>(texName);

	SerializeValue(data, Offset);

	VShader = ResourceManager::FindResourceType<VertexShader>("Sprite.vert");    // the vertex shader
	PShader = ResourceManager::FindResourceType<PixelShader>("Sprite.pix");     // the pixel shader

	SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    Vertices.push_back(Vertex(Offset + Real3(-0.5f, 0.5f, 0.0f), Color, Real2(0, 0)));    // vertex 0
	Vertices.push_back(Vertex(Offset + Real3(0.5f, 0.5f, 0.0f), Color, Real2(1, 0)));     // vertex 1
	Vertices.push_back(Vertex(Offset + Real3(-0.5f, -0.5f, 0.0f), Color, Real2(0, 1)));   // 2
	Vertices.push_back(Vertex(Offset + Real3(0.5f, -0.5f, 0.0f), Color, Real2(1, 1)));    // 3

    Indices.push_back(Unsigned3(0, 1, 2));
    Indices.push_back(Unsigned3(3, 0, 1));
    //Indices.push_back(Unsigned3(4, 1, 0));
	
}

void Sprite::SetColor(const Real4& color) 
{ 
	Color = color;
	GraphicsManager::NormalizeColor(Color);
	for (unsigned i = 0; i < Vertices.size(); ++i)
	{
		Vertices[i].Color = Color;
	}
	UpdateVertexBuffer();
};

void Sprite::Create()
{
    this->GraphicsComponent::Create();
    
}
void Sprite::Initialize()
{
    UpdateVertexBuffer();
    UpdateIndexBuffer();
	//EventConnect(GameSession, "EngineUpdate", &Sprite::Update, this);
}
void Sprite::Uninitialize()
{
    //EventDisconnect(GameSession, this, "EngineUpdate", this);
}
void Sprite::Destroy()
{
    graphics->RemoveGraphicsComponent(this);
}

void Sprite::Update(UpdateEvent* event)
{
    
}