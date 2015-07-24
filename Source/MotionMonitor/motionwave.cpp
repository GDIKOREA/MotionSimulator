
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

sMotionData sMotionData::operator*(const float s)
{
	sMotionData data;
	data.yaw = yaw * s;
	data.pitch = pitch * s;
	data.roll = roll * s;
	data.heave = heave * s;
	return data;
}

sMotionData sMotionData::operator+(const sMotionData &rhs)
{
	sMotionData data;
	data.yaw = yaw + rhs.yaw;
	data.pitch = pitch + rhs.pitch;
	data.roll = roll + rhs.roll;
	data.heave = heave + rhs.heave;
	return data;
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

		float s = m_playIncTime / m_samplingTime;
		s = min(s, 1); // saturation

		if (m_playIndex < (int)m_wave.size())
		{
			out = (m_wave[m_playIndex] * s) + (m_wave[m_playIndex - 1] * (1 - s));
			isNextData = true;
		}

		// ���ø� �ð��� ������ ���� ������ �����Ѵ�.
		if (m_playIncTime >= m_samplingTime)
		{
			if (m_playIndex < (int)m_wave.size())
			{
				++m_playIndex;
				isNextData = true;
			}

			m_playIncTime -= m_samplingTime;
			if (m_playIncTime > m_samplingTime)
				m_playIncTime = 0;
		}
	}

	return isNextData;
}


void cMotionWave::StopPlay()
{
	m_isPlay = false;
}


// ��� ���̺긦 �����Ѵ�.
void cMotionWave::Make(const int samplingRate, const int samplingCount, const sMotionData &data)
{
	m_samplingRate = samplingRate;
	m_samplingTime = 1.f / (float)samplingRate;
	m_storeCount = samplingCount;

	m_wave.clear();
	m_wave.reserve(samplingCount);
	for (int i = 0; i < samplingCount; ++i)
	{
		m_wave.push_back(data);
	}
}


// ���ö��� ����� �����.
// samplingRate ũ��� �ϳ��� ���ø� �������� �����Ѵ�.
// �� ���ø� ������ interpolationRate ũ�⸸ŭ ���ø��� �߰��ȴ�.
void cMotionWave::MakeSpline(const int samplingRate, const int interpolationRate)
{
	const int MAX_AXIS = 4;

	cSpline spline[MAX_AXIS]; // yaw, pitch, roll, heave
	for (int i = 0; i < MAX_AXIS; ++i)
		spline[i].Init(true, samplingRate, interpolationRate);

	// �ӽ� ������ �޸𸮸� �����Ѵ�.
	vector<sMotionData> splineWave;
	const int resSize = (m_wave.size() / samplingRate) * interpolationRate;
	splineWave.reserve(resSize);

	// ���ö��� ��� ����ϰ� ����� splineWave�� �����Ѵ�.
	for (u_int i = 0; i < m_wave.size(); i += samplingRate)
	{
		const float t = m_samplingTime * i;

		const Vector2 yaw(t, m_wave[i].yaw);
		const Vector2 pitch(t, m_wave[i].pitch);
		const Vector2 roll(t, m_wave[i].roll);
		const Vector2 heave(t, m_wave[i].heave);
		spline[0].AddPoint(yaw);
		spline[1].AddPoint(pitch);
		spline[2].AddPoint(roll);
		spline[3].AddPoint(heave);

		bool ret = true;
		vector<Vector2> out[MAX_AXIS];
		for (int k = 0; k < MAX_AXIS; ++k)
			ret = spline[k].GetInterpolations(0, 1, out[k]) && ret;

		if (ret)
		{
			for (u_int m = 0; m < out[0].size(); ++m)
			{
				sMotionData data;
				data.yaw = out[0][m].y;
				data.pitch = out[1][m].y;
				data.roll = out[2][m].y;
				data.heave = out[3][m].y;
				splineWave.push_back(data);
			}
		}
		else
		{
			// ���ö��� � ���� ù��°,�ι�° ���ø� ���� ������� ���ϴ� ������ �־�,
			// ���� ������ �״�� �����ϴ� ������ �� ������ �ذ��ߴ�.
			if (spline[0].GetStoreCount() <= 2)
			{
				sMotionData data;
				data.yaw = m_wave[i].yaw;
				data.pitch = m_wave[i].pitch;
				data.roll = m_wave[i].roll;
				data.heave = m_wave[i].heave;

				for (int m = 0; m < interpolationRate; ++m)
				{
					splineWave.push_back(data);
				}
			}

		}
	}

	// ���ö��� � ����� ��� �����ٸ�, ������ ������Ʈ �Ѵ�.
	m_wave = splineWave;
	m_samplingRate /= samplingRate;
	m_samplingRate *= interpolationRate;
	m_samplingTime = 1.f / (float)m_samplingRate;
}


// assign ������
cMotionWave& cMotionWave::operator = (const cMotionWave &rhs)
{
	if (this != &rhs)
	{
		m_samplingRate = rhs.m_samplingRate;
		m_samplingTime = rhs.m_samplingTime;

		m_isRecord = false;
		m_wave = rhs.m_wave;

		m_recordTime = 0; // ������ ������ �ð����� ���� ����� �ð��� ��Ÿ����. (Second ����)
		m_recordIncTime = 0; // m_samplingTime ���� Ŭ ��, ������ �����Ѵ�.
		m_lastData.clear();
		m_storeCount = 0;

		// Play
		m_playTime = 0;
		m_playIncTime = 0;
		m_isPlay = false;
		m_playIndex = 0;
	}
	return *this;
}


// ��ǿ��̺긦 insertIndex ��ġ�� size��ŭ �����Ѵ�.
bool cMotionWave::Insert(const cMotionWave &src, const int insertIndex, const int size) // size=-1
{
	const int copySize = (size == -1) ? src.m_wave.size() : size;
	if ((int)src.m_wave.size() < copySize)
		return false;

	for (int i = 0; i < copySize; ++i)
		m_wave.push_back(sMotionData()); // �ӽ������� �߰��Ѵ�.

	// size ��ŭ, ���������� ����Ʈ�Ѵ�.
	//std::rotate(m_wave.begin() + insertIndex + copySize, m_wave.begin() + insertIndex, m_wave.end());
	if (copySize > 1)
		memmove(&m_wave[insertIndex + copySize], &m_wave[insertIndex], sizeof(m_wave[0]) * copySize);

	// insertIndex ��ġ���� �ϳ��� �����Ѵ�.
	for (int i = 0; i < copySize; ++i)
	{
		// ���� ó��
		if ((int)m_wave.size() <= i + insertIndex)
			break;
		if ((int)src.m_wave.size() <= i)
			break;

		m_wave[i + insertIndex] = src.m_wave[i];
	}

	return true;
}


// removeIndex��ġ�� ��ǿ��̺� ������ size��ŭ �����Ѵ�.
bool cMotionWave::Remove(const int removeIndex, const int size)
{
	// size ��ŭ, �������� ����Ʈ�Ѵ�.
	std::rotate(m_wave.begin() + removeIndex, m_wave.begin() + removeIndex + size, m_wave.end());

	// ����Ʈ�� ��ŭ, �����Ѵ�.
	for (int i = 0; i < size; ++i)
		m_wave.pop_back();

	return true;
}
