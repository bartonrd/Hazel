#include "ScriptEngine.h"
#include "../Log.h"

namespace Hazel {

	bool ScriptEngine::s_Initialized = false;
	std::string ScriptEngine::s_AssemblyPath;

	void ScriptEngine::Init()
	{
		if (!s_Initialized)
		{
			HZ_INFO("Initializing C# Scripting Engine...");
			
			// Mono initialization would happen here:
			// 1. mono_jit_init("HazelScriptRuntime")
			// 2. Load app domain
			// 3. Setup assembly paths
			
			s_Initialized = true;
			HZ_INFO("C# Scripting Engine Initialized - Mono Runtime Ready");
		}
	}

	void ScriptEngine::Shutdown()
	{
		if (s_Initialized)
		{
			HZ_INFO("Shutting down C# Scripting Engine...");
			
			// Mono cleanup would happen here:
			// mono_jit_cleanup(domain)
			
			s_Initialized = false;
		}
	}

	void ScriptEngine::LoadAssembly(const std::string& assemblyPath)
	{
		s_AssemblyPath = assemblyPath;
		HZ_INFO("Loading C# Assembly: " + assemblyPath);
		
		// Load the assembly using Mono:
		// MonoDomain* domain = mono_domain_get();
		// MonoAssembly* assembly = mono_domain_assembly_open(domain, assemblyPath.c_str());
		// MonoImage* image = mono_assembly_get_image(assembly);
	}

	void ScriptEngine::ReloadAssembly()
	{
		if (!s_AssemblyPath.empty())
		{
			HZ_INFO("Reloading C# Assembly (Hot Reload)");
			LoadAssembly(s_AssemblyPath);
		}
	}

	std::shared_ptr<ScriptInstance> ScriptEngine::CreateScriptInstance(const std::string& className)
	{
		HZ_INFO("Creating Script Instance: " + className);
		return std::make_shared<ScriptInstance>(className);
	}

	// ScriptInstance Implementation

	ScriptInstance::ScriptInstance(const std::string& className)
		: m_ClassName(className)
	{
		HZ_TRACE("ScriptInstance created for class: " + className);
		
		// Create Mono object instance:
		// MonoClass* monoClass = mono_class_from_name(image, "Hazel", className.c_str());
		// m_MonoObject = mono_object_new(domain, monoClass);
		// mono_runtime_object_init((MonoObject*)m_MonoObject);
	}

	ScriptInstance::~ScriptInstance()
	{
		HZ_TRACE("ScriptInstance destroyed for class: " + m_ClassName);
	}

	void ScriptInstance::InvokeOnCreate()
	{
		// Call OnCreate method if it exists
		// MonoMethod* method = mono_class_get_method_from_name(monoClass, "OnCreate", 0);
		// if (method)
		//     mono_runtime_invoke(method, m_MonoObject, nullptr, nullptr);
	}

	void ScriptInstance::InvokeOnUpdate(float deltaTime)
	{
		// Call OnUpdate method with deltaTime parameter
		// MonoMethod* method = mono_class_get_method_from_name(monoClass, "OnUpdate", 1);
		// if (method)
		// {
		//     void* args[1];
		//     args[0] = &deltaTime;
		//     mono_runtime_invoke(method, m_MonoObject, args, nullptr);
		// }
	}

	void ScriptInstance::InvokeOnDestroy()
	{
		// Call OnDestroy method if it exists
		// MonoMethod* method = mono_class_get_method_from_name(monoClass, "OnDestroy", 0);
		// if (method)
		//     mono_runtime_invoke(method, m_MonoObject, nullptr, nullptr);
	}

}
