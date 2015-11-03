#ifndef SCENE_READER_H
#define SCENE_READER_H

#include <queue>
#include "SceneUtility.h"

// Allows messages in a scene to be accessed in order.
typedef std::queue<SpeechBubble> SceneQueue;

class SceneReader
{
private:
    //
    // Class variables
    //

    SceneQueue sceneLines; // Stores scene lines

public:
    //
    // Constructors
    //
    
    // Creates and enqueues all SpeechBubbles for a scene
    // Predefine: sceneID is the ID for the scene in which you wish to create.
    SceneReader(const SceneID& sceneID);

    //
    // Queries
    //
    
    // Returns the size of the scene.
    int size();

    // Returns if the SceneReader is empty.
    bool isEmpty();

    // Returns current SpeechBubble.
    SpeechBubble currentMessage();

    //
    // Commands
    //

    // Cycles to the next SpeechBubble and returns it.
    SpeechBubble nextMessage();    
};

#endif
