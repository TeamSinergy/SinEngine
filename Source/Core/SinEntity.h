#pragma once

class DataNode
{
};

class SinEntity
{
public:
	String Name;

	SinEntity() {}
	//Optional constructor to set the name equal to the passed in name.
	SinEntity(String name) : Name (name){}
	virtual void Serialize(DataNode* node) {}
	virtual void Create() {}
	virtual void Initialize() {}
	virtual void Update(float dt) {}
	virtual void Unitialize() {}
	virtual void Destroy() {}
	~SinEntity() {}
private:

};