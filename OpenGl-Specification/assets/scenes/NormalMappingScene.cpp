#include "NormalMappingScene.h"
#include "../../src/Material.h"
#include "../../src/Texture.h"
#include "../meshes/Sphere.h"

using namespace std;
using namespace glm;

namespace OpenGL
{
	void NormalMappingScene::BeginScene(GLFWwindow* context)
	{
		mCamera = make_shared<Camera>(vec3(.0f, .0f, 4.0f), .02f);
		m_NormalMapShader = make_shared<Shader>("assets/shaders/NormalMapping/vertNormalMappingShader.glsl", "assets/shaders/NormalMapping/fragNormalMappingShader.glsl");
		m_LightSource = make_shared<Light>();
		mSphere = make_shared<Sphere>();
		mNormalMap = make_shared<Texture2D>("assets/textures/normalmap_brick.png");
	}

	void NormalMappingScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		mCamera->ProcessInput(context);

		glClearColor(.0f, .0f, .1f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		RenderSkyBox(context, currentTime);
		RenderShadow(context, currentTime);
		RenderGeometry(context, currentTime);
	}

	void NormalMappingScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{
	}

	void NormalMappingScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
	}

	void NormalMappingScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		mat4 sphereModelMatrix = translate(mat4(1.0f), vec3(.0f));
		
		m_NormalMapShader->SetUniformMatrix4("uModel", sphereModelMatrix);
		m_NormalMapShader->SetUniformMatrix4("uView", mCamera->GetViewMatrix());
		m_NormalMapShader->SetUniformMatrix4("uProjection", mCamera->GetProjMatrix());

		m_LightSource->CommitToProgram(*m_NormalMapShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *mNormalMap);
		m_NormalMapShader->SetUniformInt("uNormalMap", 0);
		//mSphere->Draw(*m_NormalMapShader);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}