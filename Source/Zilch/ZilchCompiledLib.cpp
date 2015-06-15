#include "Precompiled.h"
#include "ZilchCompiledLib.h"
#include "Serializer.h"
#include <String.h>
#include "ZilchComponent.h"
ZilchCompiledLib* ZILCH = NULL;

ZilchCompiledLib::ZilchCompiledLib(String systemName) : SinEntity(systemName)
{
    ZILCH = this;
    LibList = HashMap<String, Zilch::LibraryRef >();
}
void ZilchCompiledLib::Serialize(DataNode* node)
{
    //Never going to be called?
}
void ZilchCompiledLib::Create() 
{
    
    //Debugger.Host(8000); 

    // Setup the console so that when we call 'Console.WriteLine' it outputs to stdio
    Zilch::EventConnect(&Console::Events, Events::ConsoleWrite, DefaultWriteText);

    // We can also setup the console so that any 'Read' functions will attempt to read from stdin
    Zilch::EventConnect(&Console::Events, Events::ConsoleRead, DefaultReadText);

    // This class encompasses all compilation errors that can occur when compiling Zilch code
    // Its responsibility is to provide friendly error messages, error codes, and callbacks to the user
    //CompilationErrors errors; //this is a public member of Zilch system

    // Here, we can register our own callback for when compilation errors occur
    // The default callback prints the file, line/character number, and message to stderr
    Zilch::EventConnect(&CompiledProject, Events::CompilationError, Zilch::DefaultErrorCallback);

    
}
void ZilchCompiledLib::Initialize() 
{
    SinSole::SetTextColor(ConsoleColors::DarkGreen);
    std::cout << "========= Initializing Zilch ==========" << std::endl;

    LoadZilchFiles(CompiledProject);

    // Create a list of dependent libraries, in our case we're really not adding anything to this
    // A side note: the Core library in Zilch is always added as a dependency, because Core includes
    // things like Integer, Boolean, Real, the basic vector types, String, etc
    Module dependencies;
    dependencies.push_back(SinningZilch::GetInstance().GetLibrary());
    CompileScripts(CompiledProject, dependencies);

    ExecState = dependencies.Link();
    ErrorIf(ExecState == nullptr, "Failed to link libraries together");
    Zilch::EventConnect(ExecState, Events::UnhandledException, Zilch::DefaultExceptionCallback);
    
    //Debugger.AddProject(&CompiledProject);
    //Debugger.AddState(ExecState);
    

    GameObject::SerializeFunction = new Array<Type*>();
    GameObject::SerializeFunction->push_back(ZilchTypeId(DataNode*));
    GameObject::Default = new Array<Type*>();
    GameObject::EmptyEventData = new EventData();
    /////////////////////////////////////////////////////////////////
    std::cout << "Zilch Initialized" << std::endl;
}
void ZilchCompiledLib::Uninitialize()
{

}
void ZilchCompiledLib::Destroy() 
{
    
    delete GameObject::SerializeFunction;
    delete GameObject::Default;
    delete GameObject::EmptyEventData;

    delete ExecState;
    ExecState = nullptr;
    
}

void ZilchCompiledLib::LoadZilchFiles(Zilch::Project & project)
{
    std::cout << "Loading Scripts \n";

    String filepath;
    Array<String> files;
    Serializer::FindAllFilesInFolder("../Assets", ".z", files, true);
    for (auto i : files)
    {
        filepath = String::Join("/", ScriptFilePath, i);
        std::cout << "Loading: " << ConsoleColors::Yellow << i.c_str() << ConsoleColors::DarkGreen << std::endl;
        project.AddCodeFromFile(filepath);
    }
}

namespace ZilchHandler
{
    void GetComponent(Call& call, ExceptionReport& report)
    {
        GameObject* gameObject = (GameObject*)call.GetHandle(Call::This).Dereference();

        BoundType* componentType = (BoundType*)call.GetFunction()->UserData;

        Component* comp = gameObject->FindComponentByName(componentType->Name);

        call.DisableReturnChecks();
        call.Set(Call::Return, comp);
    }

    void TypeParsedCallback(ParseEvent* event)
    {
        if (TypeBinding::IsA(event->Type, ZilchTypeId(ZilchComponent)))
        {
            Zilch::Property* prop = event->Builder->AddExtensionProperty(ZilchTypeId(GameObject), event->Type->Name, event->Type, NULL, ZilchHandler::GetComponent, MemberOptions::None);
            prop->Get->UserData = event->Type;
        }
    }
}

void ZilchCompiledLib::CompileScripts(Zilch::Project& project, Zilch::Module& dependencies)
{
    std::cout << "Compiling Scripts " << std::endl;
    //This will loop through all the scripts//
    // The 'Compile' function actually returns a Zilch Library
    // Be careful! If the code fails to compile, this function will return null
    //not sure what "Test" is?!?

    //Adds the callback to get ZilchCOmponents from just saying .comp
    EventConnect(&project, Events::TypeParsed, ZilchHandler::TypeParsedCallback);

    CompiledLib = project.Compile("LibraryOfSin", dependencies, EvaluationMode::Project);
    
    ErrorIf(CompiledLib == nullptr, "Failed to compiler library");

    // We want to link together all the libraries that we depended upon, along with our own library
    dependencies.push_back(CompiledLib);
}

Zilch::LibraryRef* ZilchCompiledLib::GetZilchLib(String& ScriptName)
{
    return &LibList[ScriptName];
}

String StdStringToZString(const std::string& input)
{
    return String(input.c_str(), input.size());
}
std::string ZStringToStdString(const String& input)
{
    return std::string(input.c_str(), input.size());
}

ZilchDefineRedirectType(std::string, StdStringToZString, ZStringToStdString);
//ZilchDefineImplicitRedirectType(std::string);

//

