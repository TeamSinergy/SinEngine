/*****************************************************************************/
/*!
\file   AudioComponent.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Creates a component for audio
*/      
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include <Precompiled.h>

#pragma endregion

DefineType(AudioComponent, SinningZilch)
{
    BindComponent(AudioComponent);
}
  /*---------------------------------------------------------------------------
  // Class Implementation
  ---------------------------------------------------------------------------*/

  /*---------------------------------------------------------------------------
  // Constructors
  ---------------------------------------------------------------------------*/
  #pragma region Constructors

  //DefineComponentName(AudioComponent);

  AudioComponent::AudioComponent()
  {
    Positional = false;
    Playing = false;
    Mute = false;
    LowPassed = false;
    LowCuttoff = 6000;
    LowResonance = 1;
    HighPassed = false;
    HighCutoff = 150;
    HighResonance = 1;
    MicEffect = false;
  }

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Public Variables
  ---------------------------------------------------------------------------*/
  #pragma region Global Variables

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

  void AudioComponent::Serialize(DataNode* node)
  {
    DataComponent* data = static_cast<DataComponent*>(node);
    SerializeValue(data, Positional);
    SerializeValue(data, Playing);
    SerializeValue(data, Mute);
    SerializeValue(data, LowPassed);
    SerializeValue(data, LowCuttoff);
    SerializeValue(data, LowResonance);
    SerializeValue(data, HighPassed);
    SerializeValue(data, HighCutoff);
    SerializeValue(data, HighResonance);
    SerializeValue(data, MicEffect);
    SerializeValue(data, Looping);
    SerializeValue(data, FileName);
    SerializeValue(data, SoundID);
    SerializeValue(data, SoundName);
    //Sound::SoundID SoundType;
    SerializeValue(data, Played);

    if (SoundID == "BG")
      SoundType = Sound::MUSIC;
    else if (SoundID == "2D")
      SoundType = Sound::SOUND_2D;
    else if (SoundID == "3D")
      SoundType = Sound::SOUND_3D;    

    
  }

  void AudioComponent::Initialize()
  {
    NewSound = Space->SoundSpace->LoadSound(FileName.c_str(), const_cast<char*>(SoundName.c_str()), SoundType, 1.0f);
    
    //LoadSound("FireA.ogg");
    Owner->AudioComponent = this;
    Space->SoundSpace->AddAudioComponent(this);

  }

  void AudioComponent::LoadSound(String name)
  {
    NewSound = Space->SoundSpace->LoadSound(name.c_str(), "Test", Sound::SOUND_3D, 1.0f);
  }

  void AudioComponent::PlaySound()
  {
    Playing = true;
    Positional = true;
    LowPassed = true;
    LowCuttoff = 6000;
    LowResonance = 1;
    HighPassed = false;
    HighCutoff = 150;
    HighResonance = 1;
    MicEffect = true;
  }

  void AudioComponent::Update()
  {
    if (Positional)
      NewSound->SetPosition(Owner->Transform->GetTranslation());

    if (Playing)
    {
      if (SoundType != Sound::SOUND_2D)
      {
        NewSound->Play();
      }
      else if (SoundType == Sound::SOUND_2D && Played == false)
      {
        NewSound->Play();
        Played = true;
      }
    }

    if (LowPassed && Playing)
    {
      NewSound->LowPassFilter();
      NewSound->SetLPF(LowCuttoff, LowResonance);

      LowPassed = false;
    }

    if (HighPassed && Playing)
    {
      NewSound->HighPassFilter();
      NewSound->SetHPF(HighCutoff, HighResonance);

      HighPassed = false;
    }

    if (MicEffect && Playing)
      NewSound->micEffectUpdate();

    if (Playing && SoundType != Sound::SOUND_2D)
      NewSound->SetMute(Mute);
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

  AudioComponent::~AudioComponent()
  {
    if (NewSound != nullptr && NewSound->Get_Channel() != nullptr)
      NewSound->Stop();
    Space->SoundSpace->DeleteAudioComponent(this);
  }

  #pragma endregion



//-----------------------------------------------------------------------------
