#pragma once

class MediaImagehandler
{
public:
    MediaImagehandler(void){}
public:
    virtual ~MediaImagehandler(void){}
public:
    virtual bool media_image_handler(unsigned char *data,int size,int width, int height,double audio_wsec)= 0;
};
