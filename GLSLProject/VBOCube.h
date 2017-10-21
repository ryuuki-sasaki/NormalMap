//======================================
//	参考図書:OpenGL4.0シェーディング言語
//	付属コード使用
//	制作者:David Wolff.
//======================================
#ifndef VBOCUBE_H
#define VBOCUBE_H

class VBOCube
{
private:
	unsigned int vaoHandle;

public:
	VBOCube();
	void render();
};

#endif // VBOCUBE_H