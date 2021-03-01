/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Main.cpp
Purpose: Initializes and loops over the game contents
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_1
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 5, 2020
- End Header --------------------------------------------------------*/


#include <Windows.h>

#include <SDL.h>
#include "glew.h"
#include <gl/GL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>


#include "stdio.h"

#include <iostream>
#include <string>

#include "Matrix2D.h"

#include "ObjectFactory.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "Moderator.h"
#include "LoadShaders.h"

#include "GameObject.h"
#include "GameObjectManager.h"

#include "Components/Component.h"
#include "Components/Controller.h"
#include "Components/Transform.h"
#include "Components/Sprite.h"
#include "Components/Character.h"
#include "Components/Body.h"

#include <gdiplus.h>
#include "../glm/glm/glm.hpp"
#include "../glm/glm/gtc/matrix_transform.hpp"

#include "AudioManager.h"

#pragma comment (lib, "Gdiplus.lib")

# define PI           3.14159265358979323846  /* pi */


// #pragma comment (lib, "opengl32.lib")
// #pragma comment (lib, "glew32.lib")

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

InputManager* gpInputManager;
FrameRateController* gpFRC;
ResourceManager* gpResourceManager;
GameObjectManager* gpGameObjectManager;
ObjectFactory* gpObjectFactory;
CollisionManager* gpCollisionManager;
EventManager* gpEventManager;
Moderator* gpModerator;

AudioManager* gpAudioManager;

bool DEBUG;
int gameType = 1;

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}


#pragma comment(lib, "legacy_stdio_definitions.lib")


GLint gRenderID, gCircID, gRectID;



static Uint8* audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

void my_audio_callback(void* userdata, Uint8* stream, int len) {

	if (audio_len == 0)
		return;

	len = (len > audio_len ? audio_len : len);
	//SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

	audio_pos += len;
	audio_len -= len;
}


int main(int argc, char* args[])
{

	/*
	if (AllocConsole()) {
		FILE* file;

		freopen_s(&file, "SONOUT$", "wt", stdout);
		freopen_s(&file, "SONOUT$", "wt", stderr);
		freopen_s(&file, "SONOUT$", "wt", stdin);

		SetConsoleTitle(L"SDL 2.0 Test");
	}
	*/

	int screenSize[] = { 1200, 800 };

	SDL_Window *pWindow;
	int error = 0;
	bool appIsRunning = true;

	SDL_Surface* pWindowSurface = NULL;
	SDL_Surface* pImageSurface = NULL;

	gpInputManager = new InputManager();
	gpFRC = new FrameRateController(75);			// ok I know it's odd but my monitor is 75Hz
	gpResourceManager = new ResourceManager();
	gpGameObjectManager = new GameObjectManager();
	gpObjectFactory = new ObjectFactory();
	gpCollisionManager = new CollisionManager();
	gpEventManager = new EventManager();
	gpModerator = new Moderator();

	gpAudioManager = new AudioManager();

	DEBUG = false;

	SDL_GLContext openGL_Context;

	// Initialize SDL
	if((error = SDL_Init( SDL_INIT_VIDEO )) < 0 )
	{
		printf("Couldn't initialize SDL, error %i\n", error);
		return 1;
	}

	// OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	

	pWindow = SDL_CreateWindow("SDL2 window",		// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		screenSize[0],										// width, in pixels
		screenSize[1],										// height, in pixels
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	// Check that the window was successfully made
	if (NULL == pWindow)
	{
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// Create the context, and make it current
	openGL_Context = SDL_GL_CreateContext(pWindow);

	if (glewInit() != GLEW_OK)
		printf("Couldn't init GLEW library\n");

	
	// Initialize GDIplus
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	LoadShaders();

	// Get the window surface
	pWindowSurface = SDL_GetWindowSurface(pWindow);

	SDL_Rect destinationRect;
	SDL_Renderer* pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

	int levelNo = 0;

	// gpObjectFactory->LoadLevel(("..\\Resources\\Level" + std::to_string(levelNo) + ".json").c_str());

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);


	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	

	// 0 -> Positions
	// 1 -> Colors
	// 2 -> Texels
	GLuint bufferIDs[3];

	glGenBuffers(3, bufferIDs);
	
	int vertexNum = 4;
	
	int coordsPerPosition = 3;
	int coordsPerColor = 4;
	int coordsPerTex = 2;

	int positionStride	= coordsPerPosition * sizeof(float);
	int colorStride		= coordsPerColor * sizeof(float);
	int texStride		= coordsPerTex * sizeof(float);

	GLfloat* pPositions = new GLfloat[vertexNum * coordsPerPosition];
	GLfloat* pColors = new GLfloat[vertexNum * coordsPerColor];
	GLfloat* pTex = new GLfloat[vertexNum * coordsPerTex];

	// Vertex 1
	pPositions[0] = -0.5f;	pPositions[1] = -0.5f;	pPositions[2] = 0.0f;
	pColors[0] = 1.0f;		pColors[1] = .0f;		pColors[2] = .0f;		pColors[3] = 1.0f;
	pTex[0] = 0.0f;			pTex[1] = 1.0f;

	// Vertex 2
	pPositions[3] = 0.5f;	pPositions[4] = -0.5f;	pPositions[5] = 0.0f;
	pColors[4] = .0f;		pColors[5] = 1.0f;		pColors[6] = .0f;		pColors[7] = 1.0f;
	pTex[2] = 1.0f;			pTex[3] = 1.0f;

	// Vertex 3
	pPositions[6] = 0.5f;	pPositions[7] = 0.5f;	pPositions[8] = 0.0f;
	pColors[8] = .0f;		pColors[9] = .0f;		pColors[10] = 1.0f;		pColors[11] = 1.0f;
	pTex[4] = 1.0f;			pTex[5] = 0.0f;

	
	// Vertex 4
	pPositions[9] = -0.5f;	pPositions[10] = 0.5f;	pPositions[11] = 0.0f;
	pColors[12] = 1.0f;		pColors[13] = 1.0f;		pColors[14] = 1.0f;		pColors[15] = 1.0f;
	pTex[6] = 0.0f;			pTex[7] = 0.0f;
	

	// Copy from RAM to VRAM
	//Positions
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * positionStride, pPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, coordsPerPosition, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Colors
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * colorStride, pColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, coordsPerColor, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Textures
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * texStride, pTex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
	

	// Block until all OpenGL executions are complete
	glFinish();

	delete[] pPositions;
	delete[] pColors;
	delete[] pTex;

	GLuint menuScreen = gpResourceManager->LoadTexture("../Resources/529_title.png");
	GLuint endScreen = gpResourceManager->LoadTexture("../Resources/529_end.png");
	GLuint deadScreen = gpResourceManager->LoadTexture("../Resources/529_dead.png");
	GLuint backgroundImg = gpResourceManager->LoadTexture("../Resources/madness.png");

	/****************/
	// float transformationMatrix[16];
	float angle = 0.5f;
	int val;
	glm::mat4 projectionMatrix = glm::ortho(0.f, (float)screenSize[0], (float)screenSize[1], 0.f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	gpModerator->mStage = 1;
	gpObjectFactory->LoadLevel("..\\Resources\\Level1.json");

	// Game loop
	while(true == appIsRunning)
	{
		gpAudioManager->system->update();

		// Get the time at the start of the frame
		gpFRC->FrameStart();

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				appIsRunning = false;
			}
		} // done with handling events

		gpInputManager->Update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// SDL_FillRect(pWindowSurface, NULL, 0xBBBBBB);

		gpEventManager->Update(gpFRC->GetDeltaTime());
		
		for (auto pColl : gpCollisionManager->mContacts) {
			pColl->mChars[0]->mpOwner->HandleEvent(pColl);
			pColl->mChars[1]->mpOwner->HandleEvent(pColl);
			// gpEventManager->BroadcastEvent(pColl);
			delete pColl;
		}
		gpCollisionManager->mContacts.clear();

		// update
		for (auto pGameObject : gpGameObjectManager->mGameObjects)
			pGameObject->Update();

		for (auto pGameObject : gpGameObjectManager->mGameObjects) {
			if (pGameObject->mDestroy) {
				gpGameObjectManager->DeleteObject(pGameObject);
			}
		}

		glBindVertexArray(vaoID);

		/*******************************/
		/*******************************/
		/*******************************/

		gpGameObjectManager->mDestroy = false;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_ADD);

		if (gameType == 1){
			glUseProgram(gRenderID);

			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(0, 0, 1.f));
			model = glm::scale(model, glm::vec3(2.f, 2.f, 0.0f));

			int transformationHandle = 4;
			glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, backgroundImg);


			glDrawArrays(GL_QUADS, 0, vertexNum);
		}

		gpModerator->Update();
		if (gpModerator->mStage == 0) {
			glUseProgram(gRenderID);
			// glBindVertexArray(vaoID);

			//for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				

				// glBindVertexArray(vaoID);

				glm::mat4 model(1.0f);
				model = glm::scale(model, glm::vec3(2.f, 2.f, 0.0f));

				// model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, menuScreen);


				glDrawArrays(GL_QUADS, 0, vertexNum);

				// glBindVertexArray(0);
			//}
		}
		else if (gpModerator->mStage == 666) {
			glUseProgram(gRenderID);

			glm::mat4 model(1.0f);

			model = glm::scale(model, glm::vec3(2.f, 2.f, 0.0f));

			int transformationHandle = 4;
			glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, deadScreen);


			glDrawArrays(GL_QUADS, 0, vertexNum);
		}
		else if (gpModerator->mStage == 99) {
			glUseProgram(gRenderID);

			glm::mat4 model(1.0f);

			model = glm::scale(model, glm::vec3(2.f, 2.f, 0.0f));

			int transformationHandle = 4;
			glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, endScreen);


			glDrawArrays(GL_QUADS, 0, vertexNum);
		}
		else {

			// Render terrain
			glUseProgram(gRenderID);
			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				Transform* pT = static_cast<Transform*>(pGameObject->GetComponent(TYPE_TRANSFORM));
				Sprite* pS = static_cast<Sprite*>(pGameObject->GetComponent(TYPE_SPRITE));
				Controller* pC = static_cast<Controller*>(pGameObject->GetComponent(TYPE_PLAYER_CONTROLLER));
				Body* pB = static_cast<Body*>(pGameObject->GetComponent(TYPE_BODY));

				if (pB == nullptr || !pB->mWall) { continue; }

				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(pT->mPositionX + pT->mSpriteOffsetX, pT->mPositionY + pT->mSpriteOffsetY - pT->mHeight / 2.0f, -.95));
				model = glm::rotate(model, pT->mAngle, glm::vec3(0, 0, 1));
				if (pS->mIsAnimated && pS->mpSpriteAnimator->mIsAttacking)
				{
					if (pC->mSwingAng < -PI / 2 || pC->mSwingAng > PI / 2)
						model = glm::scale(model, glm::vec3(-pT->mWidth, -pT->mHeight, 0.0f));
					else
						model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
				}
				else if (pT->mVelHoriz > 0)
					model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
				else
					model = glm::scale(model, glm::vec3(-pT->mWidth, -pT->mHeight, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pS->mTexture);

				if (pS->mIsAnimated)
				{

					GLfloat* pTex = new GLfloat[vertexNum * coordsPerTex];

					auto currentTextureOffset = pS->mpSpriteAnimator->GetTextureCoords();

					pTex[0] = 0.0f;			pTex[1] = 0.5f;
					pTex[2] = 0.25;			pTex[3] = 0.5f;
					pTex[4] = 0.25f;		pTex[5] = 0.0f;
					pTex[6] = 0.0f;			pTex[7] = 0.0f;

					pTex[0] = currentTextureOffset.first;
					pTex[1] = currentTextureOffset.second + (1.0f / pS->mRows);
					pTex[2] = currentTextureOffset.first + (1.0f / pS->mColumns);
					pTex[3] = currentTextureOffset.second + (1.0f / pS->mRows);
					pTex[4] = currentTextureOffset.first + (1.0f / pS->mColumns);
					pTex[5] = currentTextureOffset.second;
					pTex[6] = currentTextureOffset.first;
					pTex[7] = currentTextureOffset.second;


					glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
					glBufferData(GL_ARRAY_BUFFER, vertexNum * texStride, pTex, GL_STATIC_DRAW);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);


					glDrawArrays(GL_QUADS, 0, vertexNum);

					pTex[0] = 0.0f;			pTex[1] = 1.0f;
					pTex[2] = 1.0f;			pTex[3] = 1.0f;
					pTex[4] = 1.0f;			pTex[5] = 0.0f;
					pTex[6] = 0.0f;			pTex[7] = 0.0f;
					glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
					glBufferData(GL_ARRAY_BUFFER, vertexNum * texStride, pTex, GL_STATIC_DRAW);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				else
				{

					glDrawArrays(GL_QUADS, 0, vertexNum);
				}
			}

			// Render Shadows (which indicate collision areas)
			

			// glUseProgram(gCircID);
			// glBindVertexArray(vaoID);

			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				glUseProgram(gCircID);

				Transform* pT = static_cast<Transform*>(pGameObject->GetComponent(TYPE_TRANSFORM));
				Character* pC = static_cast<Character*>(pGameObject->GetComponent(TYPE_CHARACTER));

				if (!pC)
					continue;

				// glBindVertexArray(vaoID);
				int val;

				if(DEBUG && pC->mShadowOffset == 0.f)
				{
					glm::mat4 model(1.0f);
					model = glm::translate(model, glm::vec3(pT->mPositionX, pT->mPositionY, -0.9f));
					model = glm::scale(model, glm::vec3(pC->mRadius * 2.f + (DEBUG ? 0.f : pC->mShadowOffset * 2.0f), pC->mRadius * 2.f + (DEBUG ? 0.f : pC->mShadowOffset * 2.0f), 0.0f));

					model = projectionMatrix * model;

					val = glGetUniformLocation(gCircID, "uColor1");
					glUniform4f(val, 0.0, 0.0, 0.0, 0.5);

					val = glGetUniformLocation(gCircID, "uColor2");
					glUniform4f(val, 0.0, 0.0, 0.8, 1.0);

					val = glGetUniformLocation(gCircID, "uFill");
					glUniform1f(val, 1.0);

					val = glGetUniformLocation(gCircID, "uIn");
					glUniform1f(val, 0.0);

					val = glGetUniformLocation(gCircID, "uAngs");
					glUniform2f(val, -10.0, 10.0);


					int transformationHandle = 4;
					glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

					glDrawArrays(GL_QUADS, 0, vertexNum);
				}

				/*** Telegraphs ***/


				while (!pC->mTelegraphs.empty()) {
					// for (auto pRect : pC->mTelegraphs) {
					Character::Telegraph* pTelegraph = pC->mTelegraphs.at(0);
					glm::mat4 model(1.0f);
					model = glm::translate(model, glm::vec3(pTelegraph->mX, pTelegraph->mY, -0.9f));
					model = glm::rotate(model, pTelegraph->mAng, glm::vec3(0, 0, 1));
					model = glm::scale(model, glm::vec3(pTelegraph->mOut * 2.f, pTelegraph->mOut * 2.f, 0.0f));

					model = projectionMatrix * model;

					val = glGetUniformLocation(gCircID, "uColor1");
					glUniform4f(val, pTelegraph->mColor1[0], pTelegraph->mColor1[1], pTelegraph->mColor1[2], pTelegraph->mColor1[3]);

					val = glGetUniformLocation(gCircID, "uColor2");
					glUniform4f(val, pTelegraph->mColor2[0], pTelegraph->mColor2[1], pTelegraph->mColor2[2], pTelegraph->mColor2[3]);

					val = glGetUniformLocation(gCircID, "uFill");
					glUniform1f(val, pTelegraph->mFill);

					val = glGetUniformLocation(gCircID, "uIn");
					glUniform1f(val, pTelegraph->mIn / pTelegraph->mOut);

					val = glGetUniformLocation(gCircID, "uAngs");
					glUniform2f(val, -pTelegraph->mAngWidth, pTelegraph->mAngWidth);

					int transformationHandle = 4;
					glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

					glDrawArrays(GL_QUADS, 0, vertexNum);

					delete pTelegraph;
					pC->mTelegraphs.erase(pC->mTelegraphs.begin());
				}


				glUseProgram(gRectID);
				while (!pC->mRectangles.empty()) {
					// for (auto pRect : pC->mTelegraphs) {
					Character::TeleRect* pRect = pC->mRectangles.at(0);
					glm::mat4 model(1.0f);
					model = glm::translate(model, glm::vec3(pRect->mX, pRect->mY, -0.9f));
					model = glm::rotate(model, pRect->mAng, glm::vec3(0, 0, 1));
					model = glm::scale(model, glm::vec3(pRect->mLength, pRect->mWidth, 0.0f));

					model = projectionMatrix * model;

					val = glGetUniformLocation(gRectID, "uColor1");
					glUniform4f(val, pRect->mColor1[0], pRect->mColor1[1], pRect->mColor1[2], pRect->mColor1[3]);

					val = glGetUniformLocation(gRectID, "uColor2");
					glUniform4f(val, pRect->mColor2[0], pRect->mColor2[1], pRect->mColor2[2], pRect->mColor2[3]);

					val = glGetUniformLocation(gRectID, "uFill");
					glUniform1f(val, pRect->mFill);

					int transformationHandle = 4;
					glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

					glDrawArrays(GL_QUADS, 0, vertexNum);

					delete pRect;
					pC->mRectangles.erase(pC->mRectangles.begin());
				}



				// glBindVertexArray(0);
			}
			// glBindVertexArray(0);



			// Render units
			/*
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
			*/

			glUseProgram(gRenderID);
			// glBindVertexArray(vaoID);

			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				Transform* pT = static_cast<Transform*>(pGameObject->GetComponent(TYPE_TRANSFORM));
				Sprite* pS = static_cast<Sprite*>(pGameObject->GetComponent(TYPE_SPRITE));
				Controller* pC = static_cast<Controller*>(pGameObject->GetComponent(TYPE_PLAYER_CONTROLLER));
				Body* pB = static_cast<Body*>(pGameObject->GetComponent(TYPE_BODY));

				if (pB != nullptr && pB->mWall) { continue; }
				//glBindVertexArray(vaoID);

				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(pT->mPositionX + pT->mSpriteOffsetX, pT->mPositionY + pT->mSpriteOffsetY - pT->mHeight / 2.0f, (pT->mPositionY - screenSize[1] / 2) / screenSize[1]));
				model = glm::rotate(model, pT->mAngle, glm::vec3(0, 0, 1));
				if (pS->mIsAnimated && pS->mpSpriteAnimator->mIsAttacking)
				{
					if (pC->mSwingAng < -PI/2 || pC->mSwingAng > PI/2)
						model = glm::scale(model, glm::vec3(-pT->mWidth, -pT->mHeight, 0.0f));
					else
						model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
				}
				else if(pT->mVelHoriz > 0)
					model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
				else
					model = glm::scale(model, glm::vec3(-pT->mWidth, -pT->mHeight, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pS->mTexture);

				if (pS->mIsAnimated)
				{

					GLfloat* pTex = new GLfloat[vertexNum * coordsPerTex];

					auto currentTextureOffset = pS->mpSpriteAnimator->GetTextureCoords();

					pTex[0] = 0.0f;			pTex[1] = 0.5f;
					pTex[2] = 0.25;			pTex[3] = 0.5f;
					pTex[4] = 0.25f;		pTex[5] = 0.0f;
					pTex[6] = 0.0f;			pTex[7] = 0.0f;

					pTex[0] = currentTextureOffset.first;
					pTex[1] = currentTextureOffset.second + (1.0f / pS->mRows);
					pTex[2] = currentTextureOffset.first + (1.0f / pS->mColumns);
					pTex[3] = currentTextureOffset.second + (1.0f / pS->mRows);
					pTex[4] = currentTextureOffset.first + (1.0f / pS->mColumns);
					pTex[5] = currentTextureOffset.second;
					pTex[6] = currentTextureOffset.first;
					pTex[7] = currentTextureOffset.second;

					
					glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
					glBufferData(GL_ARRAY_BUFFER, vertexNum* texStride, pTex, GL_STATIC_DRAW);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					

					glDrawArrays(GL_QUADS, 0, vertexNum);

					pTex[0] = 0.0f;			pTex[1] = 1.0f;
					pTex[2] = 1.0f;			pTex[3] = 1.0f;
					pTex[4] = 1.0f;			pTex[5] = 0.0f;
					pTex[6] = 0.0f;			pTex[7] = 0.0f;
					glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
					glBufferData(GL_ARRAY_BUFFER, vertexNum * texStride, pTex, GL_STATIC_DRAW);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				else
				{
					
					glDrawArrays(GL_QUADS, 0, vertexNum);
				}
			}


			// Health bars
			glUseProgram(gRectID);

			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				Transform* pT = static_cast<Transform*>(pGameObject->GetComponent(TYPE_TRANSFORM));
				Character* pC = static_cast<Character*>(pGameObject->GetComponent(TYPE_CHARACTER));

				if (!pC) { continue; }

				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(pT->mPositionX, pT->mPositionY - pT->mHeight + pT->mSpriteOffsetY - 10.0f, 1.0f));
				model = glm::rotate(model, 0.0f, glm::vec3(0, 0, 1));
				model = glm::scale(model, glm::vec3(64.0f, 5.0f, 0.0f));

				model = projectionMatrix * model;

				int val = glGetUniformLocation(gRectID, "uColor1");
				glUniform4f(val, 0.3, 0.8, 0.3, 1.0);

				val = glGetUniformLocation(gRectID, "uColor2");
				glUniform4f(val, 0.4, 0.4, 0.4, 1.0);

				val = glGetUniformLocation(gRectID, "uFill");
				glUniform1f(val, pC->mHP / pC->mHPMax);

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		glBindVertexArray(0);
		
		/*******************************************************************/
		/*******************************************************************/
		/*******************************************************************/


		

		// SDL_UpdateWindowSurface(pWindow);
		SDL_GL_SwapWindow(pWindow);
		
		
		if (gpInputManager->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {	// end game
			appIsRunning = false;
		}

		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_RETURN)) {	// restart level
			gpGameObjectManager->~GameObjectManager();
			// gpEventManager->Reset();
			// gpCollisionManager->Reset();

			gpModerator->mStage = 1;
			gpObjectFactory->LoadLevel("..\\Resources\\Level1.json");
		}

		
		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_RIGHT)) {	// next level
			/*
			gpGameObjectManager->~GameObjectManager();
			gpEventManager->Reset();
			gpCollisionManager->Reset();

			(levelNo == 4 ? levelNo = 0 : levelNo++);
			gpObjectFactory->LoadLevel(("..\\Resources\\Level" + std::to_string(levelNo) + ".json").c_str());
			*/
			
			gpModerator->mStage++;
			
			if (gpModerator->mStage == 0) {
				gpGameObjectManager->~GameObjectManager();
			}
			else if(gpModerator->mStage <= 4) {
				// gpGameObjectManager->~GameObjectManager();
				gpObjectFactory->LoadLevel(("..\\Resources\\Level" + std::to_string(gpModerator->mStage) + ".json").c_str());
			}
			
		}
		
		
		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_LEFT)) {	// prev level
			gpGameObjectManager->~GameObjectManager();
			gpModerator->mStage = 1;
			gpObjectFactory->LoadLevel("..\\Resources\\Level1.json");
		}

		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_F)) {
			DEBUG = !DEBUG;
		}

		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_2)) {
			gameType = 2;
			gpGameObjectManager->~GameObjectManager();
			gpObjectFactory->LoadGameObject("../Resources/Runner.json");
		}

		for (auto pGO : gpGameObjectManager->mGameObjects) {
			Character* pC = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
			if (!pC) { continue; }
			if (pC->mHP <= 0) {
				// gpObjectFactory->LoadGameObject("../Resources/Slime.json");
				gpGameObjectManager->DeleteObject(pC->mpOwner);
			}
		}


		// Lock the frame rate
		gpFRC->FrameEnd();

		
	}

	delete gpObjectFactory;
	delete gpGameObjectManager;
	delete gpFRC;
	delete gpInputManager;
	delete gpCollisionManager;
	delete gpEventManager;

	// Destroy the image
	// SDL_FreeSurface(pImageSurface);
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(3, bufferIDs);
	glUseProgram(0);
	glDeleteProgram(gRenderID);
	SDL_GL_DeleteContext(openGL_Context);

	gpResourceManager->~ResourceManager();
	

	// Close and destroy the window
	SDL_DestroyWindow(pWindow);

	// Quit SDL subsystems
	SDL_Quit();

	/*
	System:Start
	
	Load 
	Init
	Update
	Draw
	Free

	System:End
	*/
	
	return 0;
}