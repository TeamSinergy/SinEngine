/*****************************************************************************/
/*!
\file   AudioComponent.h
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Header file for the SoundSpace Class
*/
/*****************************************************************************/

#ifndef _AUDIO_COMPONENT
#define _AUDIO_COMPONENT

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes
#include "Sound.h"
#pragma endregion


  class Sound;

  /*---------------------------------------------------------------------------
  // Class
  ---------------------------------------------------------------------------*/
  class AudioComponent : public  Component
  {
    public:
        ZilchDeclareDerivedType(AudioComponent, Component);
      /*-----------------------------------------------------------------------
      // Constructors
      -----------------------------------------------------------------------*/
      #pragma region Constructors

      AudioComponent();

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Public Variables
      -----------------------------------------------------------------------*/
      #pragma region Public Variables
      Real3     position;

      #pragma endregion

      /*---------------------------------------------------------------------*/
      // Public Structs/Enums
      /*---------------------------------------------------------------------*/
      #pragma region Public Structs      

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Public Functions
      -----------------------------------------------------------------------*/
      #pragma region Public Functions

      virtual void Serialize(DataNode*);
      virtual void Initialize();
      void LoadSound(String name);
      void PlaySound();
      void Update();

      #pragma endregion


      /*-----------------------------------------------------------------------
      // Static Public Variables
      -----------------------------------------------------------------------*/
      #pragma region Static Public Variables

      #pragma endregion


      /*-----------------------------------------------------------------------
      // Static Public Functions
      -----------------------------------------------------------------------*/
      #pragma region Static Public Functions

      #pragma endregion


      /*-----------------------------------------------------------------------
      // Destructor
      -----------------------------------------------------------------------*/
      #pragma region Destructors

      ~AudioComponent();

      #pragma endregion

    private:

      /*-----------------------------------------------------------------------
      // Private Variables
      -----------------------------------------------------------------------*/
      #pragma region Private Variables

      bool           Positional;
      bool           Playing;
      bool           Mute = false;
      bool           LowPassed;
      float          LowCuttoff;
      float          LowResonance;
      bool           HighPassed;
      float          HighCutoff;
      float          HighResonance;
      bool           MicEffect;
      Sound          *NewSound;
      String         FileName;
      String         SoundID;
      String         SoundName;
      Sound::SoundID SoundType;
      bool           Played = false;
      bool           Looping = false;
      #pragma endregion


      /*-----------------------------------------------------------------------
      // Private Structs
      -----------------------------------------------------------------------*/
      #pragma region Private Structs    

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Private Functions
      -----------------------------------------------------------------------*/
      #pragma region Private Functions

      #pragma endregion


      /*-----------------------------------------------------------------------
      // Static Functions
      -----------------------------------------------------------------------*/
      #pragma region Static Functions

      #pragma endregion

  };


//-----------------------------------------------------------------------------

#endif				// _AUDIO_COMPONENT