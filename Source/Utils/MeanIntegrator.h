/*
  ==============================================================================

    MeanIntegrator.h
    Created: 30 Apr 2018 4:41:06pm
    Author:  jensg

  ==============================================================================
*/

#pragma once

#include <cstdint>

#include <Eigen/Dense>

template<typename T, size_t N>
class MeanIntegrator 
{
public:
	uint32_t size();
	void size(uint32_t numsamples);
	void integrate(const Eigen::Array<float, N, 1>& input, Eigen::Array<float, N, 1>& output);
private:
	Eigen::Matrix<T, N, Eigen::Dynamic> data;
	int pos = 0;
};

template<typename T, size_t N>
uint32_t MeanIntegrator<T, N>::size()
{
	return data.cols();
}

template<typename T, size_t N>
void MeanIntegrator<T, N>::size(uint32_t numsamples) 
{
	auto prevnumsamples = data.cols();
	data.conservativeResize(Eigen::NoChange, numsamples);
	if (numsamples > prevnumsamples) {
		data.rightCols(numsamples - prevnumsamples).setZero(); // = 0.0f;
	}
	if (numsamples > 0) {
		pos = pos % numsamples;
	}
}

template<typename T, size_t N>
void MeanIntegrator<T, N>::integrate(const Eigen::Array<float, N, 1>& input, Eigen::Array<float, N, 1>& output)
{
	if (data.cols() == 0) {
		return;
	}
	data.col(pos) = input;
	pos = (pos + 1) % data.cols();
	output = data.rowwise().mean();
}