#include "SceneReader.h"

// Creates and enqueues all SpeechBubbles for a scene
// Predefine: fileName is the file in which the desired scene is stored.
// sceneID is the ID for the scene in which you wish to create.
SceneReader(const FileName& fileName, const SceneID& sceneID)
{
    for (int i = 0; ; ++i)
    {
	SpeechBubble toQueue = getBubble(fileName, sceneID, i);
	if (toQueue.first == SceneID() || toQueue.second == Message())
	{
	    sceneLines.enqueue(toQueue);
	}

	else
	{
	    break;
	}
    }
}
