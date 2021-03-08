#include <opencv2/opencv.hpp>

#ifndef VSRC_H_INCLUDE
#define VSRC_H_INCLUDE

using namespace cv;
using namespace std;

namespace vidsrc {

	class vsrc {
		
	public:
		vsrc(int dataset);
		int GetNumSeqs() { return _NumSeqs; }
		int GetNumCategs() { return _NumCategs; }
		string GetCategory(int c);
		string GetSequence(int s);
		string GetDatasetPath();
		string GetImagePath();
		string GetVideoPath(int category, int sequence) { return GetDatasetPath() + "/" + GetCategory(category) + "/" + GetSequence(sequence) + GetImagePath(); };

	private:

		int _dataset;
		int _NumCategs;
		int _NumSeqs;

		//Sources
		string _dataset_path_1 = "C:/Users/javier.rico/Desktop/IPCV/UAM/AVSA/AVSA2020datasets/dataset_lab3";
		string _dataset_cat_1[1] = { "" };
		string _baseline_seq_1[1] = { "lab3.1/singleball.mp4" };
		string _image_path_1 = "";

		string _dataset_path_2 = "C:/Users/javier.rico/Desktop/IPCV/UAM/AVSA/AVSA2020datasets/dataset_lab3";
		string _dataset_cat_2[1] = { "" };
		string _baseline_seq_2[4] = { "lab3.2/video2.mp4","lab3.2/video3.mp4","lab3.2/video5.mp4","lab3.2/video6.mp4" };
		string _image_path_2 = "";

		string _dataset_path_3 = "C:/Users/javier.rico/Desktop/IPCV/UAM/AVSA/AVSA2020datasets/dataset_lab3";;
		string _dataset_cat_3[1];
		string _baseline_seq_3[4]{ "lab3.3/abandonedBox_600_1000_clip.mp4","lab3.3/boats_6950_7900_clip.mp4","lab3.3/pedestrians_800_1025_clip.mp4","lab3.3/streetCornerAtNight_0_100_clip.mp4" };
		string _image_path_3 = "";

		string _dataset_path_4 = "C:/Users/javier.rico/Desktop/IPCV/UAM/AVSA/AVSA2020datasets";
		string _dataset_cat_4[1] = { "" };
		string _baseline_seq_4[8] = { "ETRI/ETRI_od_A.avi","PETS2006/PETS2006_S1/PETS2006_S1_C3.mpeg","PETS2006/PETS2006_S4/PETS2006_S4_C3.avi","PETS2006/PETS2006_S5/PETS2006_S5_C3.mpeg","VISOR/visor_Video00.avi","VISOR/visor_Video01.avi","VISOR/visor_Video02.avi","VISOR/visor_Video03.avi" };
		string _image_path_4 = "";

		//Results
		string _project_root_path = "C:/Users/javier.rico/Desktop/IPCV/UAM/AVSA/Lab3/AVSA2020results/";
		string _project_name = "Lab3AVSA2020";
		string _results_path = _project_root_path + "/" + _project_name + "/results";


		//void CreateResultsDirectories();
	};
}

#endif