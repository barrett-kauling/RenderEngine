# RenderEngine

MMSEV Copyright NASA, OBJ written from NASA LaRC Advanced Concepts Lab (ACL)

TODO:   -- Fix GLUT_KEY_[DIRECTION] key view ZY plane translation
        -- Startup 0 key ResetView()
        -- Tokenize input parameters (GUI?)
              -- Directory
              -- Mesh
              -- Debug
                    -- Keyboard
                    -- Function
                    -- Key
              -- Shaders 
        -- Parse texture images
        -- Link texture images to meshes
        -- Add lighting to process normals
        -- Write Vertex/Fragment Shaders (re: special effects!)

See MMSEV-Blender for desired end state.
        
Unzip MMSEV-C into resource/ before running! :D ::Barrett

[1.0.0]
--Init_Mesh() before InitializeView()
--Add RenderStatus* parameter
--Switch to (char*) and parse materials
... GLUT_KEY ZY plane seems to be okay, however you choose to orient your CSYS :P (lick!)
