
#include "stdafx.h"
#include "mixingconfig.h"


const string g_minxingConfigFileName = "mixing_conf.cfg";

cMixingConfig::cMixingConfig()
{

}

cMixingConfig::~cMixingConfig()
{
	//Save(g_minxingConfigFileName);
}


void cMixingConfig::InitDefault() 
{
	m_inputType = 0;

	m_input1_enable = true;
	m_rate1_all = 1;
	m_rate1_yaw = 1;
	m_rate1_pitch = 1;
	m_rate1_roll = 1;
	m_rate1_heave = 1;

	m_input2_enable = false;
	m_rate2_all = 1;
	m_rate2_yaw = 1;
	m_rate2_pitch = 1;
	m_rate2_roll = 1;
	m_rate2_heave = 1;

	m_input3_enable = false;
	m_rate3_all = 1;
	m_rate3_yaw = 1;
	m_rate3_pitch = 1;
	m_rate3_roll = 1;
	m_rate3_heave = 1;
}


void cMixingConfig::UpdateParseData() 
{
	m_inputType = 0;

	const string input = m_options["input"];
	if (!input.empty())
	{
		vector<string> toks;
		common::tokenizer(input, "+", "", toks);
		
		for each (auto &str in toks)
		{
			if (str == "joystick")
			{
				m_inputType |= INPUT_JOYSTICK;
			}
			else if (str == "udp")
			{
				m_inputType |= INPUT_UDP;
			}
			else if (str == "motionwave")
			{
				m_inputType |= INPUT_MOTIONWAVE;
			}
		}
	}

	m_input1_enable = GetBool("mixing_input1_enable");
	m_rate1_all = GetFloat("mixing_rate1_all");
	m_rate1_yaw = GetFloat("mixing_rate1_yaw");
	m_rate1_pitch = GetFloat("mixing_rate1_pitch");
	m_rate1_roll = GetFloat("mixing_rate1_roll");
	m_rate1_heave = GetFloat("mixing_rate1_heave");

	m_input2_enable = GetBool("mixing_input2_enable");
	m_rate2_all = GetFloat("mixing_rate2_all");
	m_rate2_yaw = GetFloat("mixing_rate2_yaw");
	m_rate2_pitch = GetFloat("mixing_rate2_pitch");
	m_rate2_roll = GetFloat("mixing_rate2_roll");
	m_rate2_heave = GetFloat("mixing_rate2_heave");

	m_input3_enable = GetBool("mixing_input3_enable");
	m_rate3_all = GetFloat("mixing_rate3_all");
	m_rate3_yaw = GetFloat("mixing_rate3_yaw");
	m_rate3_pitch = GetFloat("mixing_rate3_pitch");
	m_rate3_roll = GetFloat("mixing_rate3_roll");
	m_rate3_heave = GetFloat("mixing_rate3_heave");
}

