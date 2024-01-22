#include "PDInteractor.h"
#include "../../shared/PhaseVocoder.h"
#include "../../shared/PhaseBuffer.h"

#include "../../shared/SpectralAudioPlugin.h"
#include "../../shared/utilities.h"

#include "PDProcessor.h"

std::unique_ptr<StandardFFTProcessor> PDInteractor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount) {
    return std::make_unique<PhaseDistortionProcess>(fftSize, hopSize, hopSize * (index%numOverlaps), sampleRate, this->getPhaseBuffer());
}

void PDInteractor::bind_parameters(PDParameters& param) {
    m_distortion_amount = param.getAmountParameter();
    m_distortion_type = param.getTypeParameter();
    m_dynamic = param.getDynamicParameter();
}

void PDInteractor::prepareProcess(StandardFFTProcessor * spectralProcessor) {
    auto& processor = *((PhaseDistortionProcess*)spectralProcessor);

    processor.set_distortion_amount(*m_distortion_amount);
    processor.set_phase_distortion_type(m_distortion_type->getIndex());
    processor.set_dynamic(*m_dynamic);
}
