/**
 *
 * OFDevCon Example Code Sprint
 * Quaternion Example for rotating a sphere as an arcball
 * Dragging the mouse up down left right to apply an intuitive rotation to an object.
 *
 * Created by James George on 2/23/2012
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofNoFill();
	ofSetFrameRate(30);
	ofBackground(0);

	//this slows down the rotate a little bit
	dampen = .4;
    
    canvas = new ofxUICanvas(0, 0, 100, 130);
    canvas->addToggle("Wireframe", &wireframeEnabled);
    slider = canvas->addIntSlider("Iterations", 0, Terrain::MAX_ITERATIONS, 0);
    resetBtn = canvas->addButton("Reset", false);
    saveBtn = canvas->addButton("Save", false);
    loadBtn = canvas->addButton("Load", false);
//    canvas->addMultiImageToggle("Wireframe", "GUI/toggle.png", &wireframeEnabled); //, <#float w#>, <#float h#>)
    canvas->setColorBack(ofxUIColor::red);
//    terrain.diamondSquareIteration();
    
    
    ofAddListener(canvas->newGUIEvent,this,&ofApp::guiEvent);
    
    
    if (!terrainShader.load("shader")) {
        std::cerr << "Shaders not loaded\n";
    }

    

}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    if (e.widget == slider) {
        if (slider->getValue() > iteration) {
            iteration = slider->getValue();
            terrain.diamondSquareIterationByIdx();
        }
        else {
            slider->setValue(iteration);
        }
    }
    else if (e.widget == resetBtn) {
        terrain.reset();
        iteration = 0;
        slider->setValue(0);
    }
    else if (e.widget == saveBtn) {
        terrain.save();
    }
    else if (e.widget == loadBtn) {
        terrain.load();
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
//    glEnable( GL_CULL_FACE );
//    glFrontFace(GL_CCW);
	//translate so that 0,0 is the center of the screen
    if (wireframeEnabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 40);  
	//Extract the rotation from the current rotation
    ofVec3f axis;  
    float angle;  
    curRot.getRotate(angle, axis);  
	
	//apply the quaternion's rotation to the viewport and draw the sphere
	ofRotate(angle, axis.x, axis.y, axis.z);  
//	ofDrawSphere(0, 0, 0, 200);
//    mesh.draw(ofPolyRenderMode.)
    
    terrainShader.begin();
    terrainShader.setUniform1f("maxHeight", terrain.maxHeight);
    terrainShader.setUniform1f("minHeight", terrain.minHeight);
    terrain.draw();
	terrainShader.end();
	ofPopMatrix();  
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ')
    terrain.diamondSquareIterationByIdx();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (canvas->isHit(x, y)) {
        return;
    }
	//every time the mouse is dragged, track the change
	//accumulate the changes inside of curRot through multiplication
    ofVec2f mouse(x,y);  
    ofQuaternion yRot((x-lastMouse.x)*dampen, ofVec3f(0,1,0));  
    ofQuaternion xRot((y-lastMouse.y)*dampen, ofVec3f(-1,0,0));  
    curRot *= yRot*xRot;  
    lastMouse = mouse;  
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //store the last mouse point when it's first pressed to prevent popping
	lastMouse = ofVec2f(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
