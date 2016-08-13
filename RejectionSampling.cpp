
#include "RejectionSampling.h"

/*
* Reza Adhitya Saputra
* reza.adhitya.saputra@gmail.com
*
*/


RejectionSampling::RejectionSampling()
{
}

RejectionSampling::~RejectionSampling()
{
}

//bool sortByValue(const PixelData &data1, const PixelData &data2) { return data1.value > data2.value; }

// gray values should be [0.0, 1.0]
std::vector<MyPoint> RejectionSampling::GeneratePoints(std::vector<float> grayValues, int numPt, int img_width, int img_height)
{
    std::vector<MyPoint> randomPoints;

    using T = int;

    // input
    std::vector<int> keys;

	for (size_t a = 0; a < grayValues.size(); a++)
	{
		grayValues[a] = std::exp(grayValues[a] * 5.0f);
	}

    for(size_t a = 0; a < grayValues.size(); a++)
    {
        keys.push_back(a);
    }

    sampler<T> sample(keys, grayValues);

	std::cout << "Sampler generated\n";

    std::map<T, size_t> hist;
    for (size_t n = 0; n < numPt; ++n)
    {
        T num = sample.generateRandom();
        hist[num]++;
    }

	std::cout << "Histogram generated\n";

    std::vector<PixelData> data;
	data.clear();
    for (auto i: hist)
    {
        data.push_back(PixelData(i.first, i.second));
    }

    //std::random_shuffle ( data.begin(), data.end(), PixelData::myrandom);
    //std::random_shuffle ( data.begin(), data.end());

    // fix me
    sort(data.begin(), data.end(), PixelData::sortByValue);

	for (size_t a = 0; a < data.size(); a++)
    {
        int key = data[a].key;
		if (key >= grayValues.size())
		{
			continue;
		}

        //float val = data[a].value;
        float val = grayValues[key];		

        int xIndex = key % img_width;
        int yIndex = key / img_width;
        if(val > 0.25) // fix me
        {
            randomPoints.push_back( MyPoint(xIndex, yIndex) );
        }
    }

    return randomPoints;
}


