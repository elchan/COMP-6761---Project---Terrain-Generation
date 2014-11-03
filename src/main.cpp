#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"
#include <memory>


//========================================================================
int main( ){
    
    ofSetOpenGLVersion(3, 3);
//    std::shared_ptr<ofAppGLFWWindow> glWindow(new ofAppGLFWWindow());
//    glWindow->setOpenGLVersion(3, 3);
    
    
//    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
