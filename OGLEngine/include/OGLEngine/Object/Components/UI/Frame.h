#pragma once

#include "UI.h"

namespace T1WD {
	class Frame : public UI {
	public:
		Frame();
		~Frame();

		void Awake() override;

		void Draw() override;
	};
}