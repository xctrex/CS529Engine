Breakfasteroids:

Gameplay - basic asteroids. The ship takes damage if it
collides with an asteroid. Asteroids take damage if they collide
with bullets. Objects cannot go off screen. Destroy all the asteroids
to win the level.

Controls:
Click the left mouse button to toggle the controls screen on and off
Click the right mouse button to toggle debug drawing
Space fires bullets
Ctrl sets off a bomb at mouse's location. Limit 2 bombs per level
Up arrow accelerates
Down arrow decelerates
Left and right arrows rotate


Rubric
Debug Drawing
    Basic debug drawing of circles can be seen by right clicking the mouse
        debug drawing was implemented with D2D/D3D interop, which requires
        SP1 and the "Platform Update for Windows 7" to be installed
        
Object Architecture
    Basic C++ Objects present
    +4 Component based design
    +2 Object and component handles for object management
        -The handle classes are in gameobject.h/cpp and component.h/cpp
        -While the game is playing, if an object is killed, that object
        is added to a list of objects to be cleaned up at the end of the 
        frame. This list is part of the CoreEngine class in Core.h
        
Communication
    Basic Events present in Event.h/cpp

Data Driven Design
    Basic Text Serialization present
    +2 Creation of objects from data files
        -The Assets\Level*.xml files have object descriptions
        Each object is a composition of components. The code for
        generating each component is in the Initialize function that is
        a part of each component. All of the initialization is done using
        in xml using the tinyxml2 xml parser. The GameObject class in GameObject.h/cpp
        has the code for aggregating components into an object via xml.
    +2 Archetypes can be found in the Assets\Archetypes.xml file. Part of each object/components
        initialization is to check if an archetype is specified in the xml. Individual attributes
        of the archetype can be overridden.
    +? More advanced serialization - I'm not sure what qualifies as more advanced, but my initialization
        is recursive, allowing me to create a hierarchy of archetypes. For examle, in Archetypes.xml
        there is a DefaultSpriteArchetype which has the default height and width. Each of the cereal sprite
        Archetypes derive from that so they don't have to individually specify the default height and width,
        except for the CocoPuffSpriteArchetype which overrides those values anyways. These lower level archetypes
        are in turn used as part of the asteroid game object archetypes.
    +2 Level files are in the Assets\ folder
Graphics
    Hardware accelerate sprite rendering present using D3D11 and the DirectXTK (the replacement for the now
        deprecated D3DX libraries)
    Basic Sprite transformation present. This can be seen in the draw function of the Sprite class
        in Sprite.h/.cpp
    +? Sprite Batching - I use the DirectXTK for sprite batching, so I don't know if that qualifies or if I
        would have to roll my own completely. The graphics system uses a SpriteBatch object in "DrawSprites" to
        iterate through the system's list of sprites queuing them up and sending them all at to the GPU at once.
    +? Other advanced graphics features - I am using D2D/D3D interop for debug drawing and text. This can be seen
        in the GraphicsSystem::DrawDebug function, where I use D2D to render bounding circles on top of my sprites.
        I also use DWrite for text, which can be seen in the Text class, which works similar to the Sprite class.
        DWrite allows me to specify system fonts by name in the XML in order to use data to choose which font to use
        (as long as it is a system font that is installed, otherwise it falls back on a default font).
        The one caveat to the D2D/D3D interop is that it requires d2d1_1.h, which is only available on Win7 if it has
        SP1 and the "Platform Update for Windows 7" installed. Originally I had planned to check and make sure this worked
        on the lab machines over the weekend, but didn't realize it would be closed so I'm not sure if this will work, and
        since the project is required to run on the lab machines I'm not sure if this gets credit.
Physics
    Basic collision detection between circles present. This is controlled by the RigidBody component and the collision equasions
        are from a modified version of the 2D math library we wrote earlier in the semester. Collisions are easily seen in the
        game between the ship, asteroids, edges of the window, and bullets. (The ship does not collide with bullets, by design).
    Basic Collision responses present - the ship and asteroids bounce off each other and the wall..
    +? Collisions between different body types - I have Circle to static line collision, which can be seen between the objects and
        the edge of the screen, but I'm not expecting that to be enough to
        qualify.
Game Requirements
    Human player control of the ship present
    Game runs at 30fps
    
    
    Collision between projectiles, player and enemies present
    Two different types of enemies, regular asteroids and asteroids that shrink and get faster as their health goes down.
    Two weapons - regular bullets shot by space bar and bombs placed at mouses location by hitting Ctrl
    Controls screen - left click to toggle controls screen
    Requirements easy to test - 
    Game has a win/lose condition - destroy all the asteroids to move on to the next level. Run into the asteroids too many times and
        the ship will be destroyed.
    Game is in C++
    Game is fun/interesting/surprising? It's breakfast...and asteroids...BreakfAsteroids!
    
    Game runs on Lab machines - as mentioned above and in my e-mail, it should, everything runs on Win7, but I haven't tested it out recently since I was planning
    to do that this weekend. If it does not run for whatever reason, please e-mail me at t.walton@digipen.edu or tommywalton@live.com, and I will
    take care of it. I plan to verify that it works when DigiPen opens tomorrow morning.
    
Tommy
