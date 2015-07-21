
#include "stdafx.h"
#include "motionwave.h"


//-------------------------------------------------------------------------------------------
// sMotionData

void sMotionData::clear()
{
	yaw = 0;
	pitch = 0;
	roll = 0;
	heave = 0;
}

sMotionData& sMotionData::operator += (const sMotionData &rhs)
{
	if (this != &rhs)
	{
		yaw += rhs.yaw;
		pitch += rhs.pitch;
		roll += rhs.roll;
		heave += rhs.heave;
	}
	return *this;
}

sMotionData& sMotionData::operator /= (const int n)
{
	yaw /= n;
	pitch /= n;
	roll /= n;
	heave /= n;
	return *this;
}
//-------------------------------------------------------------------------------------------




cMotionWave::cMotionWave() :
	m_samplingRate(20)
	, m_isRecord(false)
	, m_isPlay(false)
	, m_recordTime(0)
	, m_recordIncTime(0)
{
	m_samplingTime = 1.f / (float)m_samplingRate;

	// �� 1�а����� ������ ������ �� �ִ� ���۸� ��Ƴ��´�.
	Init(m_samplingRate * 60);
}

cMotionWave::~cMotionWave()
{

}


// ������� ���۸� �̸� ��� ���´�.
void cMotionWave::Init(const int reserveSamplingCount)
{
	if (reserveSamplingCount > 0)
		m_wave.reserve(reserveSamplingCount);
}


// ��ǿ��̺� ������ �о�´�.
bool cMotionWave::Read(const string &fileName)
{
	using namespace std;

	ifstream ifs(fileName, ios::binary);
	if (!ifs.is_open())
		return false;

	char header[4];
	ifs.read(header, 4);

	if ((header[0] != 'M') ||
		(header[1] != 'W') ||
		(header[2] != 'A') ||
		(header[3] != 'V'))
		return false;

	int samplingRate = 0;
	ifs.read((char*)&samplingRate, sizeof(int));

	int samplingCount = 0;
	ifs.read((char*)&samplingCount, sizeof(int));

	m_wave.clear();

	if (samplingCount > 0)
	{
		m_wave.reserve(samplingCount);
		for (int i = 0; i < samplingCount; ++i)
		{
			sMotionData data;
			ifs.read((char*)&data, sizeof(sMotionData));
			m_wave.push_back(data);
		}
	}

	m_samplingRate = samplingRate;
	m_samplingTime = 1.f / (float)m_samplingRate;

	return true;
}


// ��ǿ��̺� ������ �����Ѵ�.
bool cMotionWave::Write(const string &fileName)
{
	using namespace std;

	ofstream ofs(fileName, ios::binary);
	if (!ofs.is_open())
		return false;

	ofs.write("MWAV", 4);

	const int samplingRate = m_samplingRate;
	ofs.write((char*)&samplingRate, sizeof(int));

	const int samplingCount = m_wave.size();
	ofs.write((char*)&samplingCount, sizeof(int));

	for (int i = 0; i < samplingCount; ++i)
	{
		ofs.write((char*)&m_wave[i], sizeof(sMotionData));
	}

	return true;
}


// ������� ������ �����ϱ� �����Ѵ�. (�ʱ�ȭ �Ѵ�)
void cMotionWave::StartRecord()
{
	m_isRecord = true;
	m_recordTime = 0;
	m_storeCount = 0;
	m_wave.clear();

	// �� 1�а����� ������ ������ �� �ִ� ���۸� ��Ƴ��´�.
	Init(m_samplingRate * 60);
}


// ������� ���� ������ �����Ѵ�.
void cMotionWave::Stop()
{
	m_isRecord = false;
}


// ��� ������ �ϳ��� �����Ѵ�.
// ���ø��ð��� �Ѱܼ�, ������ ������ �Ǹ� true�� ���ϵǰ�, ����� ���� out�� �־ ���ϵȴ�.
// out �� NULL �̸�, �ƹ��� ����.
bool cMotionWave::Record(const float deltaSeconds, const sMotionData &data, sMotionData *out) // out = NULL
{
	bool isWrite = false; // ���� ����Ǹ� true�� �ȴ�.

	if (m_recordTime == 0)
	{
		m_recordTime = 0.00001f;
		m_recordIncTime = 0.00001f;
		m_wave.push_back(data);

		if (out)
			*out = data;

		m_storeCount = 0;
		m_lastData.clear();
		isWrite = true;
	}
	else
	{
		m_recordTime += deltaSeconds;
		m_recordIncTime += deltaSeconds;
		++m_storeCount;
		m_lastData += data;

		// ���ø� �ð��� ������ �����Ѵ�.
		if (m_recordIncTime >= m_samplingTime)
		{
			m_lastData /= m_storeCount;
			m_wave.push_back(m_lastData);

			if (out)
				*out = m_lastData;

			//m_recordIncTime -= m_samplingTime;
			m_recordIncTime = 0;
			m_storeCount = 0;
			m_lastData.clear();
			isWrite = true;
		}
	}

	return isWrite;
}


void cMotionWave::StartPlay()
{
	m_isPlay = true;
	m_playTime = 0;
	m_playIncTime = 0;
	m_playIndex = 0;
}


bool cMotionWave::Play(const float deltaSeconds, sMotionData &out)
{
	RETV(!m_isPlay, false);
	RETV(m_wave.empty(), false);

	bool isNextData = false;

	if (m_playTime == 0)
	{
		m_playTime = 0.0001f;
		m_playIncTime = 0.001f;
		out = m_wave[0];
		++m_playIndex;
		isNextData = true;
	}
	else
	{
		m_playTime += deltaSeconds;
		m_playIncTime += deltaSeconds;

		// ���ø� �ð��� ������ ���� ������ �����Ѵ�.
		if (m_playIncTime >= m_samplingTime)
		{
			if (m_playIndex < (int)m_wave.size())
			{
				out = m_wave[m_playIndex];
				++m_playIndex;

				isNextData = true;
			}

			//m_playIncTime -= m_samplingTime;
			m_playIncTime = 0;
		}
	}

	return isNextData;
}


void cMotionWave::StopPlay()
{
	m_isPlay = false;
}
