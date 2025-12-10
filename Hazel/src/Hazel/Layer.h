#pragma once

#include "Core.h"
#include "Events/Event.h"
#include <string>

namespace Hazel {

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4251) // Disable warning about STL types in DLL interface
#endif

	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

}
