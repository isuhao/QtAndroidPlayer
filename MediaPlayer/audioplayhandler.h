#pragma once

class AudioPlay_handler
{
public:
    AudioPlay_handler(void){}
public:
    virtual ~AudioPlay_handler(void){}
public:
    virtual bool start_playing() = 0;
    virtual bool slose_playing() = 0;
};
