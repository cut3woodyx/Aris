How2Use:
==================================================
File > New | Open | Save | Save As | Exit
==================================================
New -> Clears Objects and Map 
  (Technically, just deactivates it, so the objects aren't really lost)

Open -> Opens 3 types of files - .lesav, .txt and .csv
  [Ctrl-O]
  .lesav opens the txt and csv file paths inside.
  .txt loads objects.
  .csv loads map.

Save -> Saves to current .lesav file.
  [Ctrl-S]
  Runs (Save as) if no filename registered.

Saves As -> Can save as .lesav, .txt or .csv
  Saving as .lesav saves both to current file location.
    Also saves Prefab details.
  Saving as .txt file saves only the objects.
  Saving as .csv file saves only the map.

Exit -> Exits the program.
  [Esc]

==================================================
Options
==================================================
Fullscreen/Windowed - Sets to Fullscreen/Windowed

==================================================
Play
==================================================
Play -> Plays the current setup. 
  Upon stopping, reloads the previous setup.


==================================================
Object Inspector
==================================================
You can left click to select an Object.
--------------------------------------------------
Add Object, Delete Object
--------------------------------------------------
Add Object -> Adds a new object to the pool.

Delete Object -> Deletes the selected Object.

--------------------------------------------------
Add Component, Delete Component
--------------------------------------------------
Add Component -> Adds a new Component to the object.

Delete Component -> Deletes the selected Component.

--------------------------------------------------
Change Tag, Add Tag, Delete Tag
--------------------------------------------------
Change Tag -> Changes tag of current Object

Add Tag -> Adds a new tag to the current file.

Delete Tag -> Removes a tag from the current file.

--------------------------------------------------
Sorting
--------------------------------------------------
Sort
  Sorts in alphabetical order.

Search
  Searches for objects with the given string. Case-sensitive.

Tag
  Search via Tag instead if enabled.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

You can left click to open up the component and
edit it.
--------------------------------------------------
Add Component, Delete Component
--------------------------------------------------
Add Component -> Adds a new Component.
Delete Component -> Deletes a Component.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Object must be selected in order to change its
transform.

Right click to change modes.

--------------------------------------------------
Translate, Scale, Scale Collision, Rotate
--------------------------------------------------
Translate -> Click and Drag the object to move it around.

Scale -> Click and Drag the object to scale it.

Scale Collision -> If there is collision, Scales the collision box.

Rotate -> Click and Drag the Circle above the object to rotate it.

==================================================
Map Inspector
==================================================
Click on a tile to change its texture.
--------------------------------------------------
Brush
--------------------------------------------------
Sets Brush to a texture. On clicking a tile, changes its texture to 
  the currently selected one.

Sets if tile is to be a Collidable Tile or not.
--------------------------------------------------
New Map
--------------------------------------------------
Set a new width and height for the map.
  Can choose to clear map, or crop/expand current map.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

--------------------------------------------------
Add Tile Texture, Delete Tile Texture
--------------------------------------------------
Add Tile Texture -> Adds a texture file.

Delete Tile Texture -> Removes a currently loaded Texture.

==================================================
Prefabs
==================================================
You can drag prefabs onto the game world to create a single new instance.

You can drag an object from the Object List to the prefabs to create one.

==================================================
Performance Viewer
==================================================
Shows data used by the program.

Memory Usage 
  Amount of VRAM used by the program.
  
CPU Usage
  CPU percentage used by the program.
  Also shows percentage used by each individual system.
  

==================================================
Miscellaneous
==================================================
Ctrl-Left Mouse Button | Middle Mouse Button
  Move the camera.
  
Right Mouse Button
  Change mode to modify.
  Position -> Scale -> Scale Collision -> Rotation
  Clicking on the object or certain parts of the object will allow
    modification to the currently selected mode.
  
Mouse-Wheel
  Zooms in/out the camera.
  
Ctrl-V
  Copies currently selected object.
 
Ctrl-F
  Focuses Camera on currently selected object.

Delete
  Deletes currently selected object.
  
Ctrl-Z
  Undo button.
  Only undoes singular changes, and only Position, Scale and Rotation changes

Tilde (~)
  Hides the Editor.
  
Plus (+), End
  Hides all UI objects.

Clicking on Objects directly to select
  You can directly select objects by clicking on it. This ignores UI elements.
  
Multiple objects can be selected by holding down shift.
  Limited modifications allowed.
  
Window Docking
  Move windows around to dock to certain locations in the screen.





