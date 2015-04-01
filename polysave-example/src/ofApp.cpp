#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  polysave.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
  ofSetFrameRate(30);
  ofSetVerticalSync(true);

}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(0);
  ofRectangle r = polysave.getRectangleByName("my_rect");
  ofLog() << r;
  ofPolyline p;
  for (int x=0; x<r.width; x++) {
    int y = r.height*ofNoise(ofGetElapsedTimef(), x/100.0);
    p.addVertex(x,y);
  }
  ofPushMatrix();
  ofSetColor(255);
  ofTranslate(r.getTopLeft());
  p.draw();
  ofPopMatrix();

  ofPolyline path = polysave.getPolylineByName("my_path");
  float t = ofWrap(ofMap(ofGetElapsedTimef(), 0, 4, 0, 1), 0, 1);
  ofPushMatrix();
  ofTranslate(path.getPointAtPercent(t));
  ofCircle(0, 0, 8);
  ofPopMatrix();

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
