#include <Precompiled.h>
#include "ZilchComponent.h"

DefineType(ZilchComponent, SinningZilch)
{
    BindComponent(ZilchComponent);
    BindFieldGet(Owner);
    BindFieldGet(Space);
    BindFieldGet(GameSession);
}

void ZilchComponent::Serialize(DataNode* node)
{
    DataComponent* data = static_cast<DataComponent*>(node);

    BoundType* thisType = ZILCH->CompiledLib->BoundTypes[Name];

    Handle handle = Handle(ZILCH->GetDependencies(), thisType, (byte*)this);

    auto fields = thisType->InstanceFields.values();
    while (!fields.empty())
    {
        Field* field = fields.front();
        if (data->HasProperty(field->Name))
        {
            DataProperty* valueData = data->FindProperty(field->Name);
            const BoundType* valueType = valueData->GetType();
            ErrorIf(valueType->ToString() != field->PropertyType->ToString(), "The property %s on the component %s has conflicting types.", field->Name.c_str(), Name.c_str());
            fields.popFront();
            Call call(field->Set, ZILCH->GetDependencies());
            call.SetHandle(Zilch::Call::This, handle);
            const BoundType*& type = valueType;
            if (valueData->GetType()->CopyMode == TypeCopyMode::ReferenceType)
            {   
                if (type == ZilchTypeId(String))
                {
                    String copy = **(String**)valueData->GetHandle().Dereference();
                    call.SetHandle(0, copy);
                }
            }
            else //It is a value type
            {
                
                if (type == ZilchTypeId(Integer))
                {
                    call.Set(0, *(Integer*)valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Boolean))
                {
                    call.Set(0, *(Boolean*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Real))
                {
                    call.Set(0, *(Real*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Integer2))
                {
                    call.Set(0, *(Integer2*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Integer3))
                {
                    call.Set(0, *(Integer3*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Integer4))
                {
                    call.Set(0, *(Integer4*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Boolean2))
                {
                    call.Set(0, *(Boolean2*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Boolean3))
                {
                    call.Set(0, *(Boolean3*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Boolean4))
                {
                    call.Set(0, *(Boolean4*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Real2))
                {
                    call.Set(0, *(Real2*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Real3))
                {
                    call.Set(0, *(Real3*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Real4))
                {
                    call.Set(0, *(Real4*)&valueData->GetHandle().Data);
                }
                else if (type == ZilchTypeId(Quaternion))
                {
                    call.Set(0, *(Quaternion*)&valueData->GetHandle().Data);
                }
                else
                {
                    Error("Invalid type %s on component &s.", type->ToString().c_str(), Name.c_str());
                }
            }

            call.Invoke(ZILCH->Report);
        }
    }
}
void ZilchComponent::Create()
{
    
}
void ZilchComponent::Initialize()
{

}
void ZilchComponent::Uninitialize()
{

}
void ZilchComponent::Destroy()
{

}
