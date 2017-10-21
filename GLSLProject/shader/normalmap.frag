//=====================================
//制作者：佐々木隆貴
//日付：2016/5/9
//=====================================
#version 430
 
 //入力属性
in vec3 lightDir;
in vec3 viewDir;
in vec2 texCoord;

//サンプラー変数
//テクスチャユニットを参照する
uniform sampler2D colorTex;	
uniform sampler2D normalMapTex;
uniform sampler2D heightMapTex;

struct lightInfo
{
	vec4 position;	//頂点座標でのライト位置
	vec3 intensity;	//ADSの光の強度
};
uniform lightInfo light;

struct materialInfo
{
	vec3 ka;		//アンビエント　反射率
	vec3 ks;		//スペキュラ	反射率
	float shininess;	//スペキュラ輝き係数
};
uniform materialInfo material;

layout (location=0) out vec4 flagColor;	//出力変数

//=====================================
//光の強度を求める
//=====================================
vec3 PhongModel( vec3 norm, vec3 diffR )
{
	vec3 r = reflect(-lightDir, norm);	//光源からの入射ベクトルに対する反射ベクトル

	vec3 ambient = light.intensity * material.ka;	//アンビエント成分計算

	float sDotN = max(dot(lightDir, norm), 0.0);	//正規化されたベクトルの内積をとる(dotで0以下(角度90度を超える)の値を値をとらないようにする)
	vec3 diffuse = light.intensity * diffR * sDotN;	//ディフューズ成分計算

	vec3 spec = vec3(0.0);

	//サーフェイスに到達する光があるか
	if(sDotN > 0)
	{
		spec = light.intensity * material.ks * pow(max(dot(r, viewDir), 0.0), material.shininess);	//スペキュラ成分計算
	}

	return ambient + diffuse + spec;
}

void main()
{
	//高さマップから高さベクトルを取り出す
	float height = texture(heightMapTex, texCoord).a;

	//視差分だけテクスチャ座標をずらす
	vec2 toTexCoord = texCoord.xy - viewDir.xy * height * 0.05;

	//法線マップから法線を取り出す
	vec4 normal = 2.0 * texture(normalMapTex, toTexCoord) - 1.0;

	//色テクスチャをディフューズ反射率として扱う
	vec4 texColor = texture(colorTex, toTexCoord);

	flagColor = vec4(PhongModel(normal.xyz, texColor.rgb), 1.0);	
}													  
