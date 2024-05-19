#pragma once

#include <OGLEngine/Object/GameObject.h>
#include <OGLEngine/Object/Component/Component.h>

class PlayerController : public T1WD::Component{
public:
	PlayerController();
	~PlayerController();

	void Awake() override;
	void CleanUp() override;

	void Draw() override;
};