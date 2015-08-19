/*****************************************************************************/
/*!
\file   AudioEvents.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Creates the infrastructure for the Audio Events
*/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include <Precompiled.h>
#include "AudioEvents.h"
#include "Sound.h"
#pragma endregion


  /*---------------------------------------------------------------------------
  // Class Implementation
  ---------------------------------------------------------------------------*/

  AudioEvents *AUDIOEVENTS = nullptr;
  /*---------------------------------------------------------------------------
  // Constructors
  ---------------------------------------------------------------------------*/
  #pragma region Constructors

  Sound *test, *test2;

  AudioEvents::AudioEvents()
  {
    ErrorIf(AUDIOEVENTS != nullptr, "ERROR created AudioEvents twice!");
    AUDIOEVENTS = this;
  }
  #pragma endregion

  /*---------------------------------------------------------------------------
  // Public Variables
  ---------------------------------------------------------------------------*/
  #pragma region Public Variables


  #pragma endregion

  /*-------------------------------------------------------------------------*/
  // Public Structs
  /*-------------------------------------------------------------------------*/
  #pragma region Public Structs

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Public Functions
  ---------------------------------------------------------------------------*/
  #pragma region Public Functions

  void AudioEvents::Initialize()
  {
      //JOSH CONNECT TO EVENTS
    /*EVENTSYSTEM->mConnect<FocusEvent, AudioEvents>(Events::FocusEvent, this, &AudioEvents::AudioEventsUpdate);
    EVENTSYSTEM->mConnect<KeyEvent, AudioEvents>(Events::KEY_ANY, this, &AudioEvents::OnKeyPressed);
    EVENTSYSTEM->mConnect<UpdateEvent, AudioEvents>(Events::UPDATEEVENT, this, &AudioEvents::OnUpdate);*/

    //test = SoundSpace->LoadSound("CreditsMusic.ogg", "NOISE", Sound::MUSIC, 1.0f);
    //test2 = SoundSpace->LoadSound("MainMenuScroll.ogg", "meh", Sound::SOUND_2D, 1.0f);
    //test->Play();
  }

  void AudioEvents::AudioEventsUpdate(FocusEvent* e)
  {
    if (e == nullptr)
      return;
    //JOSH DEAL WITH FOCUS EVENT
    if (e->InFocus)
    {      
      //Space->SoundSpace->SetPaused(false, Sound::SoundID::SFX_ALL);
      SetPauseUnmanagedSounds(false);
    }
    else
    {
      //SoundSpace->SetPaused(true, Sound::SoundID::SFX_ALL);
      SetPauseUnmanagedSounds(true);
    }
  }

  void AudioEvents::OnKeyPressed(KeyEvent* key)
  {
      
  }

  void AudioEvents::OnUpdate(UpdateEvent *e)
  {
  }

  void AudioEvents::SetPauseUnmanagedSounds(bool paused)
  {
    for (auto Sound : unmanagedSounds)
    {
      Sound->Get_Channel()->setPaused(paused);
    }
  }

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Static Public Variables
  ---------------------------------------------------------------------------*/
  #pragma region Static Public Variables

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Static Public Functions
  ---------------------------------------------------------------------------*/
  #pragma region Static Public Functions

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Destructor
  ---------------------------------------------------------------------------*/
  #pragma region Destructor

  AudioEvents::~AudioEvents()
  {
  
  }

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Private Variables
  ---------------------------------------------------------------------------*/
  #pragma region Private Variables

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Private Structs
  ---------------------------------------------------------------------------*/
  #pragma region Private Structs

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Private Functions
  ---------------------------------------------------------------------------*/
  #pragma region Private Functions

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Static Functions
  ---------------------------------------------------------------------------*/
  #pragma region Static Functions

  #pragma endregion



//-----------------------------------------------------------------------------