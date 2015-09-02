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

  void Sound::GenerateNoise()
  {
    FMOD_RESULT result;
    bool active;

    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }
    
    result = pFMODSoundSpace->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &objects_DSP.dsp_noise);
    ErrCheck(result);

    result = pFMODSoundSpace->playDSP(objects_DSP.dsp_noise, 0, false, &pChannel);
    ErrCheck(result);

    result = objects_DSP.dsp_noise->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, 5);
    ErrCheck(result);

    result = objects_DSP.dsp_noise->getActive(&active);
    ErrCheck(result);

    //if (active)
    //{
    //  result = pChannel->removeDSP(objects_DSP.dsp_noise);
    //  ErrCheck(result);
    //}

    if (!active)
    {
      result = pChannel->addDSP(0, objects_DSP.dsp_noise);
      ErrCheck(result);
    }
  }

  void Sound::UpdateNoise()
  {
      String name = GetSoundName();

      if (name == "NOISE" && objects_DSP.dsp_noise)
      {
          if (_windCutOff == true && _windBandwidth == true)
          {
              this->SetLPF(_CutOffCounter += 15.0f, _BandwidthCounter += 0.05f);

              if (_CutOffCounter > 1500 && _BandwidthCounter > 5.0)
              {
                  _windCutOff = false;
                  _windBandwidth = false;
              }
          }
          else if (_windCutOff == false && _windBandwidth == false)
          {
              this->SetLPF(_CutOffCounter -= 15.0f, _BandwidthCounter -= 0.05f);

              if (_CutOffCounter < 150 && _BandwidthCounter < 1.0)
              {
                  _windCutOff = true;
                  _windBandwidth = true;
              }
          }
      }
  }
  

//-----------------------------------------------------------------------------