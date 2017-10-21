//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/9
//=====================================
#version 430
 
 //���͑���
in vec3 lightDir;
in vec3 viewDir;
in vec2 texCoord;

//�T���v���[�ϐ�
//�e�N�X�`�����j�b�g���Q�Ƃ���
uniform sampler2D colorTex;	
uniform sampler2D normalMapTex;
uniform sampler2D heightMapTex;

struct lightInfo
{
	vec4 position;	//���_���W�ł̃��C�g�ʒu
	vec3 intensity;	//ADS�̌��̋��x
};
uniform lightInfo light;

struct materialInfo
{
	vec3 ka;		//�A���r�G���g�@���˗�
	vec3 ks;		//�X�y�L����	���˗�
	float shininess;	//�X�y�L�����P���W��
};
uniform materialInfo material;

layout (location=0) out vec4 flagColor;	//�o�͕ϐ�

//=====================================
//���̋��x�����߂�
//=====================================
vec3 PhongModel( vec3 norm, vec3 diffR )
{
	vec3 r = reflect(-lightDir, norm);	//��������̓��˃x�N�g���ɑ΂��锽�˃x�N�g��

	vec3 ambient = light.intensity * material.ka;	//�A���r�G���g�����v�Z

	float sDotN = max(dot(lightDir, norm), 0.0);	//���K�����ꂽ�x�N�g���̓��ς��Ƃ�(dot��0�ȉ�(�p�x90�x�𒴂���)�̒l��l���Ƃ�Ȃ��悤�ɂ���)
	vec3 diffuse = light.intensity * diffR * sDotN;	//�f�B�t���[�Y�����v�Z

	vec3 spec = vec3(0.0);

	//�T�[�t�F�C�X�ɓ��B����������邩
	if(sDotN > 0)
	{
		spec = light.intensity * material.ks * pow(max(dot(r, viewDir), 0.0), material.shininess);	//�X�y�L���������v�Z
	}

	return ambient + diffuse + spec;
}

void main()
{
	//�����}�b�v���獂���x�N�g�������o��
	float height = texture(heightMapTex, texCoord).a;

	//�����������e�N�X�`�����W�����炷
	vec2 toTexCoord = texCoord.xy - viewDir.xy * height * 0.05;

	//�@���}�b�v����@�������o��
	vec4 normal = 2.0 * texture(normalMapTex, toTexCoord) - 1.0;

	//�F�e�N�X�`�����f�B�t���[�Y���˗��Ƃ��Ĉ���
	vec4 texColor = texture(colorTex, toTexCoord);

	flagColor = vec4(PhongModel(normal.xyz, texColor.rgb), 1.0);	
}													  
