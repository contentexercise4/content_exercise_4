// TMFSample.cpp : ���̃t�@�C���ɂ� 'main' �֐����܂܂�Ă��܂��B�v���O�������s�̊J�n�ƏI���������ōs���܂��B
//

#include"pch.h"
#include "TestEnviroment.h"
#define N 7

/*
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

#define N 7


using namespace glm;

double lastTime;
glm::mat4 ProjectionM;
glm::mat4 ViewM;
float horizontalAngle = 3.14f;				// �����p�A-Z����
float verticalAngle = 0.0f;					// �����p�A0�A�������𒭂߂Ă���
glm::vec3 position = glm::vec3(0, 0, 5);	// �ʒu
double cube[47][4] = { 0 };					//cube[47][x_max,x_min,z_max,z_min]
int ModeSelect = 0;							//��ʑJ�ڂɗp����. 0:�X�^�[�g��� 1:�v���C��� 2:���U���g���
*/

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
	printf("BMP : %s\n", imagepath);
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
		if (hit(cube) == true) {
			position -= direction * deltaTime * speed;
		}
	}
	// ���֓����܂��B
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
		if (hit(cube) == true) {
			position += direction * deltaTime * speed;
		}
	}
	// �O���������܂܁A�E�֕��s�ړ����܂��B
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
		if (hit(cube) == true) {
			position -= right * deltaTime * speed;
		}
	}
	// �O���������܂܁A���֕��s�ړ����܂��B
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
		if (hit(cube) == true) {
			position += right * deltaTime * speed;
		}
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

	// Enter�L�[����������v���C��ʂɑJ�ڂ��܂�
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		ModeSelect = 1;
	}

	// �ˉe�s��F����45&deg;�A4:3��A�`��͈�0.1�P��100�P��
	ProjectionM = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	//ProjectionM = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// �J�����s��
	ViewM = glm::lookAt(
		position,				// �J�����̈ʒu
		position + direction,	// �ڕW�n�_
		up						// ����� (0,-1,0�ɃZ�b�g����Ə㉺�t���܂ɂȂ�܂��B)
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
	GLFWwindow* window = glfwCreateWindow(1960, 1080, "OpenGL Simple", NULL, NULL);

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

	//�V��������. ��̏������R���p�N�g�ɂ���.
	// #define N 5 ��21�s�ڂɒ�`����Ă��܂�. .obj(OBJFile) ��.bmp(Texture) �𑝂₷�Ƃ���N�̒l��1���₵�Ă�������. 

	int ver[N] = { 0 };					// �֐� loadOBJnoUV() �̒��ŃC���N�������g�����      
	GLuint vertexbuffer[N];				// ���ꂪ���_�o�b�t�@���w���������̂ƂȂ�܂��B
	std::vector<glm::vec3> vertices[N];	// .obj�t�@�C����x, y, z���W���i�[
	std::vector<glm::vec3>  normals[N]; // �����ɂ͎g���܂���B
	std::vector<glm::vec2>  uvs[N];		// �e�N�X�`����\��t���邽�߂̍��W������܂�
	GLuint uvbuffer[N];					// ���ꂪUV�o�b�t�@���w���������̂ƂȂ�܂��B
	GLuint VertexArrayID[N];			// .obj��.bmp�Ƃ����т��邽�߂�ID

	std::string OBJFile[N];				//.obj�t�@�C�����i�[����
	OBJFile[0] = "Map01uv.obj";
	OBJFile[1] = "Map01Bottomuv.obj";
	OBJFile[2] = "itemsample.obj";
	OBJFile[3] = "start.obj";
	OBJFile[4] = "title.obj";
	OBJFile[5] = "enter.obj";
	OBJFile[6] = "start.obj";

	GLuint Texture[N];					//.bmp�t�@�C����ǂݍ���, �֐� loadBMP_custom() �̒���ID�����蓖�Ă���. (���Ȃ킿, ID������)
	Texture[0] = loadBMP_custom("texsample.bmp");
	Texture[1] = loadBMP_custom("mapbottom.bmp");
	Texture[2] = loadBMP_custom("sampletex.bmp");
	Texture[3] = loadBMP_custom("start.bmp");
	Texture[4] = loadBMP_custom("black.bmp");
	Texture[5] = loadBMP_custom("enter.bmp");
	Texture[6] = loadBMP_custom("result.bmp");



	for (int i = 0; i < N; i++) {
		//�R�����g�A�E�g���� 722�s�� �` 925�s�ڂ̏������܂Ƃ߂��֐�
		prosessingOfOBJ(&ver[i], &vertexbuffer[i], vertices[i], normals[i], uvs[i], OBJFile[i], &uvbuffer[i], &VertexArrayID[i]);
	}

	decideCube(vertices[0]);			//�����蔻��ɗp����z��cube[][]�ɒl���i�[����

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// �O�p�`��`���܂��I
	//glDrawArrays(GL_TRIANGLES, 0, 3); // ���_0����n�܂�܂��B���v3�̒��_�ł��B&rarr;1�̎O�p�`�ł��B

	glDisableVertexAttribArray(0);

	// �O�p�`�̕`��I
	//glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`


	printf("\n");
	printf("%d\n", vertices[0].size());


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

	int p = 0;
	int flush = 0;		//1000�ɓ��B������0�ɖ߂�

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE) {
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);					// ���܂��Ȃ�
		glEnable(GL_DEPTH_TEST);								// �f�v�X�e�X�g��L���ɂ���
		glDepthFunc(GL_LESS);									// �O�̂��̂����J�����ɋ߂���΁A�t���O�����g���󂯓����
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// �X�N���[�����N���A����

		//if (hit(cube) == true) {
		//	p++;
		//	std::cout << "atari" << p << "\n";
		//}

		// �L�[�{�[�h�ƃ}�E�X�̃C���v�b�g����MVP�s����v�Z����
		computeMatricesFromInputs(window);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// ���݃o�C���h���Ă���V�F�[�_��"MVP" uniform�ɕϊ��𑗂�
		// �����_�����O����e���f�����ƁA�Ȃ��Ȃ�MVP���Ⴄ����ł��B(���Ȃ��Ƃ�M�̕������Ⴂ�܂��B)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		if (ModeSelect == 0) {

			flush++;

			//�X�^�[�g��ʂ�`��
			glBindTexture(GL_TEXTURE_2D, Texture[3]);
			glBindVertexArray(VertexArrayID[3]);
			//�O�p�`�̕`��I
			//glDrawArrays(GL_TRIANGLES, 0, ver4); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`
			glDrawArrays(GL_TRIANGLES, 0, ver[3]); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`

			//�^�����ȉ�ʂ𒣂�t����I�u�W�F�N�g��`��
			glBindTexture(GL_TEXTURE_2D, Texture[4]);
			glBindVertexArray(VertexArrayID[4]);
			//�O�p�`�̕`��I
			//glDrawArrays(GL_TRIANGLES, 0, ver5); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`
			glDrawArrays(GL_TRIANGLES, 0, ver[4]); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`

			if (flush < 250) {
				//Enter��ʂ𒣂�t����I�u�W�F�N�g��`��
				glBindTexture(GL_TEXTURE_2D, Texture[5]);
				glBindVertexArray(VertexArrayID[5]);
				//�O�p�`�̕`��I
				//glDrawArrays(GL_TRIANGLES, 0, ver4); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`
				glDrawArrays(GL_TRIANGLES, 0, ver[5]); // 12*3���_��0����n�܂� -> 12���̎O�p�` -> 6���̐����`
			}
			else if (flush == 500) {
				flush = 0;
			}
		}

		if (ModeSelect == 1) {

			// �V�F�[�_���g��

			//glUseProgram(programID);

			//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices, GL_STATIC_DRAW);
			//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


			// �O�p�`��`���܂��I
			//glDrawArrays(GL_TRIANGLES, 0, 3); // ���_0����n�܂�܂��B���v3�̒��_�ł��B&rarr;1�̎O�p�`�ł��B

			// �J�����̂ق��������Ă��Ȃ��@���̎O�p�`���J�����O���܂��B
			// glEnable(GL_CULL_FACE);

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

			//glDisableVertexAttribArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

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