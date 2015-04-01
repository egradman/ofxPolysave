#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  polysave.setup();

  image.loadImage("pictureframe.jpg");

  textblock.init("mono.ttf", 30);
  textblock.setText("this is a test of your community broadcasting system");
}

//--------------------------------------------------------------
void ofApp::update(){
  ofSetFrameRate(30);
  ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(0);

  image.draw(polysave.getRectangleByName("image"));

  ofRectangle r = polysave.getRectangleByName("textblock");
  textblock.wrapTextX(r.width);

  textblock.drawCenter(r.getCenter().x, r.getTop());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
