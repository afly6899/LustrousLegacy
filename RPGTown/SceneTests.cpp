#include <gtest/gtest.h>
#include "SceneReader.h"

TEST(SceneUtility, GetBubble)
{
    FileName fileName("SceneTest.txt");
    SceneID scene("Scene1");
    EXPECT_EQ(SpeechBubble("Bob McCabe", "Wassup!"), getBubble(fileName, scene, 0));
    EXPECT_EQ(SpeechBubble("Bob McCabe", "Hey! What's wrong man?"), getBubble(fileName, scene, 1));
    EXPECT_EQ(SpeechBubble("Joe McGee", "My google tests are failing."),
	      getBubble(fileName, scene, 2));
    EXPECT_EQ(SpeechBubble("Bob McCabe", "That sucks!"), getBubble(fileName, scene, 3));
    EXPECT_EQ(SpeechBubble(SceneID(), Message()), getBubble(fileName, scene, 4));
}

TEST(SceneReader, Default)
{
    SceneReader sceneReader("SceneTest.txt", "Scene1");
    EXPECT_EQ(SpeechBubble("Bob McCabe", "Wassup!"), sceneReader.currentMessage());
    EXPECT_EQ(SpeechBubble("Bob McCabe", "Wassup!"), sceneReader.nextMessage());
    EXPECT_EQ(SpeechBubble("Bob McCabe", "Hey! What's wrong man?"), sceneReader.currentMessage());
    EXPECT_EQ(SpeechBubble("Bob McCabe", "Hey! What's wrong man?"), sceneReader.nextMessage());
    EXPECT_EQ(SpeechBubble("Joe McGee", "My google tests are failing."),
	      sceneReader.currentMessage());
    EXPECT_EQ(SpeechBubble("Joe McGee", "My google tests are failing."),
	      sceneReader.nextMessage());
    EXPECT_EQ(SpeechBubble("Bob McCabe", "That sucks!"), sceneReader.currentMessage());
    EXPECT_EQ(SpeechBubble("Bob McCabe", "That sucks!"), sceneReader.nextMessage());
    EXPECT_THROW(sceneReader.currentMessage(), SceneReaderEmptyException);
    EXPECT_THROW(sceneReader.nextMessage(), SceneReaderEmptyException);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
