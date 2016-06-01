#pragma once
#ifndef SPEECHSTEP_H_
#define SPEECHSTEP_H_

#include <SFML/Graphics.hpp>
#include <map>
#include "Textbox.h"
#include "Step.h"

// Each Speech Step is for ONE SCENE - just pass in the textbox?
class SpeechStep : public Step {
public:

	SpeechStep(std::string* message, bool textbox);
	virtual ~SpeechStep();
	virtual bool run(float elapsedTime, Actor& actor);

	// for debugging
	virtual std::string getType() { return type; }

private:
	std::string* dialogue;
	bool textboxOn;
	bool textFinished = false;
	std::string scriptname;

	// for debugging
	std::string type = "Speech";
};
#endif