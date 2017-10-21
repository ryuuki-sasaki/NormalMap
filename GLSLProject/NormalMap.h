//=====================================
//制作者：佐々木隆貴
//日付：2016/5/9
//=====================================
#ifndef NORMAL_MAP__H
#define NORMAL_MAP__H

#include <glm/glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;


class GLSLProgram;
class DDSImage;
class VBOCube;

class NormalMap
{
public:
	GLSLProgram* prog;	//プログラムオブジェクト
	DDSImage* textures;	//テクスチャ
	VBOCube* cube;	//キューブ

	//行列
	mat4 model;
	mat4 view;
	mat4 projection;

	int windowWidth = 512;		//ウィンドウ幅
	int windowHeight = 512;		//ウィンドウ高さ

public:
	NormalMap(){ ; }
	~NormalMap();
	void Render();
	void InitShader();
	void Resize(int w, int h);
	void LoadTexture(const char *diffuseTex, const char *normalMap, const char *heightlMap);
	void SetTexture();
	void MouseInfo(int button, int state, int x, int y);
	void MotionInfo(int x, int y);

private:
	void SetMatrices();
	void CompileAndLinkShader();
};

#endif