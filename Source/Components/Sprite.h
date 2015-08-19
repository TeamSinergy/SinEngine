#pragma once
#include "Component.h"
#include "GraphicsSystem.h"
#include "GraphicsComponent.h"

class Sprite : public GraphicsComponent
{
	ZilchDeclareDerivedType(Sprite, Component);
    void Serialize(DataNode* node) override; //Serialize this through zilch
    void Create() override;
    void Initialize() override;

    void Update(UpdateEvent* event);

	void SetColor(const Real4& color);
	const Real4& GetColor() { return Color; }

    void Uninitialize() override;
    void Destroy() override;

private:
	Real4 Color = Real4(1, 1, 1, 1);

    
};