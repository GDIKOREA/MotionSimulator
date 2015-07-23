//
// cMotionWave
//
// ��� �ùķ����� �Է����� ���̴� ������ �����Ѵ�.
// ��� ���� ������ �����ϰ� �о���� ����� �Ѵ�.
//
//
#pragma once


struct sMotionData
{
	float yaw;
	float pitch;
	float roll;
	float heave;

	void clear();
	sMotionData& operator+=(const sMotionData &rhs);
	sMotionData& operator/=(const int n);
	sMotionData operator*(const float s);
	sMotionData operator+(const sMotionData &s);

};


class cMotionWave
{
public:
	cMotionWave();
	virtual ~cMotionWave();

	void Init(const int reserveSamplingCount);
	bool Read(const string &fileName);
	bool Write(const string &fileName);
	void Make(const int samplingRate, const int samplingCount, const sMotionData &data);
	void MakeSpline(const int samplingRate, const int interpolationRate);
	bool Insert(const cMotionWave &src, const int insertIndex, const int size=-1);
	bool Remove(const int removeIndex, const int size);

	void StartRecord();
	void Stop();
	bool Record(const float deltaSeconds, const sMotionData &data, sMotionData *out=NULL);

	void StartPlay();
	bool Play(const float deltaSeconds, sMotionData &out);
	void StopPlay();

	cMotionWave& operator=(const cMotionWave &rhs);


public:
	int m_samplingRate; // 20 Hz
	float m_samplingTime; // ���ø� �ð� ���� { 1 / m_samplingRate }
	bool m_isRecord;
	vector<sMotionData> m_wave;

	// Record
	float m_recordTime; // ������ ������ �ð����� ���� ����� �ð��� ��Ÿ����. (Second ����)
	float m_recordIncTime; // m_samplingTime ���� Ŭ ��, ������ �����Ѵ�.
	sMotionData m_lastData;
	int m_storeCount;

	// Play
	float m_playTime;
	float m_playIncTime;
	bool m_isPlay;
	int m_playIndex;
};
