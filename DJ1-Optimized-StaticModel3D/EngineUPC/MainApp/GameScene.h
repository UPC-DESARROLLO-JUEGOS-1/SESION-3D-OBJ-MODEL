#pragma once

#include <FrameworkUPC\BaseScene.h>

class GameScene : public BaseScene
{
public:
	GameScene(SceneGraph* sceneGraph) :
		BaseScene::BaseScene(sceneGraph)
	{}
	~GameScene()
	{
		
	}

	void Initialize();
	void Update(float dt);
	void Draw(float dt);

	void OnKeyDown(SDL_Keycode key);
	void OnKeyUp(SDL_Keycode key);

private:
	
};