#ifndef ComponentIncluded
#define ComponentIncluded
#include "GameObject.h"

#include <memory>

namespace D3D
{
	class TransformComponent;

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void OnSceneLoad() {}

		virtual void OnSceneUnload() {}

		virtual void Update() {}

		virtual void FixedUpdate() {}

		virtual void LateUpdate() {}

		virtual void Render() {}

		virtual void OnGUI() {}

		virtual void Destroy() { m_ShouldDestroy = true; }
		bool ShouldDestroy() { return m_ShouldDestroy; }

		template <class T>
		friend std::shared_ptr<T> GameObject::AddComponent();

		template <class T>
		std::shared_ptr<T> GetComponent() const;

		template <class T>
		bool HasComponent() const;

		GameObject* GetOwner() const { return m_pOwner; }

		std::shared_ptr<TransformComponent> GetTransform() { return m_pOwner->GetTransform(); }

		bool IsActive() const { return m_IsActive; }
		void SetActive(bool isActive) { m_IsActive = isActive; }

	private:
		bool m_IsActive{ true };

		GameObject* m_pOwner{};

		void SetOwner(GameObject* pOwner) { m_pOwner = pOwner; }

		bool m_ShouldDestroy{ false };
	};

	template <class T>
	inline std::shared_ptr<T> Component::GetComponent() const
	{
		if (!std::is_base_of<Component, T>())
			return false;

		if (m_pOwner == nullptr)
			return false;

		return m_pOwner->GetComponent<T>();
	}

	template <class T>
	inline bool Component::HasComponent() const
	{
		if (!std::is_base_of<Component, T>())
			return false;

		if (m_pOwner == nullptr)
			return false;

		return m_pOwner->HasComponent<T>();
	}
}
#endif // !ComponentIncluded