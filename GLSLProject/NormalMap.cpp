//=====================================
//制作者：佐々木隆貴
//日付：2016/5/9
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
//テクスチャのロード
//*fileName:読み込むファイル
//=====================================
void NormalMap::LoadTexture(const char *diffuseTex, const char *normalMap, const char *heightlMap)
{
	textures = new DDSImage[3];
	//アクティブなテクスチャユニットをGL_TEXTUREnに設定
	glActiveTexture(GL_TEXTURE0);
	textures[0].Load(diffuseTex);

	glActiveTexture(GL_TEXTURE1);
	textures[1].Load(normalMap);

	glActiveTexture(GL_TEXTURE2);
	textures[2].Load(heightlMap);
}

//=====================================
//テクスチャのセット
//=====================================
void NormalMap::SetTexture()
{
	GLuint programHandle = prog->getHandle();

	//サンプラーユニフォーム変数がglActiveTextureで指定したテクスチャユニットnを参照するように設定
	int loc = glGetUniformLocation(programHandle, "colorTex");
	if (loc >= 0)
		glUniform1i(loc, 0);
	else
		fprintf(stderr, "ユニフォーム変数 colorTex が見つかりません\n");

	loc = glGetUniformLocation(programHandle, "normalMapTex");
	if (loc >= 0)
		glUniform1i(loc, 1);
	else
		fprintf(stderr, "ユニフォーム変数 normalMapTex が見つかりません\n");

	loc = glGetUniformLocation(programHandle, "heightMapTex");
	if (loc >= 0)
		glUniform1i(loc, 2);
	else
		fprintf(stderr, "ユニフォーム変数 heightMapTex が見つかりません\n");
}

//=====================================
//シェーダー初期化
//=====================================
void NormalMap::InitShader()
{
	prog = new GLSLProgram();
	CompileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	cube = new VBOCube();

	//ビュー
	view = view = glm::lookAt(vec3(-1.0f, 0.25f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	//プロジェクション
	projection = mat4(1.0f);

	prog->setUniform("light.intensity", vec3(0.9f, 0.9f, 0.9f));
}

//=====================================
//描画
//=====================================
void NormalMap::Render()
{
	//ウィンドウを塗りつぶす(カラーバッファ、デプスバッファ指定)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//視点の設定
	camera.Set();

	//ビュー
	view = camera.GetView();

	//ユニフォーム変数に値設定
	prog->setUniform("light.position", vec4(0.0f, 0.0f, 0.0f, 1.0f));	//光源の位置
	prog->setUniform("material.ka", 0.2f, 0.2f, 0.2f);	//アンビエント　反射率
	prog->setUniform("material.ks", 0.05f, 0.05f, 0.05f);	//スペキュラ	反射率
	prog->setUniform("material.shininess", 1.0f);	//スペキュラの輝き係数(値が大きくなるに連れてハイライトは小さくなる)

	model = mat4(1.0f);
	SetMatrices();
	cube->render();	
}

//=====================================
//マテリアル設定
//=====================================
void NormalMap::SetMatrices()
{
	mat4 mv = view * model;
	prog->setUniform("modelViewMatrix", mv);	//モデルビュー行列
	prog->setUniform("normalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));	//モデルビュー行列(vvec3との計算のため、3 * 3)
	prog->setUniform("MVP", projection * mv);	//投影  * モデルビュー行列
}

//=====================================
//ウィンドウの拡大縮小に対する処理
//=====================================
void NormalMap::Resize(int w, int h)
{
	glViewport(0, 0, w, h);
	windowWidth = w;
	windowHeight = h;
	//プロジェクション設定
	projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

//=========================================
//マウスによる入力
//button:押されたボタンの情報
//state:ボタンが押されたか
//x,y:位置
//=========================================-
void NormalMap::MouseInfo(int button, int state, int x, int y)
{
	camera.MouseInput(button, state, x, y);
}

//=========================================
//マウスドラッグ時
//=========================================
void NormalMap::MotionInfo(int x, int y)
{
	camera.MouseMotion(x, y);
}

//=====================================
//プログラムオブジェクト作成
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