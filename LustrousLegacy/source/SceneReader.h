#ifndef SCENE_READER_H
#define SCENE_READER_H

#include <queue>
#include "SceneUtility.h"

class SceneReaderEmptyException {};

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
    // Predefine: fileName is the file in which the desired scene is stored.
    // sceneID is the ID for the scene in which you wish to create.
    SceneReader(const FileName& fileName, const SceneID& sceneID);
    SceneReader& operator=(const SceneReader& copy);

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

    // Returns current SpeechBubble and Cycles to the next SpeechBubble.
    SpeechBubble nextMessage();    
};

#endif
