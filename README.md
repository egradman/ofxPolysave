## Summary

ofxPolysave lets you designate named rectangles of interest in your app at runtime.  Simply create a polysave object in your app and call `setup()`.  With a keystroke, you enter polysave mode.  From there, you can add a rectangle, drag its corners, and give it a name.  Rectangles are automatically saved to `polysave.xml`.

The polysave object has a `getRectangleByName(string name)` method.


