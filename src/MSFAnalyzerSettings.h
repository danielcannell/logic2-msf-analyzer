#pragma once

#include <memory>
#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>


class MSFAnalyzerSettings : public AnalyzerSettings
{
public:
    MSFAnalyzerSettings();
    virtual ~MSFAnalyzerSettings();

    bool SetSettingsFromInterfaces() override;
    void LoadSettings(const char* settings) override;
    const char* SaveSettings() override;

    void UpdateInterfacesFromSettings();

    Channel mInputChannel = UNDEFINED_CHANNEL;
    bool mInverted = false;

protected:
    std::unique_ptr<AnalyzerSettingInterfaceChannel> mInputChannelInterface;
    std::unique_ptr<AnalyzerSettingInterfaceNumberList> mInvertedInterface;
};
