/*
  ==============================================================================

    WideIIRFilter.cpp
    Created: 22 Sep 2017 9:11:54pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#include "WideIIRFilter.h"
#include "WideIIRFilterEigen.h"

std::unique_ptr<WideIIRFilter> WideIIRFilter::make(const EQSpec& eqspec) {
    switch(eqspec.eqtype()) {
		case EQSpec::EQType::EQ_1_1_BAND:
            return std::make_unique<WideIIRFilterEigen<12ul>>(eqspec);
        case EQSpec::EQType::EQ_1_3_BAND:
            return std::make_unique<WideIIRFilterEigen<36ul>>(eqspec);
        case EQSpec::EQType::EQ_1_6_BAND:
            return std::make_unique<WideIIRFilterEigen<72ul>>(eqspec);
        case EQSpec::EQType::EQ_1_12_BAND:
            return std::make_unique<WideIIRFilterEigen<144ul>>(eqspec);
        case EQSpec::EQType::EQ_1_24_BAND:
            return std::make_unique<WideIIRFilterEigen<288ul>>(eqspec);
        case EQSpec::EQType::EQ_1_48_BAND:
            return std::make_unique<WideIIRFilterEigen<576ul>>(eqspec);
        case EQSpec::EQType::EQ_1_96_BAND:
            return std::make_unique<WideIIRFilterEigen<1152ul>>(eqspec);
        case EQSpec::EQType::EQ_1_192_BAND:
            return std::make_unique<WideIIRFilterEigen<2304ul>>(eqspec);
    }
	assert(0 && "eq type mismatch");
	return 0;
}
