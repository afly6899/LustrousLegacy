#include "SceneUtility.h"

// Predefine: fileName is the name of the file containing the script.
// sceneID is the ID for the desired scene.
// lineNumber is the desired line number for a given line (first line is 0).
// Returns a SpeechBubble built on the information of the line specified by a given scene and line
// number.
SpeechBubble getBubble(const FileName& fileName, const SceneID& sceneID, const int& lineNumber)
{
    std::string line;
    std::ifstream file(fileName);
    SpeechBubble returnBubble;

    while (getline(file, line))
    {
	if (line == sceneID)
	{
	    getline(file, line);
	    break;	    
	}
    }

    for (int currentLineNum = 0; line.find(":") != -1; ++currentLineNum, getline(file, line))
    {
	if (lineNumber == currentLineNum)
	{
	    returnBubble.first = line.substr(0, line.find(":"));
	    returnBubble.second = line.substr(line.find(":")+1, line.size()-line.find(":"));
	}
    }

    return returnBubble;
}
