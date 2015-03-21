#include "ofxPolysave.h"
#include "ofxXmlSettings.h"

ofxPolysave::ofxPolysave() : corner("NONE") {
}

void ofxPolysave::setup() {
  active = false;
  ofAddListener(ofEvents().keyPressed, this, &ofxPolysave::keyPressed, OF_EVENT_ORDER_AFTER_APP );  
  ofAddListener(ofEvents().draw, this, &ofxPolysave::draw, OF_EVENT_ORDER_AFTER_APP);  
  ofAddListener(ofEvents().mousePressed, this, &ofxPolysave::mousePressed, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseReleased, this, &ofxPolysave::mouseReleased, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseDragged, this, &ofxPolysave::mouseDragged, OF_EVENT_ORDER_AFTER_APP);  

  load();
}

void ofxPolysave::draw(ofEventArgs &eventArgs) {
  if (!active || cur_rect_idx == -1) return;
  NamedRectangle &r = rects[cur_rect_idx];
  ofPushStyle();
  ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
  ofNoFill();
  ofRect(r);
  stringstream ss;
  ss << r.name << " x" << r.x << " y" << r.y << " w"<<r.width << " h"<<r.height;
  ofDrawBitmapStringHighlight(ss.str(), r.getLeft() + 10, r.getTop() + 20);
  ofPopStyle();
}

void ofxPolysave::mousePressed(ofMouseEventArgs &eventArgs) {
  if (!active || cur_rect_idx == -1) return;
  ofVec2f p(eventArgs.x, eventArgs.y);
  NamedRectangle &r = rects[cur_rect_idx];
  if (p.distance(r.getTopLeft()) < 10) corner = "TL";
  else if (p.distance(r.getTopRight()) < 10) corner = "TR";
  else if (p.distance(r.getBottomRight()) < 10) corner = "BR";
  else if (p.distance(r.getBottomLeft()) < 10) corner = "BL";
  else if (r.inside(p)) corner = "CENTER";
  else corner = "NONE";
}

void ofxPolysave::mouseReleased(ofMouseEventArgs &eventArgs) {
  if (!active || cur_rect_idx == -1) return;
  corner = "NONE";
}

void ofxPolysave::mouseDragged(ofMouseEventArgs &eventArgs) {
  if (!active || cur_rect_idx == -1) return;
  if (corner == "NONE") return;
  NamedRectangle &r = rects[cur_rect_idx];
  ofVec2f p(eventArgs.x, eventArgs.y);

  ofPoint tl = r.getTopLeft();
  ofPoint br = r.getBottomRight();

  if (corner == "TL") {
      tl = p;
  } else if (corner == "TR") {
      br.x = p.x;
      tl.y = p.y;
  } else if (corner == "BR") {
      br = p;
  } else if (corner == "BL") {
      tl.x = p.x;
      br.y = p.y;
  } else if (corner == "CENTER") {
    ofRectangle r2;
    r2.setFromCenter(p, r.width, r.height);
    tl = r2.getTopLeft();
    br = r2.getBottomRight();
  }
  r.set(tl, br);
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
    case '+':
      // create a new rect
      new_rect();
      break;
    case '=':
      rects[cur_rect_idx].width = rects[cur_rect_idx].height;
      break;
    case OF_KEY_BACKSPACE:
      rects[cur_rect_idx].name = rects[cur_rect_idx].name.substr(0, rects[cur_rect_idx].name.size()-1);
      break;
    case OF_KEY_RETURN:
      break;
    case ']':
      if (cur_rect_idx != -1) 
        cur_rect_idx = (cur_rect_idx + 1) % rects.size();
      break;
  }
  if (('a' <= key && key <= 'z') || ('0' <= key && key <= '9') || key == '_') {
    rects[cur_rect_idx].name += string(1, key);
  }
}

void ofxPolysave::new_rect() {
  NamedRectangle r;
  r.name = "unnamed";
  r.set(10, 10, ofGetWidth()-10, ofGetHeight()-10);
  rects.push_back(r);
  cur_rect_idx = rects.size()-1;
}

void ofxPolysave::load() {
  ofxXmlSettings xml;
  xml.loadFile("polysave.xml");
  rects.clear();
  for (int i=0; i<xml.getNumTags("rect"); i++) {
    NamedRectangle r;
    r.name = xml.getAttribute("rect", "name", "!!!", i);
    xml.pushTag("rect", i);
    r.x = xml.getValue("x", 10);
    r.y = xml.getValue("y", 10);
    r.width = xml.getValue("width", ofGetWidth()-10);
    r.height = xml.getValue("height", ofGetHeight()-10);
    rects.push_back(r);
    xml.popTag();
  }

  if (rects.size())
    cur_rect_idx = 0;
  else
    cur_rect_idx = -1;
}

void ofxPolysave::save() {
  ofxXmlSettings xml;
  for (int i=0; i<xml.getNumTags("rect"); i++) {
    xml.removeTag("rect", i);
  }
  for (int i=0; i<rects.size(); i++) {
    NamedRectangle &r = rects[i];
    xml.addTag("rect");
    xml.setAttribute("rect", "name", r.name, i);
    xml.pushTag("rect", i);
    xml.setValue("x", r.x);
    xml.setValue("y", r.y);
    xml.setValue("width", r.width);
    xml.setValue("height", r.height);
    xml.popTag();
  }
  xml.saveFile("polysave.xml");
}

ofRectangle ofxPolysave::getRectangleByName(string name) {
  for (vector<NamedRectangle>::iterator r=rects.begin(); r!=rects.end(); r++) {
    if (name == r->name)
      return ofRectangle(*r);
  }
  return ofRectangle(30, 30, ofGetWidth()-60, ofGetHeight()-60);
}


