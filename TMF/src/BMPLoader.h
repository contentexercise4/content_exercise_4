/************************************************************************
　　BMPLoader.h

　　BMP Texture Loader

　　Date : October 11, 2007
　　Version : 1.0
　　Author : Pocol
　　Memo :
*************************************************************************/

#ifndef _BMP_LOADER_H_INCLUDED_
#define _BMP_LOADER_H_INCLUDED_

//
//　include
//
#include <iostream>
#include <GL/glut.h>
using namespace std;


/////////////////////////////////////////////////////////////////////////
//　　BMPImage class
/////////////////////////////////////////////////////////////////////////
class BMPImage
{
protected:
	GLuint imageSize;
	GLubyte *imageData;
	GLenum format;
	GLuint internalFormat;
	GLuint width;
	GLuint height;
	GLuint bpp;

public:
	GLuint ID;
	BMPImage();
	~BMPImage();
	bool ReadBMP(const char *filename);
	GLuint Load(const char *filename);
};

#endif		//　_BMP_LOADER_H_INCLUDED_