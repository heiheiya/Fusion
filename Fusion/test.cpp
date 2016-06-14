#include <vector>
#include <iostream>
#include <direct.h>
#include "FileFind.h"
#include "Fusion.h"

using namespace std;

const int BUFLEN = 200;

void help()
{
	cout << "---Usage:Fusion.exe n(n>0 for n th dir, n==0 for all)proceess n th dir in resource )" <<endl
		 <<"c(c=0 NULL, c=1 color correction) r(radius) s(sigma) m(midtone) " << endl;
}

bool process(string srgbdir_path, string srgbdir_name, string smonodir_path, string smonodir_name,int num, int correction = 0, int radius = 15, float sigma = 9, float midtone = 128)
{
	cout << "----------Enter " << num << "sud dir----------" << endl;
	int rgblen = 0;
	int monolen = 0;
	vector<string> rgbimg_dir_path(0), rgbimg_dir_name(0);
	vector<string> monoimg_dir_path(0), monoimg_dir_name(0);
	FindSubDir((srgbdir_path).c_str(), rgbimg_dir_path, rgbimg_dir_name,rgblen);
	FindSubDir((smonodir_path).c_str(), monoimg_dir_path, monoimg_dir_name,monolen);

	Mat rgb;
	Mat mono;

	string rgb_name;
	string mono_name;

	char *rgb_dir = (char*)malloc(512 * sizeof(char));
	char *mono_dir = (char*)malloc(512 * sizeof(char));
	char *fusion_dir = (char*)malloc(512 * sizeof(char));
	memset(rgb_dir, '\0', 512 * sizeof(char));
	memset(mono_dir, '\0', 512 * sizeof(char));
	memset(fusion_dir, '\0', 512 * sizeof(char));

	string s = "output";
	_mkdir(s.c_str());

	const char* bsp_t = srgbdir_name.c_str();
	s += "\\";
	s += srgbdir_name;
	const char* bsp_timestamp = s.c_str();
	_mkdir(bsp_timestamp);
	String s1;

	stringstream ss;
	string str;

	if (rgblen == 0)
	{
		cout << "cannot find pic" << endl;
		return 0;
	}
	cout << "There are " << rgblen << "  images in this dir" << endl;
	for (int i = 0; i < rgblen; i++)
	{
		sprintf_s(fusion_dir, BUFLEN, "output/%s/%s", bsp_t, rgbimg_dir_name[i].c_str());
		_mkdir(fusion_dir);

		cout << "----------start process image----------" << i + 1 << endl;
		rgb_name.clear();
		rgb_name = rgbimg_dir_path[i];
		rgb_name += "/rcv_rgb_denoise.jpg";
		rgb = imread(rgb_name.c_str());
		if (rgb.empty())
		{
			cout << "Failed to Load rgb img %d" << i + 1 << endl;
			continue;
			cout << "Press Enter finish program" << endl;
			getchar();
			return -1;
		}

		mono_name.clear();
		mono_name = monoimg_dir_path[i];
		mono_name += "/rcv_mono_cop.jpg";
		mono = imread(mono_name.c_str());
		if (mono.empty())
		{
			cout << "Failed to Load mono img %d" << i + 1 << endl;
			continue;
			cout << "Press Enter finish program" << endl;
			getchar();
			return -1;
		}

		Mat fusionMat;
		fusion(rgb, mono, fusionMat);

		s1.clear();
		s1 = fusion_dir;
		s1 += "/rcv_fusion.jpg";
		imwrite(s1, fusionMat);

		if (correction == 1)
		{
			Mat corrMat;
			nonLinearMask(fusionMat, radius, sigma, midtone, corrMat);

			s1.clear();
			s1 = fusion_dir;
			s1 += "/rcv_fusion_correction.jpg";
			imwrite(s1, corrMat);
		}


		cout << "----------finish process image----------" << i + 1 << endl << endl;


		ss.str("");
		ss << (i + 1);
		str.clear();
		str = ss.str();
	}
	cout << "----------Finish" << num << "sub dir----------" << endl << endl;
}


int main(int argc, char* argv[])
{
	help();
	if (argc < 2)
	{
		cout << "Usage:Fusion.exe 1" << endl;
		cout << "Press Enter to finish the program" << endl;
		getchar();
		return -1;
	}
	int nrgbSub = 0;
	int nmonoSub = 0;
	vector<string> rgb_sub_dir_path(0), rgb_sub_dir_name(0);
	vector<string> mono_sub_dir_path(0), mono_sub_dir_name(0);
	FindSubDir("resource1", mono_sub_dir_path, mono_sub_dir_name, nmonoSub);
	FindSubDir("resource2", rgb_sub_dir_path, rgb_sub_dir_name, nrgbSub);

	if (nrgbSub == 0 || nmonoSub == 0)
	{
		cout << "Make sure you have correct directory" << endl;
		cout << "no sub dir in resource" << endl;
		cout << "Press Enter to finish the program" << endl;
		getchar();
		return -1;
	}

	int dirNum = atoi(argv[1]);
	//int dirNum = 3;

	int start = 0;
	int end = nrgbSub;
	if (dirNum == 0 && nrgbSub > 0)
	{
		cout << "Process all bsp_timestamp" << endl;

	}
	else if (dirNum > 0 && dirNum <= nrgbSub)
	{
		cout << "Process one bsp_timestamp" << endl;
		start = dirNum-1;
		end = dirNum ;
	}
	else if (dirNum<0 || dirNum>nrgbSub)
	{
		cout << "Make sure you enter correct number" << endl;
		cout << "Press Enter to finish the program" << endl;
		getchar();
		getchar();
		return -1;
	}

	int correction = 0;
	int radius = 15;
	float sigma = 9;
	float midtone = 128;
	if (argc > 2)
	{
		correction = atoi(argv[2]);
		radius = atof(argv[3]);
		sigma = atof(argv[4]);
		midtone = atof(argv[5]);
	}

	cout << "----------Start batch test----------" << endl;
	for (int i = start; i < end; i++)
	{

		cout << "Process " << (i + 1) << " rgb name is:" << rgb_sub_dir_name[i].c_str() << endl;
		cout << "Process " << (i + 1) << " mono name is:" << mono_sub_dir_name[i].c_str() << endl;
		bool state = process(rgb_sub_dir_path[i], rgb_sub_dir_name[i], mono_sub_dir_path[i], mono_sub_dir_name[i], i, correction, radius, sigma, midtone);
		if (!state)
			continue;

		cout << "finish Process " << (i + 1) << " rgb name is:" << rgb_sub_dir_name[i].c_str() << endl;
		cout << "finish Process " << (i + 1) << " mono name is:" << mono_sub_dir_name[i].c_str() << endl;
	}
	cout << "----------Finish batch test----------" << endl << endl;
}