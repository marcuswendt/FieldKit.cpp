/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/06/2010.
 */

#pragma once

#include "fieldkit/fbx/FBX.h"

namespace fk { namespace fbx {

	class Scene;

	class SceneController {
	public:
		SceneController();
		SceneController(Scene* scene);
		~SceneController() {};

		void setAnimation(int item);
		void nextAnimation();
		void prevAnimation();

		void setCamera(int index);
		void nextCamera();
		void prevCamera();

		void setSpeed(float ms);
		float getSpeed();

		void update();

		// Accessors
		void setScene(Scene* scene) { this->scene = scene; }
		Scene* getScene() { return this->scene; }

	protected:
		Scene* scene;

		int animIndex;
		int cameraIndex;
	};
} }