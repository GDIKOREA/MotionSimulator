//
// pid, defference, scaling ����� �ϴ� Ŭ����
//
// ��ũ��Ʈ�κ��� �Ķ���� ���� �޾ƿ� ��, ����Ѵ�.
//
#pragma once

#include "modulator.h"


class cSimpleModulator : public common::cConfig, public cModulator
{
public:
	cSimpleModulator();
	virtual ~cSimpleModulator();


protected:
	virtual void InitDefault() override;
	virtual void UpdateParseData() override;
};
