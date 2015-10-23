
#include "stdafx.h"
#include "MotionController.h"



cMotionController::cMotionController()
	: m_yaw(255)
	, m_pitch(255)
	, m_roll(255)
	, m_heave(512)
{

}

cMotionController::~cMotionController()
{

}


bool cMotionController::ReadConfigFile(const string &fileName)
{

	return true;
}


bool cMotionController::WriteConfigFile(const string &fileName)
{

	return true;
}

