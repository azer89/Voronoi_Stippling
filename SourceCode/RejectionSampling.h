#ifndef REJECTIONSAMPLING_H
#define REJECTIONSAMPLING_H

/*
* Reza Adhitya Saputra
* reza.adhitya.saputra@gmail.com
*
*/

#include "stdafx.h"
#include "MyPoint.h"

#include <map>
#include <random>
#include <vector>
#include <algorithm>

struct PixelData
{
public:
	int key;
	int value;

	PixelData(int key, int value)
	{
		this->key = key;
		this->value = value;
	}

	static bool sortByValue(const PixelData& data1, const PixelData& data2) { return data1.value > data2.value; }
	//static int myrandom (int i) { return std::rand()%i;}
};

//bool sortByValue(const PixelData &data1, const PixelData &data2) { return data1.value > data2.value; }

// http://stackoverflow.com/questions/22975359/how-to-optimize-rejection-sampling
template <typename T>
class sampler
{
	std::vector<T> keys;
	std::discrete_distribution<T> distr;

public:
	// this only works for visual c++
	sampler(const std::vector<T>& keys, const std::vector<float>& prob)
	{
		this->keys = keys;

		std::size_t i = 0;
		this->distr = std::discrete_distribution<>(prob.size(),
			0, // smaller value
			1, // bigger value
			[&prob, &i](float) // reference i by address
			{
				auto w = prob[i];
				++i;
				return w;
			});
	}

	/*
	// If you use g++
	sampler(const std::vector<T>& keys, const std::vector<float>& prob) :
	keys(keys), distr(prob.begin(), prob.end()) { }
	*/

	T generateRandom()
	{
		static std::random_device rd;   // non-deterministic generator
		static std::mt19937 gen(rd());  // Mersenne Twister pesudorandom number generator (a 32-bit word length)
		return keys[distr(gen)];
	}
};


class RejectionSampling
{
public:
	RejectionSampling();
	~RejectionSampling();

	std::vector<MyPoint> GeneratePoints(std::vector<float> grayValues, std::vector<int> alphaValues, int numPt, int img_width, int img_height);
};

#endif // REJECTIONSAMPLING_H
