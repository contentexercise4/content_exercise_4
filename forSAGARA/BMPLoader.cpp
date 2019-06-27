/************************************************************************
　　BMPLoader.cpp

　　BMP Texture Loader

　　Date : October 11, 2007
　　Version : 1.0
　　Author : Pocol
　　Memo :
*************************************************************************/

//
//　include
//
#include <fstream>
#include <windows.h>
#include "BMPLoader.h"
using namespace std;

//　disable Warning C4996
#pragma warning(disable : 4996)


//////////////////////////////////////////////////////////////////////////
//　　BMPImage class
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------------------------
//　　BMPImage
//　　Desc : コンストラクタ
//-----------------------------------------------------------------------------------------------------
BMPImage::BMPImage()
{
	imageSize = 0;
	imageData = NULL;
	format = GL_RGB;
	internalFormat = GL_RGB;
	width = 0;
	height = 0;
	bpp = 0;
	ID = 0;
}

//-----------------------------------------------------------------------------------------------------
//　　~BMPImage
//　　Desc : デストラクタ
//-----------------------------------------------------------------------------------------------------
BMPImage::~BMPImage()
{
	if ( imageData )
	{
		delete [] imageData;
		imageData = NULL;
	}
}

//-----------------------------------------------------------------------------------------------------
//　　ReadBMP
//　　Desc : BMPファイルの読み込み
//-----------------------------------------------------------------------------------------------------
bool BMPImage::ReadBMP(const char *filename)
{
	FILE *fp;

	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
	GLubyte temp = 0;

	//　ファイルを開く
	if ( (fp = fopen(filename, "rb")) == NULL )
	{
		cout << "Error : 指定されたファイルが開けません\n";
		cout << "File Name : " << filename << endl;
		return false;
	}

	//　ヘッダー情報の読み取り
	fread(&header, sizeof(BITMAPFILEHEADER), 1, fp);

	//　ファイルチェック
	if ( header.bfType != 0x4d42 )
	{
		cout << "Error : Bitmapファイルではありません\n";
		fclose(fp);
		return false;
	}

	//　ヘッダー情報の読み取り
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	//　幅と高さを取得
	width = bitmapInfoHeader.biWidth;
	height = bitmapInfoHeader.biHeight;

	if ( bitmapInfoHeader.biSizeImage == 0 )
	{
		bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * 3;
	}

	fseek(fp, header.bfOffBits, SEEK_SET);

	//　データサイズを決定し，メモリを確保
	imageSize = bitmapInfoHeader.biSizeImage;
	imageData = new GLubyte [imageSize];

	//　エラーチェック
	if ( !imageData )
	{
		cout << "Error : メモリが確保できませんでした\n";
		delete [] imageData;
		fclose(fp);
		return false;
	}

	//　ピクセルデータの読み込み
	fread(imageData, 1, bitmapInfoHeader.biSizeImage, fp);

	//　BGR　→　RGBに変換
	for ( int i=0; i<(int)imageSize; i+=3 )
	{
		temp = imageData[i+0];
		imageData[i+0] = imageData[i+2];
		imageData[i+2] = temp;
	}

	//　ファイルを閉じる
	fclose(fp);

	return true;
}

//-----------------------------------------------------------------------------------------------------
//　　Load
//　　Desc : BMPファイルを読み込み，テクスチャを生成
//-----------------------------------------------------------------------------------------------------
GLuint BMPImage::Load(const char *filename)
{
	if ( !ReadBMP(filename) )
		return 0;

	glGenTextures(1, &ID);

	glBindTexture(GL_TEXTURE_2D, ID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	if ( imageData )
	{
		delete [] imageData;
		imageData = NULL;
	}
	return ID;
}
