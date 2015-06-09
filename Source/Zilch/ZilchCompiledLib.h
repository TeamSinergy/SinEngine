#pragma once
#include "Precompiled.h"
#define ScriptFilePath "../Assets/ZilchScripts"

ZilchStaticLibrary(SinningZilch);

class ZilchCompiledLib : public SinEntity
{
public:
    ZilchCompiledLib() {}
    ZilchCompiledLib(String systemName);
    void Serialize(DataNode* node);
    void Create() override;
    void Initialize() override;
    void Uninitialize() override;
    void Destroy() override;
    ~ZilchCompiledLib() {}

    //Returns pointer to the required LibraryRef
    LibraryRef* GetZilchLib(String& ScriptName);

    //Returns pointer to the dependency library
    ExecutableState* GetDependencies(){ return ExecState; }

    ExceptionReport Report;
    
    LibraryRef CompiledLib;

    /* This class encompasses all compilation errors that can occur when compiling
    Zilch code. Its responsibility is to provide friendly error messages,
    error codes, and callbacks to the user*/
    CompilationErrors Errors;

    Project CompiledProject;

    EventHandler::Global;

    HashMap<String, ComponentCreator> Components;
private:

    int ScriptCount;
    

    //Debugger Debugger;    /* Link all the libraries together into one ExecutableState*/
    ExecutableState* ExecState;



    //LoadAllZilchFiles into the static Library
    void LoadZilchFiles(Zilch::Project & project);
    //Compile all the scripts
    void CompileScripts(Zilch::Project& project, Zilch::Module& dependencies);

    //Unordered map of compiled Zilch libraries
    HashMap<String, Zilch::LibraryRef> LibList;

};

extern ZilchCompiledLib* ZILCH;

//ZilchDefines so we don't need to redo ALL the binding
#define DefineType(Type, Library) ZilchDefineType(##Library, ##Type, #Type, builder, Type)
#define BindConstructor() ZilchBindConstructor(builder, type, ZilchSelf, ZilchNoNames)
#define BindDestructor() ZilchBindDestructor(builder, type, ZilchSelf)
#define BindVirtualConstructor(...) ZilchBindConstructorVirtual(builder, type, ZilchSelf, ZilchNoNames, __VA_ARGS__)

#define BindMethod(Method) ZilchBindMethod(builder, type, &ZilchSelf::Method, ZilchNoOverload, #Method, ZilchNoNames)
#define BindMethodAs(Method, Name) ZilchBindMethod(builder, type, &ZilchSelf::Method, ZilchNoOverload, Name, ZilchNoNames)

#define BindMethodOverload(Method, ReturnType, ...) (ZilchBindMethod(builder, type, &ZilchSelf::Method, (ReturnType (ZilchSelf::*)(__VA_ARGS__)), #Method, ZilchNoNames))
#define BindMethodOverloadAs(Method, Name, ReturnType, ...) (ZilchBindMethod(builder, type, &ZilchSelf::Method, (ReturnType (ZilchSelf::*)(__VA_ARGS__)), Name, ZilchNoNames))

#define BindStaticMethod(Method) ZilchBindMethod(builder, type, &ZilchSelf::Method, ZilchNoOverload, #Method, ZilchNoNames)
#define BindStaticMethodAs(Method, Name) ZilchBindMethod(builder, type, &ZilchSelf::Method, ZilchNoOverload, Name, ZilchNoNames)

#define BindStaticMethodOverload(Method, ReturnType, ...) (ZilchBindMethod(builder, type, ZilchSelf::Method, (ReturnType (*)(__VA_ARGS__)), #Method, ZilchNoNames))
#define BindStaticMethodOverloadAs(Method, Name, ReturnType, ...) (ZilchBindMethod(builder, type, ZilchSelf::Method, (ReturnType (*)(__VA_ARGS__)), Name, ZilchNoNames))

#define BindFieldGet(Type) (ZilchBindField(builder, type, (&ZilchSelf::##Type), #Type, Zilch::PropertyBinding::Get))
#define BindFieldSet(Type) (ZilchBindField(builder, type, (&ZilchSelf::##Type), #Type, Zilch::PropertyBinding::Set))
#define BindFieldGetSet(Type) (ZilchBindField(builder, type, (&ZilchSelf::##Type), #Type, Zilch::PropertyBinding::GetSet))

#define BindFieldGetAs(Type, Name) (ZilchBindField(builder, type, (&ZilchSelf::##Type), Name, Zilch::PropertyBinding::Get))
#define BindFieldSetAs(Type, Name) (ZilchBindField(builder, type, (&ZilchSelf::##Type), Name, Zilch::PropertyBinding::Set))
#define BindFieldGetSetAs(Type, Name) (ZilchBindField(builder, type, (&ZilchSelf::##Type), Name, Zilch::PropertyBinding::GetSet))

#define BindStaticFieldGet(Type) (ZilchBindField(builder, type, (&ZilchSelf::Type), #Type, Zilch::PropertyBinding::Get))
#define BindStaticFieldSet(Type) (ZilchBindField(builder, type, (&ZilchSelf::Type), #Type, Zilch::PropertyBinding::Set))
#define BindStaticFieldGetSet(Type) (ZilchBindField(builder, type, (&ZilchSelf::Type), #Type, Zilch::PropertyBinding::GetSet))

#define BindStaticFieldGetAs(Type, Name) (ZilchBindField(builder, type, (&ZilchSelf::Type), Name, Zilch::PropertyBinding::Get))
#define BindStaticFieldSetAs(Type, Name) (ZilchBindField(builder, type, (&ZilchSelf::Type), Name, Zilch::PropertyBinding::Set))
#define BindStaticFieldGetSetAs(Type, Name) (ZilchBindField(builder, type, (&ZilchSelf::Type), Name, Zilch::PropertyBinding::GetSet))