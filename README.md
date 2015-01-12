Re-Quake-II
===========

Reimplementation of id Tech 2 and Quake II with modern rendering pipeline and game engine features.

The renderer interface is intended to accomodate a generic renderer implementation (essentially meaning either Direct3D or OpenGL). The current implementation does not run in Direct3D (and may require some changes to the interface in order to do so) but the interface itself is more similar to Direct3D, so the transition should be feasible.

Currently no server module, the client is just set up to run at a fixed time-step and render a single map (and a hardcoded model at the world origin), allowing the player to fly through it. Visibility clusters are currently implemented. Road map (roughly in order of implementation) is currently:

  1. Correct Lighting and Effects
  2. Properly Animating Textures
  3. Physics and Map Collision Tests
  
Screenshot: http://i.imgur.com/DKDF4Yl.jpg

Libraries used: SDL2, GLEW.
  
Thanks for checking it out!
