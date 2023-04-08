#include "MSFAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


MSFAnalyzerSettings::MSFAnalyzerSettings()
{
    mInputChannelInterface = std::make_unique<AnalyzerSettingInterfaceChannel>();
    mInputChannelInterface->SetTitleAndTooltip("Input Channel", "Standard MSF Radio Time");
    mInputChannelInterface->SetChannel(mInputChannel);

    mInvertedInterface = std::make_unique<AnalyzerSettingInterfaceNumberList>();
    mInvertedInterface->SetTitleAndTooltip("Signal inversion", "Specify if the signal is inverted");
    mInvertedInterface->AddNumber(0.0, "Non Inverted (Carrier present is high)", "");
    mInvertedInterface->AddNumber(1.0, "Inverted", "");
    mInvertedInterface->SetNumber(mInverted ? 1.0 : 0.0);

    AddInterface(mInputChannelInterface.get());
    AddInterface(mInvertedInterface.get());

    AddExportOption(0, "Export as text/csv file");
    AddExportExtension(0, "text", "txt");
    AddExportExtension(0, "csv", "csv");

    ClearChannels();
    AddChannel(mInputChannel, "MSF", false);
}


MSFAnalyzerSettings::~MSFAnalyzerSettings()
{
}


bool MSFAnalyzerSettings::SetSettingsFromInterfaces()
{
    mInputChannel = mInputChannelInterface->GetChannel();
    mInverted = mInvertedInterface->GetNumber() > 0.5;

    ClearChannels();
    AddChannel(mInputChannel, "MSF Radio Time", true);

    return true;
}


void MSFAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mInputChannelInterface->SetChannel(mInputChannel);
    mInvertedInterface->SetNumber(mInverted ? 1.0 : 0.0);
}


void MSFAnalyzerSettings::LoadSettings(const char* settings)
{
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    text_archive >> mInputChannel;
    text_archive >> mInverted;

    ClearChannels();
    AddChannel(mInputChannel, "MSF Radio Time", true);

    UpdateInterfacesFromSettings();
}


const char* MSFAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << mInputChannel;
    text_archive << mInverted;

    return SetReturnString(text_archive.GetString());
}
