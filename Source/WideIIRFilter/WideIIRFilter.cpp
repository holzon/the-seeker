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
            return std::make_unique<WideIIRFilterEigen<11ul>>(eqspec);
        case EQSpec::EQType::EQ_1_3_BAND:
            return std::make_unique<WideIIRFilterEigen<33ul>>(eqspec);
        case EQSpec::EQType::EQ_1_6_BAND:
            return std::make_unique<WideIIRFilterEigen<67ul>>(eqspec);
        case EQSpec::EQType::EQ_1_12_BAND:
            return std::make_unique<WideIIRFilterEigen<135ul>>(eqspec);
        case EQSpec::EQType::EQ_1_24_BAND:
            return std::make_unique<WideIIRFilterEigen<271ul>>(eqspec);
        case EQSpec::EQType::EQ_1_48_BAND:
            return std::make_unique<WideIIRFilterEigen<543ul>>(eqspec);
        case EQSpec::EQType::EQ_1_96_BAND:
            return std::make_unique<WideIIRFilterEigen<1085ul>>(eqspec);
        case EQSpec::EQType::EQ_1_192_BAND:
            return std::make_unique<WideIIRFilterEigen<2169ul>>(eqspec);
    }
	assert(0 && "eq type mismatch");
	return 0;
}
