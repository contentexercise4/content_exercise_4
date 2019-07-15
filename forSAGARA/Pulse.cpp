//---------------------------------------------------------------------------------------------------
// File : Pulse.cpp
//
// Desc : Pulse Effect
// Date : Dec. 28, 2007
// Version : 1.0
// Author : Pocol
//----------------------------------------------------------------------------------------------------

//
// include
//
#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include "Pulse.h"
using namespace std;

GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };//白色

////////////////////////////////////////////////////////////////////////
//  Pluse class
////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------
// Name : Pulse()
// Desc : コンストラクタ#1
//---------------------------------------------------------------------------------------------------
Pulse::Pulse()
{
	m_fColor[0] = 1.0f;
	m_fColor[1] = 1.0f;
	m_fColor[2] = 1.0f;
	m_fColor[3] = 1.0f;
	m_fSpeed = 1.0f;
	m_bPulse = false;
}

//--------------------------------------------------------------------------------------------------
// Name : Pulse()
// Desc : コンストラクタ#2
//--------------------------------------------------------------------------------------------------
Pulse::Pulse(float r, float g, float b, float a, float speed, bool pulse)
{
	m_fColor[0] = r;
	m_fColor[1] = g;
	m_fColor[2] = b;
	m_fColor[3] = a;
	m_fSpeed = speed;
	m_bPulse = pulse;
}

//----------------------------------------------------------------------------------------------------
// Name : operator = ()
//----------------------------------------------------------------------------------------------------
Pulse& Pulse::operator =(const Pulse &pIn)
{
	m_fColor[0] = pIn.m_fColor[0];
	m_fColor[1] = pIn.m_fColor[1];
	m_fColor[2] = pIn.m_fColor[2];
	m_fColor[3] = pIn.m_fColor[3];
	m_fSpeed = pIn.m_fSpeed;
	m_bPulse = pIn.m_bPulse;
	return (*this);
}

//---------------------------------------------------------------------------------------------------
// Name : SetColor()
// Desc : 色の設定
//---------------------------------------------------------------------------------------------------
void Pulse::SetColor(float r, float g, float b, float a)
{
	m_fColor[0] = r;
	m_fColor[1] = g;
	m_fColor[2] = b;
	m_fColor[3] = a;
}

//---------------------------------------------------------------------------------------------------
// Name : SetSpeed()
// Desc : スピードの設定
//---------------------------------------------------------------------------------------------------
void Pulse::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

//---------------------------------------------------------------------------------------------------
// Name : SetPulse()
// Desc : フラグの設定
//---------------------------------------------------------------------------------------------------
void Pulse::SetPulse(bool flag)
{
	m_bPulse = flag;
}

//---------------------------------------------------------------------------------------------------
// Name : SetPulse()
// Desc : タイプの設定
//---------------------------------------------------------------------------------------------------
void Pulse::SetType(float type_tmp)
{
	type = type_tmp;
}

//---------------------------------------------------------------------------------------------------
// Name : GetColor()
// Desc : 色の取得
//---------------------------------------------------------------------------------------------------
float* Pulse::GetColor()
{
	return m_fColor;
}

//---------------------------------------------------------------------------------------------------
// Name : GetSpeed()
// Desc : スピードの取得
//---------------------------------------------------------------------------------------------------
float Pulse::GetSpeed()
{
	return m_fSpeed;
}

//---------------------------------------------------------------------------------------------------
// Name : GetPulse()
// Desc : フラグの取得
//---------------------------------------------------------------------------------------------------
bool Pulse::GetPulse()
{
	return m_bPulse;
}

//---------------------------------------------------------------------------------------------------
// Name : Render()
// Desc : 描画処理
//---------------------------------------------------------------------------------------------------
void Pulse::Render(float fTime)
{
	static float phase = 0.0f;
	float r=0.0f, g=0.0f, b=0.0f;

	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT );
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if ( m_bPulse )	phase = 2.0*(1.0f + (float)sin(fTime * m_fSpeed) );	
	else phase = (float)fmod(fTime * m_fSpeed, 4.0f);	

	r = m_fColor[0] - ((m_fColor[0]/3.0f) * phase);
	g = m_fColor[1] - ((m_fColor[1]/3.0f) * phase);
	b = m_fColor[2] - ((m_fColor[2]/3.0f) * phase);
	m_fColor[3] = m_fColor[3] - ((m_fColor[3]/3.0f) * phase);
	glColor4f(r, g, b, m_fColor[3]);
	//cout << type << endl;

	if (type == 1 || type == 2 || type == 3 || type == 4 || type == 5) {
		glEnable(GL_TEXTURE_2D);
		if (type == 1) {
			glBindTexture(GL_TEXTURE_2D, 12);
		}
		if (type == 2) {
			glBindTexture(GL_TEXTURE_2D, 13);
		}
		if (type == 3) {
			glBindTexture(GL_TEXTURE_2D, 14);
		}
		if (type == 4) {
			glBindTexture(GL_TEXTURE_2D, 15);
		}
		if (type == 5) {
			glBindTexture(GL_TEXTURE_2D, 16);
		}
		//GLfloat color2[4] = { r, g, b, m_fColor[3] };
		//glColor4fv(color2);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		//glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color);
	}

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();

	if (type==1 || type==2 || type==3 || type == 4 || type == 5) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopAttrib();

}