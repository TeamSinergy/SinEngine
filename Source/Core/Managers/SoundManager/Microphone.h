/******************************************************************************/
/*!
\file   Microphone.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Microphone active
*/
/******************************************************************************/

#pragma once



	class Microphone : public Component
	{
	public:
		Microphone(){}
		~Microphone(){}
		void Serialize(DataNode* data);
		void Initialize();

		const static String Name;
		bool Active;
	};
