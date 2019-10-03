#pragma once

#include "../define.h"

class Volume
{   
    private:
        bool  readRawfile(const char *path);
        
    public:
        unsigned int volumeId;

        Volume(const char *path);
        ~Volume();
};