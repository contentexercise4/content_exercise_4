#version 330 core
// アウトプットデータ
out vec3 color;
// 頂点シェーダから書き込まれた色
//in vec3 fragmentColor;
// 頂点シェーダからの値を書き込みます
in vec2 UV;
// すべてのメッシュで一定の値
uniform sampler2D myTextureSampler;
void main() {
	//color = vec3(1, 0, 0);
	
	// アウトプットカラー = 頂点シェーダで指定された、
    // 周辺の3つの頂点間で書き込まれた色
    //color = fragmentColor;
    
     // アウトプットカラー = 指定したUV座標のテクスチャの色
    color = texture( myTextureSampler, UV ).rgb;
}