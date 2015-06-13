#pragma once
#include <Precompiled.h>

#define ComponentPointer(Name) Name* Name = nullptr

class DataNode
{
    ZilchDeclareBaseType(DataNode, TypeCopyMode::ReferenceType);
    DataNode() {};
    virtual ~DataNode() {};
};

class SinEntity : public EventHandler
{
public:
    ZilchDeclareDerivedType(SinEntity, EventHandler);
    String Name;

    SinEntity() : EventHandler() {}
    //Optional constructor to set the name equal to the passed in name.
    SinEntity(const String& name) : EventHandler(), Name(name){}
    virtual void Serialize(DataNode* node) {}
    virtual void Create() {}
    virtual void Initialize() {}
    virtual void Uninitialize() {}
    virtual void Destroy() {}
    virtual ~SinEntity() {}

};