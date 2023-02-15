#include "SequencerTestPanel.h"
#pragma once
#include "SequencerTestPanel.h"

namespace DuckEngine
{
    SequencerTestPanel::SequencerTestPanel()
    {
        mySequence.mFrameMin = -100;
        mySequence.mFrameMax = 1000;
        mySequence.myItems.push_back(MySequence::MySequenceItem{ 0, 10, 30, false });
        mySequence.myItems.push_back(MySequence::MySequenceItem{ 1, 20, 30, true });
        mySequence.myItems.push_back(MySequence::MySequenceItem{ 3, 12, 60, false });
        mySequence.myItems.push_back(MySequence::MySequenceItem{ 2, 61, 90, false });
        mySequence.myItems.push_back(MySequence::MySequenceItem{ 4, 90, 99, false });
    }

    void SequencerTestPanel::OnImGuiRender()
    {
        ImGui::Begin("Sequencer", NULL, 0);

        ImGui::PushItemWidth(130);
        ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
        ImGui::SameLine();
        ImGui::InputInt("Frame ", &currentFrame);
        ImGui::SameLine();
        ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
        ImGui::PopItemWidth();
        Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
        // add a UI to edit that particular item
        if (selectedEntry != -1)
        {
            const MySequence::MySequenceItem& item = mySequence.myItems[selectedEntry];
            ImGui::Text("I am a %s, please edit me", SequencerItemTypeNames[item.mType]);
            // switch (type) ....
        }

        ImGui::End();
    }
}
