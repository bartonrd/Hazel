#pragma once

#include "../Core.h"
#include <string>
#include <memory>

namespace Hazel {

	// Forward declarations
	class ScriptInstance;

	class HAZEL_API ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::string& assemblyPath);
		static void ReloadAssembly();

		static std::shared_ptr<ScriptInstance> CreateScriptInstance(const std::string& className);

		static bool IsInitialized() { return s_Initialized; }

	private:
		static bool s_Initialized;
		static std::string s_AssemblyPath;
	};

	class HAZEL_API ScriptInstance
	{
	public:
		ScriptInstance(const std::string& className);
		~ScriptInstance();

		void InvokeOnCreate();
		void InvokeOnUpdate(float deltaTime);
		void InvokeOnDestroy();

		template<typename T>
		T GetFieldValue(const std::string& fieldName)
		{
			// This would use Mono runtime to get field value
			return T();
		}

		template<typename T>
		void SetFieldValue(const std::string& fieldName, T value)
		{
			// This would use Mono runtime to set field value
		}

	private:
		std::string m_ClassName;
		void* m_MonoObject = nullptr;  // Would be MonoObject* with Mono
	};

}
