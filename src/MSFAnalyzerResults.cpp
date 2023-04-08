#include "MSFAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "MSFAnalyzer.h"
#include "MSFAnalyzerSettings.h"
#include <iostream>
#include <fstream>


MSFAnalyzerResults::MSFAnalyzerResults(MSFAnalyzer* analyzer, MSFAnalyzerSettings* settings)
    : AnalyzerResults(), mSettings{settings}, mAnalyzer{analyzer}
{
}


MSFAnalyzerResults::~MSFAnalyzerResults()
{
}


void MSFAnalyzerResults::GenerateBubbleText(U64 frameIndex, Channel& channel, DisplayBase displayBase)
{
    Frame frame = GetFrame(frameIndex);

    int a = static_cast<int>((frame.mData1 >> 8) & 1);
    int b = static_cast<int>((frame.mData1 >> 7) & 1);
    bool error = (frame.mFlags >> MSF_FLAGS_ERROR) & 1;
    bool second00 = (frame.mFlags >> MSF_FLAGS_SECOND00) & 1;

    ClearResultStrings();

    // We know statically that the sprintf buffer will not overflow
    #pragma warning(push)
    #pragma warning(disable : 4996) // warning C4996: 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.

    if (second00)
    {
        AddResultString("Second 00");
    }
    else if (error)
    {
        char resultStr[128];
        AddResultString("!");
        sprintf(resultStr, "A=%d, B=%d (error)", a, b);
        AddResultString(resultStr);
    }
    else
    {
        char resultStr[128];
        sprintf(resultStr, "A=%d, B=%d", a, b);
        AddResultString(resultStr);
    }

    #pragma warning(pop)
}


void MSFAnalyzerResults::GenerateExportFile(const char* file, DisplayBase displayBase, U32 exportTypeUserId)
{
    std::ofstream fileStream(file, std::ios::out);

    U64 triggerSample = mAnalyzer->GetTriggerSample();
    U32 sampleRate = mAnalyzer->GetSampleRate();

    fileStream << "Time [s],A,B,Second 00,Error" << std::endl;

    U64 numFrames = GetNumFrames();
    for (U32 i = 0; i < numFrames; i++)
    {
        Frame frame = GetFrame(i);

        char timeStr[128];
        AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, triggerSample, sampleRate, timeStr, 128);
        int a = static_cast<int>((frame.mData1 >> 8) & 1);
        int b = static_cast<int>((frame.mData1 >> 7) & 1);
        bool error = (frame.mFlags >> MSF_FLAGS_ERROR) & 1;
        bool second00 = (frame.mFlags >> MSF_FLAGS_SECOND00) & 1;

        fileStream << timeStr << ',' << a << ',' << b << ',' << (int)second00 << ',' << (int)error << '\n';

        if (UpdateExportProgressAndCheckForCancel(i, numFrames) == true)
        {
            fileStream.close();
            return;
        }
    }

    fileStream.close();
}


void MSFAnalyzerResults::GenerateFrameTabularText(U64 frameIndex, DisplayBase displayBase)
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
    Frame frame = GetFrame(frameIndex);
    ClearTabularText();

    int a = static_cast<int>((frame.mData1 >> 8) & 1);
    int b = static_cast<int>((frame.mData1 >> 7) & 1);
    bool error = (frame.mFlags >> MSF_FLAGS_ERROR) & 1;
    bool second00 = (frame.mFlags >> MSF_FLAGS_SECOND00) & 1;

    AddTabularText(a ? "1" : "0", b ? "1" : "0", second00 ? "1" : "0", error ? "error" : "");
#endif
}


void MSFAnalyzerResults::GeneratePacketTabularText(U64 packetId, DisplayBase displayBase)
{
    // not supported
}


void MSFAnalyzerResults::GenerateTransactionTabularText(U64 transactionId, DisplayBase displayBase)
{
    // not supported
}
