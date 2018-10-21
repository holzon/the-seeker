/*
  ==============================================================================

    EQProcessor.h
    Created: 26 Sep 2017 8:44:39pm
    Author:  JENS OLSSON

  ==============================================================================
*/

#pragma once

#include "../EQSpec/EQSpec.h"

class EQProcessor {
public:
    virtual ~EQProcessor() = default;
    virtual void setRms(float ms) = 0;
    virtual void rms(std::vector<float>& values, std::vector<float>& gradient) = 0;
    virtual void setEqType(EQSpec::EQType type) = 0;
    virtual EQSpec::EQType eqtype() const = 0;
    virtual int numbands() const = 0;
    virtual EQSpec eqspec() const = 0;
    virtual int level() const = 0;
    virtual void level(int value) = 0;
};
