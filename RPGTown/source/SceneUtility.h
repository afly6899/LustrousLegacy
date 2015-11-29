#ifndef SCENE_UTILITY_H
#define SCENE_UTILITY_H

#include <fstream>
#include <string>
#include <utility>

// Typedefs just in-case I wish to swap some of these out for other data types.
typedef std::string FileName;
typedef std::string SceneID;
typedef std::string Message;
typedef std::string CharacterID;

// Contains the character ID and the message.
// The same info is expressed as a speech bubble in comics, hence the name.
typedef std::pair<CharacterID, Message> SpeechBubble;

// Predefine: fileName is the name of the file containing the script.
// sceneID is the ID for the desired scene.
// lineNumber is the desired line number for a given line (first line is 0).
// Returns a SpeechBubble built on the information of the line specified by a given scene and line
// number.
SpeechBubble getBubble(const FileName& fileName, const SceneID& sceneID, const int& lineNumber);

#endif
