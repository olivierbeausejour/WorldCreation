#pragma once

#include "wcpch.h"

#include "entity/components/transformComponent.h"
#include "entity/entity.h"
#include "interface/customCursor.h"
#include "inputs/mouseCommands.h"

#define EDITOR_CAMERA_MOUSE_SPEED 0.005f;
#define EDITOR_CAMERA_SCROLL_SPEED 0.1f;
#define EDITOR_CAMERA_ROTATION_SPEED 0.003f
#define EDITOR_CAMERA_DEFAULT_FOV 60.0f;
#define EDITOR_CAMERA_DEFAULT_NEAR_CLIP_PLANE 0.01f;
#define EDITOR_CAMERA_DEFAULT_FAR_CLIP_PLANE 1000.0f;

class EditorCamera : public ofCamera
{

public:

	EditorCamera();
	~EditorCamera() {};

	void begin() { ofCamera::begin(); };
	void end() { ofCamera::end(); };;

	void setGlobalPosition(float x, float y, float z) { transform.setLocalPosition(x, y, z); };
	void setGlobalRotation(float x, float y, float z) { transform.setLocalEulerAngles(x, y, z); };
	glm::vec3 getWorldPosition() { return transform.getGlobalPosition(); }
	glm::quat getWorldRotation() { return transform.getGlobalRotation(); }
	glm::vec3 getDirection() { return transform.getForwardDirection(); }
	glm::mat4 getTransformationMatrix();


	void setOrthographic() { ofCamera::enableOrtho(); };
	void setPerspective() { ofCamera::disableOrtho(); };

	glm::mat4 getProjectionMatrix() { return ofCamera::getProjectionMatrix(); }
	glm::mat4 getViewMatrix() { return ofCamera::getModelViewMatrix(); }

	void update();
	void mouseScrolled(float amount);
	void mouseDragged(int x, int y, MouseCommandsButtonsEnum button);

	void drawFrustrum() { ofCamera::drawFrustum(); }
	
private:

	void setCameraTransform();

	glm::vec3 cameraTranslation;
	glm::vec2 cameraRotation;

	float editorCameraFOV;
	float editorCameraNearClipPlane;
	float editorCameraFarClipPlane;
	
	bool dragging;
	
	Entity* target = nullptr;
	TransformComponent transform;

};