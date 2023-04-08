#pragma once

#include <AnalyzerResults.h>


class MSFAnalyzer;
class MSFAnalyzerSettings;


enum MsfFlags
{
    MSF_FLAGS_ERROR = 0,
    MSF_FLAGS_SECOND00 = 1,
};


class MSFAnalyzerResults : public AnalyzerResults
{
public:
    MSFAnalyzerResults(MSFAnalyzer* analyzer, MSFAnalyzerSettings* settings);
    virtual ~MSFAnalyzerResults();

    void GenerateBubbleText(U64 frameIndex, Channel& channel, DisplayBase displayBase) override;
    void GenerateExportFile(const char* file, DisplayBase displayBase, U32 exportTypeUserId) override;

    void GenerateFrameTabularText(U64 frameIndex, DisplayBase displayBase) override;
    void GeneratePacketTabularText(U64 packetId, DisplayBase displayBase) override;
    void GenerateTransactionTabularText(U64 transactionId, DisplayBase displayBase) override;

protected:
    MSFAnalyzerSettings* mSettings;
    MSFAnalyzer* mAnalyzer;
};
