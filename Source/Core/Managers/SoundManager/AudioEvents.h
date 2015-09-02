/*****************************************************************************/
/*!
\file   AudioEvents.h
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Creates the infrastructure for the Audio Events
*/
/*****************************************************************************/

#ifndef _AUDIOEVENTS_H
#define _AUDIOEVENTS_H

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes
#include "Events.h"
#pragma endregion


  class FocusEvent;
  class KeyEvent;
  class UpdateEvent;
  class Sound;

  /*---------------------------------------------------------------------------
  // Class
  ---------------------------------------------------------------------------*/
  class AudioEvents
  {
    public:

      /*-----------------------------------------------------------------------
      // Constructors
      -----------------------------------------------------------------------*/
      #pragma region Constructors

      AudioEvents();

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Public Variables
      -----------------------------------------------------------------------*/
      #pragma region Public Variables

      std::vector<Sound*> unmanagedSounds;
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

      void Initialize();
      void AudioEventsUpdate(FocusEvent* e);
      void OnKeyPressed(KeyEvent* _key);
      void OnUpdate(UpdateEvent *e);
      void SetPauseUnmanagedSounds(bool paused);

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

      virtual ~AudioEvents();

      #pragma endregion

    private:

      /*-----------------------------------------------------------------------
      // Private Variables
      -----------------------------------------------------------------------*/
      #pragma region Private Variables

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

  extern AudioEvents *AUDIOEVENTS;


//-----------------------------------------------------------------------------

#endif				// _AUDIOEVENTS_H