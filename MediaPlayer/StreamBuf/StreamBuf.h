#pragma once

class CStreamBuf
{
public:
	CStreamBuf(const void *pData ,unsigned int nLens);
	CStreamBuf(const CStreamBuf & );
	CStreamBuf & operator = (const CStreamBuf & ); //��ֵ��   

public:
	~CStreamBuf(void);
public:
	char * Buf();
	unsigned int Len();
private:
	char	*pBufferAddr;			//���ݵ�ַ
	unsigned int nLen;				//��Ч����
};
