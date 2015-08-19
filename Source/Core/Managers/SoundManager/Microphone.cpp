/******************************************************************************/
/*!
\file   Microphone.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Microphone active
*/
/******************************************************************************/

#include <Precompiled.h>


	void Microphone::Serialize(DataNode* data)
	{
		//bool Active;
		/*DataNode* temp = data->FindElement(data, "Active");
		temp->GetValue(&Active);*/
	}

	void Microphone::Initialize()
	{
		Owner->Microphone = this;
	}

	
