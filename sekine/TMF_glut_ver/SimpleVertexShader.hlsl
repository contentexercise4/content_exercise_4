#version 330 core
// すべてのメッシュで一定の値
uniform mat4 MVP;

// インプット頂点データ。このシェーダの実行ごとに異なります。
layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec3 vertexColor;
layout(location = 1) in vec2 vertexUV;

// アウトプットデータ。各頂点に書き込まれます。
//out vec3 fragmentColor;
out vec2 UV;
void main(){

    // クリップ空間での、頂点のアウトプット位置：MVP * 位置
    vec4 v = vec4(vertexPosition_modelspace,1); // 同次4Dベクトルに変換します。覚えていますか？
    gl_Position = MVP * v;
    
    // 各フラグメントの色を作るために
    // 各頂点の色は書き込まれます。
    //fragmentColor = vertexColor;
    
    // 頂点のUV座標です。特別な空間はありません。
    UV = vertexUV;
    
   //gl_Position.xyz = vertexPosition_modelspace;
	//gl_Position.w = 1.0;
}
