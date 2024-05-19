#pragma once

namespace T1WD {
#ifndef GameObject
	class GameObject;
#endif
	class Component {
	public:
		Component();
		~Component();

		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void Draw() {}
		virtual void LateDraw() {}
		virtual void CleanUp() {}

		GameObject* gameobject = nullptr;
		const char* name = nullptr;
	};
}