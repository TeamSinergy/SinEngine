/*****************************************************************************/
/*!
\file   Sound.h
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Header file for the Sound Class
*/
/*****************************************************************************/

#ifndef _SOUND_H
#define _SOUND_H

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include "fmod.hpp"
#include "fmod_DSP.h"
#include "fmod_errors.h"

#pragma endregion


  #define DISTANCE_FACTOR 0.04

  /*---------------------------------------------------------------------------
  // Class
  ---------------------------------------------------------------------------*/
  class Sound
  {
    public:
		ZilchDeclareBaseType(Sound, Zilch::TypeCopyMode::ReferenceType);
    
      /*-----------------------------------------------------------------------
      // Constructors
      -----------------------------------------------------------------------*/
      #pragma region Constructors

      Sound();

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Public Variables
      -----------------------------------------------------------------------*/
      #pragma region Public Variables

      static bool system_on_;
      FMOD::Sound *pSound;

      #pragma endregion

      /*---------------------------------------------------------------------*/
      // Public Structs/Enums
      /*---------------------------------------------------------------------*/
      #pragma region Public Structs/Enums

      // Sound ID's dedicated to types of sounds
      enum SoundID
      {
        SOUND_2D = 0,
        SOUND_3D,
        SFX_ALL,
        MUSIC
      };

      // Custom reverb presets
      enum ReverbPresetName
      {
        OFF,              GENERIC,      PADDEDCELL,     ROOM,
        BATHROOM,         LIVINGROOM,   STONEROOM,      AUDITORIUM,
        CONCERTHALL,      CAVE,         ARENA,          HANGAR,
        CARPETTEDHALLWAY, HALLWAY,      STONECORRIDOR,  ALLEY,
        FOREST,           CITY,         MOUNTAINS,      QUARRY,
        PLAIN,            PARKINGLOT,   SEWERPIPE,      UNDERWATER
      };

      // All the DSP objects
      struct objects_DSP
      {
        FMOD::DSP                     *dsp_lpf = 0;
        FMOD::DSP                     *dsp_hpf = 0;
        FMOD::DSP                     *dsp_reverb = 0;
        FMOD::DSP                     *dsp_noise = 0;
        FMOD::DSP                     *dsp_sweepA = 0;
        FMOD::DSP                     *dsp_sweepB = 0;
      }objects_DSP;

      // Reverb Preset Properties
      struct ReverbPreset
      {
        float DecayTime;
        float EarlyDelay;
        float LateDelay;
        float HFReference;
        float HFDecayRatio;
        float Diffusion;
        float Density;
        float LowShelfFrequency;
        float LowShelfGain;
        float HighCut;
        float EarlyLateMix;
        float WetLevel;
        float DryLevel;
      };

      #pragma endregion   

      /*-----------------------------------------------------------------------
      // Public Functions
      -----------------------------------------------------------------------*/
      #pragma region Public Functions

      void            ErrCheck(FMOD_RESULT result);
      void            Load(FMOD::System* pSystem, 
                        const String& filename,
                        const String& soundName,
                           SoundID id);
      void            Play();
      void            VolumeFade(float volume, float fadeTime);
      void            Stop();
      void            PlayNew();
      void            AddSound(const String& filename);
      virtual void    Update(const float &dt);
      void            UpdateVolumeFade(const float& dt);
      void            micEffectUpdate();

      // DSP
      void            LowPassFilter();
      void            SetLPF(float cutoff = 5000.0f, float resonance = 1.0f);
      void            HighPassFilter();
      void            SetHPF(float cutoff = 5000.0f, float resonance = 1.0f);      
      void            Reverb();
      void            AddReverbPreset(Sound::ReverbPresetName preset);
      void            SetFrequency1();
      void            SweepEQ1(float center, float bandwidth, float gain, float sweepTime);
      void            UpdateFrequency1(const float& dt);
      void            SetFrequency2();
      void            SweepEQ2(float center, float bandwidth, float gain, float sweepTime);
      void            UpdateFrequency2(const float& dt);
      void            EQConsoleOut(float currentCenter, float currentBandwidth, float currentGain);

      // Procedural Audio
      void            UpdateNoise();
      void            GenerateNoise();

      // Setters
      ReverbPreset    SetReverbPreset(ReverbPresetName preset);
      void            SetChannelGroup(FMOD::ChannelGroup* channelGroup);          
      void            SetVolume(const float volume);
      void            SetMute(bool muteState);
      void            SetPause(bool pauseState);
      void            SetPan(char pan);
      void            SetLoop(const bool loopState, int index);
      void            SetLoopPoints(const float startPos, 
                                    const float endPos = 0, 
                                    const int index = 0);
      void            SetType(unsigned type, unsigned index);
      void            SetPosition(Real3 pos);

      // Getters
      float           GetMasterVolume(void);
      bool            GetPlaying();
      bool            GetPaused();
      float           GetVolume();   
      unsigned        GetTime();
      unsigned        GetID(){return ID;}
      String          GetSoundName(){ return SoundName;}
      float*          GetVolumePtr();

      FMOD::Channel* Get_Channel() {return pChannel;}
      FMOD::DSP*     Get_LPF ()    {return objects_DSP.dsp_lpf;}
      FMOD::DSP*     Get_HPF()     {return objects_DSP.dsp_hpf;}
      FMOD::DSP*     Get_RVRB()    {return objects_DSP.dsp_reverb;}
      FMOD::DSP*     Get_NOISE()   {return objects_DSP.dsp_noise;}

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

      ~Sound();

      #pragma endregion   

    private:

      /*-----------------------------------------------------------------------
      // Private Variables
      -----------------------------------------------------------------------*/
      #pragma region Private Variables

      unsigned                      ID;
      float                         _volume;
      float                         _paused;
      float                         _pitch;
      float                         _volValue;
      float                         _fadeValue;
      float                         _fadeValA1;
      float                         _fadeValA2;
      float                         _fadeValA3;
      float                         _fadeValB1;
      float                         _fadeValB2;
      float                         _fadeValB3;
      float                         _centerValA;
      float                         _centerValB;
      float                         _bandwidthValA;
      float                         _bandwidthValB;
      float                         _gainValA;
      float                         _gainValB;
      float                         _micLPF = 6000.0f;
      bool                          test = true;
      bool                          test2 = true;
      bool                          checkPos = true;
      bool                          _EQStateA = false;
      bool                          _EQStateB = false;
      std::vector<FMOD::Sound*>     sound_queue_;
      FMOD::Channel                 *pChannel;
      FMOD::ChannelGroup            *pChannelGroup;
      FMOD::System                  *pFMODSoundSpace;
      String                        SoundName;      
      FMOD_VECTOR                   _position;
      float                         _CutOffCounter;
      float                         _BandwidthCounter;
      bool                          _windCutOff = true;
      bool                          _windBandwidth = true;

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


#endif        // _SOUND_H