#pragma once

#include "../Core.h"
#include "ScriptEngine.h"
#include <memory>
#include <string>

namespace Hazel {

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4251) // Disable warning about STL types in DLL interface
#endif

	class HAZEL_API ScriptComponent
	{
	public:
		ScriptComponent() = default;
		ScriptComponent(const std::string& scriptClassName);

		void OnCreate();
		void OnUpdate(float deltaTime);
		void OnDestroy();

		const std::string& GetClassName() const { return m_ClassName; }
		void SetClassName(const std::string& className) { m_ClassName = className; }

		std::shared_ptr<ScriptInstance> GetInstance() { return m_Instance; }

	private:
		std::string m_ClassName;
		std::shared_ptr<ScriptInstance> m_Instance;
	};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

}
