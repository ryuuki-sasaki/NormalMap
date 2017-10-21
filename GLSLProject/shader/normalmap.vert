//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/9
//=====================================
#version 430
 
//���͕ϐ�
layout (location=0) in vec3 vertexPosition;	
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 vertexTexCoord;

struct lightInfo
{
	vec4 position;	//���_���W�ł̃��C�g�ʒu
	vec3 intensity;	//ADS�̌��̋��x
};
uniform lightInfo light;

//�o�͕ϐ�
out vec3 lightDir;
out vec3 viewDir;
out vec2 texCoord;

//���j�t�H�[���ϐ�
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 MVP;	//���e  * ���f���r���[

void main()
{
	//�@�������_���W�ɕϊ�
	vec3 norm = normalize(normalMatrix * vertexNormal);
	//�ڐ����v�Z
	vec3 t = cross(norm, vec3(0.0, 1.0, 0.0));
	//�ڐ������_���W�ɕ�
	vec3 tang = normalize(normalMatrix * vec3(t));


	//�]�@�����v�Z
	vec3 binormal = cross(norm, t);

	//�ڋ�Ԃɕϊ����邽�߂̍s��	
	mat3 toObjectLoccal = mat3(tang.x, binormal.x, norm.x,
							   tang.y, binormal.y, norm.y,
							   tang.z, binormal.z, norm.z);

	//�ʒu�����_���W�œ���	
	vec3 pos = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));

	//���C�g�����Ǝ���������ڋ�Ԃɕϊ�
	lightDir = normalize(toObjectLoccal * (light.position.xyz - pos));
	viewDir = toObjectLoccal * normalize (-pos);

	//�e�N�X�`�����W��n��
	texCoord = vertexTexCoord;

	//�ʒu���N���b�v���W�ɕϊ����Č�ɓn��
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}
