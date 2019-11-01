//
//  main.cpp
//  LearnOpenGL
//
//  Created by changbo on 2019/10/12.
//  Copyright © 2019 CB. All rights reserved.
//

#include "GLTools.h"
#include "GLShaderManager.h"
#include "GLFrustum.h"
#include "GLBatch.h"
#include "GLFrame.h"
#include "GLMatrixStack.h"
#include "GLGeometryTransform.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

// 着色器管理器
GLShaderManager      shaderManager;

// 模型视图矩阵
GLMatrixStack        modelViewMatrix;

// 投影矩阵
GLMatrixStack        projectionMatrix;

// 角色帧 照相机角色帧（全局照相机实例）
GLFrame              cameraFrame;

GLFrame              objectFrame;

// 视景体
GLFrustum            viewFrustum;

// 金字塔批处理
GLBatch               pyramidBatch;

// 纹理变量，一般使用无符号整型
GLuint                textureID;

// 几何图形变换管道
GLGeometryTransform   transformPipeline;

// 矩阵
M3DMatrix44f          shadowMatrix;



// 绘制金字塔
void MakePyramid(GLBatch& pyramidBatch)
{
    // 通过 pyramidBatch 组建三角形批次
    // 参数1：类型
    // 参数2：顶点数（金字塔18个顶点）
    // 参数3：批次中应用1个纹理，默认0或者不填该参数为没有纹理
    pyramidBatch.Begin(GL_TRIANGLES, 18, 1);
    
    
    // 金字塔底部
    // 底部四边形 = 三角形X + 三角形Y
    
    // 三角形X
    // 设置法线
    // Normal3f：添加一个表面法线（法线坐标 与 Vertex顶点坐标中的Y轴一致）
    // 表面法线是有方向的向量，代表表面或者顶点面对的方向（相反的方向）。在多数的光照模式下是必须使用。
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    
    
    
    // 设置纹理坐标
    // 参数1：texture，纹理层次，对于使用存储着色器来进行渲染，设置为0
    // 参数2：s：对应顶点坐标中的x坐标 (s,t,r,q对应顶点坐标的x,y,z,w)
    // 参数3：t:对应顶点坐标中的y
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    
    // vBlackLeft点
    pyramidBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    
    //vBlackRight点
    pyramidBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    
    //vFrontRight点
    pyramidBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    
    //三角形B
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    pyramidBatch.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    pyramidBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    pyramidBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    //塔顶
    M3DVector3f vApex = { 0.0f, 1.0f, 0.0f };
    M3DVector3f vFrontLeft = { -1.0f, -1.0f, 1.0f };
    M3DVector3f vFrontRight = { 1.0f, -1.0f, 1.0f };
    M3DVector3f vBackLeft = { -1.0f, -1.0f, -1.0f };
    M3DVector3f vBackRight = { 1.0f, -1.0f, -1.0f };
    M3DVector3f n;
    
    
    // 金字塔前面
    // 三角形：（Apex，vFrontLeft，vFrontRight）
    // 纹理坐标设置，获取从三点找到一个法线坐标(三点确定一个面)
    
    // void m3dFindNormal(result,point1, point2,point3);
    // 参数1：结果
    // 参数2-4：3个顶点数据
    
    m3dFindNormal(n, vApex, vFrontLeft, vFrontRight);
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.5f, 1.0f);
    pyramidBatch.Vertex3fv(vApex);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    pyramidBatch.Vertex3fv(vFrontLeft);

    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    pyramidBatch.Vertex3fv(vFrontRight);
    
    
    // 金字塔左边
    // 三角形：（vApex, vBackLeft, vFrontLeft）
    m3dFindNormal(n, vApex, vBackLeft, vFrontLeft);
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.5f, 1.0f);
    pyramidBatch.Vertex3fv(vApex);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    pyramidBatch.Vertex3fv(vBackLeft);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    pyramidBatch.Vertex3fv(vFrontLeft);
    
    
    // 金字塔右边
    // 三角形：（vApex, vFrontRight, vBackRight）
    m3dFindNormal(n, vApex, vFrontRight, vBackRight);
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.5f, 1.0f);
    pyramidBatch.Vertex3fv(vApex);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    pyramidBatch.Vertex3fv(vFrontRight);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    pyramidBatch.Vertex3fv(vBackRight);
    
    
    // 金字塔后边
    // 三角形：（vApex, vBackRight, vBackLeft）
    m3dFindNormal(n, vApex, vBackRight, vBackLeft);
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.5f, 1.0f);
    pyramidBatch.Vertex3fv(vApex);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    pyramidBatch.Vertex3fv(vBackRight);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    pyramidBatch.Vertex3fv(vBackLeft);
    
    
    // 结束批次设置
    pyramidBatch.End();
}


// 将TGA文件加载为2D纹理
bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
    GLbyte *pBits;
    int nWidth, nHeight, nComponents;
    GLenum eFormat;
    
    //1、读纹理位，读取像素
    //参数1：纹理文件名称
    //参数2：文件宽度地址
    //参数3：文件高度地址
    //参数4：文件组件地址
    //参数5：文件格式地址
    //返回值：pBits,指向图像数据的指针
    pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
    if(pBits == NULL)
        return false;
    
    
    //2、设置纹理参数
    //参数1：纹理维度
    //参数2：为S/T坐标设置模式
    //参数3：wrapMode,环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    
    //参数1：纹理维度
    //参数2：线性过滤
    //参数3：wrapMode,环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    
    //3、精密包装像素数据
    //参数1：GL_UNPACK_ALIGNMENT,指定OpenGL如何从数据缓存区中解包图像数据
    //参数2:针对GL_UNPACK_ALIGNMENT 设置的值
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    //载入纹理
    //参数1：纹理维度
    //参数2：mip贴图层次
    //参数3：纹理单元存储的颜色成分（从读取像素图是获得）
    //参数4：加载纹理宽
    //参数5：加载纹理高
    //参数6：加载纹理的深度
    //参数7：像素数据的数据类型（GL_UNSIGNED_BYTE，每个颜色分量都是一个8位无符号整数）
    //参数8：指向纹理图像数据的指针
    glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
    eFormat, GL_UNSIGNED_BYTE, pBits);
    
    //使用完毕释放pBits
    free(pBits);
    
    
    //只有minFilter 等于以下四种模式，才可以生成Mip贴图
    // GL_NEAREST_MIPMAP_NEAREST 具有非常好的性能，并且闪烁现象非常弱
    // GL_LINEAR_MIPMAP_NEAREST 常常用于对游戏进行加速，它使用了高质量的线性过滤器
    // GL_LINEAR_MIPMAP_LINEAR 和 GL_NEAREST_MIPMAP_LINEAR 过滤器在Mip层之间执行了一些额外的插值，以消除他们之间的过滤痕迹。
    // GL_LINEAR_MIPMAP_LINEAR 三线性Mip贴图。纹理过滤的黄金准则，具有最高的精度。
    if(minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST ||
       minFilter == GL_NEAREST_MIPMAP_LINEAR || minFilter == GL_NEAREST_MIPMAP_NEAREST) {
        //加载Mip,纹理生成所有的Mip层
        //参数：GL_TEXTURE_1D、GL_TEXTURE_2D、GL_TEXTURE_3D
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    return true;
}


//为程序作一次性的设置
void SetupRC()
{
    //设置背影颜色
    //glClearColor(0.67f,1.0f,0.83f,1.0f);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f );
    
    //初始化着色管理器
    shaderManager.InitializeStockShaders();
    
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    
    //分配纹理对象 参数1:纹理对象个数，参数2:纹理对象指针
    glGenTextures(1, &textureID);
    
    //绑定纹理状态 参数1：纹理状态2D 参数2：纹理对象
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    
    //将TGA文件加载为2D纹理。
    //参数1：纹理文件名称
    //参数2&参数3：需要缩小&放大的过滤器
    //参数4：纹理坐标环绕模式
    LoadTGATexture("brick.tga", GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_CLAMP_TO_EDGE);
    
    
    //创造金字塔pyramidBatch
    MakePyramid(pyramidBatch);
    
    //相机frame MoveForward(平移)
    //参数：Z，深度（屏幕到图形的Z轴距离）
    cameraFrame.MoveForward(-10);
}


//清理…例如删除纹理对象
void ShutdownRC(void)
{
    glDeleteTextures(1, &textureID);
}


//开始渲染
void RenderScene(void)
{
    
    static GLfloat vLightPos [] = {1.0f, 1.0f, 0.0f}; //光源位置
    static GLfloat vWhite [] = {1.0f, 1.0f, 1.0f, 1.0f}; // 颜色值
    
    // 清除缓存区
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    // 当前模型视图压栈
    modelViewMatrix.PushMatrix();
    // 添加照相机矩阵
    M3DMatrix44f mCamera;
    // 从camraFrame中获取一个4*4矩阵
    cameraFrame.GetCameraMatrix(mCamera);
    // 矩阵 乘以 矩阵堆栈顶部矩阵，相乘结果存储到 堆栈的顶部 将照相机矩阵 与 当前模型矩阵相乘 压入栈顶
    modelViewMatrix.MultMatrix(mCamera);
    
    // 创建mObjectFrame矩阵
    M3DMatrix44f mObjectFrame;
    // 从objectFrame中获取矩阵，objectFrame保存的是特殊键位的变换矩阵
    objectFrame.GetMatrix(mObjectFrame);
    // 矩阵 乘以 矩阵堆栈顶部矩阵，相乘结果存储到 堆栈的顶部 将世界变换矩阵 与 当前模型矩阵相乘 压入栈顶
    modelViewMatrix.MultMatrix(mObjectFrame);
    
    // 绑定纹理，因为我们的项目中只有一个纹理。如果有多个纹理。绑定纹理很重要
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    
    // 点光源着色器
    // 参数1：GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF（着色器标签）
    // 参数2：模型视图矩阵
    // 参数3：投影矩阵
    // 参数4：视点坐标系中的光源位置
    // 参数5：基本漫反射颜色
    // 参数6：图形颜色（用纹理就不需要设置颜色。设置为0）
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
                                 transformPipeline.GetModelViewMatrix(),
                                 transformPipeline.GetProjectionMatrix(),
                                 vLightPos, vWhite, 0);
    
    // pyramidBatch 绘制
    pyramidBatch.Draw();
    
    // 模型视图出栈，恢复矩阵（push一次就要pop一次）
    modelViewMatrix.PopMatrix();
    
    // 将在后台缓冲区进行渲染，然后在结束时交换到前台
    glutSwapBuffers();
}


void SpecialKeys(int key, int x, int y) {
    
    if (key == GLUT_KEY_UP) {
        objectFrame.RotateWorld(m3dDegToRad(-5.0f), 1.0f, 0.0f, 0.0f);
    }
    if (key == GLUT_KEY_DOWN) {
        objectFrame.RotateWorld(m3dDegToRad(5.0f), 1.0f, 0.0f, 0.0f);
    }
    if (key == GLUT_KEY_LEFT) {
        objectFrame.RotateWorld(m3dDegToRad(-5.0f), 0.0f, 1.0f, 0.0f);
    }
    if (key == GLUT_KEY_RIGHT) {
        objectFrame.RotateWorld(m3dDegToRad(5.0f), 0.0f, 1.0f, 0.0f);
    }
    
    glutPostRedisplay();
}


//窗口大小改变时接受新的宽度和高度，其中0,0代表窗口中视口的左下角坐标，w，h代表像素
void ChangeSize(int w,int h)
{
    glViewport(0,0, w, h);
    
    // 创建投影矩阵
    viewFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0f, 500.0f);
    
    // 获取viewFrustum投影矩阵,然后加载到投影矩阵堆栈上
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    
    // 设置变换管道以使用两个矩阵堆栈（变换矩阵modelViewMatrix ，投影矩阵projectionMatrix）
    // 初始化GLGeometryTransform 的实例transformPipeline.通过将它的内部指针设置为模型视图矩阵堆栈 和 投影矩阵堆栈实例，来完成初始化
    // 当然这个操作也可以在SetupRC 函数中完成，但是在窗口大小改变时或者窗口创建时设置它们并没有坏处。而且这样可以一次性完成矩阵和管线的设置。
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}



int main(int argc,char* argv[])
{
    //设置当前工作目录，针对MAC OS X
    gltSetWorkingDirectory(argv[0]);
    
    //初始化GLUT库
    glutInit(&argc, argv);
    
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(800,500);
    
    glutCreateWindow("Pyramid");
    
    //注册回调函数
    glutReshapeFunc(ChangeSize);
    
    glutSpecialFunc(SpecialKeys);
    
    glutDisplayFunc(RenderScene);
    
    //驱动程序的初始化中没有出现任何问题。
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        fprintf(stderr,"glew error:%s\n",glewGetErrorString(err));
        return 1;
    }
    
    //调用SetupRC
    SetupRC();
    
    glutMainLoop();
    
    ShutdownRC();
    
    return 0;
}
