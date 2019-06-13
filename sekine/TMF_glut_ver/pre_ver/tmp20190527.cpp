// TMFSample.cpp : ���̃t�@�C���ɂ� 'main' �֐����܂܂�Ă��܂��B�v���O�������s�̊J�n�ƏI���������ōs���܂��B
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
#include <glm/gtx/transform.hpp> // <glm/glm.hpp>�̌�
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include<GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5

using namespace glm;

double lastTime;
glm::mat4 ProjectionM;
glm::mat4 ViewM;
// �����p�A-Z����
float horizontalAngle = 3.14f;
// �����p�A0�A�������𒭂߂Ă���
float verticalAngle = 0.0f;
// �ʒu
glm::vec3 position = glm::vec3(0, 0, 5);
int vercount = 0;
int vercount2 = 0;

// 3���_��\��3�̃x�N�g���̔z��

// ���_�B3�̘A�����鐔����3�����̒��_�ł��B3�̘A�����钸�_�͎O�p�`���Ӗ����܂��B
// �����̂́A���ꂼ�ꂪ2���̎O�p�`����Ȃ�6�ʂ��琬��܂��B������6*2=12���̎O�p�`��12*3�̒��_�����܂��B
static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // �O�p�`1:�J�n
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // �O�p�`1:�I��
	1.0f, 1.0f,-1.0f, // �O�p�`2:�J�n
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // �O�p�`2:�I��
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


// �e���_�Ɉ�̐F�B�����̓����_���ɍ���܂����B
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

// �e���_��2�̒l�A������Blender�ō��܂����B�ǂ�����č�邩�͂��ꂩ��������܂��B
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
	// BMP�t�@�C���̃w�b�_����ǂݍ��܂��f�[�^
	unsigned char header[54]; // �eBMP�t�@�C����54�o�C�g�̃w�b�_����n�܂�܂��B
	unsigned int dataPos;     // ���ۂ̃f�[�^���t�@�C���̂ǂ̈ʒu�ɂ��邩
	unsigned int width, height;
	unsigned int imageSize;   // = ��*����*3
	// ���ۂ�RGB�f�[�^
	unsigned char * data;

	// �t�@�C�����J���܂��B
	FILE * file;
	//= fopen(imagepath, "rb");
	errno_t error;
	error = fopen_s(&file, imagepath, "rb");



	if (error) {
		printf("�摜���J���܂���ł����Bn");
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) { // 54�o�C�g�ǂݍ��߂Ȃ���΁A��肪����܂��B
		printf("BMP�t�@�C���ł͂���܂���Bn");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("BMP�t�@�C���ł͂���܂���Bn");
		return 0;
	}

	// �o�C�g�z�񂩂琮����ǂݍ���
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// BMP�t�@�C���̒��ɂ̓~�X�t�H�[�}�b�g������܂��B���̏��𐄑����܂��B
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : 1�o�C�g�͐ԁA�΁A�̊e�F�ł��B
	if (dataPos == 0)      dataPos = 54; // �����BMP�w�b�_�͏I���ł��B

	// �o�b�t�@�����
	data = new unsigned char[imageSize];

	// �t�@�C��������ۂ̃f�[�^���o�b�t�@�ɓǂݍ���
	fread(data, 1, imageSize, file);

	//���ׂĂ̓�������ɂ���̂ŁA�t�@�C���͕��܂��B
	fclose(file);

	// �ЂƂ�OpenGL�e�N�X�`�������܂��B
	GLuint textureID;
	glGenTextures(1, &textureID);

	// �V���ɍ��ꂽ�e�N�X�`����"�o�C���h"���܂��B�܂肱�������̃e�N�X�`���֐��͂��̃e�N�X�`����ύX���܂��B
	glBindTexture(GL_TEXTURE_2D, textureID);

	// OpenGL�ɉ摜��n���܂��B
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// �摜���g��(MAGnifying)����Ƃ��͐��`(LINEAR)�t�B���^�����O���g���܂��B
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// �摜���k��(MINifying)����Ƃ��A���`(LINEAR)�t�B���^�����A��̃~�b�v�}�b�v����`(LINEARYLY)�ɍ��������̂��g���܂��B
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ���̂悤�ɂ��ă~�b�v�}�b�v�����܂��B
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;

}

GLuint loadTGA_glfw(const char * imagepath) {

	// ���OpenGL�e�N�X�`�������܂��B
	GLuint textureID;
	glGenTextures(1, &textureID);

	// �V����������e�N�X�`����"�o�C���h"���܂��B����ȍ~�̊֐��͂��ׂĂ��̃e�N�X�`����ύX���܂��B
	glBindTexture(GL_TEXTURE_2D, textureID);

	// �������p�����[�^��glTexImage2D���Ă�ŁA�t�@�C����ǂݍ��݂܂��B
	//glfwLoadTexture2D(imagepath, 0);

	// 3�̐��`�t�B���^�����O
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// �쐬�����e�N�X�`����ID��Ԃ��܂��B
	return textureID;
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// �V�F�[�_�����܂��B
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// �t�@�C�����璸�_�V�F�[�_�̃R�[�h��ǂݍ��݂܂��B
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}

	// �t�@�C������t���O�����g�V�F�[�_��ǂݍ��݂܂��B
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


	// ���_�V�F�[�_���R���p�C�����܂��B
	printf("Compiling shader : %sn", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// ���_�V�F�[�_���`�F�b�N���܂��B
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	//std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	//glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	//fprintf(stdout, "%sn", &VertexShaderErrorMessage[0]);

	// �t���O�����g�V�F�[�_���R���p�C�����܂��B
	printf("Compiling shader : %sn", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// �t���O�����g�V�F�[�_���`�F�b�N���܂��B
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	//std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	//glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	//fprintf(stdout, "%sn", &FragmentShaderErrorMessage[0]);


	// �v���O�����������N���܂��B
	fprintf(stdout, "Linking programn");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// �v���O�������`�F�b�N���܂��B
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

void computeMatricesFromInputs(GLFWwindow* window) {


	// �ʒu
	//glm::vec3 position = glm::vec3(0, 0, 5);

	// �����p�A-Z����
	//float horizontalAngle = 3.14f;

	// �����p�A0�A�������𒭂߂Ă���
	//float verticalAngle = 0.0f;

	// ��������
	float initialFoV = 45.0f;

	float speed = 4.0f; // 3 units / second
	float mouseSpeed = 0.005f;

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;


	int xpos = 512, ypos = 384;
	// �E�������܂��B
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		xpos -= 156;
	}
	//���������܂��B
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		xpos += 156;
	}
	// ��������܂��B
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		ypos += 96;
	}
	// ���������܂��B
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		ypos -= 96;
	}

	// �V���ȕ������v�Z���܂��B
	horizontalAngle += mouseSpeed * deltaTime * float(1024 / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(768 / 2 - ypos);

	// �����F���ʍ��W���璼�p���W�ɕϊ����܂��B
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// �E�x�N�g��
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// ��x�N�g���F�E�ƑO�x�N�g���ɐ���
	glm::vec3 up = glm::cross(right, direction);

	// �O�֓����܂��B
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// ���֓����܂��B
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// �O���������܂܁A�E�֕��s�ړ����܂��B
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// �O���������܂܁A���֕��s�ړ����܂��B
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	float FoV = 45.0f;
	// �Y�[�����܂��B
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		FoV = initialFoV - 5 * 3.0f;
	}
	// �Y�[���A�E�g���܂��B
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		FoV = initialFoV + 5 * 3.0f;
	}

	// �ˉe�s��F����45&deg;�A4:3��A�`��͈�0.1�P��100�P��
	ProjectionM = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	//ProjectionM = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// �J�����s��
	ViewM = glm::lookAt(
		position,           // �J�����̈ʒu
		position + direction, // �ڕW�n�_
		up                  // ����� (0,-1,0�ɃZ�b�g����Ə㉺�t���܂ɂȂ�܂��B)
	);

}

glm::mat4  getProjectionMatrix() {

	//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	return ProjectionM;
}

glm::mat4 getViewMatrix() {

	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(4, 3, 3), // ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
	//	glm::vec3(0, 0, 0), // ���_�����Ă��܂��B
	//	glm::vec3(0, 1, 0)  // ���������(0,-1,0�ɃZ�b�g����Ə㉺�t�]���܂��B)
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
	//	printf("�t�@�C�����J���܂���!n");
	//	return false;
	//}

	// �t�@�C�����J���܂��B
	FILE * file;
	errno_t error;
	error = fopen_s(&file, path, "r");
	char lineHeader[2048];
	while (1) {

		// �s�̍ŏ��̕������ǂݍ��݂܂��B
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF) {
			printf("b");
			// �e�O�p�`�̊e���_
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
			break; // EOF = End Of File. ���[�v���I�����܂��B
		}
		// �����łȂ����lineHeader���p�[�X���܂��B

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
	//	printf("�t�@�C�����J���܂���!n");
	//	return false;
	//}

	// �t�@�C�����J���܂��B
	FILE * file;
	errno_t error;
	error = fopen_s(&file, path, "r");

	while (1) {

		char lineHeader[1024];
		// �s�̍ŏ��̕������ǂݍ��݂܂��B
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF) {
			printf("b");
			// �e�O�p�`�̊e���_
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
			break; // EOF = End Of File. ���[�v���I�����܂��B
		}
		// �����łȂ����lineHeader���p�[�X���܂��B

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
	glGenBuffers(1, vertexbuffer);									// �o�b�t�@��1���Avertexbuffer�Ɍ���ID�����܂��B
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// ���̃R�}���h��'vertexbuffer'�o�b�t�@�ɂ��Ăł��B
	if (!loadOBJ(OBJFile.c_str(), vertices, uvs, normals, *ver)) {	// ����obj�t�@�C����ǂݍ��߂Ȃ�������
		printf("[ERROR] No File of %s\n", OBJFile);					// �G���[����Ԃ�
	}
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, uvbuffer);										// �o�b�t�@��1���Auvbuffer�Ɍ���ID�����܂��B
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);						// ���̃R�}���h��'uvbuffer'�o�b�t�@�ɂ��Ăł��B
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, VertexArrayID);							// �o�b�t�@��1���AVertexArrayID�Ɍ���ID�����܂��B
	glBindVertexArray(*VertexArrayID);								// ���̃R�}���h��'VertexArrayID'�o�b�t�@�ɂ��Ăł��B

	// �ŏ��̑����o�b�t�@�F���_
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
	glVertexAttribPointer(
		0,                  // ����0�F0�ɓ��ɗ��R�͂���܂���B�������A�V�F�[�_����layout�Ƃ��킹�Ȃ��Ƃ����܂���B
		3,                  // �T�C�Y
		GL_FLOAT,           // �^�C�v
		GL_FALSE,           // ���K���H
		0,                  // �X�g���C�h
		(void*)0            // �z��o�b�t�@�I�t�Z�b�g
	);


	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
	glVertexAttribPointer(
		1,                                // �����B1�Ƃ��������ɈӖ��͂���܂���B�������V�F�[�_��layout�Ƃ��킹��K�v������܂��B
		2,                                // �T�C�Y
		GL_FLOAT,                         // �^�C�v
		GL_FALSE,                         // ���K���H
		0,                                // �X�g���C�h
		(void*)0                          // �z��o�b�t�@�I�t�Z�b�g
	);

	// ���_��OpenGL�ɓn���܂��B
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}


int main() {

	// GLFW������
	if (glfwInit() == GL_FALSE) {
		return -1;
	}

	// �E�B���h�E����
	GLFWwindow* window = glfwCreateWindow(1280, 640, "OpenGL Simple", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "GLFW�E�B���h�E�̃I�[�v���Ɏ��s���܂����B ����Intel��GPU�Ȃ��, 3.3�ɑΉ����Ă��܂���B�`���[�g���A���̃o�[�W����2.1�������Ă��������Bn");
		glfwTerminate();
		return -1;
	}

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x �A���`�G�C���A�X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // �Â�OpenGL�͎g���܂���B

	glfwMakeContextCurrent(window);

	glewExperimental = true; // �R�A�v���t�@�C���ŕK�v�ƂȂ�܂��B
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "GLEW�̏������Ɏ��s���܂����Bn");
		return -1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		int ver = 0, ver2 = 0, ver3 = 0;

		// ���ꂪ���_�o�b�t�@���w���������̂ƂȂ�܂��B
		GLuint vertexbuffer;
		// �o�b�t�@��1���Avertexbuffer�Ɍ���ID�����܂��B
		glGenBuffers(1, &vertexbuffer);
		// ���̃R�}���h��'vertexbuffer'�o�b�t�@�ɂ��Ăł��B
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		// .obj�t�@�C����ǂݍ��݂܂��B
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3>  normals; // �����ɂ͎g���܂���B
		std::vector<glm::vec2>  uvs;
		//bool res = loadOBJ("cubetex.obj", vertices, uvs, normals,ver);
		bool res = loadOBJ("Map01uv.obj", vertices, uvs, normals, ver);
		//bool res = loadOBJ("MapBottomuv.obj", vertices, uvs, normals, ver);
		//bool res = loadOBJnoUV("Map01_wall.obj", vertices, normals, ver);
		//bool res = loadOBJnoUV("title.obj", vertices, normals, ver);
		//bool res = loadOBJnoUV("cubetex.obj", vertices, normals,ver);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		// ���ꂪUV�o�b�t�@���w���������̂ƂȂ�܂��B
		GLuint uvbuffer;
		// �o�b�t�@��1���Auvbuffer�Ɍ���ID�����܂��B
		glGenBuffers(1, &uvbuffer);
		// ���̃R�}���h��'uvbuffer'�o�b�t�@�ɂ��Ăł��B
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


		//GLuint colorbuffer;
		//glGenBuffers(1, &colorbuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


		// ���ꂪ���_�o�b�t�@���w���������̂ƂȂ�܂��B
		GLuint vertexbuffer2;
		// �o�b�t�@��1���Avertexbuffer�Ɍ���ID�����܂��B
		glGenBuffers(1, &vertexbuffer2);
		// ���̃R�}���h��'vertexbuffer'�o�b�t�@�ɂ��Ăł��B
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);

		// .obj�t�@�C����ǂݍ��݂܂��B
		std::vector<glm::vec3> vertices2;
		std::vector<glm::vec3>  normals2; // �����ɂ͎g���܂���B
		std::vector<glm::vec2>  uvs2;
		//bool res2 = loadOBJ("MapBottomuv.obj", vertices2, uvs2, normals2, ver2);
		bool res2 = loadOBJ("Map01Bottomuv.obj", vertices2, uvs2, normals2, ver2);
		//bool res2 = loadOBJnoUV("ball.obj", vertices2, normals2, ver2);
		//bool res2 = loadOBJnoUV("title.obj", vertices2, normals2, ver2);
		//bool res2 = loadOBJnoUV("kantan1.obj", vertices2, normals2,ver2);
		glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glm::vec3), &vertices2[0], GL_STATIC_DRAW);

		// ���ꂪUV�o�b�t�@���w���������̂ƂȂ�܂��B
		GLuint uvbuffer2;
		// �o�b�t�@��1���Auvbuffer�Ɍ���ID�����܂��B
		glGenBuffers(1, &uvbuffer2);
		// ���̃R�}���h��'uvbuffer2'�o�b�t�@�ɂ��Ăł��B
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
		glBufferData(GL_ARRAY_BUFFER, uvs2.size() * sizeof(glm::vec2), &uvs2[0], GL_STATIC_DRAW);


		//GLuint colorbuffer2;
		//glGenBuffers(1, &colorbuffer2);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


		// ���ꂪ���_�o�b�t�@���w���������̂ƂȂ�܂��B
		GLuint vertexbuffer3;
		// �o�b�t�@��1���Avertexbuffer�Ɍ���ID�����܂��B
		glGenBuffers(1, &vertexbuffer3);
		// ���̃R�}���h��'vertexbuffer'�o�b�t�@�ɂ��Ăł��B
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);

		// .obj�t�@�C����ǂݍ��݂܂��B
		std::vector<glm::vec3> vertices3;
		std::vector<glm::vec3>  normals3; // �����ɂ͎g���܂���B
		std::vector<glm::vec2>  uvs3;
		bool res3 = loadOBJ("itemsample.obj", vertices3, uvs3, normals3, ver3);
		//bool res3 = loadOBJ("cubetex.obj", vertices3, uvs3, normals3, ver3);
		//bool res3 = loadOBJnoUV("Map01_bottom.obj", vertices3, normals3, ver3);
		glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(glm::vec3), &vertices3[0], GL_STATIC_DRAW);


		// ���ꂪUV�o�b�t�@���w���������̂ƂȂ�܂��B
		GLuint uvbuffer3;
		// �o�b�t�@��1���Auvbuffer3�Ɍ���ID�����܂��B
		glGenBuffers(1, &uvbuffer3);
		// ���̃R�}���h��'uvbuffer3'�o�b�t�@�ɂ��Ăł��B
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
		glBufferData(GL_ARRAY_BUFFER, uvs3.size() * sizeof(glm::vec2), &uvs3[0], GL_STATIC_DRAW);

		//GLuint colorbuffer2;
		//glGenBuffers(1, &colorbuffer2);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);




		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// �ŏ��̑����o�b�t�@�F���_

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // ����0�F0�ɓ��ɗ��R�͂���܂���B�������A�V�F�[�_����layout�Ƃ��킹�Ȃ��Ƃ����܂���B
			3,                  // �T�C�Y
			GL_FLOAT,           // �^�C�v
			GL_FALSE,           // ���K���H
			0,                  // �X�g���C�h
			(void*)0            // �z��o�b�t�@�I�t�Z�b�g
		);


		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // �����B1�Ƃ��������ɈӖ��͂���܂���B�������V�F�[�_��layout�Ƃ��킹��K�v������܂��B
			2,                                // �T�C�Y
			GL_FLOAT,                         // �^�C�v
			GL_FALSE,                         // ���K���H
			0,                                // �X�g���C�h
			(void*)0                          // �z��o�b�t�@�I�t�Z�b�g
		);


		GLuint VertexArrayID2;
		glGenVertexArrays(1, &VertexArrayID2);
		glBindVertexArray(VertexArrayID2);

		// �ŏ��̑����o�b�t�@�F���_

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glVertexAttribPointer(
			0,                  // ����0�F0�ɓ��ɗ��R�͂���܂���B�������A�V�F�[�_����layout�Ƃ��킹�Ȃ��Ƃ����܂���B
			3,                  // �T�C�Y
			GL_FLOAT,           // �^�C�v
			GL_FALSE,           // ���K���H
			0,                  // �X�g���C�h
			(void*)0            // �z��o�b�t�@�I�t�Z�b�g
		);


		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
		glVertexAttribPointer(
			1,                                // �����B1�Ƃ��������ɈӖ��͂���܂���B�������V�F�[�_��layout�Ƃ��킹��K�v������܂��B
			2,                                // �T�C�Y
			GL_FLOAT,                         // �^�C�v
			GL_FALSE,                         // ���K���H
			0,                                // �X�g���C�h
			(void*)0                          // �z��o�b�t�@�I�t�Z�b�g
		);

		GLuint VertexArrayID3;
		glGenVertexArrays(1, &VertexArrayID3);
		glBindVertexArray(VertexArrayID3);

		// �ŏ��̑����o�b�t�@�F���_

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
		glVertexAttribPointer(
			0,                  // ����0�F0�ɓ��ɗ��R�͂���܂���B�������A�V�F�[�_����layout�Ƃ��킹�Ȃ��Ƃ����܂���B
			3,                  // �T�C�Y
			GL_FLOAT,           // �^�C�v
			GL_FALSE,           // ���K���H
			0,                  // �X�g���C�h
			(void*)0            // �z��o�b�t�@�I�t�Z�b�g
		);


		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer3);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
		glVertexAttribPointer(
			1,                                // �����B1�Ƃ��������ɈӖ��͂���܂���B�������V�F�[�_��layout�Ƃ��킹��K�v������܂��B
			2,                                // �T�C�Y
			GL_FLOAT,                         // �^�C�v
			GL_FALSE,                         // ���K���H
			0,                                // �X�g���C�h
			(void*)0                          // �z��o�b�t�@�I�t�Z�b�g
		);

		// ���_��OpenGL�ɓn���܂��B

		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//�V��������. ��̏������R���p�N�g�ɂ���.
		// #define N 5 ��21�s�ڂɒ�`����Ă��܂�. .obj(OBJFile) ��.bmp(Texture) �𑝂₷�Ƃ���N�̒l��1���₵�Ă�������. 

	int ver[N] = { 0 };					// �֐� loadOBJnoUV() �̒��ŃC���N�������g�����      
	GLuint vertexbuffer[N];				// ���ꂪ���_�o�b�t�@���w���������̂ƂȂ�܂��B
	std::vector<glm::vec3> vertices[N];	// .obj�t�@�C����ǂݍ��ނ̂Ɏg���܂�
	std::vector<glm::vec3>  normals[N]; // �����ɂ͎g���܂���B
	std::vector<glm::vec2>  uvs[N];		// �Ȃ񂾂낱��.
	GLuint uvbuffer[N];					// ���ꂪUV�o�b�t�@���w���������̂ƂȂ�܂��B
	GLuint VertexArrayID[N];			// .obj��.bmp�Ƃ����т��邽�߂�ID

	std::string OBJFile[N];				//.obj�t�@�C�����i�[����
	OBJFile[0] = "Map01uv.obj";
	OBJFile[1] = "Map01Bottomuv.obj";
	OBJFile[2] = "itemsample.obj";
	OBJFile[3] = "start.obj";
	OBJFile[4] = "title.obj";

	GLuint Texture[N];					//.bmp�t�@�C����ǂݍ���, �֐� loadBMP_custom() �̒���ID�����蓖�Ă���. (���Ȃ킿, ID������)
	Texture[0] = loadBMP_custom("texsample.bmp");
	Texture[1] = loadBMP_custom("mapbottom.bmp");
	Texture[2] = loadBMP_custom("sampletex.bmp");
	Texture[3] = loadBMP_custom("start.bmp");
	Texture[4] = loadBMP_custom("black.bmp");

	for (int i = 0; i < N; i++) {
		//�R�����g�A�E�g���� 722�s�� �` 925�s�ڂ̏������܂Ƃ߂��֐�
		prosessingOfOBJ(&ver[i], &vertexbuffer[i], vertices[i], normals[i], uvs[i], OBJFile[i], &uvbuffer[i], &VertexArrayID[i]);
	}



	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// �O�p�`��`���܂��I
	//glDrawArrays(GL_TRIANGLES, 0, 3); // ���_0����n�܂�܂��B���v3�̒��_�ł��B&rarr;1�̎O�p�`�ł��B

	glDisableVertexAttribArray(0);

	// �O�p�`�̕`��I
	//glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`


	printf("\n");
	printf("%d\n", vertices[0].size());

	glm::vec3 cube(0, 0, 0);

	for (int i = 0; i < vertices[0].size(); i++) {
		//printf("%f\n", vertices[i]);
		//if (i % 3 == 0) cube.x += vertices[i].x;
		//else if (i % 3 == 1) cube.y += vertices[i].y;
		//else if (i % 3 == 2) cube.z += vertices[i].z;

		cube.x += vertices[0][i].x;
		cube.y += vertices[0][i].y;
		cube.z += vertices[0][i].z;
	}
	printf("(x, y, z) = %.5f  %.5f  %.5f\n", cube.x, cube.y, cube.z);


	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		return -1;
	}


	// �V�F�[�_����GLSL�v���O���������R���p�C������B
	GLuint programID = LoadShaders("SimpleVertexShader.hlsl", "SimpleFragmentShader.hlsl");
	//GLuint programID2 = LoadShaders("SimpleVertexShader.hlsl", "SimpleFragmentShader.hlsl");


	// �ˉe�s��F45&deg;�̎��E�A�A�X�y�N�g��4:3�A�\���͈́F0.1�P��  100�P��
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// �J�����s��
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
		glm::vec3(0, 0, 0), // ���_�����Ă��܂��B
		glm::vec3(0, -1, 0)  // ���������(0,-1,0�ɃZ�b�g����Ə㉺�t�]���܂��B)
	);
	// ���f���s��F�P�ʍs��(���f���͌��_�ɂ���܂��B)
	glm::mat4 Model = glm::mat4(1.0f);  // �e���f����ς���I
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // �s��̊|���Z�͋t�ɂȂ邱�Ƃ��v���o���Ă��������B



	// "MVP" uniform�ւ̃n���h�����擾���܂��B
	// ������������
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	//GLuint MatrixID2 = glGetUniformLocation(programID2, "MVP");

	for (int i = 0; i < N; i++) {
		printf("ver[%d] = %d\n", i, ver[i]);

	}


	// ���ŃG�X�P�[�v�L�[���������̂𑨂���̂�ۏ؂��܂��B
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE) {
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);					// ���܂��Ȃ�
		glEnable(GL_DEPTH_TEST);								// �f�v�X�e�X�g��L���ɂ���
		glDepthFunc(GL_LESS);									// �O�̂��̂����J�����ɋ߂���΁A�t���O�����g���󂯓����
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// �X�N���[�����N���A����

		// �L�[�{�[�h�ƃ}�E�X�̃C���v�b�g����MVP�s����v�Z����
		computeMatricesFromInputs(window);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;


		// ���݃o�C���h���Ă���V�F�[�_��"MVP" uniform�ɕϊ��𑗂�
		// �����_�����O����e���f�����ƁA�Ȃ��Ȃ�MVP���Ⴄ����ł��B(���Ȃ��Ƃ�M�̕������Ⴂ�܂��B)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// �V�F�[�_���g��

		//glUseProgram(programID);

		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices, GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


		// �O�p�`��`���܂��I
		//glDrawArrays(GL_TRIANGLES, 0, 3); // ���_0����n�܂�܂��B���v3�̒��_�ł��B&rarr;1�̎O�p�`�ł��B

		// �J�����̂ق��������Ă��Ȃ��@���̎O�p�`���J�����O���܂��B
		//glEnable(GL_CULL_FACE);

		//�ǂ�`��
		glBindTexture(GL_TEXTURE_2D, Texture[0]);
		glBindVertexArray(VertexArrayID[0]);
		//�O�p�`�̕`��I
		//glDrawArrays(GL_TRIANGLES, 0, ver); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`
		glDrawArrays(GL_TRIANGLES, 0, ver[0]); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`

		//glDrawArrays(GL_TRIANGLES, 0, 72);
		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		//����`��
		glBindTexture(GL_TEXTURE_2D, Texture[1]);
		glBindVertexArray(VertexArrayID[1]);
		//�O�p�`�̕`��I
		//glDrawArrays(GL_TRIANGLES, 0, ver2); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`
		glDrawArrays(GL_TRIANGLES, 0, ver[1]); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`

		//�A�C�e����`��
		glBindTexture(GL_TEXTURE_2D, Texture[2]);
		glBindVertexArray(VertexArrayID[2]);
		//�O�p�`�̕`��I
		//glDrawArrays(GL_TRIANGLES, 0, ver3); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`
		glDrawArrays(GL_TRIANGLES, 0, ver[2]); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`

		//�^�����ȉ�ʂ𒣂�t����I�u�W�F�N�g��`��
		glBindTexture(GL_TEXTURE_2D, Texture[3]);
		glBindVertexArray(VertexArrayID[3]);
		//�O�p�`�̕`��I
		//glDrawArrays(GL_TRIANGLES, 0, ver4); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`
		glDrawArrays(GL_TRIANGLES, 0, ver[3]); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`

		//�X�^�[�g��ʂ�`��
		glBindTexture(GL_TEXTURE_2D, Texture[4]);
		glBindVertexArray(VertexArrayID[4]);
		//�O�p�`�̕`��I
		//glDrawArrays(GL_TRIANGLES, 0, ver5); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`
		glDrawArrays(GL_TRIANGLES, 0, ver[4]); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`


		//glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();


		//printf("%3f  %3f  %3f\n", position.x, position.y, position.z);

		//if (fabs(position.x - cube.x) < 1.0 && fabs(position.z - cube.z) < 1.0) {
		//	std::cout << "�������Ă��܂�\n";
		//}
		//else {
		//	std::cout << "�������ĂȂ���[�[�[�[�[�[�[\n";
		//}

	}

	glfwTerminate();

	return 0;



}


// �v���O�����̎��s: Ctrl + F5 �܂��� [�f�o�b�O] > [�f�o�b�O�Ȃ��ŊJ�n] ���j���[
// �v���O�����̃f�o�b�O: F5 �܂��� [�f�o�b�O] > [�f�o�b�O�̊J�n] ���j���[

// ��Ƃ��J�n���邽�߂̃q���g: 
//    1. �\�����[�V���� �G�N�X�v���[���[ �E�B���h�E���g�p���ăt�@�C����ǉ�/�Ǘ����܂� 
//   2. �`�[�� �G�N�X�v���[���[ �E�B���h�E���g�p���ă\�[�X�Ǘ��ɐڑ����܂�
//   3. �o�̓E�B���h�E���g�p���āA�r���h�o�͂Ƃ��̑��̃��b�Z�[�W��\�����܂�
//   4. �G���[�ꗗ�E�B���h�E���g�p���ăG���[��\�����܂�
//   5. [�v���W�F�N�g] > [�V�������ڂ̒ǉ�] �ƈړ����ĐV�����R�[�h �t�@�C�����쐬���邩�A[�v���W�F�N�g] > [�����̍��ڂ̒ǉ�] �ƈړ����Ċ����̃R�[�h �t�@�C�����v���W�F�N�g�ɒǉ����܂�
//   6. ��قǂ��̃v���W�F�N�g���ĂъJ���ꍇ�A[�t�@�C��] > [�J��] > [�v���W�F�N�g] �ƈړ����� .sln �t�@�C����I�����܂�

/*
�s�����
glm::mat4 myMatrix;
glm::vec4 myVector;
// ���炩�̕��@��myMatrix��myVector�������B
glm::vec4 transformedVector = myMatrix * myVector; // ������x�����܂����A���̏��Ԃł��I����͏d�v�Ȃ��Ƃł�
���s�ړ��s��
glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f));
glm::vec4 myVector(10.0f, 10.0f, 10.0f, 0.0f);
glm::vec4 transformedVector = myMatrix * myVector; // guess the result
�P�ʍs��
glm::mat4 myIdentityMatrix = glm::mat4(1.0f);
�g��k���s��
glm::mat4 myScalingMatrix = glm::scale(2.0f, 2.0f ,2.0f);
��]�s��
glm::vec3 myRotationAxis( ??, ??, ??);
glm::rotate( angle_in_degrees, myRotationAxis );
�ϊ��̑g�ݍ��킹
TransformedVector = TranslationMatrix * RotationMatrix * ScaleMatrix * OriginalVector;

glm::mat4 myModelMatrix = myTranslationMatrix * myRotationMatrix * myScaleMatrix;
glm::vec4 myTransformedVector = myModelMatrix * myOriginalVector;

�r���[�s��
// #include <glm/gtc/matrix_transform.hpp> �� #include <glm/gtx/transform.hpp> ���g���܂��B
glm::mat4 ViewMatrix = glm::translate(glm::mat4(), glm::vec3(-3.0f, 0.0f, 0.0f));

glm::mat4 CameraMatrix = glm::LookAt(
	cameraPosition, // ���[���h��Ԃł̃J�����̈ʒu
	cameraTarget,   // ���[���h��Ԃł̌������ʒu
	upVector        // ���Ԃ�glm::vec3(0,1,0)�ł��B�����(0,-1,0)�ɂ�����㉺�t���܂ɂȂ�܂��B������܂��ǂ��ł��傤�B
);

�ˉe�s��
// �ǂނ̂�����s������܂��B����ł��A���ʂ̕W����4x4�s��ł��B
glm::mat4 projectionMatrix = glm::perspective(
	glm::radians(FoV), // ���������̃r���[�̍L����x�����i���W�A���j�B�܂�A"�Y�[��"�̓x�����B"�J���������Y"���l���Ă��������B�ʏ�90&deg;(�����C�h) �� 30&deg; (�ƂĂ��Y�[���C�����Ă�)�̊Ԃł��B
	4.0f / 3.0f,       // �A�X�y�N�g��B�E�B���h�E�̃T�C�Y�Ɉˑ����܂��B4/3 == 800/600 == 1280/960�ƂȂ��Ă��܂��B
	0.1f,              // �߂��̃N���b�s���O���ʁB�ł��邾���傫�����܂��B�������Ȃ��Ɛ��m���̖�肪�o�Ă���ł��傤�B
	100.0f             // �����̃N���b�s���O���ʁB�ł��邾�����������܂��B
);


*/
/*
mat4 myMatrix;
vec4 myVector;
// ���炩�̕��@��myMatrix��myVector�������B
vec4 transformedVector = myMatrix * myVector; // �����ł��AGLM�ƂƂĂ����Ă��܂��B

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