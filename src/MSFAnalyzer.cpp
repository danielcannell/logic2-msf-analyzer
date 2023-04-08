#include "MSFAnalyzer.h"
#include "MSFAnalyzerSettings.h"
#include <AnalyzerChannelData.h>


MSFAnalyzer::MSFAnalyzer() : Analyzer2()
{
    SetAnalyzerSettings(mSettings.get());
}


MSFAnalyzer::~MSFAnalyzer()
{
    KillThread();
}


void MSFAnalyzer::SetupResults()
{
    mResults = std::make_unique<MSFAnalyzerResults>(this, mSettings.get());
    SetAnalyzerResults(mResults.get());
    mResults->AddChannelBubblesWillAppearOn(mSettings->mInputChannel);
}


void MSFAnalyzer::WorkerThread()
{
    U32 mSampleRateHz = GetSampleRate();

    AnalyzerChannelData* mChannel = GetAnalyzerChannelData(mSettings->mInputChannel);

    const BitState carrier = mSettings->mInverted ? BIT_LOW : BIT_HIGH;
    const BitState noCarrier = mSettings->mInverted ? BIT_HIGH : BIT_LOW;

    while (true)
    {
        if (mChannel->GetBitState() == noCarrier)
        {
            mChannel->AdvanceToNextEdge();
        }

        mChannel->AdvanceToNextEdge();
        mClockGenerator.Init(20.0, mSampleRateHz);
        U64 startingSample = mChannel->GetSampleNumber();
        mChannel->Advance(mClockGenerator.AdvanceByHalfPeriod());

        U64 data = 0;

        for (U32 i = 0; i < 10; i++)
        {
            mResults->AddMarker(mChannel->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel);

            data <<= 1;
            if (mChannel->GetBitState() == carrier)
            {
                data |= 1;
            }

            if (i != 9)
            {
                mChannel->Advance(mClockGenerator.AdvanceByHalfPeriod());
                mChannel->Advance(mClockGenerator.AdvanceByHalfPeriod());
            }
        }

        Frame frame;

        frame.mData1 = data;
        frame.mFlags = 0;

        if ((data & 0b1111111111) == 0b0000011111)
        {
            frame.mFlags |= 1 << MSF_FLAGS_SECOND00;
        }
        else if ((data & 0b1001111111) != 0b0001111111)
        {
            frame.mFlags |= 1 << MSF_FLAGS_ERROR;
        }

        frame.mStartingSampleInclusive = startingSample;
        frame.mEndingSampleInclusive = mChannel->GetSampleNumber();

        mResults->AddFrame(frame);
        mResults->CommitResults();
        ReportProgress(frame.mEndingSampleInclusive);
    }
}


bool MSFAnalyzer::NeedsRerun()
{
    return false;
}


U32 MSFAnalyzer::GenerateSimulationData(U64 minimumSampleIndex, U32 deviceSampleRate, SimulationChannelDescriptor** simulationChannels)
{
    if (!mSimulationDataGenerator)
    {
        mSimulationDataGenerator = std::make_unique<MSFSimulationDataGenerator>();
        mSimulationDataGenerator->Initialize(GetSimulationSampleRate(), mSettings.get());
    }

    return mSimulationDataGenerator->GenerateSimulationData(minimumSampleIndex, deviceSampleRate, simulationChannels);
}


U32 MSFAnalyzer::GetMinimumSampleRateHz()
{
    return 40;
}


const char* MSFAnalyzer::GetAnalyzerName() const
{
    return "MSF Radio Time";
}


const char* GetAnalyzerName()
{
    return "MSF Radio Time";
}


Analyzer* CreateAnalyzer()
{
    return new MSFAnalyzer();
}


void DestroyAnalyzer(Analyzer* analyzer)
{
    delete analyzer;
}
