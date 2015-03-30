#pragma once
#include <Precompiled.h>
#include "DataNode.h"

class DataNode
{
public:
	String CurrentLine;
	DataNode* StepIn;
	DataNode* StepOut;
	DataNode* Next;
	DataNode* Previous;

	union
	{
		int Int_;
		unsigned int UInt_;
		long Long_;
		float Float_;
		double Double_;
		bool Bool_;
		String* String_;
		Real2* Real2_;
		Real3* Real3_;
		Real4* Real4_;
		Integer2* Int2_;
		Integer3* Int3_;
		Integer4* Int4_;
		std::vector<float>* VecN_;
		String* Enum_;
		String* Custom_;
	} value_;
};