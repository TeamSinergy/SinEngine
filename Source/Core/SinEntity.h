#pragma once

#define ComponentPointer(Name) Name* Name = nullptr

class DataNode
{
};

class SinEntity : public EventHandler
{
public:
	ZilchDeclareDerivedType(SinEntity, Zilch::EventHandler);
	String Name;

	SinEntity() {}
	//Optional constructor to set the name equal to the passed in name.
	SinEntity(String name) : Name (name){}
	virtual void Serialize(DataNode* node) {}
	virtual void Create() {}
	virtual void Initialize() {}
	virtual void Uninitialize() {}
	virtual void Destroy() {}
	~SinEntity() {}
private:

};