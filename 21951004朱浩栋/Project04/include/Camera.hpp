#pragma once

#include    <CELLMath.hpp>
using namespace CELL;

class Camera
{
public:
    real3       _eye;
    real3       _target;
    real3       _up;//!y轴方向
    real3       _right;//!x轴方向
    real3       _dir;
    matrix4r    _matView;//!观察矩阵
    matrix4r    _matProj;//!投影矩阵
    matrix4r    _matWorld;//!?
    real2       _viewSize;//!?
    real3       _oldLength;//!?
public:
    Camera(const real3& target = real3(0,0,0),
            const real3& eye = real3(0,0,100),
            const real3& right = real3(1,0,0))
    {
        _viewSize   =   real2(256,256);//!?
        //!init
        _matView    =   CELL::matrix4r(1);
        _matProj    =   CELL::matrix4r(1);
        _matWorld   =   CELL::matrix4r(1);
        _oldLength  =   10;//!?

        _target     =   target;
        _eye        =   eye;
        _dir        =   normalize(_target - _eye);
        _right      =   right;
        _up         =   normalize(cross(_right,_dir));

    }
    ~Camera()
    {}

    void   calcDir()
    {
        _dir = normalize(_target - _eye);
    }

    //!get&set
    real3 getEye() const 
    { 
        return _eye;
    }
    void  setEye(CELL::real3 val)
    { 
        _eye    =   val; 
    }
    real3 getTarget() const 
    { 
        return _target;
    }
    void  setTarget(CELL::real3 val)
    { 
        _target = val;
    }
    real3 getRight() const
    {
        return  _right;
    }
    void  setRight(CELL::real3 val)
    {
        _right  =   val;
    }
    real3 getUp() const
    {
        return _up;
    }
    void  setUp(CELL::real3 val)
    {
        _up = val;
    }
    real3 getDir() const
    {
        return  _dir;
    }
    real2 getViewSize()
    {
        return  _viewSize;
    }
    void  setViewSize(const real2& viewSize)
    {
        _viewSize   =   viewSize;
    }
    void  setViewSize(real x,real y)
    {
        _viewSize   =   real2(x,y);
    }
    const matrix4r& getProject() const
    {
        return  _matProj;
    }
    void  setProject(const matrix4r& proj)
    {
        _matProj    =   proj;
    }
    const matrix4r&  getView() const
    {
        return  _matView;
    }

    //!calc P
    void  ortho( real left, real right, real bottom, real top, real zNear, real zFar )
    {
        _matProj    =   CELL::ortho(left,right,bottom,top,zNear,zFar);
    }
    void  perspective(real fovy, real aspect, real zNear, real zFar)
    {
        _matProj    =   CELL::perspective<real>(fovy,aspect,zNear,zFar);
    }

    //!?
    bool  project( const real4& world, real4& screen )
    {
        screen  =   (_matProj * _matView * _matWorld) * world;
        if (screen.w == 0.0f)
        {
            return false;
        }
        screen.x    /=  screen.w;
        screen.y    /=  screen.w;
        screen.z    /=  screen.w;

        // map to range 0 - 1
        screen.x    =   screen.x * 0.5f + 0.5f;
        screen.y    =   screen.y * 0.5f + 0.5f;
        screen.z    =   screen.z * 0.5f + 0.5f;

        // map to viewport
        screen.x    =   screen.x * _viewSize.x;
        screen.y    =   _viewSize.y - (screen.y * _viewSize.y);
        return  true;
    }
    bool   unProject( const real4& screen, real4& world )
    {
        real4 v;
        v.x =   screen.x;
        v.y =   screen.y;
        v.z =   screen.z;
        v.w =   1.0;

        // map from viewport to 0 - 1
        v.x =   (v.x) /_viewSize.x;
        v.y =   (_viewSize.y - v.y) /_viewSize.y;
        //v.y = (v.y - _viewPort.Y) / _viewPort.Height;

        // map to range -1 to 1
        v.x =   v.x * 2.0f - 1.0f;
        v.y =   v.y * 2.0f - 1.0f;
        v.z =   v.z * 2.0f - 1.0f;

        CELL::matrix4r  inverse = (_matProj * _matView * _matWorld).inverse();

        v   =   v * inverse;
        if (v.w == 0.0f)
        {
            return false;
        }
        world   =   v / v.w;
        return true;
    }

    //!Convert
    real2  worldToScreen( const real3& world)
    {
        real4  worlds(world.x,world.y,world.z,1);
        real4  screens;
        project(worlds,screens);
        return  real2(screens.x,screens.y);
    }
    real3  screenToWorld(const real2& screen)
    {
        real4  screens(screen.x,screen.y,0,1);
        real4  world;
        unProject(screens,world);
        return  real3(world.x,world.y,world.z);
    }
    real3  screenToWorld(real x,real y)
    {
        real4  screens(x,y,0,1);
        real4  world;
        unProject(screens,world);
        return  real3(world.x,world.y,world.z);
    }

    //!?
    Ray createRayFromScreen(int x,int y)
    {
        real4  minWorld;
        real4  maxWorld;

        real4  screen(real(x),real(y),0,1);
        real4  screen1(real(x),real(y),1,1);

        unProject(screen,minWorld);
        unProject(screen1,maxWorld);
        Ray     ray;
        ray.setOrigin(real3(minWorld.x,minWorld.y,minWorld.z));

        real3  dir(maxWorld.x - minWorld.x,maxWorld.y - minWorld.y, maxWorld.z - minWorld.z);
        ray.setDirection(normalize(dir));
        return  ray;
    }

    virtual void    rotateViewY(real angle) 
    { 
        real        len(0);
        matrix4r    mat(1);
        mat.rotate(angle, real3(0, 1, 0));
        _dir        =   _dir * mat;
        _up         =   _up * mat;
        _right      =   CELL::normalize(cross(_dir, _up));
        len         =   CELL::length(_eye - _target);
        _eye        =   _target - _dir * len;
        _matView    =   CELL::lookAt<real>(_eye, _target, _up);
    }
    virtual void    rotateViewX(real angle) 
    { 
        real        len(0);
        matrix4r    mat(1);
        mat.rotate(angle,_right);
        _dir        =   _dir * mat;
        _up         =   _up * mat;
        _right      =   CELL::normalize(cross(_dir,_up));
        len         =   CELL::length(_eye - _target);
        _eye        =   _target - _dir * len;
        _matView    =   CELL::lookAt(_eye,_target,_up);
    }
    virtual void    rotateViewXByCenter(real angle,real3  pos)
    {
        //! 计算眼睛到鼠标点的方向
        real3  vDir    =   pos - _eye;
        /// 计算旋转点和眼睛之间的距离
        real   len1    =   length(vDir);
        /// 旋转点和眼睛直接的方向
                vDir    =   normalize(vDir);
        real   len     =   0;
            
        matrix4r mat(1);
        mat.rotate(angle, _right);

        vDir    =   vDir * mat;
        /// 推倒出眼睛的位置
        _eye    =   pos - vDir * len1;

        _dir    =   _dir * mat;
        _up     =   _up * mat;
        _right  =   CELL::normalize(CELL::cross(_dir, _up));
        len     =   CELL::length(_eye - _target);
        /// 推导出观察中心点的位置
        _target     =   _eye + _dir * len;
        _matView    =   CELL::lookAt<real>(_eye, _target, _up);

    }
    virtual void    rotateViewYByCenter(real angle,real3  pos)
    {
        real        len(0);
        real        len1(0);
        matrix4r    mat(1);
        mat.rotate(angle, real3(0, 1, 0));

        real3   vDir = pos - _eye;

        len1    =   CELL::length(vDir);
        vDir    =   CELL::normalize(vDir);
        vDir    =   vDir * mat;
        _eye    =   pos - vDir * len1;

        _dir    =   _dir * mat;
        _up     =   _up * mat;
        _right  =   CELL::normalize(cross(_dir, _up));
        len     =   CELL::length(_eye - _target);
        _target =   _eye + _dir * len;
        _matView=   CELL::lookAt<real>(_eye, _target, _up);
    }
    virtual void    rotateViewZByCenter(real angle,real3  pos)
    {
        real        len(0);
        real        len1(0);
        matrix4r    mat(1);
        mat.rotate(angle, real3(0, 0, 1));

        real3   vDir = pos - _eye;

        len1    =   CELL::length(vDir);
        vDir    =   CELL::normalize(vDir);
        vDir    =   vDir * mat;
        _eye    =   pos - vDir * len1;

        _dir    =   _dir * mat;
        _up     =   _up * mat;
        _right  =   CELL::normalize(cross(_dir, _up));
        len     =   CELL::length(_eye - _target);
        _target =   _eye + _dir * len;
        _matView=   CELL::lookAt<real>(_eye, _target, _up);
    }
    virtual void    scaleCameraByPos(const real3& pos,real persent)
    {

        real3   dir     =   CELL::normalize(pos - _eye);

        real    dis     =   CELL::length(pos - _eye) * persent;

        real    disCam  =   CELL::length(_target - _eye) * persent;

        real3   dirCam  =   CELL::normalize(_target - _eye);

        _eye    =   pos - dir * dis;
        _target =   _eye + dirCam * disCam;

        update();
    }

    virtual void    update()
    {
        _matView    =   CELL::lookAt(_eye,_target,_up);
        //!
        calcDir();
    }
};
