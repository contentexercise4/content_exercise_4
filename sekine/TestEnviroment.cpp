// TMFSample.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include"pch.h"
#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include <algorithm>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtx/transform.hpp> // <glm/glm.hpp>の後
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include<GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define N 7

using namespace glm;

double lastTime;
glm::mat4 ProjectionM;
glm::mat4 ViewM;
// 水平角、-Z方向
float horizontalAngle = 3.14f;
// 鉛直角、0、水平線を眺めている
float verticalAngle = 0.0f;
// 位置
glm::vec3 position = glm::vec3(0, 0, 5);
int vercount = 0;
int vercount2 = 0;
double cube[47][4] = { 0 }; //cube[47][x_max,x_min,z_max,z_min]
int ModeSelect = 0;			//画面遷移に用いる. 0:スタート画面 1:プレイ画面 2:リザルト画面

// 3頂点を表す3つのベクトルの配列

// 頂点。3つの連続する数字は3次元の頂点です。3つの連続する頂点は三角形を意味します。
// 立方体は、それぞれが2枚の三角形からなる6面から成ります。だから6*2=12枚の三角形と12*3個の頂点を作ります。
static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // 三角形1:開始
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // 三角形1:終了
	1.0f, 1.0f,-1.0f, // 三角形2:開始
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // 三角形2:終了
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};


// 各頂点に一つの色。これらはランダムに作られました。
static const GLfloat g_color_buffer_data[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};

// 各頂点に2つの値、これらはBlenderで作りました。どうやって作るかはこれから説明します。
static const GLfloat g_uv_buffer_data[] = {
	0.000059f, 1.0f - 0.000004f,
	0.000103f, 1.0f - 0.336048f,
	0.335973f, 1.0f - 0.335903f,
	1.000023f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.336024f, 1.0f - 0.671877f,
	0.667969f, 1.0f - 0.671889f,
	1.000023f, 1.0f - 0.000013f,
	0.668104f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.000059f, 1.0f - 0.000004f,
	0.335973f, 1.0f - 0.335903f,
	0.336098f, 1.0f - 0.000071f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.336024f, 1.0f - 0.671877f,
	1.000004f, 1.0f - 0.671847f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.668104f, 1.0f - 0.000013f,
	0.335973f, 1.0f - 0.335903f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.668104f, 1.0f - 0.000013f,
	0.336098f, 1.0f - 0.000071f,
	0.000103f, 1.0f - 0.336048f,
	0.000004f, 1.0f - 0.671870f,
	0.336024f, 1.0f - 0.671877f,
	0.000103f, 1.0f - 0.336048f,
	0.336024f, 1.0f - 0.671877f,
	0.335973f, 1.0f - 0.335903f,
	0.667969f, 1.0f - 0.671889f,
	1.000004f, 1.0f - 0.671847f,
	0.667979f, 1.0f - 0.335851f
};

GLuint loadBMP_custom(const char * imagepath) {
	// BMPファイルのヘッダから読み込まれるデータ
	unsigned char header[54]; // 各BMPファイルは54バイトのヘッダから始まります。
	unsigned int dataPos;     // 実際のデータがファイルのどの位置にあるか
	unsigned int width, height;
	unsigned int imageSize;   // = 横*高さ*3
	// 実際のRGBデータ
	unsigned char * data;

	// ファイルを開きます。
	FILE * file;
	//= fopen(imagepath, "rb");
	errno_t error;
	error = fopen_s(&file, imagepath, "rb");



	if (error) {
		printf("画像が開けませんでした。n");
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) { // 54バイト読み込めなければ、問題があります。
		printf("BMPファイルではありません。n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("BMPファイルではありません。n");
		return 0;
	}

	// バイト配列から整数を読み込む
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// BMPファイルの中にはミスフォーマットがあります。その情報を推測します。
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : 1バイトは赤、緑、青の各色です。
	if (dataPos == 0)      dataPos = 54; // これでBMPヘッダは終わりです。

	// バッファを作る
	data = new unsigned char[imageSize];

	// ファイルから実際のデータをバッファに読み込む
	fread(data, 1, imageSize, file);

	//すべてはメモリ上にあるので、ファイルは閉じます。
	fclose(file);

	// ひとつのOpenGLテクスチャを作ります。
	GLuint textureID;
	glGenTextures(1, &textureID);

	// 新たに作られたテクスチャを"バインド"します。つまりここから後のテクスチャ関数はこのテクスチャを変更します。
	glBindTexture(GL_TEXTURE_2D, textureID);

	// OpenGLに画像を渡します。
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// 画像を拡大(MAGnifying)するときは線形(LINEAR)フィルタリングを使います。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 画像を縮小(MINifying)するとき、線形(LINEAR)フィルタした、二つのミップマップを線形(LINEARYLY)に混ぜたものを使います。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// 次のようにしてミップマップを作ります。
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;

}

GLuint loadTGA_glfw(const char * imagepath) {

	// 一つのOpenGLテクスチャを作ります。
	GLuint textureID;
	glGenTextures(1, &textureID);

	// 新しく作ったテクスチャを"バインド"します。これ以降の関数はすべてこのテクスチャを変更します。
	glBindTexture(GL_TEXTURE_2D, textureID);

	// 正しいパラメータでglTexImage2Dを呼んで、ファイルを読み込みます。
	//glfwLoadTexture2D(imagepath, 0);

	// 3つの線形フィルタリング
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// 作成したテクスチャのIDを返します。
	return textureID;
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// シェーダを作ります。
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// ファイルから頂点シェーダのコードを読み込みます。
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}

	// ファイルからフラグメントシェーダを読み込みます。
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// 頂点シェーダをコンパイルします。
	printf("Compiling shader : %sn", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// 頂点シェーダをチェックします。
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	//std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	//glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	//fprintf(stdout, "%sn", &VertexShaderErrorMessage[0]);

	// フラグメントシェーダをコンパイルします。
	printf("Compiling shader : %sn", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// フラグメントシェーダをチェックします。
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	//std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	//glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	//fprintf(stdout, "%sn", &FragmentShaderErrorMessage[0]);


	// プログラムをリンクします。
	fprintf(stdout, "Linking programn");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// プログラムをチェックします。
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%sn", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	glUseProgram(ProgramID);


	//GLuint ProgramID = glCreateProgram();
	return ProgramID;
}

bool hit(double cube[47][4]) {
	for (int i = 0; i < 47; i++) {
		if ((position.x < cube[i][0] && position.x > cube[i][1]) &&
			(position.z < cube[i][2] && position.z > cube[i][3])) {
			return true;
		}
	}
}

void decideCube(std::vector<glm::vec3> &vertices) {
	double x_max = 0, x_min = 0, z_max = 0, z_min = 0;
	double tmp;
	int ik = 0, il = 0;
	int k = 1, l = 1;

	while (1) {
		if (ik % 36 == 0) {
			tmp = vertices[ik].x;
			ik++;
		}
		while (tmp == vertices[ik].x) {
			ik++;
		}
		if (tmp > vertices[ik].x) {
			x_max = tmp;
			x_min = vertices[ik].x;
		}
		else {
			x_max = vertices[ik].x;
			x_min = tmp;
		}
		cube[k - 1][0] = x_max;
		cube[k - 1][1] = x_min;
		ik = k * 36;
		k++;
		if (k == 48) { break; }
	}
	while (1) {
		if (il % 36 == 0) {
			tmp = vertices[il].z;
			il++;
		}
		while (tmp == vertices[il].z) {
			il++;
		}
		if (tmp > vertices[il].z) {
			z_max = tmp;
			z_min = vertices[il].z;
		}
		else {
			z_max = vertices[il].z;
			z_min = tmp;
		}
		cube[l - 1][2] = z_max;
		cube[l - 1][3] = z_min;
		il = l * 36;
		l++;
		if (l == 48) { break; }
	}

	std::cout << k << "   " << l << "     " << (int)(vertices.size()) / 36 << "\n";
}

void computeMatricesFromInputs(GLFWwindow* window) {


	// 位置
	//glm::vec3 position = glm::vec3(0, 0, 5);

	// 水平角、-Z方向
	//float horizontalAngle = 3.14f;

	// 鉛直角、0、水平線を眺めている
	//float verticalAngle = 0.0f;

	// 初期視野
	float initialFoV = 45.0f;

	float speed = 4.0f; // 3 units / second
	float mouseSpeed = 0.005f;

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;


	int xpos = 512, ypos = 384;
	// 右を向きます。
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		xpos -= 156;
	}
	//左を向きます。
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		xpos += 156;
	}
	// 上を向きます。
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		ypos += 96;
	}
	// 下を向きます。
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		ypos -= 96;
	}

	// 新たな方向を計算します。
	horizontalAngle += mouseSpeed * deltaTime * float(1024 / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(768 / 2 - ypos);

	// 方向：球面座標から直角座標に変換します。
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// 右ベクトル
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// 上ベクトル：右と前ベクトルに垂直
	glm::vec3 up = glm::cross(right, direction);

	// 前へ動きます。
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
		if (hit(cube) == true) {
			position -= direction * deltaTime * speed;
		}
	}
	// 後ろへ動きます。
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
		if (hit(cube) == true) {
			position += direction * deltaTime * speed;
		}
	}
	// 前を向いたまま、右へ平行移動します。
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
		if (hit(cube) == true) {
			position -= right * deltaTime * speed;
		}
	}
	// 前を向いたまま、左へ平行移動します。
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
		if (hit(cube) == true) {
			position += right * deltaTime * speed;
		}
	}

	float FoV = 45.0f;
	// ズームします。
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		FoV = initialFoV - 5 * 3.0f;
	}
	// ズームアウトします。
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		FoV = initialFoV + 5 * 3.0f;
	}

	// Enterキーを押したらプレイ画面に遷移します
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		ModeSelect = 1;
	}

	// 射影行列：視野45&deg;、4:3比、描画範囲0.1単位100単位
	ProjectionM = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	//ProjectionM = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// カメラ行列
	ViewM = glm::lookAt(
		position,           // カメラの位置
		position + direction, // 目標地点
		up                  // 上方向 (0,-1,0にセットすると上下逆さまになります。)
	);

}

glm::mat4  getProjectionMatrix() {

	//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	return ProjectionM;
}

glm::mat4 getViewMatrix() {

	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(4, 3, 3), // ワールド空間でカメラは(4,3,3)にあります。
	//	glm::vec3(0, 0, 0), // 原点を見ています。
	//	glm::vec3(0, 1, 0)  // 頭が上方向(0,-1,0にセットすると上下逆転します。)
	//);
	//return View;

	return ViewM;
}

bool loadOBJ(const char * path, std::vector<glm::vec3>  & out_vertices, std::vector<glm::vec2>  & out_uvs, std::vector<glm::vec3>  & out_normals, int &ver) {

	std::vector<int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	//FILE * file = fopen(path, "r");
	//if (file == NULL) {
	//	printf("ファイルを開けません!n");
	//	return false;
	//}

	// ファイルを開きます。
	FILE * file;
	errno_t error;
	error = fopen_s(&file, path, "r");
	char lineHeader[2048];
	while (1) {

		// 行の最初の文字列を読み込みます。
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF) {
			printf("b");
			// 各三角形の各頂点
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {
				unsigned int vertexIndexa = vertexIndices[i];
				glm::vec3 vertex = temp_vertices[vertexIndexa - 1];
				out_vertices.push_back(vertex);
				ver++;
				//std::cout << vertexIndex;
			}
			printf("v");

			for (unsigned int i = 0; i < uvIndices.size(); i++) {
				//for (unsigned int i = 0; i < 12; i++) {
				unsigned int uvIndexa = uvIndices[i];
				glm::vec2 uv;
				uv = temp_uvs[uvIndexa - 1];
				out_uvs.push_back(uv);
			}

			printf("u");
			for (unsigned int i = 0; i < normalIndices.size(); i++) {
				unsigned int normalIndexa = normalIndices[i];
				glm::vec3 normal = temp_normals[normalIndexa - 1];
				out_normals.push_back(normal);
			}
			printf("a");
			break; // EOF = End Of File. ループを終了します。
		}
		// そうでなければlineHeaderをパースします。

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %fn", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			printf("v");
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf_s(file, "%f %fn", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
			printf("vt");
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf_s(file, "%f %f %fn", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
			printf("vn");
		}
		else if (strcmp(lineHeader, "f") == 0) {
			printf("f");
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%dn", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			//int matches = fscanf_s(file, "%d//%d %d//%d %d//%dn", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other optionsn");
				return false;
			}
			printf("o");
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			printf("e");
		}


	}

	return true;
}

bool loadOBJnoUV(const char * path, std::vector<glm::vec3>  & out_vertices, std::vector<glm::vec3>  & out_normals, int &ver) {

	std::vector<int> vertexIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;

	//FILE * file = fopen(path, "r");
	//if (file == NULL) {
	//	printf("ファイルを開けません!n");
	//	return false;
	//}

	// ファイルを開きます。
	FILE * file;
	errno_t error;
	error = fopen_s(&file, path, "r");

	while (1) {

		char lineHeader[1024];
		// 行の最初の文字列を読み込みます。
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF) {
			printf("b");
			// 各三角形の各頂点
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {
				unsigned int vertexIndexa = vertexIndices[i];
				glm::vec3 vertex = temp_vertices[vertexIndexa - 1];
				out_vertices.push_back(vertex);
				ver++;
				//std::cout << vertexIndex;
			}
			printf("v");

			printf("u");
			for (unsigned int i = 0; i < normalIndices.size(); i++) {
				unsigned int normalIndexa = normalIndices[i];
				glm::vec3 normal = temp_normals[normalIndexa - 1];
				out_normals.push_back(normal);
			}
			printf("a");
			break; // EOF = End Of File. ループを終了します。
		}
		// そうでなければlineHeaderをパースします。

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %fn", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			printf("v");
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf_s(file, "%f %f %fn", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
			printf("vn");
		}
		else if (strcmp(lineHeader, "f") == 0) {
			printf("f");
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d//%d %d//%d %d//%dn", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			//int matches = fscanf_s(file, "%d//%d %d//%d %d//%dn", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 6) {
				printf("File can't be read by our simple parser : ( Try exporting with other optionsn");
				return false;
			}
			printf("o");
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			printf("e");
		}


	}

	return true;
}

void prosessingOfOBJ(int *ver, GLuint *vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string OBJFile, GLuint *uvbuffer, GLuint *VertexArrayID) {
	glGenBuffers(1, vertexbuffer);									// バッファを1つ作り、vertexbufferに結果IDを入れます。
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// 次のコマンドは'vertexbuffer'バッファについてです。
	if (!loadOBJ(OBJFile.c_str(), vertices, uvs, normals, *ver)) {	// もしobjファイルを読み込めなかったら
		printf("[ERROR] No File of %s\n", OBJFile);					// エラー文を返す
	}
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, uvbuffer);										// バッファを1つ作り、uvbufferに結果IDを入れます。
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);						// 次のコマンドは'uvbuffer'バッファについてです。
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, VertexArrayID);							// バッファを1つ作り、VertexArrayIDに結果IDを入れます。
	glBindVertexArray(*VertexArrayID);								// 次のコマンドは'VertexArrayID'バッファについてです。

	// 最初の属性バッファ：頂点
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
	glVertexAttribPointer(
		0,                  // 属性0：0に特に理由はありません。しかし、シェーダ内のlayoutとあわせないといけません。
		3,                  // サイズ
		GL_FLOAT,           // タイプ
		GL_FALSE,           // 正規化？
		0,                  // ストライド
		(void*)0            // 配列バッファオフセット
	);


	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
	glVertexAttribPointer(
		1,                                // 属性。1という数字に意味はありません。ただしシェーダのlayoutとあわせる必要があります。
		2,                                // サイズ
		GL_FLOAT,                         // タイプ
		GL_FALSE,                         // 正規化？
		0,                                // ストライド
		(void*)0                          // 配列バッファオフセット
	);

	// 頂点をOpenGLに渡します。
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}


int main() {

	// GLFW初期化
	if (glfwInit() == GL_FALSE) {
		return -1;
	}

	// ウィンドウ生成
	GLFWwindow* window = glfwCreateWindow(1280, 640, "OpenGL Simple", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "GLFWウィンドウのオープンに失敗しました。 もしIntelのGPUならば, 3.3に対応していません。チュートリアルのバージョン2.1を試してください。n");
		glfwTerminate();
		return -1;
	}

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x アンチエイリアス
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 古いOpenGLは使いません。

	glfwMakeContextCurrent(window);

	glewExperimental = true; // コアプロファイルで必要となります。
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "GLEWの初期化に失敗しました。n");
		return -1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		int ver = 0, ver2 = 0, ver3 = 0;

		// これが頂点バッファを指し示すものとなります。
		GLuint vertexbuffer;
		// バッファを1つ作り、vertexbufferに結果IDを入れます。
		glGenBuffers(1, &vertexbuffer);
		// 次のコマンドは'vertexbuffer'バッファについてです。
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		// .objファイルを読み込みます。
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3>  normals; // すぐには使いません。
		std::vector<glm::vec2>  uvs;
		//bool res = loadOBJ("cubetex.obj", vertices, uvs, normals,ver);
		bool res = loadOBJ("Map01uv.obj", vertices, uvs, normals, ver);
		//bool res = loadOBJ("MapBottomuv.obj", vertices, uvs, normals, ver);
		//bool res = loadOBJnoUV("Map01_wall.obj", vertices, normals, ver);
		//bool res = loadOBJnoUV("title.obj", vertices, normals, ver);
		//bool res = loadOBJnoUV("cubetex.obj", vertices, normals,ver);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		// これがUVバッファを指し示すものとなります。
		GLuint uvbuffer;
		// バッファを1つ作り、uvbufferに結果IDを入れます。
		glGenBuffers(1, &uvbuffer);
		// 次のコマンドは'uvbuffer'バッファについてです。
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


		//GLuint colorbuffer;
		//glGenBuffers(1, &colorbuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


		// これが頂点バッファを指し示すものとなります。
		GLuint vertexbuffer2;
		// バッファを1つ作り、vertexbufferに結果IDを入れます。
		glGenBuffers(1, &vertexbuffer2);
		// 次のコマンドは'vertexbuffer'バッファについてです。
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);

		// .objファイルを読み込みます。
		std::vector<glm::vec3> vertices2;
		std::vector<glm::vec3>  normals2; // すぐには使いません。
		std::vector<glm::vec2>  uvs2;
		//bool res2 = loadOBJ("MapBottomuv.obj", vertices2, uvs2, normals2, ver2);
		bool res2 = loadOBJ("Map01Bottomuv.obj", vertices2, uvs2, normals2, ver2);
		//bool res2 = loadOBJnoUV("ball.obj", vertices2, normals2, ver2);
		//bool res2 = loadOBJnoUV("title.obj", vertices2, normals2, ver2);
		//bool res2 = loadOBJnoUV("kantan1.obj", vertices2, normals2,ver2);
		glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glm::vec3), &vertices2[0], GL_STATIC_DRAW);

		// これがUVバッファを指し示すものとなります。
		GLuint uvbuffer2;
		// バッファを1つ作り、uvbufferに結果IDを入れます。
		glGenBuffers(1, &uvbuffer2);
		// 次のコマンドは'uvbuffer2'バッファについてです。
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
		glBufferData(GL_ARRAY_BUFFER, uvs2.size() * sizeof(glm::vec2), &uvs2[0], GL_STATIC_DRAW);


		//GLuint colorbuffer2;
		//glGenBuffers(1, &colorbuffer2);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


		// これが頂点バッファを指し示すものとなります。
		GLuint vertexbuffer3;
		// バッファを1つ作り、vertexbufferに結果IDを入れます。
		glGenBuffers(1, &vertexbuffer3);
		// 次のコマンドは'vertexbuffer'バッファについてです。
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);

		// .objファイルを読み込みます。
		std::vector<glm::vec3> vertices3;
		std::vector<glm::vec3>  normals3; // すぐには使いません。
		std::vector<glm::vec2>  uvs3;
		bool res3 = loadOBJ("itemsample.obj", vertices3, uvs3, normals3, ver3);
		//bool res3 = loadOBJ("cubetex.obj", vertices3, uvs3, normals3, ver3);
		//bool res3 = loadOBJnoUV("Map01_bottom.obj", vertices3, normals3, ver3);
		glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(glm::vec3), &vertices3[0], GL_STATIC_DRAW);


		// これがUVバッファを指し示すものとなります。
		GLuint uvbuffer3;
		// バッファを1つ作り、uvbuffer3に結果IDを入れます。
		glGenBuffers(1, &uvbuffer3);
		// 次のコマンドは'uvbuffer3'バッファについてです。
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
		glBufferData(GL_ARRAY_BUFFER, uvs3.size() * sizeof(glm::vec2), &uvs3[0], GL_STATIC_DRAW);

		//GLuint colorbuffer2;
		//glGenBuffers(1, &colorbuffer2);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);




		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// 最初の属性バッファ：頂点

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // 属性0：0に特に理由はありません。しかし、シェーダ内のlayoutとあわせないといけません。
			3,                  // サイズ
			GL_FLOAT,           // タイプ
			GL_FALSE,           // 正規化？
			0,                  // ストライド
			(void*)0            // 配列バッファオフセット
		);


		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // 属性。1という数字に意味はありません。ただしシェーダのlayoutとあわせる必要があります。
			2,                                // サイズ
			GL_FLOAT,                         // タイプ
			GL_FALSE,                         // 正規化？
			0,                                // ストライド
			(void*)0                          // 配列バッファオフセット
		);


		GLuint VertexArrayID2;
		glGenVertexArrays(1, &VertexArrayID2);
		glBindVertexArray(VertexArrayID2);

		// 最初の属性バッファ：頂点

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glVertexAttribPointer(
			0,                  // 属性0：0に特に理由はありません。しかし、シェーダ内のlayoutとあわせないといけません。
			3,                  // サイズ
			GL_FLOAT,           // タイプ
			GL_FALSE,           // 正規化？
			0,                  // ストライド
			(void*)0            // 配列バッファオフセット
		);


		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
		glVertexAttribPointer(
			1,                                // 属性。1という数字に意味はありません。ただしシェーダのlayoutとあわせる必要があります。
			2,                                // サイズ
			GL_FLOAT,                         // タイプ
			GL_FALSE,                         // 正規化？
			0,                                // ストライド
			(void*)0                          // 配列バッファオフセット
		);

		GLuint VertexArrayID3;
		glGenVertexArrays(1, &VertexArrayID3);
		glBindVertexArray(VertexArrayID3);

		// 最初の属性バッファ：頂点

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
		glVertexAttribPointer(
			0,                  // 属性0：0に特に理由はありません。しかし、シェーダ内のlayoutとあわせないといけません。
			3,                  // サイズ
			GL_FLOAT,           // タイプ
			GL_FALSE,           // 正規化？
			0,                  // ストライド
			(void*)0            // 配列バッファオフセット
		);


		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer3);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
		glVertexAttribPointer(
			1,                                // 属性。1という数字に意味はありません。ただしシェーダのlayoutとあわせる必要があります。
			2,                                // サイズ
			GL_FLOAT,                         // タイプ
			GL_FALSE,                         // 正規化？
			0,                                // ストライド
			(void*)0                          // 配列バッファオフセット
		);

		// 頂点をOpenGLに渡します。

		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	*/
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//新しい部分. 上の処理をコンパクトにした.
	// #define N 5 が21行目に定義されています. .obj(OBJFile) と.bmp(Texture) を増やすときはNの値も1増やしてください. 

	int ver[N] = { 0 };					// 関数 loadOBJnoUV() の中でインクリメントされる      
	GLuint vertexbuffer[N];				// これが頂点バッファを指し示すものとなります。
	std::vector<glm::vec3> vertices[N];	// .objファイルを読み込むのに使います
	std::vector<glm::vec3>  normals[N]; // すぐには使いません。
	std::vector<glm::vec2>  uvs[N];		// なんだろこれ.
	GLuint uvbuffer[N];					// これがUVバッファを指し示すものとなります。
	GLuint VertexArrayID[N];			// .objと.bmpとを結びつけるためのID

	std::string OBJFile[N];				//.objファイルを格納する
	OBJFile[0] = "Map01uv.obj";
	OBJFile[1] = "Map01Bottomuv.obj";
	OBJFile[2] = "itemsample.obj";
	OBJFile[3] = "start.obj";
	OBJFile[4] = "title.obj";
	OBJFile[5] = "enter.obj";
	OBJFile[6] = "start.obj";

	GLuint Texture[N];					//.bmpファイルを読み込み, 関数 loadBMP_custom() の中でIDを割り当てられる. (すなわち, IDを持つ)
	Texture[0] = loadBMP_custom("texsample.bmp");
	Texture[1] = loadBMP_custom("mapbottom.bmp");
	Texture[2] = loadBMP_custom("sampletex.bmp");
	Texture[3] = loadBMP_custom("start.bmp");
	Texture[4] = loadBMP_custom("black.bmp");
	Texture[5] = loadBMP_custom("enter.bmp");
	Texture[6] = loadBMP_custom("result.bmp");



	for (int i = 0; i < N; i++) {
		//コメントアウトした 722行目 ～ 925行目の処理をまとめた関数
		prosessingOfOBJ(&ver[i], &vertexbuffer[i], vertices[i], normals[i], uvs[i], OBJFile[i], &uvbuffer[i], &VertexArrayID[i]);
	}

	decideCube(vertices[0]);			//当たり判定に用いる配列cube[][]に値を格納する

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// 三角形を描きます！
	//glDrawArrays(GL_TRIANGLES, 0, 3); // 頂点0から始まります。合計3つの頂点です。&rarr;1つの三角形です。

	glDisableVertexAttribArray(0);

	// 三角形の描画！
	//glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形


	printf("\n");
	printf("%d\n", vertices[0].size());


	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		return -1;
	}


	// シェーダからGLSLプログラムを作りコンパイルする。
	GLuint programID = LoadShaders("SimpleVertexShader.hlsl", "SimpleFragmentShader.hlsl");
	//GLuint programID2 = LoadShaders("SimpleVertexShader.hlsl", "SimpleFragmentShader.hlsl");


	// 射影行列：45&deg;の視界、アスペクト比4:3、表示範囲：0.1単位  100単位
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// カメラ行列
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // ワールド空間でカメラは(4,3,3)にあります。
		glm::vec3(0, 0, 0), // 原点を見ています。
		glm::vec3(0, -1, 0)  // 頭が上方向(0,-1,0にセットすると上下逆転します。)
	);
	// モデル行列：単位行列(モデルは原点にあります。)
	glm::mat4 Model = glm::mat4(1.0f);  // 各モデルを変える！
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // 行列の掛け算は逆になることを思い出してください。



	// "MVP" uniformへのハンドルを取得します。
	// 初期化時だけ
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	//GLuint MatrixID2 = glGetUniformLocation(programID2, "MVP");

	for (int i = 0; i < N; i++) {
		printf("ver[%d] = %d\n", i, ver[i]);

	}


	// 下でエスケープキーが押されるのを捉えるのを保証します。
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	int p = 0;
	int flush = 0;		//1000に到達したら0に戻す
	
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE) {
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);					// おまじない
		glEnable(GL_DEPTH_TEST);								// デプステストを有効にする
		glDepthFunc(GL_LESS);									// 前のものよりもカメラに近ければ、フラグメントを受け入れる
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// スクリーンをクリアする

		if (hit(cube) == true) {
			p++;
			std::cout << "atari" << p << "\n";
		}

		// キーボードとマウスのインプットからMVP行列を計算する
		computeMatricesFromInputs(window);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// 現在バインドしているシェーダの"MVP" uniformに変換を送る
		// レンダリングする各モデルごと、なぜならMVPが違うからです。(少なくともMの部分が違います。)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		if (ModeSelect == 0) {

			flush++;
			
			//スタート画面を描画
			glBindTexture(GL_TEXTURE_2D, Texture[3]);
			glBindVertexArray(VertexArrayID[3]);
			//三角形の描画！
			//glDrawArrays(GL_TRIANGLES, 0, ver4); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
			glDrawArrays(GL_TRIANGLES, 0, ver[3]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
			
			//真っ黒な画面を張り付けるオブジェクトを描画
			glBindTexture(GL_TEXTURE_2D, Texture[4]);
			glBindVertexArray(VertexArrayID[4]);
			//三角形の描画！
			//glDrawArrays(GL_TRIANGLES, 0, ver5); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
			glDrawArrays(GL_TRIANGLES, 0, ver[4]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
			
			if (flush < 500) {
				//Enter画面を張り付けるオブジェクトを描画
				glBindTexture(GL_TEXTURE_2D, Texture[5]);
				glBindVertexArray(VertexArrayID[5]);
				//三角形の描画！
				//glDrawArrays(GL_TRIANGLES, 0, ver4); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
				glDrawArrays(GL_TRIANGLES, 0, ver[5]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
			}
			else if (flush == 1000) {
				flush = 0;
			}
		}

		if (ModeSelect == 1) {

			// シェーダを使う

			//glUseProgram(programID);

			//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices, GL_STATIC_DRAW);
			//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


			// 三角形を描きます！
			//glDrawArrays(GL_TRIANGLES, 0, 3); // 頂点0から始まります。合計3つの頂点です。&rarr;1つの三角形です。

			// カメラのほうを向いていない法線の三角形をカリングします。
			//glEnable(GL_CULL_FACE);



			//壁を描画
			glBindTexture(GL_TEXTURE_2D, Texture[0]);
			glBindVertexArray(VertexArrayID[0]);
			//三角形の描画！
			//glDrawArrays(GL_TRIANGLES, 0, ver); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
			glDrawArrays(GL_TRIANGLES, 0, ver[0]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形

			//glDrawArrays(GL_TRIANGLES, 0, 72);
			//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

			//床を描画
			glBindTexture(GL_TEXTURE_2D, Texture[1]);
			glBindVertexArray(VertexArrayID[1]);
			//三角形の描画！
			//glDrawArrays(GL_TRIANGLES, 0, ver2); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
			glDrawArrays(GL_TRIANGLES, 0, ver[1]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形

			//アイテムを描画
			glBindTexture(GL_TEXTURE_2D, Texture[2]);
			glBindVertexArray(VertexArrayID[2]);
			//三角形の描画！
			//glDrawArrays(GL_TRIANGLES, 0, ver3); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
			glDrawArrays(GL_TRIANGLES, 0, ver[2]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形

			//glDisableVertexAttribArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();

	return 0;



}


// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します

/*
行列入門
glm::mat4 myMatrix;
glm::vec4 myVector;
// 何らかの方法でmyMatrixとmyVector満たす。
glm::vec4 transformedVector = myMatrix * myVector; // もう一度言いますが、この順番です！これは重要なことです
平行移動行列
glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f));
glm::vec4 myVector(10.0f, 10.0f, 10.0f, 0.0f);
glm::vec4 transformedVector = myMatrix * myVector; // guess the result
単位行列
glm::mat4 myIdentityMatrix = glm::mat4(1.0f);
拡大縮小行列
glm::mat4 myScalingMatrix = glm::scale(2.0f, 2.0f ,2.0f);
回転行列
glm::vec3 myRotationAxis( ??, ??, ??);
glm::rotate( angle_in_degrees, myRotationAxis );
変換の組み合わせ
TransformedVector = TranslationMatrix * RotationMatrix * ScaleMatrix * OriginalVector;

glm::mat4 myModelMatrix = myTranslationMatrix * myRotationMatrix * myScaleMatrix;
glm::vec4 myTransformedVector = myModelMatrix * myOriginalVector;

ビュー行列
// #include <glm/gtc/matrix_transform.hpp> と #include <glm/gtx/transform.hpp> を使います。
glm::mat4 ViewMatrix = glm::translate(glm::mat4(), glm::vec3(-3.0f, 0.0f, 0.0f));

glm::mat4 CameraMatrix = glm::LookAt(
	cameraPosition, // ワールド空間でのカメラの位置
	cameraTarget,   // ワールド空間での見たい位置
	upVector        // たぶんglm::vec3(0,1,0)です。一方で(0,-1,0)にしたら上下逆さまになります。それもまた良いでしょう。
);

射影行列
// 読むのが難しい行列を作ります。それでも、普通の標準の4x4行列です。
glm::mat4 projectionMatrix = glm::perspective(
	glm::radians(FoV), // 垂直方向のビューの広がり度合い（ラジアン）。つまり、"ズーム"の度合い。"カメラレンズ"を考えてください。通常90&deg;(超ワイド) と 30&deg; (とてもズームインしてる)の間です。
	4.0f / 3.0f,       // アスペクト比。ウィンドウのサイズに依存します。4/3 == 800/600 == 1280/960となっています。
	0.1f,              // 近くのクリッピング平面。できるだけ大きくします。そうしないと正確さの問題が出てくるでしょう。
	100.0f             // 遠くのクリッピング平面。できるだけ小さくします。
);


*/
/*
mat4 myMatrix;
vec4 myVector;
// 何らかの方法でmyMatrixとmyVector満たす。
vec4 transformedVector = myMatrix * myVector; // そうです、GLMととても似ています。

vec4 transformedVector = myMatrix * myVector;
*/

/*

#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
void main() {
	gl_Position.xyz = vertexPosition_modelspace;
	gl_Position.w = 1.0;
}
*/