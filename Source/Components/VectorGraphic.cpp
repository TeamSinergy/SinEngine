#include <Precompiled.h>
#include "VectorGraphic.h"

DefineType(VectorGraphic, SinningZilch)
{
    BindComponent(VectorGraphic)
}

void VectorGraphic::Serialize(DataNode* node)
{
    this->GraphicsComponent::Serialize(node);

    Owner->VectorGraphic = this;

    /*Vertices.push_back(Vertex(Real3(0.45f, 0.5f, 0), Real4(0.0f, 1.0f, 0.0f, 1.0f)));
    Vertices.push_back(Vertex(Real3(0.45f, -0.5, 0), Real4(0.0f, 1.0f, 0.0f, 1.0f)));

    Vertices.push_back(Vertex(Real3(-0.45f, -0.5f, 0), Real4(0.0f, 0.0f, 1.0f, 1.0f)));
    Vertices.push_back(Vertex(Real3(-0.45f, 0.5f, 0), Real4(1.0f, 0.0f, 0.0f, 1.0f)));
    Vertices.push_back(Vertex(Real3(0.45f, 0.8f, -0.5), Real4(1.0f, 1.0f, 0.0f, 1.0f)));*/
    Vertices.push_back(Vertex(Real3(-1.0f, 1.0f, 1.0f), Real4(1.0f, 0.0f, 1.0f, 1.0f), Real2(0, 0)));    // vertex 0
    Vertices.push_back(Vertex(Real3(1.0f, 1.0f, 1.0f), Real4(0.0f, 1.0f, 1.0f, 1.0f), Real2(1, 0)));     // vertex 1
    Vertices.push_back(Vertex(Real3(-1.0f, -1.0f, 1.0f), Real4(0.0f, 1.0f, 1.0f, 1.0f), Real2(0, 1)));   // 2
    Vertices.push_back(Vertex(Real3(1.0f, -1.0f, 1.0f), Real4(1.0f, 1.0f, 1.0f, 1.0f), Real2(1, 1)));    // 3
    Vertices.push_back(Vertex(Real3(-1.0f, 1.0f, -1.0f), Real4(1.0f, 0.0f, 0.0f, 1.0f), Real2(0, 0)));     // ...
    Vertices.push_back(Vertex(Real3(1.0f, 1.0f, -1.0f), Real4(0.0f, 0.0f, 1.0f, 1.0f), Real2(1, 0)));
    Vertices.push_back(Vertex(Real3(-1.0f, -1.0f, -1.0f), Real4(1.0f, 0.0f, 1.0f, 1.0f), Real2(0, 1)));
    Vertices.push_back(Vertex(Real3(1.0f, -1.0f, -1.0f), Real4(0.0f, 1.0f, 1.0f, 1.0f), Real2(1, 1)));

    /*Indices.push_back(Unsigned3(0, 1, 2));
    Indices.push_back(Unsigned3(0, 3, 2));*/
    //Indices.push_back(Unsigned3(4, 1, 0));

    //LIST
    Indices.push_back(Unsigned3(0, 1, 2));   // side 1
    Indices.push_back(Unsigned3(2, 1, 3));
    Indices.push_back(Unsigned3(4, 0, 6));    // side 2
    Indices.push_back(Unsigned3(6, 0, 2));
    Indices.push_back(Unsigned3(7, 5, 6));   // side 3
    Indices.push_back(Unsigned3(6, 5, 4));
    Indices.push_back(Unsigned3(3, 1, 7));    // side 4
    Indices.push_back(Unsigned3(7, 1, 5));
    Indices.push_back(Unsigned3(4, 5, 0));    // side 5
    Indices.push_back(Unsigned3(0, 5, 1));
    Indices.push_back(Unsigned3(3, 7, 2));    // side 6
    Indices.push_back(Unsigned3(2, 7, 6));


    //A triangle strip includes the previous two indices.
    //A triangle list does not.
    //A triangle strip without and index buffer will require more vertices.
}

void VectorGraphic::Create()
{
    this->GraphicsComponent::Create();
    
}
void VectorGraphic::Initialize()
{
    UpdateVertexBuffer();
    UpdateIndexBuffer();
    EventConnect(GameSession, "EngineUpdate", &VectorGraphic::Update, this);
}
void VectorGraphic::Uninitialize()
{
    EventDisconnect(GameSession, this, "EngineUpdate", this);
}
void VectorGraphic::Destroy()
{
    graphics->RemoveGraphicsComponent(this);
}

void VectorGraphic::Update(UpdateEvent* event)
{
    //WorldRotation.z += 90.0f *event->Dt;
    //Owner->Transform->GetWorldTranslation().z -= 1.0f * event->Dt;
    //Owner->Transform->GetWorldRotation().z += 100.0f * event->Dt;
    //Owner->Transform->GetWorldScale().y += 10.0f * event->Dt;
    //WorldRotation.z += 10.0f * event->Dt;
}