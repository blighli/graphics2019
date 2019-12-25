//
// Created by Moving on 2019/12/2 0002.
//

#ifndef MYGL_VERTEX_H
#define MYGL_VERTEX_H



class Vertex {
public:
    float x,y,z;
    float nx,ny,nz;
    float r,g,b;//r=-1时，g和b分别视作u和v
};

#endif //MYGL_VERTEX_H
