#pragma once

#include "ofMain.h"

class NamedShape {
  public:
    string name;
    vector<ofPoint> points;

    bool is_closed = false;

    virtual void draw() = 0;
    virtual string get_type() = 0;

    ofPolyline get_polyline() {
      ofPolyline poly;
      for (vector<ofPoint>::iterator p=points.begin(); p!=points.end(); p++) {
        poly.addVertex(*p);
      }
      poly.setClosed(is_closed);
      return poly;
    }
};

class NamedPolyline : public NamedShape {
  public:
    void draw();
    string get_type() { return "polyline"; }
};

class NamedRectangle : public NamedPolyline {
  public:
    void draw();
    string get_type() { return "rectangle"; }
};

class NamedEllipse : public NamedPolyline {
  public:
    void draw();
    string get_type() { return "ellipse"; }
};



class ofxPolysave {
  public:
    ofxPolysave();
    void setup();
    void draw(ofEventArgs &eventArgs);
    void update(ofEventArgs &eventArgs);
    void mousePressed(ofMouseEventArgs &eventArgs);
    void mouseReleased(ofMouseEventArgs &eventArgs);
    void mouseDragged(ofMouseEventArgs &eventArgs);
    void keyPressed(ofKeyEventArgs &eventArgs);
    void keyReleased(ofKeyEventArgs &eventArgs);
    void load();
    void save();
    ofRectangle getRectangleByName(string name);
    ofPolyline  getPolylineByName(string name);
    void advanceShape(int direction);
    void createShape(NamedShape *shape);

    vector<string> getNames();

    string corner; // which corner is being moved

  private:
    void new_rect();
    void new_polyline();
    void delete_cur_shape();

    int cur_idx;
    bool active;
    bool show_help = true;

    vector<NamedShape *> shapes;
    NamedShape *cur_shape;

    string move_mode;
    int move_idx;
    ofPoint move_start;
    vector<ofPoint> move_points_start; // where moving points started

    bool shift_pressed = false;
    bool command_mode = false;



};
