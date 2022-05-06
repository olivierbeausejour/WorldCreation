#include "interface.h"

CommandFactory Interface::commandFactory;

Interface::Interface()
{
	Interface::setSelectedEntity(nullptr);
	hierarchyPanel = std::make_unique<HierarchyPanel>();
	selectedEntityPanel = std::make_unique<EntityPropertiesPanel>();
	sceneUtilityPanel = std::make_unique<SceneUtilityPanel>();
	canvasUtilityPanel = std::make_unique<CanvasUtilityPanel>();
	environnementPanel = std::make_unique<EnvironnementPanel>();

	mainSceneViewport = new SceneViewport();
	canvasViewport = new CanvasViewport();

	addSceneViewport(mainSceneViewport);
}

Interface::~Interface()
{
	for (auto viewport : sceneViewports)
		delete viewport;
	delete canvasViewport;
}

void Interface::setup(ofAppGLFWWindow* ofWindow, Scene& scene, std::shared_ptr<CanvasRenderer> canvasRenderer)
{
	gui.setup(nullptr, false);

	ImGuiIO& io = EditorUI::GetIO(); (void)io;
	ImFont* comicFont = io.Fonts->AddFontFromFileTTF("data/editor/fonts/comic.ttf", 18.0f);
	ImFont* segoeFont = io.Fonts->AddFontFromFileTTF("data/editor/fonts/SEGOEUI.ttf", 18.0f);
	currentFont = segoeFont;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	
	setColors();

	ImGuiStyle& style = EditorUI::GetStyle();

	style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	GLFWwindow* window = ofWindow->getGLFWWindow();
	hierarchyPanel->setCurrentScene(&scene);
	environnementPanel->setCurrentScene(&scene);

	selectedEntityPanel->setup(&scene);

	sceneUtilityPanel->setup(&gui);

	// Setup Platform/Renderer bindings
 	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	CustomCursor::setup();

	this->canvasRenderer = canvasRenderer;
	commandFactory = CommandFactory();
}

void Interface::update()
{
	for (auto viewport : sceneViewports)
	{
		if (!viewport->getIsOpen())
		{
			removeSceneViewport(viewport);
			continue;
		}

		viewport->update();
	}
}

Entity* Interface::getSelectedEntity()
{
	return selectedEntity;
}

void Interface::setSelectedEntity(Entity* entity)
{
	selectedEntity = entity;
}

// The current dark theme is from Yan Chernikov at this resource :
// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/ImGui/ImGuiLayer.cpp
void Interface::setColors()
{
	auto& colors = EditorUI::GetStyle().Colors;

	// background color
	colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

	// header colors
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

	// button colors
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

	// frame colors
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

	// tab colors
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

	// title colors
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
}

void Interface::preDraw()
{
	gui.begin();
	if (currentFont != nullptr) EditorUI::PushFont(currentFont);

	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = EditorUI::GetMainViewport();
	EditorUI::SetNextWindowPos(viewport->GetWorkPos());
	EditorUI::SetNextWindowSize(viewport->GetWorkSize());
	EditorUI::SetNextWindowViewport(viewport->ID);

	EditorUI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	EditorUI::Begin("InvisibleWindow", nullptr, windowFlags);
	EditorUI::PopStyleVar();

	ImGuiID dockSpaceId = EditorUI::GetID("InvisibleWindowDockSpace");

	EditorUI::DockSpace(dockSpaceId);
	showMainMenu();

	EditorUI::End();
}


void Interface::postDraw()
{
	// TODO: Eventually property panel should draw selectedEntityPanel 
	// or any Property Panel for any type of things and not just entities
	//propertyPanel->draw();
	hierarchyPanel->draw();
	selectedEntityPanel->draw();
	sceneUtilityPanel->draw();
	canvasUtilityPanel->draw();
	environnementPanel->draw();
	
	if (currentFont != nullptr) 
		EditorUI::PopFont();
	gui.end();

	CustomCursor::draw();
}

void Interface::drawSceneViewport(ofFbo* sceneFramebuffer, SceneViewport* sceneViewport)
{
	sceneViewport->setFramebuffer(sceneFramebuffer);
	sceneViewport->draw();
}

void Interface::drawCanvasViewport(ofFbo* canvasFramebuffer)
{
	canvasViewport->setFramebuffer(canvasFramebuffer);
	canvasViewport->draw();
}

void Interface::showMainMenu()
{
	Action menuAction = Action::NONE;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Import An Image"))
			{
				if (!loadImageFromFile(menuAction))
				{
					ImGui::OpenPopup("Error");
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("New Scene Window"))
			{
				if (!executeCommand(Action::CREATE_SCENE_VIEWPORT, this))
				{
					ImGui::OpenPopup("Error");
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_NoResize))
		showErrorPopup(Action::IMPORT_IMAGE);
}

void Interface::showErrorPopup(const Action& action)
{
	ImGui::Text(getErrorMessage(action).c_str());

	ImGui::Spacing();
	ImGui::SameLine(375);
	if (ImGui::Button("OK", ImVec2(120, 0)))
		ImGui::CloseCurrentPopup();

	ImGui::EndPopup();
}

bool Interface::loadImageFromFile(Action& menuAction)
{
	menuAction = Action::IMPORT_IMAGE;
	ImageImport imageImport;
	ofImage* image = new ofImage();
	imageImport.imagePtr = image;

	if (Interface::executeCommand(menuAction, &imageImport))
	{
		Entity* entity = new Entity();
		ImageComponent* imageComponent = new ImageComponent(image);

		float rendererWidth = canvasRenderer->getRendererWidth();
		float rendererHeight = canvasRenderer->getRendererHeight();

		float xRatio = rendererWidth / imageImport.imagePtr->getWidth();
		float yRatio = rendererHeight / imageImport.imagePtr->getHeight();
		float availableRatio = min(1.0f, min(xRatio, yRatio));

		entity->setDrawableComponent(imageComponent);

		float x = (rendererWidth - imageImport.imagePtr->getWidth() <= 0 && xRatio == availableRatio) ? 0 : rand() % (int)(rendererWidth - imageImport.imagePtr->getWidth());
		float y = (rendererHeight - imageImport.imagePtr->getHeight() <= 0 && yRatio == availableRatio) ? 0 : rand() % (int)(rendererHeight - imageImport.imagePtr->getHeight());

		imageComponent->getTransform()->setLocalPosition(glm::vec3(x, y, 0));
		imageComponent->setSize(glm::vec3(imageImport.imagePtr->getWidth() * availableRatio, imageImport.imagePtr->getHeight() * availableRatio, 0));

		canvasRenderer->addImage(entity);

		return true;
	}

	delete image;
	return false;
}

std::string Interface::getErrorMessage(const Action& action)
{
	switch (action) 
	{
		// Find ImGui bug where pop-up seems to be shown later (and therefore, "action" is reset, taking the default path)
		case Action::IMPORT_IMAGE:
			return "Error reading the selected image. Try to open a PNG, JPEG or GIF file.\n\n";
			break;
		case Action::CREATE_SCENE_VIEWPORT:
			return "Error creating new scene viewport. Please try again later.\n\n";
			break;
		case Action::IMPORT_MODEL:
			return "Error reading the selected model. Try to open a DAE, PLY, OBJ or FBX file.\n\n";
			break;
		default:
			return "An error occured. Please try again later.\n\n";
			break;
	}
}

bool Interface::executeCommand(const Action& action, void* object)
{
	Command* command = commandFactory.getCommand(action, object);
	bool result = command->executeCommand();
	delete command;
	command = nullptr;
	return result;
}

void Interface::addSceneViewport(SceneViewport* viewport)
{
	if (find(sceneViewports.begin(), sceneViewports.end(), viewport) == sceneViewports.end())
	{
		sceneViewports.push_back(std::move(viewport));
		selectedEntityPanel->getDrawableComponentPropertiesPanel()->addViewport(viewport);
	}
}

void Interface::removeSceneViewport(SceneViewport* viewport)
{
	auto abstractViewportIterator = find(sceneViewports.begin(), sceneViewports.end(), viewport);
	if (abstractViewportIterator != sceneViewports.end())
	{
		sceneViewports.erase(abstractViewportIterator);
		selectedEntityPanel->getDrawableComponentPropertiesPanel()->removeViewport(viewport);
		delete viewport;
	}
}
