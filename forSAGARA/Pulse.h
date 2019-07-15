//-------------------------------------------------------------------------------------------------
// File : Pulse.h
//
// Desc : Pulse Effect
// Date : Dec. 28, 2007
// Version : 1.0
// Author : Pocol
//-------------------------------------------------------------------------------------------------

#ifndef _PULSE_H_INCLUDED_
#define _PULSE_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////
// Pulse class
/////////////////////////////////////////////////////////////////////////
class Pulse
{
protected:
	float m_fColor[4];
	float m_fSpeed;
	bool m_bPulse;
	float type = 0;
public:
	Pulse();
	Pulse(float r, float g, float b, float a, float speed=1.0f, bool pulse=false);
	void Render(float fTime);
	void SetColor(float r, float g, float b, float a);
	void SetSpeed(float speed);
	void SetPulse(bool flag);
	void SetType(float type_tmp);
	float* GetColor();
	float GetSpeed();
	bool GetPulse();
	Pulse& operator = (const Pulse &pIn);
};

#endif	// _PULSE_H_INCLUDED_