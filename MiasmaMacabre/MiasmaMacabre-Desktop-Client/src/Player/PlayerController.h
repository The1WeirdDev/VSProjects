#pragma once

#include <OGLEngine/Object/GameObject.h>
#include <OGLEngine/Object/Component/Component.h>
#include <OGLEngine/Object/Components/Camera.h>

class PlayerController : public T1WD::Component{
public:
	PlayerController();
	~PlayerController();

	void Awake() override;
	void CleanUp() override;

	void Update() override;
	void Draw() override;

	T1WD::Camera* camera = nullptr;
};