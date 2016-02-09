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

	SpeechStep(std::string fileName, Textbox* textbox);
	virtual ~SpeechStep();
	virtual bool run(float elapsedTime, Actor& actor);

private:
	std::string currentMessage;
	std::string fileName;
	Textbox* textbox;
};
#endif