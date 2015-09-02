/*****************************************************************************/
/*!
\file   Sound.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This file has all the method implementations for the Sound class
*/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include <Precompiled.h>
#include "Sound.h"
#include "SoundSpace.h"
#include "Transform.h"
#include "ZilchCompiledLib.h"

#pragma endregion

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

DefineType(Sound, SinningZilch)
{
//type->HandleManager = ZilchManagerId(Zilch::PointerManager);
    
BindMethod(SetVolume);
BindMethod(SetPan);
BindMethod(SetPause);
BindMethod(Stop);
BindMethod(Play);
BindMethod(VolumeFade);
//ZilchBindConstructor();
}
/*---------------------------------------------------------------------------
// Class Implementation
---------------------------------------------------------------------------*/
  
/*---------------------------------------------------------------------------
// Constructors
---------------------------------------------------------------------------*/
#pragma region Constructors  

Sound::Sound(void) : ID(0),
                    pChannel(NULL), 
                    pChannelGroup(NULL), 
                    _volume(1.0f),
                    pFMODSoundSpace(NULL)

{
_volume        = 0.0f;
_paused        = 0.0f;
_pitch         = 0.0f;
_volValue      = 0.0f;
_fadeValue     = 0.0f;
_fadeValA1     = 0.0f;
_fadeValA2     = 0.0f;
_fadeValA3     = 0.0f;
_fadeValB1     = 0.0f;
_fadeValB2     = 0.0f;
_fadeValB3     = 0.0f;
_centerValA    = 0.0f;
_centerValB    = 0.0f;
_bandwidthValA = 0.0f;
_bandwidthValB = 0.0f;
_gainValA      = 0.0f;
_gainValB      = 0.0f;
_CutOffCounter = 0.0f;
_BandwidthCounter = 1.0f;
_position.x = 0.0f;
_position.y = 0.0f;
_position.z = 0.0f;
}

#pragma endregion

/*---------------------------------------------------------------------------
// Public Variables
---------------------------------------------------------------------------*/
#pragma region Global Variables

bool Sound::system_on_ = true;

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

/***************************************************************************/
/*!
\brief  Sets a channel group to the Sound Object

\param channelGroup
Takes in a pointer to an FMOD channel group

\return Returns nothing
*/
/***************************************************************************/
void Sound::SetChannelGroup(FMOD::ChannelGroup* channelGroup)
{
// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return; 
}

pChannelGroup = channelGroup;
}

/***************************************************************************/
/*!
\brief  Checks for FMOD related errors

\param  FMOD_RESULT
Type of error

\return Returns nothing
*/
/***************************************************************************/
void Sound::ErrCheck(FMOD_RESULT result)
{
if (result != FMOD_OK)
{
    char buffer[1000];

    sprintf(buffer, "FMOD error! (%d) %s\n",
    result, FMOD_ErrorString(result));

    // Prints the buffer to Visual Studio's output window
    OutputDebugStringA(buffer);
}
}

/***************************************************************************/
/*!
\brief  Sets the volume to the Sound Object

\param  volume
The float value of the volume

\return Returns nothing
*/
/***************************************************************************/
void Sound::SetVolume(const float volume)
{
// Checks if system is not on
if (Sound::system_on_ == false)
{
    return; 
}

_volume = volume;

if (pChannel)
{
    pChannel->setVolume(volume);
}
}

/***************************************************************************/
/*!
\brief  Pans the sound source in a stereo field

\param  pan
Pan position

\return Returns nothing
*/
/***************************************************************************/
void Sound::SetPan(char pan)
{
FMOD_RESULT result;
// Pans to left channel
if (pan == 'L')
{
    result = pChannel->setPan(-1);
    ErrCheck(result);
}

// Pans to Right channel
if (pan == 'R')
{
    result = pChannel->setPan(1);
    ErrCheck(result);
}

// Pans to center
if (pan == 'C')
{
    result = pChannel->setPan(0);
    ErrCheck(result);
}
}

/***************************************************************************/
/*!
\brief  Sets the pause state of the sound object

\param  pauseState
If needs to be paused or unpaused

\return Returns nothing
*/
/***************************************************************************/
void Sound::SetPause(bool pauseState)
{
FMOD_RESULT result;
// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return; 
}

// If the channel exists
if (pChannel)
{
    if (pauseState == true)
    {
    result = pChannel->setPaused(true);
    ErrCheck(result);
    _paused = true;
    }
    else if (pauseState == false)
    {
    result = pChannel->setPaused(false);
    ErrCheck(result);
    _paused = false;
    }
      
}
}

/***************************************************************************/
/*!
\brief  Stops the sound

\return Returns nothing
*/
/***************************************************************************/
void Sound::Stop()
{
FMOD_RESULT result;

// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return; 
}

// If the channel exists
if (pChannel)
{
    bool playing = false;

    result = pChannel->isPlaying(&playing); //CRASHLOC invalid handle for pChannel
    ErrCheck(result);

    if (playing)
    {
    result = pChannel->stop();
    ErrCheck(result);
    }
      
    pChannel = nullptr;
}
}

/***************************************************************************/
/*!
\brief  Mutes the sound

\param  muteState
If needs to be muted or unmuted

\return Returns nothing
*/
/***************************************************************************/
void Sound::SetMute(bool muteState)
{
FMOD_RESULT result;

// Checks if system is not on
if (Sound::system_on_ == false)
{
    return;
}

// If the channel exists
if (pChannel)
{
    if (muteState == true)
    {
    result = pChannel->setMute(muteState);
    ErrCheck(result);
    }
    else if (muteState == false)
    {
    result = pChannel->setMute(muteState);
    ErrCheck(result);
    }
}    
}

/***************************************************************************/
/*!
\brief  Sets the loop state of the Sound Object

\param  loopState
Bool to state if we want to have looping

\param index
The index number of the sound we want to set to loop

\return Returns nothing
*/
/***************************************************************************/
void Sound::SetLoop(const bool loopState, int index)
{
FMOD_RESULT result;

// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return; 
}

// Adds the sound object to the sound queue
FMOD::Sound* pSound = sound_queue_[index];

if (loopState == true)
{
    result = pSound->setMode(FMOD_LOOP_NORMAL); // Sets Loop On
    ErrCheck(result);
}
else
{
    result = pSound->setMode(FMOD_LOOP_OFF);    // Sets Loop Off
    ErrCheck(result);
}
}

/***************************************************************************/
/*!
\brief  Sets the loop points of the sound object

\param  startPos
Start of the loop

\param endPos
End of the loop

\return Returns nothing
*/
/***************************************************************************/
void Sound::SetLoopPoints(const float startPos, 
                        const float endPos, 
                        const int index)
{
FMOD_RESULT result;

// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return; 
}

FMOD::Sound* pSound = sound_queue_[index];

unsigned fmStartPos = static_cast<unsigned>(startPos * 1000);
unsigned fmEndPos = static_cast<unsigned>(endPos * 1000);

if (!fmEndPos)
{ 
    result = pSound->getLength(&fmEndPos, FMOD_TIMEUNIT_MS); 
    ErrCheck(result);
}

result = pSound->setLoopPoints(fmStartPos, FMOD_TIMEUNIT_MS, fmEndPos,
    FMOD_TIMEUNIT_MS);
ErrCheck(result);
}
/***************************************************************************/
/*!
\brief  Sets the mode of the Sound Object

\param  type
Type of Sound

\param index
The index number of the sound we want to assign the type to

\return Returns nothing
*/
/***************************************************************************/
void Sound::SetType(unsigned type, unsigned index)
{
FMOD_RESULT result;

// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return; 
}

// Checks if the index is larger than the size of the container
if (sound_queue_.size() < index + 1)
    return;

// Assigns the sound object to the sound queue
FMOD::Sound* pSound = sound_queue_[index];

// Sets mode of the system if it has to be 2D or 3D
switch (type)
{
case SOUND_2D:
    result = pSound->setMode(FMOD_2D);
    ErrCheck(result);
    SetLoop(false, index);
    break;
case SOUND_3D:
    result = pSound->setMode(FMOD_3D);
    ErrCheck(result);
    result = pFMODSoundSpace->set3DSettings(1.0f, float(DISTANCE_FACTOR), 1.0f);
    ErrCheck(result);
    result = pSound->set3DMinMaxDistance(1.5f * float(DISTANCE_FACTOR), 10000.0f * float(DISTANCE_FACTOR));
    ErrCheck(result);
    SetLoop(true, index);
    break;
case MUSIC:
    result = pSound->setMode(FMOD_2D);
    ErrCheck(result);
    SetLoop(true, index);
    break;
}
}

void Sound::SetPosition(Real3 pos)
{
if (Sound::system_on_ == false)
{ 
    return; 
}
  
_position.x = pos.x * float(DISTANCE_FACTOR);
_position.y = pos.y * float(DISTANCE_FACTOR);
_position.z = pos.z * float(DISTANCE_FACTOR);
}

/***************************************************************************/
/*!
\brief  Adds the sound to the FMOD Audio System

\param  filename
Name of the audio file

\return Returns nothing
*/
/***************************************************************************/
void Sound::AddSound(const String& filename)
{
// Checks if system is not on
if (Sound::system_on_ == false)
{
    return; 
}

FMOD::Sound* pSound;

// FMOD error check to check if sounds are being created in the system
ErrCheck(pFMODSoundSpace->createSound(filename.c_str(), FMOD_DEFAULT, 0, &pSound));

// Piles up the sound on the stack
sound_queue_.push_back(pSound);

unsigned index = sound_queue_.size() - 1;

// Sets the sound object ID and index
SetType(ID, index);
}

/***************************************************************************/
/*!
\brief  Gets the playing state of a sound

\return Returns a bool stating if the song is playing or not
*/
/***************************************************************************/
bool Sound::GetPlaying()
{
// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return false; 
}

bool playing = false; // If not playing

// If channel exists
if (pChannel)
{
    pChannel->isPlaying(&playing); // Check if channel is playing
}

return playing;
}

/***************************************************************************/
/*!
\brief  Gets the paused state of the Sound Object

\return Returns the paused state 
*/
/***************************************************************************/
bool Sound::GetPaused()
{
// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return false; 
}

bool paused = false; // If the channel is paused

// If the channel exists
if (pChannel)
{
    pChannel->getPaused(&paused); // Checks the paused state
}

return paused;
}

/***************************************************************************/
/*!
\brief  Plays the loaded sound

\return Returns nothing
*/
/***************************************************************************/
void Sound::Play()
{
FMOD_RESULT result;

// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return; 
}

// If the sound is already playing then return

if (ID != SOUND_2D)
{
    if (GetPlaying())
    {
    return;
    }
}    

// If the sound is paused
if (GetPaused())
{
    if (pChannel) // If the channel exists
    {
    result = pChannel->setPaused(false); // Unpause
    ErrCheck(result);
    }
    return;
}

// Play new sound
PlayNew();
//VolumeFade(1, 0);
}

/***************************************************************************/
/*!
\brief  Plays the sound Object that is loaded

\return Returns nothing
*/
/***************************************************************************/
void Sound::PlayNew()
{
FMOD_RESULT result;

if (Sound::system_on_ == false)
{
    return; 
}

FMOD::Sound* pSound = sound_queue_[0]; // Adds the new sound object

// Calls the FMOD playSound function
result = pFMODSoundSpace->playSound(pSound, 0, false, &pChannel);
ErrCheck(result);

// If the channel group exists
if (pChannelGroup)
{ 
    result = pChannel->setChannelGroup(pChannelGroup); // Add channel to the group
    ErrCheck(result);
}
    
_paused = false; // Set paused state to false

// Sets the default frequencies
SetFrequency1();
SetFrequency2();
}

/***************************************************************************/
/*!
\brief  Fades the volume in or out

\param  volume
The volume that needs to be faded into 

\param fadeTime
The amount of time need to reach the required volume

\return Returns nothing
*/
/***************************************************************************/
void Sound::VolumeFade(float volume, float fadeTime)
{
float currentVolume;
float fadeSpeed;

_volValue = volume;

currentVolume = GetVolume();

// Speed = distance/time
fadeSpeed = (volume - currentVolume) / fadeTime;
_fadeValue = fadeSpeed;
}

/***************************************************************************/
/*!
\brief  Gets the master volume

\return Returns a float with the volume value
*/
/***************************************************************************/
float Sound::GetMasterVolume(void)
{
FMOD_RESULT result;

// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return 0; 
}

float volume = false;

if (pChannelGroup)
{
    result = pChannelGroup->getVolume(&volume); // Gets the master volume
    ErrCheck(result);
}
return volume;
}

/***************************************************************************/
/*!
\brief  Gets the volume level

\return Returns a float with the volume level
*/
/***************************************************************************/
float Sound::GetVolume()
{
// Checks if system is not on
if (Sound::system_on_ == false)
{ 
    return 0; 
}

return _volume;
}

/***************************************************************************/
/*!
\brief  A getter for the volume

\return Returns a pointer to _volume
*/
/***************************************************************************/
float* Sound::GetVolumePtr ()
{
// Checks if system is not on
if (Sound::system_on_ == false)
{
    return 0;
}

return &_volume;
}

/***************************************************************************/
/*!
\brief  Gets the current time position of the sound

\return Returns the time position in milliseconds
*/
/***************************************************************************/
unsigned Sound::GetTime()
{
if (Sound::system_on_ == false)
{ 
    return 0; 
}

unsigned milliseconds = 0;

if (pChannel)
{
    pChannel->getPosition(&milliseconds, FMOD_TIMEUNIT_MS);
}   

return milliseconds;
}

/***************************************************************************/
/*!
\brief  Loads the sound

\param  pSystem
The FMOD audio system

\param filename
Name of the file

\param soundName
Name of the sound

\param id
Id attached to the sound object

\return Returns nothing
*/
/***************************************************************************/
void Sound::Load(FMOD::System* pSystem, const String& filename, const String& soundName, SoundID id)
{
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
        return; 
    }

    pFMODSoundSpace = pSystem;
    SoundName = soundName;
    ID = id;

    AddSound(filename);
}

/***************************************************************************/
/*!
\brief  Updates Instances

\param  dt
Frametiem of the engine

\return Returns nothing
*/
/***************************************************************************/
void Sound::Update(const float &dt)
{
FMOD_RESULT result;

// Checks if system is not on
if (Sound::system_on_ == false)
{
    return;
}    

if (pChannel)
{
    if (ID == SOUND_3D)
    {
    if (pChannel)
    {
        result = pChannel->set3DAttributes(&_position, 0);
        ErrCheck(result);
    }
    }

    // Update Functions
    UpdateVolumeFade(dt);
    UpdateFrequency1(dt);
    UpdateFrequency2(dt);
    UpdateNoise();

    SetVolume(GetVolume());


    // FOR TESTING ONLY
    if (this->GetTime() > 5000 && this->GetTime() < 5500 && test == true)
    {
    //test = false;
    //std::cout << CinderConsole::cyan << "FIVE SECONDS" << std::endl;
    //this->SweepEQ1(100.0f, 5.5f, 10.0f, 4.0f);
    //this->SweepEQ2(5000.0f, 5.0f, 10.0f, 4.0f);
    //this->VolumeFade(0.0f, 5.0f);
    }
    if (this->GetTime() > 10000 && this->GetTime() < 10500 && test2 == true)
    {
    //test2 = false;
    //std::cout << CinderConsole::green<< "TEN SECONDS" << std::endl;
    //this->SweepEQ1(20.0f, 0.2f, -30.0f, 5.0f);
    //this->SweepEQ2(20.0f, 0.2f, -30.0f, 5.0f);
    }

    /*if (CORE->IsPaused() == true)
    {
    SoundSpace->SetPauseMenuEffect(500.0f, 2.5f, 0.5f);
    this->VolumeFade(0.1f, 0.5f);
    }
    else
    {
    SoundSpace->SetPauseMenuEffect(22000.0f, 0.0f, 4.0f);
    this->VolumeFade(1.0f, 1.0f);
    }*/
}    
}

/***************************************************************************/
/*!
\brief  Updates the volume fade over time

\param  dt
The current time

\return Returns nothing
*/
/***************************************************************************/
void Sound::UpdateVolumeFade(const float& dt)
{
float currentVolume = 0;    

// If the channel exists
if (pChannel != NULL)
{
    pChannel->getVolume(&currentVolume);      

    // If volume doesn't reach the required fade volume
    if (_volValue != currentVolume)
    {
    float newVolume;

    newVolume = currentVolume + _fadeValue * dt;

    // Check bounds
    if (newVolume > _volValue && _fadeValue > 0.0f)
    {
        newVolume = _volValue;
    }
    else if (newVolume < _volValue && _fadeValue < 0.0f)
    {
        newVolume = _volValue;
    }

    // Set the volume to the new volume
    SetVolume(newVolume);
    }
}  
}  

void Sound::micEffectUpdate()
{

    ////JOSH
    //if (SoundSpace->GetMicrophoneValue() > 0.05f)
    //{
    //    if (_micLPF < 22000)
    //    {
    //    _micLPF += SoundSpace->GetMicrophoneValue() * 200.0f;
    //    }
    //}
    //else
    //{
    //    if (_micLPF > 6000)
    //    {
    //    _micLPF -= 150.0f;
    //    }
    //}

    this->SetLPF(_micLPF, 1);
}
 
/*---------------------------------------------------------------------------
// Destructor
---------------------------------------------------------------------------*/
Sound::~Sound()
{
std::vector<FMOD::Sound*>::iterator it;

for (it = sound_queue_.begin(); it != sound_queue_.end(); ++it)
{
    ErrCheck((*it)->release());
}
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