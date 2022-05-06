#include "editorCamera.h"


EditorCamera::EditorCamera() : 
	dragging(false), 
	target(nullptr)
{
	editorCameraFOV = EDITOR_CAMERA_DEFAULT_FOV;
	editorCameraNearClipPlane = EDITOR_CAMERA_DEFAULT_NEAR_CLIP_PLANE;
	editorCameraFarClipPlane = EDITOR_CAMERA_DEFAULT_FAR_CLIP_PLANE;

	ofCamera::disableOrtho();
	ofCamera::setVFlip(true);

	ofCamera::setFov(editorCameraFOV);
	ofCamera::setNearClip(editorCameraNearClipPlane);
	ofCamera::setFarClip(editorCameraFarClipPlane);

	setGlobalPosition(0.0f, -2.0f, 5.0f);
	setGlobalRotation(-40.0f, 0.0f, 0.0f);

	setCameraTransform();
}

glm::mat4 EditorCamera::getTransformationMatrix()
{
	glm::vec3 position = transform.getGlobalPosition();
	glm::mat4 localTransformMatrix = glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z));
	glm::vec3 eulerAngles = transform.getGlobalEulerAngles();
	glm::quat rotation = glm::quat(glm::vec3(-eulerAngles.x, -eulerAngles.y, -eulerAngles.z));
	localTransformMatrix = localTransformMatrix * glm::toMat4((const glm::quat&)rotation);
	return localTransformMatrix;
}

void EditorCamera::update()
{
	transform.setLocalEulerAngles(cameraRotation.y, cameraRotation.x, 0.0f);
	transform.setLocalRotation(glm::vec3(cameraRotation.y, cameraRotation.x, 0.0f));
	transform.setLocalPosition(glm::vec3(transform.getLocalPosition() + (transform.getLocalRotation() * cameraTranslation)));

	cameraTranslation = glm::vec3(0.0f);
	
	setCameraTransform();
}

void EditorCamera::mouseScrolled(float amount)
{
	cameraTranslation.z += amount * EDITOR_CAMERA_SCROLL_SPEED;
}

void EditorCamera::mouseDragged(int x, int y, MouseCommandsButtonsEnum button)
{
	if (button == MouseCommandsButtonsEnum::MIDDLE)
	{
		cameraTranslation.x -= x * EDITOR_CAMERA_MOUSE_SPEED;
		cameraTranslation.y += y * EDITOR_CAMERA_MOUSE_SPEED;
	}
	else if (button == MouseCommandsButtonsEnum::RIGHT)
	{
		cameraRotation.x -= x * EDITOR_CAMERA_ROTATION_SPEED;
		cameraRotation.y -= y * EDITOR_CAMERA_ROTATION_SPEED;
	}
}

void EditorCamera::setCameraTransform()
{
	ofCamera::setGlobalPosition(transform.getLocalPosition());
	ofCamera::setGlobalOrientation(transform.getLocalRotation());
}
