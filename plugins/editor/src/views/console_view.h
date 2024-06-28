#pragma once

#include "pch.h"
#include "Boundless/Core.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>

using namespace Boundless;

class Console : public Boundless::I::ILayer
{
public:
    Console() : autoScroll{ true } {}
    ~Console() {}

    virtual BReturn OnAttach() override {
        // Redirect stdout and stderr to our stringstream
        std::cout.rdbuf(outputStream.rdbuf());
        std::cerr.rdbuf(outputStream.rdbuf());
        return SUCCESS;
    }

    virtual BReturn OnDetach() override {
        // Reset the stream buffers to default
        std::cout.rdbuf(originalCoutBuffer);
        std::cerr.rdbuf(originalCerrBuffer);
        return SUCCESS;
    }
    virtual void OnEvent(I::IEvent* event) override {

    }
    virtual void OnGUI() override {
        ImGui::Begin("Console");

        // Calculate available space for positioning
        ImVec2 consoleSize = ImGui::GetContentRegionAvail();

        // Position the "Clear" button to the top right
        ImGui::Text("");
        ImGui::SameLine(consoleSize.x - 60.f);  // Adjust based on total width of both controls
        // Button to clear the console
        if (ImGui::Button("Clear")) {
            std::lock_guard<std::mutex> lock(outputMutex);
            logBuffer.clear();
        }

        // Align the "Auto-scroll" checkbox to the right of the "Clear" button
        ImGui::SameLine(consoleSize.x - 220.0f);  // Adjust based on total width of both controls
        ImGui::Checkbox("Auto-scroll", &autoScroll);

        // Capture the current output
        std::string newOutput = outputStream.str();
        std::vector<std::string> newLines;
        if (!newOutput.empty()) {
            SplitLines(newOutput, newLines);
        }

        // Update the log buffer with new lines (within mutex lock)
        if (!newLines.empty()) {
            std::lock_guard<std::mutex> lock(outputMutex);
            logBuffer.insert(logBuffer.end(), newLines.begin(), newLines.end());
            outputStream.str(""); // Clear the output stream
        }

        // Display the console header with styling
        ImGui::Separator();

        // Display the console output in a scrolling region
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 10), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);

        // Get the draw list to draw custom backgrounds
        auto* drawList = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetCursorScreenPos();

        // Alternate background colors and set text colors
        bool alternate = false;
        float yPos = windowPos.y;

        // Increase text size for better readability
        ImGui::SetWindowFontScale(1.25f);

        for (const auto& line : logBuffer) {
            ImVec2 textSize = ImGui::CalcTextSize(line.c_str());

            // Draw background rectangle
            drawList->AddRectFilled(ImVec2(windowPos.x, yPos), ImVec2(windowPos.x + ImGui::GetContentRegionAvail().x, yPos + textSize.y),
                alternate ? IM_COL32(41, 41, 41, 255) : IM_COL32(81, 81, 81, 255));

            // Draw the text
            drawList->AddText(ImVec2(windowPos.x, yPos), ImGui::GetColorU32(ImVec4(0.9f, 0.9f, 0.9f, 1.0f)), line.c_str());
            yPos += textSize.y;
            alternate = !alternate;
        }

        // Auto-scroll to the bottom if new lines are added
        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();

        ImGui::End();
    }

private:
    void SplitLines(const std::string& str, std::vector<std::string>& outLines) {
        std::istringstream stream(str);
        std::string line;
        while (std::getline(stream, line)) {
            outLines.push_back(line);
        }
    }

    std::ostringstream outputStream;
    std::streambuf* originalCoutBuffer = std::cout.rdbuf();
    std::streambuf* originalCerrBuffer = std::cerr.rdbuf();
    std::vector<std::string> logBuffer;
    std::mutex outputMutex;
    bool autoScroll = true;
};
