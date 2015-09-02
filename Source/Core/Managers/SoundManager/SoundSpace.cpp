/*****************************************************************************/
/*!
\file   SoundSpace.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Creates the infrastructure for the Audio System
*/      
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include <Precompiled.h>

#pragma endregion

DefineType(SoundSpace, SinningZilch)
{
BindComponent(SoundSpace);
}
/*---------------------------------------------------------------------------
// Class Implementation
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
// Constructors
---------------------------------------------------------------------------*/
#pragma region Constructors

SoundSpace::SoundSpace() : pFMODSoundSpace(NULL),
GroupMusic(NULL),
Group2DSFX(NULL),
Group3DSFX(NULL)
{
#ifdef _DEBUG
//ErrorIf(SoundSpace != NULL, "SoundSpace Audio System already created");
#endif
    

#ifdef _DEBUG
ErrorIf(CreateFMODSystem() == false, "SoundSpace not created");
#else
CreateFMODSystem ();
#endif
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

/***************************************************************************/
/*!
\brief  Checks for FMOD related errors

\param  FMOD_RESULT
Type of error

\return Returns nothing
*/
/***************************************************************************/
void SoundSpace::ErrCheck(FMOD_RESULT result)
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
void SoundSpace::Serialize(DataNode*)
{

}
/***************************************************************************/
/*!
\brief  Initialize Function

\return Returns nothing
*/
/***************************************************************************/
void SoundSpace::Initialize()
{
    Owner->SoundSpace = this;
    // Calls function the creates FMOD groups
    EventConnect(Space, Events::LogicUpdate, &SoundSpace::Update, this);
    CreateSoundGroups();
    InitPauseMenuEffect();
    InitMicData();

    SinWriteLine("SoundSpace Initialized");
}

/***************************************************************************/
/*!
\brief  This fucntion loads a sound into the Engine

\param  filename
The name of the audio file

\param  soundName
The name assigned to the file we are loading

\param type
Assigning the type of sound we are loading

\param volume
Assign volume at which the sound should play

\return Returns a Sound class Object
*/
/***************************************************************************/
Sound* SoundSpace::LoadSound(const String& filename,
    const String& soundName,
Sound::SoundID type,
float volume)
{
    Sound* newSound = new Sound(); // Dynamically creates a new 'Sound' object
    

    

    // Calls the load method from the Sound Class
    newSound->Load(pFMODSoundSpace, String::Join("/", AudioAssetsPath, filename), soundName, type);

    // Statements to check which group to assign the loaded sound to
    if (type == Sound::MUSIC)
    {
        newSound->SetChannelGroup(GroupMusic);
    }
    else if (type == Sound::SOUND_2D)
    {
        newSound->SetChannelGroup(Group2DSFX);
    }
    else
    {
        newSound->SetChannelGroup(Group3DSFX);
    }

    // Assigns volume to the sound object
    newSound->SetVolume(volume);

    // Adds the sound object to the soundMap container
    soundMap[soundName] = (newSound);

    // Returns the newly created sound object
    return newSound;
}

void SoundSpace::AddAudioComponent(AudioComponent* component)
{
audioComponentList.push_back(component);
}

void SoundSpace::DeleteAudioComponent(AudioComponent* component)
{
audioComponentList.remove(component);
}

/***************************************************************************/
/*!
\brief  Stops the specific sounds we want to stop playing

\param  id
Specific sound type that we intend to stop playing

\return Returns nothing
*/
/***************************************************************************/
void SoundSpace::StopSounds(int id)
{
FMOD_RESULT result;

// Checks if audio system is not on
if (Sound::system_on_ == false)
{
    return;
}

// Stops all Music sounds
if (id == Sound::MUSIC || id == -1)
{
    result = GroupMusic->stop();
    ErrCheck(result);
}

// Stops all 2D sound effects
if (id == Sound::SOUND_2D
    || id == Sound::SFX_ALL || id == -1)
{
    result = Group2DSFX->stop();
    ErrCheck(result);
}

// Stops all 3D sound effects
if (id == Sound::SOUND_3D
    || id == Sound::SFX_ALL || id == -1)
{
    result = Group3DSFX->stop();
    ErrCheck(result);
}
}

/***************************************************************************/
/*!
\brief  Update Function

\param  dt
Takes in the Engine's frametime

\return Returns nothing
*/
/***************************************************************************/
void SoundSpace::Update(UpdateEvent* event)
{
FMOD_RESULT result;

// Checks if audio system is not on
if (Sound::system_on_ == false)
{
    return;
}

// Updates the audio system
result = pFMODSoundSpace->update();
ErrCheck(result);

//Update mic only if plugged in -CA
if (_recordnumdrivers)
{
    UpdateMicData();
}

UpdatePauseMenuEffect(event->Dt);

// Updates all the Sound Object Instances
std::map<String, Sound*>::iterator it;
for (it = soundMap.begin(); it != soundMap.end(); ++it)
{
    it->second->Update(event->Dt);
}

for (auto *i : audioComponentList)
{
    i->Update();
}
}

/***************************************************************************/
/*!
\brief  Get method to get the sound object

\param  soundName
Name of the sound

\return Returns the element stored in the soundMap
*/
/***************************************************************************/
Sound* SoundSpace::GetSound(char* soundName)
{
return soundMap[soundName];
}

/***************************************************************************/
/*!
\brief  Checks if sound groups are muted

\param  id
Specific sound type that we intend to check

\return Returns a bool stating which group is muted
*/
/***************************************************************************/
bool SoundSpace::GetMuted(int id)
{
FMOD_RESULT result;

// Checks if audio system is not on
if (Sound::system_on_ == false)
{
    return true;
}

if (soundMap.size() == 0)
{
    return 0.0f;
}

bool sfx2DMuted = 0;
bool sfx3DMuted = 0;
bool musicMuted = 0;

// FMOD getters for checking for Mute
result = GroupMusic->getMute(&sfx2DMuted);
ErrCheck(result);

result = Group2DSFX->getMute(&sfx3DMuted);
ErrCheck(result);
    
result = Group3DSFX->getMute(&musicMuted);
ErrCheck(result);

// Statements to check which groups have been muted
if (id == -1)
{
    return musicMuted && sfx2DMuted && sfx3DMuted;
}
if (id == Sound::MUSIC)
{
    return musicMuted;
}
if (id == Sound::SOUND_3D)
{
    return sfx3DMuted;
}
if (id == Sound::SOUND_2D)
{
    return sfx2DMuted;
}
else
{
    return sfx2DMuted && sfx3DMuted;
}
}

/***************************************************************************/
/*!
\brief  Pauses the sounds at its current state of playing

\param  paused
Bool to state if paused or not

\param id
Specific sound type that we intend to pause

\return Returns nothing
*/
/***************************************************************************/
void SoundSpace::SetPaused(bool paused, int id)
{
FMOD_RESULT result;
// Checks if audio system is not on
if (Sound::system_on_ == false)
{
    return;
}

// Pauses all Music sounds
if (id == Sound::MUSIC
    || id == Sound::SFX_ALL || id == -1)
{
    result = GroupMusic->setPaused(paused);
    ErrCheck(result);
}

// Pauses all 2D sound effects
if (id == Sound::SOUND_2D
    || id == Sound::SFX_ALL || id == -1)
{
    result = Group2DSFX->setPaused(paused);
    ErrCheck(result);
}

// Pauses all 3D sound effects
if (id == Sound::SOUND_3D
    || id == Sound::SFX_ALL || id == -1)
{
    result = Group3DSFX->setPaused(paused);
    ErrCheck(result);
}
}

void SoundSpace::InitPauseMenuEffect()
{
FMOD_RESULT result;

result = pFMODSoundSpace->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &lowPassEffect);
ErrCheck(result);

result = lowPassEffect->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 22000.0f);
ErrCheck(result);

result = lowPassEffect->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, 0.0f);
ErrCheck(result);
}

void SoundSpace::SetPauseMenuEffect(float cutoff, float resonance, float time)
{
FMOD_RESULT result;
FMOD::ChannelGroup *masterGroup = 0;
float currentCutoff;
float currentResonance;
float fadeSpeedA;
float fadeSpeedB;
bool active;
char buffer[16];

result = lowPassEffect->getActive(&active);
ErrCheck(result);

result = pFMODSoundSpace->getMasterChannelGroup(&masterGroup);
ErrCheck(result);

if (!active && _lpfstate == false)
{
    result = masterGroup->addDSP(0, lowPassEffect);
    ErrCheck(result);

    result = lowPassEffect->setActive(true);
    ErrCheck(result);

    _lpfstate = true;

}

_cutoffVal = cutoff;
_resonanceVal = resonance;

result = lowPassEffect->getParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, &currentCutoff, buffer, 16);
ErrCheck(result);

result = lowPassEffect->getParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, &currentResonance, buffer, 16);
ErrCheck(result);

fadeSpeedA = (cutoff - currentCutoff) / time;
_fadeValA = fadeSpeedA;

fadeSpeedB = (resonance - currentResonance) / time;
_fadeValB = fadeSpeedB;    
}

void SoundSpace::UpdatePauseMenuEffect(const float& dt)
{
FMOD_RESULT result;
float currentCutoff;
float currentResonance;
char buffer[16];
bool active;
FMOD::ChannelGroup *masterGroup = 0;

result = pFMODSoundSpace->getMasterChannelGroup(&masterGroup);
ErrCheck(result);

if (lowPassEffect)
{
    result = lowPassEffect->getParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, &currentCutoff, buffer, 16);
    ErrCheck(result);

    if (currentCutoff == 10)
    _cutoffVal = 22000;

    result = lowPassEffect->getParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, &currentResonance, buffer, 16);
    ErrCheck(result);

    result = lowPassEffect->getActive(&active);
    ErrCheck(result);

    if (currentCutoff > 22000 && _lpfstate == true && active)
    {
    result = masterGroup->removeDSP(lowPassEffect);
    ErrCheck(result);

    result = lowPassEffect->setActive(false);
    ErrCheck(result);

    _lpfstate = false;
    }

    if (_cutoffVal != currentCutoff)
    {
    float newParam;
    newParam = currentCutoff + (_fadeValA * dt);

    if (newParam > _cutoffVal && _fadeValA > 0.0f)
    {
        newParam = _cutoffVal;
    }
    else if (newParam < _cutoffVal && _fadeValA < 0.0f)
    {
        newParam = _cutoffVal;
    }

    result = lowPassEffect->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, newParam);
    ErrCheck(result);
    }

    if (_resonanceVal != currentResonance)
    {
    float newParam;
    newParam = currentResonance + (_fadeValB * dt);

    if (newParam > _resonanceVal && _fadeValB > 0.0f)
    {
        newParam = _resonanceVal;
    }
    else if (newParam < _resonanceVal && _fadeValB < 0.0f)
    {
        newParam = _resonanceVal;
    }

    result = lowPassEffect->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, newParam);
    ErrCheck(result);
    }
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

SoundSpace :: ~SoundSpace ()
{
    FMOD_RESULT result;

    // Deletes all the sounds objects
    std::map<String, Sound*>::iterator it;
    for (it = soundMap.begin(); it != soundMap.end(); ++it)
    {
        delete it->second;
    }

    // Prints out when audio system gets destroyed
    SinWriteLine(String::Join("", Name, "Destroyed"));

    // Closes down the FMOD system
    result = pFMODSoundSpace->close();
    ErrCheck(result);


    // Releases the FMOD system
    result = pFMODSoundSpace->release();
    ErrCheck(result);

    Owner->SoundSpace = nullptr;
    pFMODSoundSpace = NULL;
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

/***************************************************************************/
/*!
\brief  Creates Sound Groups to organize sounds of different types

\return Returns nothing
*/
/***************************************************************************/
void SoundSpace::CreateSoundGroups()
{
    FMOD_RESULT result;

    // Creates group that will be holding all elements of Music
    result = pFMODSoundSpace->createChannelGroup("Music", &GroupMusic);
    ErrCheck(result);

    // Creates group that will be holding elements of non-spatial sounds
    result = pFMODSoundSpace->createChannelGroup("2DSFX", &Group2DSFX);
    ErrCheck(result);

    // Creates group that will be holding lements of spatial sounds
    result = pFMODSoundSpace->createChannelGroup("3DSFX", &Group3DSFX);
    ErrCheck(result);
}

/***************************************************************************/
/*!
\brief  Creates the FMOD system

\return Returns a bool stating if the Audio System was created or not
*/
/***************************************************************************/
bool SoundSpace::CreateFMODSystem()
{
    FMOD_RESULT result;

    // Creates the FMOD audio system
    result = FMOD::System_Create(&pFMODSoundSpace);
    if (result != FMOD_OK)
    {
        Error("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return false;
    }

    // FMOD's init functions. Sets channels and default modes
    result = pFMODSoundSpace->init(512, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        Error("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return false;
    }

    // If system was sucessfully created
    return true;
}

#pragma endregion


/*---------------------------------------------------------------------------
// Static Functions
---------------------------------------------------------------------------*/
#pragma region Static Functions

#pragma endregion


//-----------------------------------------------------------------------------
