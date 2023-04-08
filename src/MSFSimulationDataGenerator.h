#pragma once

#include <SimulationChannelDescriptor.h>
#include <AnalyzerHelpers.h>


class MSFAnalyzerSettings;


class MSFSimulationDataGenerator
{
public:
    void Initialize(U32 simulationSampleRate, MSFAnalyzerSettings* settings);
    U32 GenerateSimulationData(U64 newestSampleRequested, U32 sampleRate, SimulationChannelDescriptor** simulationChannel);

protected:
    U32 mBitIdx = 0;

    MSFAnalyzerSettings* mSettings;
    U32 mSampleRate;
    SimulationChannelDescriptor mSimulationData;
    ClockGenerator mClockGenerator;
};
