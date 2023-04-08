#include "MSFSimulationDataGenerator.h"
#include "MSFAnalyzerSettings.h"

#include <AnalyzerHelpers.h>


void MSFSimulationDataGenerator::Initialize(U32 simulationSampleRate, MSFAnalyzerSettings* settings)
{
    mSampleRate = simulationSampleRate;
    mSettings = settings;

    mClockGenerator.Init(10.0, simulationSampleRate);

    mSimulationData.SetChannel(mSettings->mInputChannel);
    mSimulationData.SetSampleRate(simulationSampleRate);
    mSimulationData.SetInitialBitState(BIT_HIGH);
}


U32 MSFSimulationDataGenerator::GenerateSimulationData(U64 largestSampleRequested, U32 sampleRate, SimulationChannelDescriptor** simulationChannel)
{
    U64 adjustedLargestSampleRequested = AnalyzerHelpers::AdjustSimulationTargetSample(largestSampleRequested, sampleRate, mSampleRate);

    while (mSimulationData.GetCurrentSampleNumber() < adjustedLargestSampleRequested)
    {
        const BitState carrier = mSettings->mInverted ? BIT_LOW : BIT_HIGH;
        const BitState noCarrier = mSettings->mInverted ? BIT_HIGH : BIT_LOW;
        mSimulationData.TransitionIfNeeded(mBitIdx == 0 ? noCarrier : carrier);

        mSimulationData.Advance(mClockGenerator.AdvanceByHalfPeriod());
        mBitIdx = (mBitIdx + 1) % 10;
    }

    *simulationChannel = &mSimulationData;
    return 1;
}
