//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/9
//=====================================
//include
#include "NormalMap.h"
#include "DDSLoader.h"
#include "VBOCube.h"
#include <iostream>
#include <GL/glew.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include "glslprogram.h"
#include "Mouse.h"
using namespace std;

//link
#pragma comment (lib, "glew32.lib")

ViewCamera camera(3.0);

NormalMap::~NormalMap()
{
	delete[] textures;
	delete prog;
	textures = 0;
	prog = 0;
}

//=====================================
//�e�N�X�`���̃��[�h
//*fileName:�ǂݍ��ރt�@�C��
//=====================================
void NormalMap::LoadTexture(const char *diffuseTex, const char *normalMap, const char *heightlMap)
{
	textures = new DDSImage[3];
	//�A�N�e�B�u�ȃe�N�X�`�����j�b�g��GL_TEXTUREn�ɐݒ�
	glActiveTexture(GL_TEXTURE0);
	textures[0].Load(diffuseTex);

	glActiveTexture(GL_TEXTURE1);
	textures[1].Load(normalMap);

	glActiveTexture(GL_TEXTURE2);
	textures[2].Load(heightlMap);
}

//=====================================
//�e�N�X�`���̃Z�b�g
//=====================================
void NormalMap::SetTexture()
{
	GLuint programHandle = prog->getHandle();

	//�T���v���[���j�t�H�[���ϐ���glActiveTexture�Ŏw�肵���e�N�X�`�����j�b�gn���Q�Ƃ���悤�ɐݒ�
	int loc = glGetUniformLocation(programHandle, "colorTex");
	if (loc >= 0)
		glUniform1i(loc, 0);
	else
		fprintf(stderr, "���j�t�H�[���ϐ� colorTex ��������܂���\n");

	loc = glGetUniformLocation(programHandle, "normalMapTex");
	if (loc >= 0)
		glUniform1i(loc, 1);
	else
		fprintf(stderr, "���j�t�H�[���ϐ� normalMapTex ��������܂���\n");

	loc = glGetUniformLocation(programHandle, "heightMapTex");
	if (loc >= 0)
		glUniform1i(loc, 2);
	else
		fprintf(stderr, "���j�t�H�[���ϐ� heightMapTex ��������܂���\n");
}

//=====================================
//�V�F�[�_�[������
//=====================================
void NormalMap::InitShader()
{
	prog = new GLSLProgram();
	CompileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	cube = new VBOCube();

	//�r���[
	view = view = glm::lookAt(vec3(-1.0f, 0.25f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	//�v���W�F�N�V����
	projection = mat4(1.0f);

	prog->setUniform("light.intensity", vec3(0.9f, 0.9f, 0.9f));
}

//=====================================
//�`��
//=====================================
void NormalMap::Render()
{
	//�E�B���h�E��h��Ԃ�(�J���[�o�b�t�@�A�f�v�X�o�b�t�@�w��)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//���_�̐ݒ�
	camera.Set();

	//�r���[
	view = camera.GetView();

	//���j�t�H�[���ϐ��ɒl�ݒ�
	prog->setUniform("light.position", vec4(0.0f, 0.0f, 0.0f, 1.0f));	//�����̈ʒu
	prog->setUniform("material.ka", 0.2f, 0.2f, 0.2f);	//�A���r�G���g�@���˗�
	prog->setUniform("material.ks", 0.05f, 0.05f, 0.05f);	//�X�y�L����	���˗�
	prog->setUniform("material.shininess", 1.0f);	//�X�y�L�����̋P���W��(�l���傫���Ȃ�ɘA��ăn�C���C�g�͏������Ȃ�)

	model = mat4(1.0f);
	SetMatrices();
	cube->render();	
}

//=====================================
//�}�e���A���ݒ�
//=====================================
void NormalMap::SetMatrices()
{
	mat4 mv = view * model;
	prog->setUniform("modelViewMatrix", mv);	//���f���r���[�s��
	prog->setUniform("normalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));	//���f���r���[�s��(vvec3�Ƃ̌v�Z�̂��߁A3 * 3)
	prog->setUniform("MVP", projection * mv);	//���e  * ���f���r���[�s��
}

//=====================================
//�E�B���h�E�̊g��k���ɑ΂��鏈��
//=====================================
void NormalMap::Resize(int w, int h)
{
	glViewport(0, 0, w, h);
	windowWidth = w;
	windowHeight = h;
	//�v���W�F�N�V�����ݒ�
	projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

//=========================================
//�}�E�X�ɂ�����
//button:�����ꂽ�{�^���̏��
//state:�{�^���������ꂽ��
//x,y:�ʒu
//=========================================-
void NormalMap::MouseInfo(int button, int state, int x, int y)
{
	camera.MouseInput(button, state, x, y);
}

//=========================================
//�}�E�X�h���b�O��
//=========================================
void NormalMap::MotionInfo(int x, int y)
{
	camera.MouseMotion(x, y);
}

//=====================================
//�v���O�����I�u�W�F�N�g�쐬
//=====================================
void NormalMap::CompileAndLinkShader()
{
	try {
		prog->compileShader("shader/normalmap.vert");
		prog->compileShader("shader/normalmap.frag");
		prog->link();
		prog->use();
		prog->printActiveUniforms();
		prog->printActiveUniformBlocks();
		prog->printActiveAttribs();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}