# 编程作业3：复杂三维模型显示 #


## 作业要求 ##
- 支持以下三维模型文件格式中至少1种：
 - OBJ
 - 3DS
 - STL
 - 提示：可以使用开源库 Open Asset Import Library
2.	支持多个光源的光照效果，使用着色器渲染
3.	支持多种视点浏览方式

## 作业环境 ##
- Window 10， VS 2015
- OpenGL 3.3
- 模型载入：assimp 5.0.0 source


## 作业3完成情况： ##
- 使用ASSIMP完成obj格式模型载入
- 实现了4个光源的光照效果，搭配模型与箱子纹理，使用着色器渲染
- 实现了鼠标键盘控制移动视角

作业参考：[LearnOpenGL](https://learnopengl-cn.github.io/03%20Model%20Loading/01%20Assimp/)

## 作业成果 ##
![result](result.gif)