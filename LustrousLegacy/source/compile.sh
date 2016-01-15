#!bin/bash
g++-4.9 -g -std=c++14 ../*.cpp TMXloader/*.cpp TMXloader/pugixml/pugixml.cpp ../ltbl/*.cpp ../ltbl/lighting/*.cpp ../ltbl/quadtree/*.cpp fader.cpp main.cpp NPC.cpp pause.cpp SceneReader.cpp SceneUtility.cpp Textbox.cpp title.cpp TutorialEvent.cpp -o lustrous-legacy -lsfml-graphics -lsfml-window -lsfml-system -I.. -lz -lsfml-audio
