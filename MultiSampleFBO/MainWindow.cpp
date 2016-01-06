#include <ZGL/QGLTool.hpp>
#include "MainWindow.hpp"
#include <QString>
#include <QList>
#include <QDebug>
#include <QTimer>

extern QList<QString> glslFileSearchPath;

class MultiSampleTextureRender {
    class Data {
    public:
        GLuint program;
        GLuint vao;
        ~Data() {
            glDeleteVertexArrays(1, &vao);
            glDeleteProgram(program);
        }
    };
    std::shared_ptr<Data> data;
    void init() {
        data = std::shared_ptr<Data>(new Data);
        constexpr static char vs[] = R"(#version 450
uniform mat4 points = mat4(
-1,1,0,1,
-1,-1,0,1,
1,1,0,1,
1,-1,0,1
);
smooth out vec2 tuv;
void main(){
gl_Position = points[gl_VertexID];
tuv = gl_Position.xy/2+0.5;
}
)";
        constexpr static char fs[] = R"(#version 450
smooth in vec2 tuv;
out vec4 color ;
layout(binding=0) uniform sampler2DMS stex;
layout(location=1) uniform int sampleSize = 8;
void main(){
ivec2 ts = textureSize( stex );
color = vec4(0,0,0,0);
int i =0;
for( ;i<sampleSize;++i ){
color += texelFetch( stex,ivec2(ts*tuv),i );
}
color /= sampleSize;
}
)";
        data->program=createProgram({ 
            {GLSLShaderType::FRAGMENT_SHADER,fs},
            {GLSLShaderType::VERTEX_SHADER,vs}
        });
        glCreateVertexArrays(1, &(data->vao));
    }
public:
    MultiSampleTextureRender() {

    }
    void render(GLuint texture,GLint samples=8) {
        if (false == data) { init(); }
        glUseProgram(data->program);
        glBindVertexArray(data->vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,texture);
        glUniform1i( 1, samples );
        glDrawArrays( GL_TRIANGLE_STRIP,0,4 );
    }
};

class MultiSampleFBO {
    MultiSampleFBO(const MultiSampleFBO &)=delete;
    MultiSampleFBO(MultiSampleFBO &&)=delete;
    MultiSampleFBO&operator=(const MultiSampleFBO &)=delete;
    MultiSampleFBO&operator=(MultiSampleFBO &&)=delete;

    int width=0;
    int height=0;
    bool isOK=false;
    GLuint fbo=0;
    GLuint depth=0;
    GLuint color=0;

public:

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    GLuint getTextureColor()const { return color; }
    GLuint getTextureDepth() const { return depth; }

    operator GLuint() const { return fbo; }

    MultiSampleFBO(const int width_=512,const int height_=512):
        width(width_),
        height(height_) {
        if (width_<1) { return; }if (height_<1) { return; }
        __init_frame_buffer();
        if (isOK) { return; }
        __free_gldata();
    }

    ~MultiSampleFBO() {
        if (fbo) { __free_gldata(); }
    }

    void draw(const GLuint drawfbo ) {
        glBindFramebuffer(  GL_FRAMEBUFFER, drawfbo );
    }

private:
    void __free_gldata() {
        glDeleteFramebuffers(1,&fbo);
        glDeleteTextures(1,&depth);
        glDeleteTextures(1,&color);
        fbo=0; depth=0; color=0;
    }
    void __init_frame_buffer( ){

        glCreateFramebuffers(1,&fbo);
        if(0==fbo){return;}

        glCreateTextures( GL_TEXTURE_2D_MULTISAMPLE,1,&color);
        glCreateTextures( GL_TEXTURE_2D_MULTISAMPLE,1,&depth);
        if(0==color){return;}
        if(0==depth){return;}

        glTextureParameteri(depth, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(depth, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(color, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(color, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTextureStorage2DMultisample(
            color,
            8 ,
            GL_RGBA32F ,
            width,
            height,
            false /*??*/
            );

        glTextureStorage2DMultisample(
            depth,
            8,
            GL_DEPTH_COMPONENT32F,
            width,
            height,
            false /*??*/
            );

        glNamedFramebufferTexture(
                    fbo,
                    GL_DEPTH_ATTACHMENT,
                    depth,0/* ?? */
                    );

        glNamedFramebufferTexture(
            fbo,
            GL_COLOR_ATTACHMENT0,
            color, 0 /* ?? */
            );

        //check
        GLenum fboStatus = glCheckNamedFramebufferStatus(
                    fbo,
                    GL_DRAW_FRAMEBUFFER);

        if (fboStatus!=GL_FRAMEBUFFER_COMPLETE){
            isOK = false;
            switch ( fboStatus )
            {
                 case GL_FRAMEBUFFER_UNDEFINED: qDebug() << "GL_FRAMEBUFFER_UNDEFINED"; break;
                 case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:qDebug() << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
                 case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:qDebug() << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
                 case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:qDebug() << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
                 case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:qDebug() << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
                 case GL_FRAMEBUFFER_UNSUPPORTED:qDebug() << "GL_FRAMEBUFFER_UNSUPPORTED"; break;
                 case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:qDebug() << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
                 case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:qDebug() << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
                 default:
                break;
            }
            return ;
        }
        else{
            isOK = true;
        }
    }
};

class MainWindow::MainWindowData {
public:
    std::shared_ptr<MultiSampleFBO> fbo;
    MultiSampleTextureRender render;
    unsigned int resizeIndex=0;
    bool isResize=false;
    MainWindowData() {}
};

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent){
    qGLWidgetInitializeGlew(this);
}

MainWindow::~MainWindow(){
    delete thisData;
}


void MainWindow::paintGL() {
    if (thisData->isResize) { 
        glClearColor(0.5f,0.5f,0.5f,1 );
        glClear(GL_COLOR_BUFFER_BIT);
       return; 
    }
    auto fbo_=thisData->fbo;
    if (fbo_) {
        
        const auto & fbo=*fbo_;
        glBindFramebuffer(GL_FRAMEBUFFER,fbo);
        glViewport(0,0,fbo.getWidth(),fbo.getHeight());
 
        glClearColor((rand()%1000)/1000.0f,(rand()%1000)/1000.0f,(rand()%1000)/1000.0f,1 );
        glClear(GL_COLOR_BUFFER_BIT);
       
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,width(),height());
        thisData->render.render(fbo.getTextureColor());
       
    }
}

void MainWindow::__resizeGL(int w,int h) {
    auto fbo=thisData->fbo;
    if ( fbo && (w==fbo->getWidth() ) && (h==fbo->getHeight() ) ) {}
    else {
        thisData->fbo=std::make_shared<MultiSampleFBO>(w,h);
        fbo=thisData->fbo;
    }
    updateGL();
}

void MainWindow::resizeGL(int w, int h){
    thisData->isResize=true;
    auto thisIndex = ++( thisData->resizeIndex);
    QTimer::singleShot(58,this,[this,thisIndex,w,h](){
        if (thisIndex==thisData->resizeIndex) {
            thisData->isResize=false;
            __resizeGL(w,h);
        }
    });
       

}

void MainWindow::initializeGL(){
    if (thisData) { return; }
    setSimpleCallbackFunction();
    thisData=new MainWindowData;
    thisData->fbo=std::make_shared<MultiSampleFBO>(width(),height());
}


