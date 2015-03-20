/******************************************************************************/
/*!
\file   ComponentSystem.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#pragma once


namespace Framework
{
	class Template_Component : public Component
	{
		public:
			Template_Component();
			~Template_Component();
			
				
			virtual void Initialize ();
			virtual void Serialize (Serializer::DataNode* data);



		// The non-base component uses DefineComponentName macro to name component
		const static std::string Name;
		
		private:


	}
}