#include "SceneUtility.h"

// Predefine: fileName is the name of the file containing the script.
// sceneID is the ID for the desired scene.
// lineNumber is the desired line number for a given line.
// Returns a SpeechBubble built on the information of the line specified by a given scene and line
// number.
SpeechBubble getBubble(const FileName& fileName, const SceneID& sceneID, const int& lineNumber)
{
    std::string line;
    std::ifstream file(fileName);
    SpeechBubble returnBubble;

    while (getline(file, line))
    {
	if (line != (sceneID + "\n"))
	{
	    returnBubble.first = sceneID;

	    // Finish
	}
    }
}
