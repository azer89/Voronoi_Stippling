
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

float DistanceToOthers(std::vector<MyPoint> points, MyPoint pt)
{
	float dist = std::numeric_limits<float>::max();
	for (int a = 0; a < points.size(); a++)
	{
		float d = pt.Distance(points[a]);
		if (d < dist)
		{
			dist = d;
		}
	}
	return dist;
}

//bool sortByValue(const PixelData &data1, const PixelData &data2) { return data1.value > data2.value; }

// gray values should be [0.0, 1.0]
std::vector<MyPoint> RejectionSampling::GeneratePoints(std::vector<float> grayValues, std::vector<int> alphaValues, int numPt, int img_width, int img_height)
{
    std::vector<MyPoint> randomPoints;

    using T = int;

    // input
    std::vector<int> keys;

	// exponentiation
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

    // fix me, why?
    sort(data.begin(), data.end(), PixelData::sortByValue);

	for (size_t a = 0; a < data.size(); a++)
    {
        int key = data[a].key;
		if (key >= grayValues.size())
			{ continue; }

        //float val = data[a].value;
        float grayVal = grayValues[key];
		int alphaVal  = alphaValues[key];

        int xIndex = key % img_width;
        int yIndex = key / img_width;

		// grayVal is the gray value, 0 == black, 1 == white
		// fix me, I have a problem of too noisy points in the white area (but can be alleviated if alphaVal == 0)

		if (grayVal  > 0.25 &&
			alphaVal > 0    &&
			DistanceToOthers(randomPoints, MyPoint(xIndex, yIndex)) > 4.0f) 
        {
            randomPoints.push_back( MyPoint(xIndex, yIndex) );
        }
    }

    return randomPoints;
}


