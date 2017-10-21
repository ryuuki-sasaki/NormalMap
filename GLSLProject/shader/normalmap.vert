//=====================================
//制作者：佐々木隆貴
//日付：2016/5/9
//=====================================
#version 430
 
//入力変数
layout (location=0) in vec3 vertexPosition;	
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 vertexTexCoord;

struct lightInfo
{
	vec4 position;	//頂点座標でのライト位置
	vec3 intensity;	//ADSの光の強度
};
uniform lightInfo light;

//出力変数
out vec3 lightDir;
out vec3 viewDir;
out vec2 texCoord;

//ユニフォーム変数
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 MVP;	//投影  * モデルビュー

void main()
{
	//法線を視点座標に変換
	vec3 norm = normalize(normalMatrix * vertexNormal);
	//接線を計算
	vec3 t = cross(norm, vec3(0.0, 1.0, 0.0));
	//接線を視点座標に変
	vec3 tang = normalize(normalMatrix * vec3(t));


	//従法線を計算
	vec3 binormal = cross(norm, t);

	//接空間に変換するための行列	
	mat3 toObjectLoccal = mat3(tang.x, binormal.x, norm.x,
							   tang.y, binormal.y, norm.y,
							   tang.z, binormal.z, norm.z);

	//位置を視点座標で得る	
	vec3 pos = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));

	//ライト方向と視線方向を接空間に変換
	lightDir = normalize(toObjectLoccal * (light.position.xyz - pos));
	viewDir = toObjectLoccal * normalize (-pos);

	//テクスチャ座標を渡す
	texCoord = vertexTexCoord;

	//位置をクリップ座標に変換して後に渡す
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}
