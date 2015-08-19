/*****************************************************************************/
/*!
\file   DSP_System.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains various functions meant for Digital Signal Processing
*/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#include <Precompiled.h>
#include "Sound.h"

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;


  /***************************************************************************/
  /*!
  \brief  Attaches a Low Pass Filter to the Signal Chain

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::LowPassFilter()
  {
    FMOD_RESULT result;
    bool active;

    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }

    result = pFMODSoundSpace->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &objects_DSP.dsp_lpf);
    ErrCheck(result);

    result = objects_DSP.dsp_lpf->getActive(&active);
    ErrCheck(result);

    if (active)
    {
      result = pChannel->removeDSP(objects_DSP.dsp_lpf);
      ErrCheck(result);
    }
    else
    {
      result = pChannel->addDSP(0, objects_DSP.dsp_lpf);
      ErrCheck(result);
    }
  }

  /***************************************************************************/
  /*!
  \brief  Sets the low pass filter parameters  

  \param  cutoff
  Lowpass cutoff frequency in hz. 10.0 to 22000.0

  \param  resonance
  Lowpass resonance Q value. 1.0 to 10.0

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SetLPF(float cutoff, float resonance)
  {
    FMOD_RESULT result;

    // Lowpass cutoff frequency in hz.   10.0 to 22000.0
    result = objects_DSP.dsp_lpf->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, cutoff);
    ErrCheck(result);

    // Lowpass resonance Q value. 1.0 to 10.0
    result = objects_DSP.dsp_lpf->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, resonance);
    ErrCheck(result);
  }

  /***************************************************************************/
  /*!
  \brief  Attaches a High Pass Filter to the Signal Chain

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::HighPassFilter()
  {
    FMOD_RESULT result;
    bool active;

    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }

    result = pFMODSoundSpace->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &objects_DSP.dsp_hpf);
    ErrCheck(result);

    result = objects_DSP.dsp_hpf->getActive(&active);
    ErrCheck(result);

    if (active)
    {
      result = pChannel->removeDSP(objects_DSP.dsp_hpf);
      ErrCheck(result);
    }
    else
    {
      result = pChannel->addDSP(0, objects_DSP.dsp_hpf);
      ErrCheck(result);
    }
  }

  /***************************************************************************/
  /*!
  \brief  Sets the High pass filter parameters

  \param  cutoff
  Highpass cutoff frequency in hz.
  Ranges from 1.0 to 22000.0

  \param  resonance
  Highpass resonance Q value.
  Ranges from 1.0 to 10.0

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SetHPF(float cutoff, float resonance)
  {
    FMOD_RESULT result;

    // Highpass cutoff frequency in hz.  1.0 to output 22000.0
    result = objects_DSP.dsp_hpf->setParameterFloat(FMOD_DSP_HIGHPASS_CUTOFF, cutoff);
    ErrCheck(result);

    // Highpass resonance Q value.  1.0 to 10.0
    result = objects_DSP.dsp_hpf->setParameterFloat(FMOD_DSP_HIGHPASS_RESONANCE, resonance);
    ErrCheck(result);

  }

  /***************************************************************************/
  /*!
  \brief  Emulates artificial reverb to the overall mix

  \param  Wet
  Reverb signal level in dB.
  Ranges from -80.0 to 20.0

  \param  Dry
  Dry signal level in dB.
  Ranges from -80.0 to 20.0

  \param  Diffusion
  Reverberation diffusion (echo density) in percent.
  Ranges from 0.0 to 100.0

  \param  Density
  Reverberation density (modal density) in percent.
  Ranges from 0.0 to 100.0

  \param  Decay
  Reverberation decay time at low-frequencies in milliseconds.
  Ranges from 100.0 to 20000.0

  \param  EarlyR
  Delay time of first reflection in milliseconds.
  Ranges from 0.0 to 300.0

  \param  Delay
  Late reverberation delay time relative to first reflection in milliseconds.
  Ranges from 0.0 to 100.0

  \param  HF_ref
  eference frequency for high-frequency decay in Hz.
  Ranges from 20.0 to 20000.0

  \param  HF_decay
  High-frequency decay time relative to decay time in percent.
  Ranges from 10.0 to 100.0

  \param  LowShelf_Hz
  Transition frequency of low-shelf filter in Hz.
  Ranges from 20.0 to 1000.0

  \param  LowShelf_Gain
  Gain of low-shelf filter in dB.
  Ranges from -36.0 to 12.0

  \param  HighCut
  Cutoff frequency of low-pass filter in Hz.
  Ranges from 20.0 to 20000.0

  \param  EarlyLateMix
  Blend ratio of late reverb to early reflections in percent.
  Ranges from 0.0 to 100.0

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::Reverb()
  {
    FMOD_RESULT result;
    bool active;

    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }

    result = pFMODSoundSpace->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &objects_DSP.dsp_reverb);
    ErrCheck(result);

    result = objects_DSP.dsp_reverb->getActive(&active);
    ErrCheck(result);

    if (active)
    {
      result = pChannel->removeDSP(objects_DSP.dsp_reverb);
      ErrCheck(result);
    }
    else
    {
      result = pChannel->addDSP(0, objects_DSP.dsp_reverb);
      ErrCheck(result);
    }
  }

  /***************************************************************************/
  /*!
  \brief  Adds the custom reverb prest

  \param  preset
  The name of the preset

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::AddReverbPreset(Sound::ReverbPresetName preset)
  {
    FMOD_RESULT result;
    ReverbPreset type;

    type = SetReverbPreset(preset);

    // Reverb signal level in dB.  Ranges from -80.0 to 20.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_WETLEVEL, type.WetLevel);
    ErrCheck(result);
    // Dry signal level in dB.  Ranges from -80.0 to 20.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_DRYLEVEL, type.DryLevel);
    ErrCheck(result);

    // Reverberation diffusion (echo density) in percent.  Ranges from 0.0 to 100.0.
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_DIFFUSION, type.Diffusion);
    ErrCheck(result);

    // Reverberation density (modal density) in percent.  Ranges from 0.0 to 100.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_DENSITY, type.Density);
    ErrCheck(result);

    // Reverberation decay time at low-frequencies in milliseconds.  Ranges from 100.0 to 20000.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_DECAYTIME, type.DecayTime);
    ErrCheck(result);

    // Delay time of first reflection in milliseconds.  Ranges from 0.0 to 300.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYDELAY, type.EarlyDelay);
    ErrCheck(result);

    // Late reverberation delay time relative to first reflection in milliseconds.  Ranges from 0.0 to 100.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_LATEDELAY, type.LateDelay);
    ErrCheck(result);

    // Reference frequency for high-frequency decay in Hz.  Ranges from 20.0 to 20000.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_HFREFERENCE, type.HFReference);
    ErrCheck(result);

    // High-frequency decay time relative to decay time in percent.  Ranges from 10.0 to 100.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_HFDECAYRATIO, type.HFDecayRatio);
    ErrCheck(result);

    // Transition frequency of low-shelf filter in Hz.  Ranges from 20.0 to 1000.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_LOWSHELFFREQUENCY, type.LowShelfFrequency);
    ErrCheck(result);

    // Gain of low-shelf filter in dB.  Ranges from -36.0 to 12.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_LOWSHELFGAIN, type.LowShelfGain);
    ErrCheck(result);

    // Cutoff frequency of low-pass filter in Hz.  Ranges from 20.0 to 20000.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_HIGHCUT, type.HighCut);
    ErrCheck(result);

    // Blend ratio of late reverb to early reflections in percent.  Ranges from 0.0 to 100.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYLATEMIX, type.EarlyLateMix);
    ErrCheck(result);
  }

  /***************************************************************************/
  /*!
  \brief  Sets the reverb properties to the custom presets

  \param  preset
  The name of the reverb preset

  \return Returns nothing
  */
  /***************************************************************************/
  Sound::ReverbPreset Sound::SetReverbPreset(Sound::ReverbPresetName preset)
  {
    ReverbPreset type;

    // Custom presets
    switch (preset)
    {
    case Sound::OFF:
      type = { 1000, 7, 11, 5000, 100, 100, 100, 250, 0, 20, 96, -80.0f, 0 };
      break;
    case Sound::GENERIC:
      type = { 1500, 7, 11, 5000, 83, 100, 100, 250, 0, 14500, 96, 20.0f, 0 };
      break;
    case Sound::PADDEDCELL:
      type = { 170, 1, 2, 5000, 10, 100, 100, 250, 0, 160, 84, 20.0f, 0 };
      break;
    case Sound::ROOM:
      type = { 400, 2, 3, 5000, 83, 100, 100, 250, 0, 6050, 88, 20.0f, 0 };
      break;
    case Sound::BATHROOM:
      type = { 1500, 7, 11, 5000, 54, 100, 60, 250, 0, 2900, 83, 20.0f, 0 };
      break;
    case Sound::LIVINGROOM:
      type = { 500, 3, 4, 5000, 10, 100, 100, 250, 0, 160, 58, 20.0f, 0 };
      break;
    case Sound::STONEROOM:
      type = { 2300, 12, 17, 5000, 64, 100, 100, 250, 0, 7800, 71, 20.0f, 0 };
      break;
    case Sound::AUDITORIUM:
      type = { 4300, 20, 30, 5000, 59, 100, 100, 250, 0, 5850, 64, 20.0f, 0 };
      break;
    case Sound::CONCERTHALL:
      type = { 3900, 20, 29, 5000, 70, 100, 100, 250, 0, 5650, 80, -9.8f, 0 };
      break;
    case Sound::CAVE:
      type = { 2900, 15, 22, 5000, 100, 100, 100, 250, 0, 20000, 59, -11.3f, 0 };
      break;
    case Sound::ARENA:
      type = { 7200, 20, 30, 5000, 33, 100, 100, 250, 0, 4500, 80, -9.6f, 0 };
      break;
    case Sound::HANGAR:
      type = { 10000, 20, 30, 5000, 23, 100, 100, 250, 0, 3400, 72, -7.4f, 0 };
      break;
    case Sound::CARPETTEDHALLWAY:
      type = { 300, 2, 30, 5000, 10, 100, 100, 250, 0, 500, 56, -24.0f, 0 };
      break;
    case Sound::HALLWAY:
      type = { 1500, 7, 11, 5000, 59, 100, 100, 250, 0, 7800, 87, -5.5f, 0 };
      break;
    case Sound::STONECORRIDOR:
      type = { 270, 13, 20, 5000, 79, 100, 100, 250, 0, 9000, 86, -6.0f, 0 };
      break;
    case Sound::ALLEY:
      type = { 1500, 7, 11, 5000, 86, 100, 100, 250, 0, 8300, 80, -9.8f, 0 };
      break;
    case Sound::FOREST:
      type = { 1500, 162, 88, 5000, 54, 79, 100, 250, 0, 760, 94, -12.3f, 0 };
      break;
    case Sound::CITY:
      type = { 1500, 7, 11, 5000, 67, 50, 100, 250, 0, 4050, 66, -26.0f, 0 };
      break;
    case Sound::MOUNTAINS:
      type = { 1500, 300, 100, 5000, 21, 27, 100, 250, 0, 1220, 82, -24.0f, 0 };
      break;
    case Sound::QUARRY:
      type = { 1500, 61, 25, 5000, 83, 100, 100, 250, 0, 3400, 100, -5.0f, 0 };
      break;
    case Sound::PLAIN:
      type = { 1500, 179, 100, 5000, 50, 21, 100, 250, 0, 1670, 65, -28.0f, 0 };
      break;
    case Sound::PARKINGLOT:
      type = { 1700, 8, 12, 5000, 100, 100, 100, 250, 0, 20000, 56, -19.5f, 0 };
      break;
    case Sound::SEWERPIPE:
      type = { 1000, 7, 11, 5000, 100, 100, 100, 250, 0, 20, 96, -80.0f, 0 };
      break;
    case Sound::UNDERWATER:
      type = { 1000, 7, 11, 5000, 100, 100, 100, 250, 0, 20, 96, -80.0f, 0 };
      break;
    default:
      type = { 1000, 7, 11, 5000, 100, 100, 100, 250, 0, 20, 96, -80.0f, 0 };
      break;
    }

    return type;
  }

  /***************************************************************************/
  /*!
  \brief  Sets the default values for the first frequency notch

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SetFrequency1()
  {
    FMOD_RESULT result;

    result = pFMODSoundSpace->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &objects_DSP.dsp_sweepA);
    ErrCheck(result);

    result = objects_DSP.dsp_sweepA->setParameterFloat(FMOD_DSP_PARAMEQ_CENTER, 20.0f);
    ErrCheck(result);

    result = objects_DSP.dsp_sweepA->setParameterFloat(FMOD_DSP_PARAMEQ_BANDWIDTH, 0.2f);
    ErrCheck(result);

    result = objects_DSP.dsp_sweepA->setParameterFloat(FMOD_DSP_PARAMEQ_GAIN, -29.0f);
    ErrCheck(result);
  }

  /***************************************************************************/
  /*!
  \brief  Sweeps the frequency values for the first notch

  \param  center
  The center frequency

  \param  bandwidth
  Bandwidth of the frequency

  \param  gain
  Gain that needs to be boosted or cut

  \param  sweepTime
  The amount of time the interpolation needs to take 

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SweepEQ1(float center, float bandwidth, float gain, float sweepTime)
  {
    FMOD_RESULT result;

    float currentCenter;
    float currentBandwidth;
    float currentGain;
    float fadeSpeedA, fadeSpeedB, fadeSpeedC;
    bool active;
    char buffer[16];

    if (pChannel == NULL)
      return;

    result = objects_DSP.dsp_sweepA->getActive(&active);
    ErrCheck(result);

    if (!active && _EQStateA == false)
    {
      result = pChannel->addDSP(0, objects_DSP.dsp_sweepA);
      ErrCheck(result);

      //FOR TESTING
      //std::cout << CinderConsole::green << "EQ A ADDED" << std::endl;

      _EQStateA = true;
    }

    _centerValA = center;
    _bandwidthValA = bandwidth;
    _gainValA = gain;

    // Get the current DSP parameters
    result = objects_DSP.dsp_sweepA->getParameterFloat(FMOD_DSP_PARAMEQ_CENTER, &currentCenter, buffer, 16);
    ErrCheck(result);

    result = objects_DSP.dsp_sweepA->getParameterFloat(FMOD_DSP_PARAMEQ_BANDWIDTH, &currentBandwidth, buffer, 16);
    ErrCheck(result);

    result = objects_DSP.dsp_sweepA->getParameterFloat(FMOD_DSP_PARAMEQ_GAIN, &currentGain, buffer, 16);
    ErrCheck(result);

    // Speed = distance / time
    fadeSpeedA = (center - currentCenter) / sweepTime;
    _fadeValA1 = fadeSpeedA;

    fadeSpeedB = (bandwidth - currentBandwidth) / sweepTime;
    _fadeValA2 = fadeSpeedB;

    fadeSpeedC = (gain - currentGain) / sweepTime;
    _fadeValA3 = fadeSpeedC;
  }

  /***************************************************************************/
  /*!
  \brief  Updates the first frequency notch

  \param  dt
  The frame time

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::UpdateFrequency1(const float &dt)
  {
    FMOD_RESULT result;
    float currentCenter;
    float currentBandwidth;
    float currentGain;

    if (pChannel != NULL && objects_DSP.dsp_sweepA)
    {
      char buffer[16];

      // Get the current DSP parameters
      result = objects_DSP.dsp_sweepA->getParameterFloat(FMOD_DSP_PARAMEQ_CENTER, &currentCenter, buffer, 16);
      ErrCheck(result);

      result = objects_DSP.dsp_sweepA->getParameterFloat(FMOD_DSP_PARAMEQ_BANDWIDTH, &currentBandwidth, buffer, 16);
      ErrCheck(result);

      result = objects_DSP.dsp_sweepA->getParameterFloat(FMOD_DSP_PARAMEQ_GAIN, &currentGain, buffer, 16);
      ErrCheck(result);      

      // When gain reaches the lowest level, remove the DSP
      if (currentGain == -30.0f && _EQStateA == true)
      {
        result = pChannel->removeDSP(objects_DSP.dsp_sweepA);
        ErrCheck(result);

        // FOR TESTING
        //std::cout << CinderConsole::yellow << "EQ A REMOVED" << std::endl;

        _EQStateA = false;
      }

      // Following are checks to see if the parameters reached the values
      // we want to interpolate to
      if (_centerValA != currentCenter)
      {
        float newParameter;

        newParameter = currentCenter + (_fadeValA1 * dt);
        if (newParameter > _centerValA && _fadeValA1 > 0.0f)
        {
          newParameter = _centerValA;
        }
        else if (newParameter < _centerValA && _fadeValA1 < 0.0f)
        {
          newParameter = _centerValA;
        }

        result = objects_DSP.dsp_sweepA->setParameterFloat(FMOD_DSP_PARAMEQ_CENTER, newParameter);
        ErrCheck(result);
      }

      if (_bandwidthValA != currentBandwidth)
      {
        float newParameter;

        newParameter = currentBandwidth + (_fadeValA2 * dt);
        if (newParameter > _bandwidthValA && _fadeValA2 > 0.0f)
        {
          newParameter = _bandwidthValA;
        }
        else if (newParameter < _bandwidthValA && _fadeValA2 < 0.0f)
        {
          newParameter = _bandwidthValA;
        }

        result = objects_DSP.dsp_sweepA->setParameterFloat(FMOD_DSP_PARAMEQ_BANDWIDTH, newParameter);
        ErrCheck(result);
      }

      if (_gainValA != currentGain)
      {
        float newParameter;

        newParameter = currentGain + (_fadeValA3 * dt);
        if (newParameter > _gainValA && _fadeValA3 > 0.0f)
        {
          newParameter = _gainValA;
        }
        else if (newParameter < _gainValA && _fadeValA3 < 0.0f)
        {
          newParameter = _gainValA;
        }

        result = objects_DSP.dsp_sweepA->setParameterFloat(FMOD_DSP_PARAMEQ_GAIN, newParameter);
        ErrCheck(result);
      }
    }
  }

  /***************************************************************************/
  /*!
  \brief  Sets the default values for the second frequency notch

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SetFrequency2()
  {
    FMOD_RESULT result;

    result = pFMODSoundSpace->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &objects_DSP.dsp_sweepB);
    ErrCheck(result);

    result = objects_DSP.dsp_sweepB->setParameterFloat(FMOD_DSP_PARAMEQ_CENTER, 20.0f);
    ErrCheck(result);

    result = objects_DSP.dsp_sweepB->setParameterFloat(FMOD_DSP_PARAMEQ_BANDWIDTH, 0.2f);
    ErrCheck(result);

    result = objects_DSP.dsp_sweepB->setParameterFloat(FMOD_DSP_PARAMEQ_GAIN, -29.0f);
    ErrCheck(result);
  }

  /***************************************************************************/
  /*!
  \brief  Sweeps the frequency values for the second notch

  \param  center
  The center frequency

  \param  bandwidth
  Bandwidth of the frequency

  \param  gain
  Gain that needs to be boosted or cut

  \param  sweepTime
  The amount of time the interpolation needs to take

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SweepEQ2(float center, float bandwidth, float gain, float sweepTime)
  {
    FMOD_RESULT result;

    float currentCenter;
    float currentBandwidth;
    float currentGain;
    float fadeSpeedA, fadeSpeedB, fadeSpeedC;
    bool active;
    char buffer[16];

    if (pChannel == NULL)
      return;

    result = objects_DSP.dsp_sweepB->getActive(&active);
    ErrCheck(result);

    if (!active && _EQStateB == false)
    {
      result = pChannel->addDSP(0, objects_DSP.dsp_sweepB);
      ErrCheck(result);

      // FOR TESTING
      //std::cout << CinderConsole::green << "EQ B ADDED" << std::endl;

      _EQStateB = true;
    }

    _centerValB = center;
    _bandwidthValB = bandwidth;
    _gainValB = gain;

    // Get the current DSP parameters
    result = objects_DSP.dsp_sweepB->getParameterFloat(FMOD_DSP_PARAMEQ_CENTER, &currentCenter, buffer, 16);
    ErrCheck(result);

    result = objects_DSP.dsp_sweepB->getParameterFloat(FMOD_DSP_PARAMEQ_BANDWIDTH, &currentBandwidth, buffer, 16);
    ErrCheck(result);

    result = objects_DSP.dsp_sweepB->getParameterFloat(FMOD_DSP_PARAMEQ_GAIN, &currentGain, buffer, 16);
    ErrCheck(result);

    fadeSpeedA = (center - currentCenter) / sweepTime;
    _fadeValB1 = fadeSpeedA;

    fadeSpeedB = (bandwidth - currentBandwidth) / sweepTime;
    _fadeValB2 = fadeSpeedB;

    fadeSpeedC = (gain - currentGain) / sweepTime;
    _fadeValB3 = fadeSpeedC;
  }  

  /***************************************************************************/
  /*!
  \brief  Updates the second frequency notch

  \param  dt
  The frame time

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::UpdateFrequency2(const float &dt)
  {
    FMOD_RESULT result;
    float currentCenter;
    float currentBandwidth;
    float currentGain;

    if (pChannel != NULL && objects_DSP.dsp_sweepB)
    {
      char buffer[16];

      // Get the current DSP parameters
      result = objects_DSP.dsp_sweepB->getParameterFloat(FMOD_DSP_PARAMEQ_CENTER, &currentCenter, buffer, 16);
      ErrCheck(result);

      result = objects_DSP.dsp_sweepB->getParameterFloat(FMOD_DSP_PARAMEQ_BANDWIDTH, &currentBandwidth, buffer, 16);
      ErrCheck(result);

      result = objects_DSP.dsp_sweepB->getParameterFloat(FMOD_DSP_PARAMEQ_GAIN, &currentGain, buffer, 16);
      ErrCheck(result);

      // Check if gain reaches the lowest value. If it reaches then the DSP is removed
      if (currentGain == -30.0f && _EQStateB == true)
      {
        /*result = pChannel->removeDSP(objects_DSP.dsp_sweepB);
        ErrCheck(result);

        std::cout << CinderConsole::yellow << "EQ B REMOVED" << std::endl;

        _EQStateB = false;*/
      }

      // FOR TESTING
      //EQConsoleOut(currentCenter, currentBandwidth, currentGain);

      // Following are checks to see if the parameters reached the values
      // we want to interpolate to
      if (_centerValB != currentCenter)
      {
        float newParameter;

        newParameter = currentCenter + (_fadeValB1 * dt);
        if (newParameter > _centerValB && _fadeValB1 > 0.0f)
        {
          newParameter = _centerValB;
        }
        else if (newParameter < _centerValB && _fadeValB1 < 0.0f)
        {
          newParameter = _centerValB;
        }

        result = objects_DSP.dsp_sweepB->setParameterFloat(FMOD_DSP_PARAMEQ_CENTER, newParameter);
        ErrCheck(result);
      }

      if (_bandwidthValB != currentBandwidth)
      {
        float newParameter;

        newParameter = currentBandwidth + (_fadeValB2 * dt);
        if (newParameter > _bandwidthValB && _fadeValB2 > 0.0f)
        {
          newParameter = _bandwidthValB;
        }
        else if (newParameter < _bandwidthValB && _fadeValB2 < 0.0f)
        {
          newParameter = _bandwidthValB;
        }

        result = objects_DSP.dsp_sweepB->setParameterFloat(FMOD_DSP_PARAMEQ_BANDWIDTH, newParameter);
        ErrCheck(result);
      }

      if (_gainValB != currentGain)
      {
        float newParameter;

        newParameter = currentGain + (_fadeValB3 * dt);
        if (newParameter > _gainValB && _fadeValB3 > 0.0f)
        {
          newParameter = _gainValB;
        }
        else if (newParameter < _gainValB && _fadeValB3 < 0.0f)
        {
          newParameter = _gainValB;
        }

        result = objects_DSP.dsp_sweepB->setParameterFloat(FMOD_DSP_PARAMEQ_GAIN, newParameter);
        ErrCheck(result);
      }
    }
  }

  /***************************************************************************/
  /*!
  \brief  Prints out the current Center, Bandwidth and Gain of a 
          frequency notch

  \param  currentCenter
  The updated center frequency

  \param  currentBandwidth
  The updated bandwidth of the frequency

  \param  currentGain
  The updated gain of the frequency

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::EQConsoleOut(float currentCenter, float currentBandwidth, float currentGain)
  {
    /*std::cout << CinderConsole::cyan
      << currentCenter
      << std::endl
      << "             "
      << CinderConsole::yellow
      << currentBandwidth
      << "             "
      << CinderConsole::green
      << currentGain
      << std::endl;*/
  }

//-----------------------------------------------------------------------------