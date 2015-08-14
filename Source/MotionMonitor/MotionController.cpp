
#include "stdafx.h"
#include "MotionController.h"



cMotionController::cMotionController()
	: m_yaw(0)
	, m_pitch(0)
	, m_roll(0)
	, m_heave(0)
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

