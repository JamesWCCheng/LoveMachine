#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>
using namespace std;

struct Range
{
	Range(int s, int e) : start(s), end(e) {}
	int start;
	int end;
};
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::vector<Range> GetRanges(const std::string &ranges)
{
	std::vector<Range> rangesVec;
	auto rangepack = split(ranges, ',');
	for (auto& r : rangepack)
	{
		auto range = split(r, '-');
		rangesVec.push_back(Range(std::stoi(range[0]), std::stoi(range[1])));
	}
	return rangesVec;
}
int main()
{
	string ranges = "0-512, 513-5566,5567-39980550";
	auto output = GetRanges(ranges);
	FILE *fp = nullptr;
	auto err = fopen_s(&fp, "test.mp4", "rb");
	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);
	rewind(fp);

	int count = 0;
	bool isSplitEnd = false;
	int total = 0;
	for (auto && r : output)
	{
		auto expectReadLen = (r.end - r.start) + 1;
		char *buffer = new char[expectReadLen];
		auto realLen = fread(buffer, 1, expectReadLen, fp);

		FILE *fpw = nullptr;
		stringstream ss;
		string filename;
		ss << count << ".bin";
		ss >> filename;
		fopen_s(&fpw, filename.c_str(), "wb");
		fwrite(buffer, 1, realLen, fpw);
		delete[]buffer;
		count++;
		if (realLen < expectReadLen)
		{
			isSplitEnd = true;
			cout << "read end" << endl;
			break;
		}
		total += realLen;
	}

	if (!isSplitEnd)
	{
		int remaining = lSize - total;
		char *buffer = new char[remaining];
		auto realLen = fread(buffer, 1, remaining, fp);
		FILE *fpw = nullptr;
		stringstream ss;
		string filename;
		ss << count << ".bin";
		ss >> filename;
		fopen_s(&fpw, filename.c_str(), "wb");
		fwrite(buffer, 1, realLen, fpw);
	}
	fclose(fp);


	return 0;
}