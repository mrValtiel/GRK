#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <GL/gl.h>
#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"

GLuint programColor;
GLuint programTexture;
Core::Shader_Loader shaderLoader;

//models
obj::Model shipModel;
obj::Model sphereModel;
obj::Model fishModel;
obj::Model rockModel;
obj::Model seaSurfaceModel;
obj::Model seabedModel;
obj::Model skyboxModel;
obj::Model skybox;
obj::Model krzak;
obj::Model babel;

//textures
GLuint textureAsteroid;
GLuint textureFish;
GLuint textureRock;
GLuint textureSeabed;
GLuint skyboxTexture01;
GLuint skyboxTexture02;
GLuint skyboxTexture03;
GLuint skyboxTexture04;
GLuint skyboxTexture05;
GLuint krzakTexture;
GLuint babelTexture;

//camera
glm::mat4 cameraMatrix, perspectiveMatrix;
glm::quat cameraRotation = glm::quat(1, 0, 0, 0);

glm::vec3 cameraPos = glm::vec3(0, 0, 5);
glm::vec3 cameraDir; // Wektor "do przodu" kamery
glm::vec3 cameraSide; // Wektor "w bok" kamery
float cameraAngle = 0;
glm::quat rotation = glm::quat(1, 0, 0, 0);
glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));
float boobleHeight = -30.0f;

//time
float time1 = glutGet(GLUT_ELAPSED_TIME) / 20.0f;
float sin1 = sinf(time1*0.8) / 2;

//keyboard
void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float moveSpeed = 0.1f;
	switch (key)
	{
	case 'z': cameraAngle -= angleSpeed; break;
	case 'x': cameraAngle += angleSpeed; break;
	case 'w': cameraPos += cameraDir * moveSpeed; break;
	case 's': cameraPos -= cameraDir * moveSpeed; break;
	case 'd': cameraPos += cameraSide * moveSpeed; break;
	case 'a': cameraPos -= cameraSide * moveSpeed; break;
	}
}

void mouse(int x, int y)
{
}

glm::mat4 createCameraMatrix()
{
	cameraDir = glm::vec3(cosf(cameraAngle - glm::radians(90.0f)), 0.0f, sinf(cameraAngle - glm::radians(90.0f)));
	glm::vec3 up = glm::vec3(0, 1, 0);
	cameraSide = glm::cross(cameraDir, up);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color, float alpha)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform1f(glGetUniformLocation(program, "objectAlpha"), alpha);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint textureId)
{
	GLuint program = programTexture;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	Core::SetActiveTexture(textureId, "textureSampler", program, 0);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}
void renderScene()
{
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.1f, 0.6f, 1.0f); //0.0 0.1 0.3 1.0

	//fish
	glm::mat4 fishInitialTransformation = glm::translate(glm::vec3(0, -0.25f, -2)) * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.25f));
	glm::mat4 fishModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f) * glm::rotate(-cameraAngle, glm::vec3(0, 1, 0)) * fishInitialTransformation;
	drawObjectTexture(&fishModel, fishModelMatrix, textureFish);

	//fishes
	fishInitialTransformation = glm::translate(glm::vec3(13, 2.25f, 25)) * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.35f));
	drawObjectTexture(&fishModel, fishInitialTransformation, textureFish);
	fishInitialTransformation = glm::translate(glm::vec3(-10, 3.25f, -22)) * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.45f));
	drawObjectTexture(&fishModel, fishInitialTransformation, textureFish);
	fishInitialTransformation = glm::translate(glm::vec3(2, 3.25f, 10)) * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.35f));
	drawObjectTexture(&fishModel, fishInitialTransformation, textureFish);
	fishInitialTransformation = glm::translate(glm::vec3(23, 4.25f, 24)) * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.45f));
	drawObjectTexture(&fishModel, fishInitialTransformation, textureFish);

	//stones
	glm::mat4 rockTransformation = glm::translate(glm::vec3(10, -6, -3)) * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(15, -6, 7)) * glm::rotate(glm::radians(140.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(2, -6, -12)) * glm::rotate(glm::radians(140.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.5f));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(0, -6, 12)) * glm::rotate(glm::radians(-20.0f), glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(1));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(-4, -6, 7)) * glm::rotate(glm::radians(180.0f), glm::vec3(1, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(-4, -6, 5)) * glm::rotate(glm::radians(65.0f), glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(1.5f));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(30, -6, 60)) * glm::rotate(glm::radians(14.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.6));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(-23, -6, -55)) * glm::rotate(glm::radians(10.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(1.8));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(54, -6, 49)) * glm::rotate(glm::radians(85.0f), glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(1.1));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(-24, -6, -29)) * glm::rotate(glm::radians(12.0f), glm::vec3(0, 1, 1)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(16, -6, 45)) * glm::rotate(glm::radians(-50.0f), glm::vec3(1, 1, 0)) * glm::scale(glm::vec3(3));
	drawObjectTexture(&rockModel, rockTransformation, textureRock);
	//more stones
	rockTransformation = glm::translate(glm::vec3(0, -7, 0)) * glm::rotate(glm::radians(0.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.5));
	drawObjectTexture(&sphereModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(13, -7, 22)) * glm::rotate(glm::radians(0.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.5));
	drawObjectTexture(&sphereModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(-46, -7, 0)) * glm::rotate(glm::radians(0.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(5.5));
	drawObjectTexture(&sphereModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(26, -7, -15)) * glm::rotate(glm::radians(0.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(3.5));
	drawObjectTexture(&sphereModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(-14, -7, 46)) * glm::rotate(glm::radians(0.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2.5));
	drawObjectTexture(&sphereModel, rockTransformation, textureRock);
	rockTransformation = glm::translate(glm::vec3(55, -7, 12)) * glm::rotate(glm::radians(0.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.5));
	drawObjectTexture(&sphereModel, rockTransformation, textureRock);

	//wodorosty
	glm::mat4 krzakTransformation = glm::translate(glm::vec3(0, -6, 7)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(7, -6, 34)) * glm::rotate(glm::radians(65.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(3));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(34, -6, -12)) * glm::rotate(glm::radians(122.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.4));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(-12, -6, -33)) * glm::rotate(glm::radians(70.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2.5));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(-58, -6, -17)) * glm::rotate(glm::radians(170.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.1));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(-34, -6, 22)) * glm::rotate(glm::radians(10.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(4.1));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(45, -6, 55)) * glm::rotate(glm::radians(45.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2.3));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(24, -6, 13)) * glm::rotate(glm::radians(77.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(46, -6, 0)) * glm::rotate(glm::radians(67.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(0, -6, 0)) * glm::rotate(glm::radians(17.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(-14, -6, 31)) * glm::rotate(glm::radians(67.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(26, -6, 11)) * glm::rotate(glm::radians(57.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(0, -6, 26)) * glm::rotate(glm::radians(47.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);
	krzakTransformation = glm::translate(glm::vec3(13, -6, -31)) * glm::rotate(glm::radians(27.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2));
	drawObjectTexture(&krzak, krzakTransformation, krzakTexture);

	//babelki
	if (boobleHeight < 30)
	{
		drawObjectTexture(&babel, glm::translate(glm::vec3(10.1f - 0.5, -4.1f + boobleHeight, 30.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(9.2f, -3.2f + boobleHeight, 32.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-8.0f, boobleHeight + 3.9f, 43.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-7.9f, boobleHeight, 43.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(13.2f, boobleHeight + 0.2f, 54.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(16.0f*sin1, boobleHeight + 8.9f, -32.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(12.1f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(16.2f*sin1, boobleHeight + 5.2f, -21.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(26.0f*sin1, boobleHeight + 2.9f, 54.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(17.1f*sin1, boobleHeight, 21.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-4.2f*sin1, boobleHeight + 1.2f, 34.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(5.0f*sin1, boobleHeight + 6.9f, 33.0f)*sin1)*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(12.1f*sin1, boobleHeight + 3.0f, 37.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(35.2f*sin1, boobleHeight + 0.3f, 39.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-34.0f*sin1, boobleHeight + 0.5f, -23.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-6.1f*sin1, -0.1f + boobleHeight, -53.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-34.2f*sin1, -0.2f + boobleHeight, -25.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(12.0f*sin1, boobleHeight + 0.9f, -1.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-9.9f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-12.2f*sin1, boobleHeight + 0.2f, -3.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(7.0f*sin1, boobleHeight + 0.9f, -3.3f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-28.1f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(13.2f*sin1, boobleHeight + 0.2f, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(10.0f*sin1, boobleHeight + 0.9f, 30.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(11.1f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-15.2f*sin1, boobleHeight + 2.2f, -14.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-16.0f*sin1, boobleHeight + 5.9f, 53.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-17.1f*sin1, boobleHeight + 3.0f, 2.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(15.2f*sin1, boobleHeight + 2.2f, 36.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-18.0f*sin1, boobleHeight + 6.9f, 24.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(15.1f*sin1, -0.1f + boobleHeight, -23.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-14.2f*sin1, -0.2f + boobleHeight, -11.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-3.0f*sin1, boobleHeight + 7.9f, 53.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(6.9f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-4.2f*sin1, boobleHeight + 2.2f, 54.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-14.0f*sin1, boobleHeight + 0.9f, 32.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(2.1f*sin1, boobleHeight, 13.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-14.2f*sin1, boobleHeight + 1.2f, 16.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-9.0f*sin1, boobleHeight + 0.5f, -32.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-43.1f*sin1, boobleHeight, -53.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-34.2f*sin1, boobleHeight + 3.2f, 2.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-54.0f*sin1, boobleHeight - 0.9f, -1.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-65.1f*sin1, boobleHeight + 0.0f, 2.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(15.2f*sin1, boobleHeight + 0.2f, 35.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(18.0f*sin1, boobleHeight + 0.9f, 20.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(5.1f*sin1, -0.1f + boobleHeight, 21.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(3.2f*sin1, -0.2f + boobleHeight, 22.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(13.0f*sin1, boobleHeight + 0.2f, 14.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(34.9f*sin1, boobleHeight, 12.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(26.2f*sin1, boobleHeight - 4.5f, 29.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-43.0f*sin1, boobleHeight + 0.9f, 23.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-4.1f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(4.2f*sin1, boobleHeight - 4.2f, 11.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(5.0f*sin1, boobleHeight + 3.9f, 18.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-15.1f*sin1, boobleHeight, -27.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-3.2f*sin1, boobleHeight - 0.1f, -23.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-16.0f*sin1, boobleHeight - 3.9f, -17.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(0.1f*sin1, boobleHeight - 32.f, -15.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(7.2f*sin1, boobleHeight + 0.2f, -13.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(5.0f*sin1, boobleHeight + 0.9f, 5.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(40.1f - 0.5, -4.1f + boobleHeight, 30.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(39.2f, -3.2f + boobleHeight, 32.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-38.0f, boobleHeight + 3.9f, 43.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-37.9f, boobleHeight, 43.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(43.2f, boobleHeight + 0.2f, 54.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(46.0f*sin1, boobleHeight + 8.9f, -32.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(42.1f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(46.2f*sin1, boobleHeight + 5.2f, -21.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(56.0f*sin1, boobleHeight + 2.9f, 54.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(47.1f*sin1, boobleHeight, 21.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-34.2f*sin1, boobleHeight + 1.2f, 34.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(35.0f*sin1, boobleHeight + 6.9f, 33.0f)*sin1)*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(42.1f*sin1, boobleHeight + 3.0f, 37.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(65.2f*sin1, boobleHeight + 0.3f, 39.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-54.0f*sin1, boobleHeight + 0.5f, -23.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-36.1f*sin1, -0.1f + boobleHeight, -53.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-54.2f*sin1, -0.2f + boobleHeight, -25.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(42.0f*sin1, boobleHeight + 0.9f, -1.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-39.9f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-42.2f*sin1, boobleHeight + 0.2f, -3.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(37.0f*sin1, boobleHeight + 0.9f, -3.3f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-58.1f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(43.2f*sin1, boobleHeight + 0.2f, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(40.0f*sin1, boobleHeight + 0.9f, 30.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(41.1f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-45.2f*sin1, boobleHeight + 2.2f, -14.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-46.0f*sin1, boobleHeight + 5.9f, 53.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-47.1f*sin1, boobleHeight + 3.0f, 2.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(45.2f*sin1, boobleHeight + 2.2f, 36.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-58.0f*sin1, boobleHeight + 6.9f, 24.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(45.1f*sin1, -0.1f + boobleHeight, -23.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-44.2f*sin1, -0.2f + boobleHeight, -11.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-33.0f*sin1, boobleHeight + 7.9f, 53.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(36.9f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-34.2f*sin1, boobleHeight + 2.2f, 54.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-44.0f*sin1, boobleHeight + 0.9f, 32.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(32.1f*sin1, boobleHeight, 13.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-44.2f*sin1, boobleHeight + 1.2f, 16.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-39.0f*sin1, boobleHeight + 0.5f, -32.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-53.1f*sin1, boobleHeight, -53.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-54.2f*sin1, boobleHeight + 3.2f, 2.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-34.0f*sin1, boobleHeight - 0.9f, -1.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-55.1f*sin1, boobleHeight + 0.0f, 2.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(45.2f*sin1, boobleHeight + 0.2f, 35.0f*sin1))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(48.0f*sin1, boobleHeight + 0.9f, 20.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(35.1f*sin1, -0.1f + boobleHeight, 21.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(33.2f*sin1, -0.2f + boobleHeight, 22.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(43.0f*sin1, boobleHeight + 0.2f, 14.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(54.9f*sin1, boobleHeight, 12.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(46.2f*sin1, boobleHeight - 4.5f, 29.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-53.0f*sin1, boobleHeight + 0.9f, 23.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-44.1f*sin1, boobleHeight, 30.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(34.2f*sin1, boobleHeight - 4.2f, 11.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(35.0f*sin1, boobleHeight + 3.9f, 18.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-45.1f*sin1, boobleHeight, -27.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-33.2f*sin1, boobleHeight - 0.1f, -23.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(-46.0f*sin1, boobleHeight - 3.9f, -17.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(30.1f*sin1, boobleHeight - 32.f, -15.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(37.2f*sin1, boobleHeight + 0.2f, -13.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		drawObjectTexture(&babel, glm::translate(glm::vec3(35.0f*sin1, boobleHeight + 0.9f, 5.0f))*glm::scale(glm::vec3(0.15f)), babelTexture);
		boobleHeight += 0.14f;
	}
	else {
		boobleHeight = 0;
	}
	//sphere model to check if something works
	//drawObjectTexture(&sphereModel, glm::translate(glm::vec3(0, 0, 0)), textureRock);

	//drawObjectColor(&seaSurfaceModel, glm::translate(glm::vec3(0, 10, 0)) * glm::scale(glm::vec3(4.0f)) , glm::vec3(0.1f), 0.01f);
	drawObjectTexture(&seabedModel, glm::translate(glm::vec3(0, -7, 0)) * glm::scale(glm::vec3(1.2f)), textureSeabed);

	//skybox
	glm::mat4 skyboxTransformation = glm::translate(glm::vec3(0,40,-60)) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(1.2f));
	drawObjectTexture(&seabedModel, skyboxTransformation, skyboxTexture01);

	skyboxTransformation = glm::translate(glm::vec3(60, 40, 0)) * glm::rotate(glm::radians(180.0f), glm::vec3(-1, 1, 0)) * glm::scale(glm::vec3(1.2f));
	drawObjectTexture(&seabedModel, skyboxTransformation, skyboxTexture02);

	skyboxTransformation = glm::translate(glm::vec3(0, 40, 60)) * glm::rotate(glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(1.2f));
	drawObjectTexture(&seabedModel, skyboxTransformation, skyboxTexture03);

	skyboxTransformation = glm::translate(glm::vec3(-60, 40, 0)) * glm::rotate(glm::radians(180.0f), glm::vec3(1, -1, 0)) * glm::scale(glm::vec3(1.2f));
	drawObjectTexture(&seabedModel, skyboxTransformation, skyboxTexture04);

	glutSwapBuffers();
}

void init()
{
	srand(time(0));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//load shaders
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");

	//load models
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	fishModel = obj::loadModelFromFile("models/fish.obj");
	rockModel = obj::loadModelFromFile("models/stone.obj");
	seaSurfaceModel = obj::loadModelFromFile("models/flat.obj");
	seabedModel = obj::loadModelFromFile("models/flat.obj");
	skyboxModel = obj::loadModelFromFile("models/skybox3.obj");
	skybox = obj::loadModelFromFile("models/skybox3.obj");
	krzak = obj::loadModelFromFile("models/krzak.obj");
	babel = obj::loadModelFromFile("models/babel.obj");

	//load textures
	textureAsteroid = Core::LoadTexture("textures/asteroid.png");
	textureFish = Core::LoadTexture("textures/fishTexture.png");
	textureRock = Core::LoadTexture("textures/rock2.png");
	textureSeabed = Core::LoadTexture("textures/ground.png");
	skyboxTexture01 = Core::LoadTexture("textures/01.png");
	skyboxTexture02 = Core::LoadTexture("textures/02.png");
	skyboxTexture03 = Core::LoadTexture("textures/03.png");
	skyboxTexture04 = Core::LoadTexture("textures/04.png");
	skyboxTexture05 = Core::LoadTexture("textures/05.png");
	krzakTexture = Core::LoadTexture("textures/krzak.png");
	babelTexture = Core::LoadTexture("textures/babelTexture.png");
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Projekt GRK");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
