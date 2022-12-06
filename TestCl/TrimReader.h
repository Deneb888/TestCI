// Copyright 2014-2017, Anitoa Systems, LLC
// All rights reserved

#pragma once

#include <string>

#define TRIM_IMAGER_SIZE 12
#define MAX_TRIMBUFF 256

#define EPKT_SZ  52					// Not include parity, made it 52 instead of 51 for qPCR version
#define NUM_EPKT 4

class CTrimNode {

public:

	double kb[TRIM_IMAGER_SIZE][6];		// New (auto) calib method with hump factor
	double fpn[2][TRIM_IMAGER_SIZE];	// 0 - lg, 1 - hg

	unsigned int rampgen;
	unsigned int range;
	unsigned int auto_v20[2];
	unsigned int auto_v15;
	unsigned int version;				// 0 or 1, the legacy old format with smaller matrices; 2 - new floating point format, output from autocal; 3 - integer version, ready to be written to flash

	double tempcal[TRIM_IMAGER_SIZE];

	int kbi[TRIM_IMAGER_SIZE][6];		// New (auto) calib method with hump factor, all are int
	int fpni[2][TRIM_IMAGER_SIZE];		// 0 - lg, 1 - hg

	CString name;

	BYTE	trim_buff[MAX_TRIMBUFF];
	int		tbuff_size;
	int		tbuff_rptr;

public:

	CTrimNode();

private:

	void Initialize();
};

#define TRIM_MAX_NODE 4
#define TRIM_MAX_WORD 640


class CTrimReader {

protected:

	CFile InFile;

	CString WordBuf[TRIM_MAX_WORD];
	int WordIndex;
	int MaxWord;
	CString CurWord;

	// from DPReader

	BYTE	trim_buff[1024];
	int		tbuff_size;
	int		tbuff_rptr;

	BYTE	version;
	BYTE	id;
	std::string  id_str;
	BYTE	serial_number1, serial_number2;
	BYTE	num_wells, num_channels, well_format, channel_format;
	
	BYTE	num_pages;						// number of EPKT_SZ byte pages needed		

public:

	CTrimNode Node[TRIM_MAX_NODE];
	CTrimNode *curNode;
	int NumNode;

public:

	CTrimReader();
	~CTrimReader();

	int Load(TCHAR*);
	void Parse();
	void ParseNode();

	int GetNumNode() {
		return NumNode;
	}

	int ADCCorrection(int NumData, BYTE HighByte, BYTE LowByte,  int pixelNum, int PCRNum, int gain_mode, int *flag);
	int ADCCorrectioni(int NumData, BYTE HighByte, BYTE LowByte, int pixelNum, int PCRNum, int gain_mode, int *flag);


	void SetV20(BYTE v20);
	void SetGainMode(int gain);
	void SetV15(BYTE v15);
	void Capture12();
	void Capture12(BYTE);
	void Capture24();
	int  ProcessRowData(int (*adc_data)[24], int gain_mode);

	void SetRangeTrim(BYTE range);
	void SetRampgen(BYTE rampgen);
	void SetTXbin(BYTE txbin);
	void SetIntTime(float);
	void SelSensor(BYTE i);
	void SetIntTime(float kfl, BYTE ch);

	void EEPROMRead();
	void OnEEPROMRead();
	void ReadTrimData();

	// from DPReader

	BYTE TrimBuff2Byte();
	void CopyEepromBuffAndRestore();
	void RestoreFromTrimBuff();

	// from new Trimreader

	int Add2TrimBuff(int i, int);
	int Add2TrimBuff(int i, BYTE);
	int WriteTrimBuff(int i);
	int TrimBuff2Int(int i);
	BYTE TrimBuff2Byte(int i);
	void RestoreTrimBuff(int k);
	void CopyEepromBuff(int k, int index_start);

	void Convert2Int(int c);


protected:

	BOOL fileLoaded;

	void ParseMatrix();
	void ParseArray(int);
	void ParseValue(int);

private:

	int GetWord();
	int Match(CString);
};



