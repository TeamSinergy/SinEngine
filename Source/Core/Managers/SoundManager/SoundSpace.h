/*****************************************************************************/
/*!
\file   SoundSpace.h
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Header file for the SoundSpace Class
*/
/*****************************************************************************/

#pragma once

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include "Sound.h"


#pragma endregion

#define LATENCY_MS (50)


class AudioComponent;
class Transform;

/*---------------------------------------------------------------------------
// Class
---------------------------------------------------------------------------*/
class SoundSpace : public Component
{
public:
ZilchDeclareDerivedType(SoundSpace, Component);
/*-----------------------------------------------------------------------
// Constructors
-----------------------------------------------------------------------*/
#pragma region Constructors
virtual void Serialize(DataNode*) override;
SoundSpace();

#pragma endregion

/*-----------------------------------------------------------------------
// Public Variables
-----------------------------------------------------------------------*/
#pragma region Public Variables

#define AudioAssetsPath "../Assets/Sounds"      
FMOD_DSP_METERING_INFO input; // Microphone input values
Transform* listener;

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
    
void          ErrCheck(FMOD_RESULT result);
void          Initialize() override;
Sound*        LoadSound(const String& filename, const String& soundName,
                        Sound::SoundID type,  float volume);      
void          StopSounds(int id);
void          AddAudioComponent(AudioComponent* component);
void          DeleteAudioComponent(AudioComponent* component);

// Pause Menu Effect
void          InitPauseMenuEffect();
void          SetPauseMenuEffect(float cutoff, float resonance, float time);
void          UpdatePauseMenuEffect(const float& dt);

// Microphone Functions
void          InitMicData();
void          LoadMicData();
void          UpdateMicData();
void          micMeter();
void          micFrequencyData();
void          micFilter(float cutoff = 150, float resonance = 1);
void          meterConsoleOut();
void          frequencyConsoleOut();
void          latencyConsoleOut();
bool          checkMicrophone();

// Called every frame
void  Update(UpdateEvent* event);
      

// Getters
Sound*        GetSound(char* soundName);
bool          GetMuted(int id = Sound::SFX_ALL);
float         GetMicrophoneValue ();
    
// Setters
void          SetPaused(bool paused = true, int id = Sound::SFX_ALL);    

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
virtual ~SoundSpace();

#pragma endregion

private:

    /*-----------------------------------------------------------------------
    // Private Variables
    -----------------------------------------------------------------------*/
    #pragma region Private Variables

    std::map<String, Sound*>          soundMap;
    std::list<AudioComponent*>        audioComponentList;
    FMOD::System                      *pFMODSoundSpace = NULL;
    FMOD::ChannelGroup                *GroupMusic;
    FMOD::ChannelGroup                *Group2DSFX;
    FMOD::ChannelGroup                *Group3DSFX;
    FMOD::DSP                         *fft;
    FMOD::DSP                         *meter;
    FMOD::DSP                         *filter;
    FMOD::DSP                         *lowPassEffect;
    FMOD::Sound                       *micSound = 0;
    FMOD::Channel                     *micChannel = 0;
    Sound                             *NewSound;
    FMOD_CREATESOUNDEXINFO            _exinfo;
    unsigned int                      _soundlength;
    unsigned int                      _recordpos;
    unsigned int                      _recorddelta;
    unsigned int                      _minrecorddelta;
    unsigned int                      _lastrecordpos;
    unsigned int                      _samplesrecorded;
    unsigned int                      _playpos;
    unsigned int                      _adjustedlatency;
    unsigned int                      _driftthreshold;
    int                               _recordrate;
    int                               _recordchannels;
    int                               _recordnumdrivers;
    float                             _smootheddelta;  
    float                             _cutoffVal;
    float                             _resonanceVal;
    float                             _fadeValA;
    float                             _fadeValB;
    bool                              _check;
    bool                              _lpfstate = false;

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

    bool CreateFMODSystem();
    void CreateSoundGroups();

    #pragma endregion


    /*-----------------------------------------------------------------------
    // Static Functions
    -----------------------------------------------------------------------*/
    #pragma region Static Functions

    #pragma endregion

};


//-----------------------------------------------------------------------------
