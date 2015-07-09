#include "StreamBuf.h"
#include <string.h>

CStreamBuf::CStreamBuf( const void *pData ,unsigned int nLens )
{

	pBufferAddr = 0;
	nLen		= 0;
	if(nLens != 0)
	{
		pBufferAddr =new char[nLens];
		memset(pBufferAddr,0,nLens);
		memcpy(pBufferAddr,pData,nLens);
		nLen	= nLens;
	}
}

CStreamBuf & CStreamBuf::operator=( const CStreamBuf & StreamBuf)
{
    if (StreamBuf.nLen==0)
    {
        return   *this;
    }
    if(this == &StreamBuf)
        return    *this;

    //    ¸³Öµ²Ù×÷...
    if(pBufferAddr)
    {
        delete []pBufferAddr;
        pBufferAddr = 0;
        nLen        = 0;
    }
    pBufferAddr =new char[StreamBuf.nLen];
    memset(pBufferAddr,0,StreamBuf.nLen);
    memcpy(pBufferAddr,StreamBuf.pBufferAddr,StreamBuf.nLen);
    nLen	= StreamBuf.nLen;

    return    *this  ;
}

CStreamBuf::CStreamBuf( const CStreamBuf & StreamBuf )
{
    pBufferAddr = 0;
    nLen		= 0;

    if (StreamBuf.nLen == 0)
	{
        return;
	}

	pBufferAddr =new char[StreamBuf.nLen];
	memset(pBufferAddr,0,StreamBuf.nLen);
	memcpy(pBufferAddr,StreamBuf.pBufferAddr,StreamBuf.nLen);
	nLen	= StreamBuf.nLen;
}

CStreamBuf::~CStreamBuf(void)
{
    if (nLen != 0)
	{
        delete []pBufferAddr;

        pBufferAddr =NULL;
        nLen	= 0;
	}
}



char * CStreamBuf::Buf()
{
	return pBufferAddr;
}

unsigned int CStreamBuf::Len()
{
	return nLen;
}
