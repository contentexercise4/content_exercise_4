#pragma once

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

using namespace glm;

double lastTime;
glm::mat4 ProjectionM;
glm::mat4 ViewM;
float horizontalAngle = 3.14f;				// �����p�A-Z����
float verticalAngle = 0.0f;					// �����p�A0�A�������𒭂߂Ă���
glm::vec3 position = glm::vec3(0, 0, 5);	// �ʒu
double cube[47][4] = { 0 };					//cube[47][x_max,x_min,z_max,z_min]
int ModeSelect = 0;							//��ʑJ�ڂɗp����. 0:�X�^�[�g��� 1:�v���C��� 2:���U���g���

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

GLuint loadBMP_custom(const char * imagepath);
GLuint loadTGA_glfw(const char * imagepath);
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
bool hit(double cube[47][4]);
void decideCube(std::vector<glm::vec3> &vertices);
void computeMatricesFromInputs(GLFWwindow* window);
glm::mat4  getProjectionMatrix();
bool loadOBJ(const char * path, std::vector<glm::vec3>  & out_vertices, std::vector<glm::vec2>  & out_uvs, std::vector<glm::vec3>  & out_normals, int &ver);
bool loadOBJnoUV(const char * path, std::vector<glm::vec3>  & out_vertices, std::vector<glm::vec3>  & out_normals, int &ver);
void prosessingOfOBJ(int *ver, GLuint *vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string OBJFile, GLuint *uvbuffer, GLuint *VertexArrayID);
