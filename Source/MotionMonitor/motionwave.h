//
// cMotionWave
//
// 모션 시뮬레이터 입력으로 쓰이는 정보를 관리한다.
// 모션 파형 정보를 저장하고 읽어오는 기능을 한다.
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
};


class cMotionWave
{
public:
	cMotionWave();
	virtual ~cMotionWave();

	void Init(const int reserveSamplingCount);
	bool Read(const string &fileName);
	bool Write(const string &fileName);

	void StartRecord();
	void Stop();
	bool Record(const float deltaSeconds, const sMotionData &data, sMotionData *out=NULL);

	void StartPlay();
	bool Play(const float deltaSeconds, sMotionData &out);
	void StopPlay();


public:
	int m_samplingRate; // 20 Hz
	float m_samplingTime; // 샘플링 시간 간격 { 1 / m_samplingRate }
	bool m_isRecord;
	vector<sMotionData> m_wave;

	// Record
	float m_recordTime; // 저장을 시작한 시간이후 부터 경과된 시간을 나타낸다. (Second 단위)
	float m_recordIncTime; // m_samplingTime 보다 클 때, 정보를 저장한다.
	sMotionData m_lastData;
	int m_storeCount;

	// Play
	float m_playTime;
	float m_playIncTime;
	bool m_isPlay;
	int m_playIndex;
};
