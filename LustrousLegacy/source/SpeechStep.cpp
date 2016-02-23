#include "SpeechStep.h"

SpeechStep::SpeechStep(std::string* messages, bool& textbox) 
: dialogue(messages), textboxOn(textbox) {
}

SpeechStep::~SpeechStep() {
}

bool SpeechStep::run(float elapsedTime, Actor & actor) {
	if (!textboxOn && !textFinished) {
		dialogue[1] = actor.getScene();
		textboxOn = true;
		textFinished = true;
	}
	return textboxOn;
}


