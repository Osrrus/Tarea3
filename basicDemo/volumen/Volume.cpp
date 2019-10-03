#include "volume.h"

Volume::Volume(const char *path){

    if(readRawfile(path)){
        
        std::cout<< "Volumen Cargado"<<std::endl;
    }
    else{
        std::cout<< "Error cargando el Volumen"<<std::endl;
    }
}

Volume::~Volume(){

}

bool Volume::readRawfile(const char *path){

    int XDIM=256, YDIM=256, ZDIM=256;
    const int size = XDIM*YDIM*ZDIM;

    FILE *pFile = fopen(path,"rb");

    if(NULL == pFile) {
        return false;
    }

    typeVoxel* dataVolume=new typeVoxel[size];
    fread(dataVolume,sizeof(typeVoxel),size,pFile);

    fclose(pFile);

    //load data into a 3D texture
    glGenTextures(1, &volumeId);
    glBindTexture(GL_TEXTURE_3D, volumeId);

    // set the texture parameters
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage3D(GL_TEXTURE_3D,0,GL_RED,XDIM,YDIM,ZDIM,0,GL_RED,GL_UNSIGNED_BYTE,dataVolume);
    delete [] dataVolume;

    return true;
}