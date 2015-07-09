#pragma once
class SynImagehandler
{
public:
    SynImagehandler(void){}
public:
    virtual ~SynImagehandler(void){}
public:
    virtual bool syn_image_handler(unsigned char *data,int size,double pts,int width, int height)= 0;
};
