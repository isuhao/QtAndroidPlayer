#pragma once

class CStreamBuf
{
public:
	CStreamBuf(const void *pData ,unsigned int nLens);
	CStreamBuf(const CStreamBuf & );
	CStreamBuf & operator = (const CStreamBuf & ); //赋值符   

public:
	~CStreamBuf(void);
public:
	char * Buf();
	unsigned int Len();
private:
	char	*pBufferAddr;			//数据地址
	unsigned int nLen;				//有效长度
};
