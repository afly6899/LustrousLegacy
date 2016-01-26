#include "SceneReader.h"

//
// Constructors
//

// Creates and enqueues all SpeechBubbles for a scene
// Predefine: fileName is the file in which the desired scene is stored.
// sceneID is the ID for the scene in which you wish to create.
SceneReader::SceneReader(const FileName& fileName, const SceneID& sceneID)
{
    for (int i = 0; ; ++i)
    {
	SpeechBubble toQueue = getBubble(fileName, sceneID, i);
	if (toQueue.first != SceneID() || toQueue.second != Message())
	{
	    sceneLines.push(toQueue);
	}

	else
	{
	    break;
	}
    }
}

//
// assignment
//

SceneReader& SceneReader::operator=(const SceneReader& copy) {
	sceneLines = copy.sceneLines;
	return *this;
}

//
// Queries
//

// Returns the size of the scene.
int SceneReader::size()
{
    return sceneLines.size();
}

// Returns if the SceneReader is empty.
bool SceneReader::isEmpty()
{
    return sceneLines.size() == 0;
}

// Returns current SpeechBubble.
SpeechBubble SceneReader::currentMessage()
{
    if (sceneLines.size() > 0)
    {
	return sceneLines.front();
    }

    else
    {
	throw(SceneReaderEmptyException());
    }
}

//
// Commands
//

// Returns current SpeechBubble and Cycles to the next SpeechBubble.
SpeechBubble SceneReader::nextMessage()
{
    if (size() > 0)
    {
	SpeechBubble toReturn(sceneLines.front());
	sceneLines.pop();
	return toReturn;
    }

    else
    {
	throw(SceneReaderEmptyException());
    }
}
