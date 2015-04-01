## The symptom

_"Can you move that a few pixels to the left?"_

_"Sure... I'll just exit, edit some constants in a header file, recompile, and restart the program."_

## The cure

ofxPolysave is an addon that embeds a tiny geometric primitive editor in your application.  With just two lines of code, you get a quick GUI for creating and editing polylines, rectangles, and ellipses.  You can give each object a unique name.  These objects are automatically saved and loaded in an XML file (polysave.xml).  The polysave object provides two functions: `getRectangleByName(string name)` and `getPolylineByName(string name)`.  

If you ever find yourself hard coding screen coordinates into your program, consider using ofxPolysave instead.  Use it to:

- Draw boxes where text should be rendered (combine with ofxTextSuite!)
- Draw boxes where images or FBOs should be rendered
- Draw quad-warping or calibration quadrilaterals
- Draw motion paths (combine with ofxTimeline!)

It's a GUI tool which is terribly useful during application development, because you can edit the geometry while your application is still running allowing you (and the person hovering over your shoulder) can see the outcome immediately.  ofxPolysave generates a datafile which is consumed elsewhere in the program, and in that regard it's inspired by ofxTimeline.

## Setup

1. Include "ofxPolysave.h" and create an ofxPolysave object in your ofApp:
    ```
    #include <ofxPolysave.h>
    
    class ofApp : public ofBaseApp{
      ...
      ofxPolysave polysave;
    }
    ```

2. Call `polysave.setup()` in the setup function of your application.  The polysave object will automatically hook into ofEvents.

3. When the polysave editor is active, it will listen for mouse and keyboard events.  If your application also responds to mouse and keyboard events, you may want to include the line `if (polysave.active) return;` at the beginning of your `keyPressed` method.

## Use

The ofxPolysave editor is toggled by the `|` key.  When active, it shows a list of keyboard commands, as well as the list of saved polys.  Create a new ellipse `:e`, rectangle `:r`, or polyline `:p`.  Unless you type the command character `:` you'll be editing the name of the currently selected poly.  `:w` will save the data file (it's saved automatically when the program exits normally).

- Rectangles have two handles: top left and bottom right.  You can move the entire rectangle by dragging inside it's bounding box.
- Ellipses have two handles: center and bottom right.  Move the entire ellipse by dragging in the region between the two handles.
- Polylines have two or more points.  Shift-click a handle to delete it.  Shift-click on the edges to add a handle.  You can open and close polylines with `:c`

To fetch a saved poly, call `polysave.getPolylineByName(string name)` or `polysave.getRectangleByName(string name)` (which return ofPolyline and ofRectangle objects respectively).  These methods are cheap, so do it in your draw routine.

## Notes

- I am not trying to turn OpenFrameworks into a WYSIWYG drawing tool!  Like ofxTimeline or ofxColorPicker, this addon is targeted at editing one very specific data type.
- There is no `getEllipseByName` function because there is no native ellipse datatype.  Get it as a rectangle and use `center`, `width`, and `height`.

## TODO

- There's a bug in the "insert handle at this point" code which occasionally yields zigzags.
- SHIFT key in rect or ellipse should constrain proportions
- Moving ellipses only works in the bounding box of the handles
