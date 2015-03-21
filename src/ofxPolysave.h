#pragma once

#include "ofMain.h"

class NamedRectangle : public ofRectangle {
public:
  string name;
};

class ofxPolysave {
  public:
    ofxPolysave();
    void setup();
    void draw(ofEventArgs &eventArgs);
    void mousePressed(ofMouseEventArgs &eventArgs);
    void mouseReleased(ofMouseEventArgs &eventArgs);
    void mouseDragged(ofMouseEventArgs &eventArgs);
    void keyPressed(ofKeyEventArgs &eventArgs);
    void load();
    void save();
    ofRectangle getRectangleByName(string name);

    string corner; // which corner is being moved

  private:
    void new_rect();

    int cur_rect_idx;
    bool active;

    vector<NamedRectangle> rects;
};
