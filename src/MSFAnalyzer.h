#pragma once

#include <memory>
#include <Analyzer.h>
#include <AnalyzerHelpers.h>
#include "MSFAnalyzerResults.h"
#include "MSFSimulationDataGenerator.h"


class MSFAnalyzerSettings;


class MSFAnalyzer : public Analyzer2
{
public:
    MSFAnalyzer();
    virtual ~MSFAnalyzer();

    void SetupResults() override;
    void WorkerThread() override;

    U32 GenerateSimulationData(U64 newestSampleRequested, U32 sampleRate, SimulationChannelDescriptor** simulationChannels) override;
    U32 GetMinimumSampleRateHz() override;

    const char* GetAnalyzerName() const override;
    bool NeedsRerun() override;

protected:
    #pragma warning(push)
    #pragma warning(disable : 4251) // warning C4251: needs to have dll-interface to be used by clients of class
    std::unique_ptr<MSFAnalyzerSettings> mSettings = std::make_unique<MSFAnalyzerSettings>();
    std::unique_ptr<MSFAnalyzerResults> mResults;
    std::unique_ptr<MSFSimulationDataGenerator> mSimulationDataGenerator;
    #pragma warning(pop)

    ClockGenerator mClockGenerator;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer* analyzer);
