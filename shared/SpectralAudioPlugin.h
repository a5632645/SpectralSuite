#pragma once

#include "JuceHeader.h"
#include "StandardFFTProcessor.h"
#include "FftSizeChoiceAdapter.h"
#include "FftStyleChoiceAdapter.h"
#include "FftOverlapsChoiceAdapter.h"
#include "FftWindowChoiceAdapter.h"
#include "FftSwitcher.h"
#include "VersionCheck.h"
#include "SpectralAudioProcessorInteractor.h"
#include "ParameterContainerComponentFactory.h"
#include "PluginParameters.h"
#include "FftCircleBuffer.h"

class SpectralAudioPluginUi;

class SpectralAudioPlugin : public AudioProcessor, private FftSwitcherThread::FftSwitcher
{
public:
    static const int FFT_OVERLAPS;
    static const int INIT_FFT_INDEX;

public:
    class DependencyFactory {
    public:
        virtual ~DependencyFactory() {};

        virtual std::shared_ptr<PluginParameters> createParams(SpectralAudioPlugin* plugin) = 0;
        virtual ParameterContainerComponent* createUi(SpectralAudioPlugin* plugin) = 0;
        virtual std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor(SpectralAudioPlugin* plugin) = 0;
        virtual Array<int> fftSizesToNotInclude() { return Array<int>(); };
    };



    SpectralAudioPlugin(
        DependencyFactory* dependencies
    );
    ~SpectralAudioPlugin();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    //==============================================================================
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // FftSwitcher methods
    void switchFftSize() override;
    void switchFftStyle() override;
    void switchOverlapCount() override;
    void switchFftWindowType() override;

    void checkForUpdates(VersionCheckThread::Listener* onUpdateAvailableListener);

private:
    //==============================================================================
    void emptyOutputs();
    void setFftSize(int fftSize);
    void initialiseParameters();
    bool isPreparingToPlay() const { return true; }
    bool isInvalidFftModificationState() const {
        return
            m_fftSwitcher.threadShouldExit()
            || m_output.empty()
            || m_inputBusDatas.empty();
    };

    std::shared_ptr<PluginParameters> parameters;
    FftCircleBuffer m_testDev;

    FftSizeChoiceAdapter m_fftSizeChoiceAdapter;
    FftStyleChoiceAdapter m_fftStyleChoiceAdapter;
    FftOverlapsChoiceAdapter m_fftOverlapsChoiceAdapter;
    FftWindowChoiceAdapter m_fftWindowChoiceAdapter;
    FftSwitcherThread m_fftSwitcher;

    int m_internalBufferReadWriteIndex;
    std::vector<BusAudioData> m_inputBusDatas;
    BusAudioData m_output;

    VersionCheckThread m_versionCheckThread;
    std::unique_ptr<DependencyFactory> m_dependencyFactory;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralAudioPlugin)
};
