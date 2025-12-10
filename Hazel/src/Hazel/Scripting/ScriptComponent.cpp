#include "ScriptComponent.h"
#include "../Log.h"

namespace Hazel {

	ScriptComponent::ScriptComponent(const std::string& scriptClassName)
		: m_ClassName(scriptClassName)
	{
		if (ScriptEngine::IsInitialized())
		{
			m_Instance = ScriptEngine::CreateScriptInstance(m_ClassName);
		}
	}

	void ScriptComponent::OnCreate()
	{
		if (m_Instance)
		{
			m_Instance->InvokeOnCreate();
		}
	}

	void ScriptComponent::OnUpdate(float deltaTime)
	{
		if (m_Instance)
		{
			m_Instance->InvokeOnUpdate(deltaTime);
		}
	}

	void ScriptComponent::OnDestroy()
	{
		if (m_Instance)
		{
			m_Instance->InvokeOnDestroy();
		}
	}

}
