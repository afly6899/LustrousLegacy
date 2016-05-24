#include <iostream>
#include "SpeechStep.h"

SpeechStep::SpeechStep(std::string* message, bool textbox) 
: dialogue(message), textboxOn(textbox) {
}

SpeechStep::~SpeechStep() {
}

bool SpeechStep::run(float elapsedTime, Actor & actor) {
	if (!textboxOn && !textFinished) {
		dialogue[1] = actor.nextEventDialogue();
		textboxOn = true;
		textFinished = true;
	}
	else if (textFinished) {
		textboxOn = false;
	}
	return textboxOn;
}


