#include "ofxPolysave.h"
#include "ofxXmlSettings.h"

ofxPolysave::ofxPolysave() : corner("NONE") {
}

void ofxPolysave::setup() {
  active = false;
  ofAddListener(ofEvents().keyPressed, this, &ofxPolysave::keyPressed, OF_EVENT_ORDER_AFTER_APP );  
  ofAddListener(ofEvents().keyReleased, this, &ofxPolysave::keyReleased, OF_EVENT_ORDER_AFTER_APP );  
  ofAddListener(ofEvents().draw, this, &ofxPolysave::draw, OF_EVENT_ORDER_AFTER_APP);  
  ofAddListener(ofEvents().update, this, &ofxPolysave::update, OF_EVENT_ORDER_AFTER_APP);  
  ofAddListener(ofEvents().mousePressed, this, &ofxPolysave::mousePressed, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseReleased, this, &ofxPolysave::mouseReleased, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseDragged, this, &ofxPolysave::mouseDragged, OF_EVENT_ORDER_AFTER_APP);  

  load();

  move_mode = "";
}

void ofxPolysave::update(ofEventArgs &eventArgs) {
  if (cur_idx != -1) {
    cur_shape = shapes[cur_idx];
  } else {
    cur_shape = NULL;
  }
}

void ofxPolysave::draw(ofEventArgs &eventArgs) {
  if (cur_idx == -1) show_help = true;
  if (active && show_help) {
    stringstream ss;
    ss<< "ofxPolysave active" << endl
      << "------------------------" << endl
      << "arrow u/d  prev/next poly  " << endl
      << "   :e      new ellipse" << endl
      << "   :r      new rectangle" << endl
      << "   :p      new polyline" << endl
      << "   :c      toggle closed" << endl
      << "   :w      write file" << endl
      << "   :d      delete poly" << endl
      << "   ?       toggle help" << endl
      << "   |       toggle editor" << endl
      << endl
      << "registered names [a-z0-9_]" << endl
      << "--------------------------" << endl;

    for (int i=0; i<shapes.size(); i++) {
      NamedShape *s = shapes[i];
      if (cur_idx == i)
        ss << "[x] " << s->name << endl;
      else
        ss << "[ ] " << s->name << endl;
    }

    ofSetColor(255);
    ofDrawBitmapStringHighlight(ss.str(), 0, 12, ofColor(0, 0, 0, 128), ofColor(255));
  }

  if (active && cur_shape) {
    ofPushStyle();
    cur_shape->draw();
    ofPopStyle();

    ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
  }
}

void ofxPolysave::mousePressed(ofMouseEventArgs &eventArgs) {
  if (!active || !cur_shape) return;
  
  move_start.set(eventArgs.x, eventArgs.y);
  move_points_start = cur_shape->points;

  move_mode = "";

  ofPolyline poly = cur_shape->get_polyline();

  unsigned int closest_idx;
  ofPoint closest_point = poly.getClosestPoint(move_start, &closest_idx);
  if (closest_idx == 0) {
    // if its zero, definitely insert AFTER 0
    closest_idx = 1;
  } else if (closest_idx == cur_shape->points.size()-1) {
    // if it's the last one, insert BEFORE the last one
    closest_idx = closest_idx;
  } else {
    ofVec2f a(cur_shape->points[closest_idx-1]);
    ofVec2f b(cur_shape->points[closest_idx  ]);
    ofVec2f c(cur_shape->points[closest_idx+1]);
    ofVec2f P(closest_point);

    // there's a bug here.  I wish getClosestPoint would yield a float!
    if ((b-a).angle(P-b) < (c-b).angle(P-b)) {
      closest_idx = closest_idx;
    } else {
      closest_idx = closest_idx+1;
    }
  }

  if (poly.getBoundingBox().inside(move_start)) {
    move_mode = "move_all";
  }

  for (int i=0; i<cur_shape->points.size(); i++) {
    if (move_start.distance(cur_shape->points[i]) < 10) {
      move_mode = "move_one";
      move_idx = i;
    }
  }

  if (shift_pressed) {
    if (move_mode == "move_one") {
      cur_shape->points.erase(cur_shape->points.begin() + move_idx);
    } else if (move_mode == "move_all") {
      cur_shape->points.insert(cur_shape->points.begin()+closest_idx, closest_point);
    }
    move_mode == "";
  }
}

void ofxPolysave::mouseReleased(ofMouseEventArgs &eventArgs) {
  move_mode = "";
}

void ofxPolysave::advanceShape(int direction) {
  cur_idx = ofWrap(cur_idx + direction, 0, shapes.size());
}

void ofxPolysave::mouseDragged(ofMouseEventArgs &eventArgs) {
  if (!active || !cur_shape) return;
  ofPoint move_pos(eventArgs.x, eventArgs.y);
  if (move_mode == "move_all") {
    for (int i=0; i<cur_shape->points.size(); i++) {
      cur_shape->points[i] = move_points_start[i] + (move_pos - move_start);
    }
  } else if (move_mode == "move_one") {
    cur_shape->points[move_idx] = move_points_start[move_idx] + (move_pos - move_start);
  }
}

void ofxPolysave::keyPressed(ofKeyEventArgs &eventArgs) {
  int key = eventArgs.key;
  if (key == '|') {
    active = !active;
    if (active)
      load();
    else
      save();
  }
  if (!active) return;

  switch (key) {
    case ':':
      command_mode = !command_mode;
      break;
    case 'd':
      break;
    case OF_KEY_SHIFT:
      shift_pressed = true;
      ofLog() << "shift pressed";
      break;
    case OF_KEY_BACKSPACE:
      if (cur_shape)
        cur_shape->name = cur_shape->name.substr(0, cur_shape->name.size()-1);
      break;
    case OF_KEY_DOWN:
      if (cur_idx != -1) advanceShape(1);
      break;
    case OF_KEY_UP:
      if (cur_idx != -1) advanceShape(-1);
      break;
    case '?':
      show_help = !show_help;

  }
  if (command_mode) {
    switch(key) {
      case 'e':
        createShape(new NamedEllipse());
        command_mode = false;
        break;
      case 'r':
        createShape(new NamedRectangle());
        command_mode = false;
        break;
      case 'p':
        createShape(new NamedPolyline());
        command_mode = false;
        break;
      case 'd':
        delete_cur_shape();
        command_mode = false;
        break;
      case 'w':
        save();
        command_mode = false;
        break;
      case 'c':
        if (cur_shape) {
          cur_shape->is_closed = !cur_shape->is_closed;
        }
        save();
        command_mode = false;
        break;
    }
  } else if (('a' <= key && key <= 'z') || ('0' <= key && key <= '9') || key == '_') {
    cur_shape->name += string(1, key);
  }
}

void ofxPolysave::keyReleased(ofKeyEventArgs &eventArgs) {
  int key = eventArgs.key;
  switch (key) {
    case OF_KEY_SHIFT:
      shift_pressed = false;
      ofLog() << "shift released";
      break;
  }
}

void ofxPolysave::createShape(NamedShape *shape) {
  shape->name = "new_shape";
  shape->points.push_back(ofPoint(20, 20));
  shape->points.push_back(ofPoint(200, 200));
  shapes.push_back(shape);
  cur_idx = shapes.size()-1;
}

void ofxPolysave::delete_cur_shape() {
  shapes.erase(shapes.begin() + cur_idx);
  if (shapes.size() == 0) {
    cur_idx = -1;
  } else {
    advanceShape(-1);
  }
}

void ofxPolysave::load() {
  ofxXmlSettings xml;
  xml.loadFile("polysave.xml");
  shapes.clear();
  for (int i=0; i<xml.getNumTags("shape"); i++) {
    NamedShape *s;
    string type = xml.getAttribute("shape", "type", "polyline", i);
    if (type == "polyline") {
      s = new NamedPolyline();
    } else if (type == "rectangle") {
      s = new NamedRectangle();
    } else if (type == "ellipse") {
      s = new NamedEllipse();
    }

    ofLog() << " loaded a " << type;

    s->name = xml.getAttribute("shape", "name", "!!!", i);
    s->is_closed = xml.getAttribute("shape", "closed", false, i);
    xml.pushTag("shape", i);
    for (int j=0; j<xml.getNumTags("point"); j++) {
      xml.pushTag("point", j);
      s->points.push_back(ofPoint(xml.getValue("x", 0), xml.getValue("y", 0)));
      xml.popTag();
    }
    shapes.push_back(s);
    xml.popTag();
  }

  if (shapes.size())
    cur_idx = 0;
  else
    cur_idx = -1;
}

void ofxPolysave::save() {
  ofxXmlSettings xml;
  for (int i=0; i<xml.getNumTags("poly"); i++) {
    xml.removeTag("poly", i);
  }
  for (int i=0; i<shapes.size(); i++) {
    NamedShape *s = shapes[i];
    xml.addTag("shape");
    xml.setAttribute("shape", "name", s->name, i);
    xml.setAttribute("shape", "type", s->get_type(), i);
    xml.setAttribute("shape", "closed", s->is_closed, i);
    xml.pushTag("shape", i);
    stringstream ss;
    for (int j=0; j<s->points.size(); j++) {
      xml.addTag("point");
      xml.pushTag("point", j);
      xml.setValue("x", s->points[j].x);
      xml.setValue("y", s->points[j].y);
      xml.popTag();
    }
    xml.popTag();
  }
  xml.saveFile("polysave.xml");
}

ofRectangle ofxPolysave::getRectangleByName(string name) {
  // replace this with a map!
  for (vector<NamedShape *>::iterator s=shapes.begin(); s!=shapes.end(); s++) {
    if (name == (*s)->name)
      return ofRectangle((*s)->points[0], (*s)->points[1]);
  }
  ofLog() << "rectangle " << name << " not found";
  return ofRectangle();
}

ofPolyline ofxPolysave::getPolylineByName(string name) {
  // replace this with a map!
  for (vector<NamedShape *>::iterator s=shapes.begin(); s!=shapes.end(); s++) {
    if (name == (*s)->name)
      return (*s)->get_polyline();
  }
  ofLog() << "polyline " << name << " not found";
  return ofPolyline();
}

void NamedPolyline::draw() {

  ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
  get_polyline().draw();

  ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
  ofNoFill();
  for (vector<ofPoint>::iterator p=points.begin(); p!=points.end(); p++) {
    ofCircle(*p, 6);
  }
}

void NamedRectangle::draw() {
  ofNoFill();

  ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
  ofRect(ofRectangle(points[0], points[1]));

  ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
  ofCircle(points[0], 6);
  ofCircle(points[1], 6);
}

void NamedEllipse::draw() {
  ofNoFill();

  ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
  ofEllipse(
    points[0],
    (points[1].x-points[0].x)*2, (points[1].y-points[0].y)*2
  );

  ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
  ofCircle(points[0], 6);
  ofCircle(points[1], 6);
}
