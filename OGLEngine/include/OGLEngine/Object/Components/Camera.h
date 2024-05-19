#pragma once

#include "../Component/Component.h"

namespace T1WD {
	class Camera : public Component {
	public:
		Camera();
		~Camera();

		void Awake() override;

		void Update() override;
	};
}