#include <opencv2/opencv.hpp>
#include "vsrc.hpp"

using namespace vidsrc;

vsrc::vsrc(int dataset)
{
	_dataset = dataset;
	//CreateResultsDirectories();
	switch (_dataset)
	{
	case 0:
		_NumSeqs = 1;
		_NumCategs = 1;
		break;
	case 1:
		_NumSeqs = sizeof(_baseline_seq_1) / sizeof(_baseline_seq_1[0]);
		_NumCategs = sizeof(_dataset_cat_1) / sizeof(_dataset_cat_1[0]);
		break;
	case 2:
		_NumSeqs = sizeof(_baseline_seq_2) / sizeof(_baseline_seq_2[0]);
		_NumCategs = sizeof(_dataset_cat_2) / sizeof(_dataset_cat_2[0]);
		break;
	case 3:
		_NumSeqs = sizeof(_baseline_seq_3) / sizeof(_baseline_seq_3[0]);
		_NumCategs = sizeof(_dataset_cat_3) / sizeof(_dataset_cat_3[0]);
		break;
	case 4:
		_NumSeqs = sizeof(_baseline_seq_4) / sizeof(_baseline_seq_4[0]);
		_NumCategs = sizeof(_dataset_cat_4) / sizeof(_dataset_cat_4[0]);
		break;
	default:
		_NumSeqs = sizeof(_baseline_seq_1) / sizeof(_baseline_seq_1[0]);
		_NumCategs = sizeof(_dataset_cat_1) / sizeof(_dataset_cat_1[0]);
	}

}

string vidsrc::vsrc::GetCategory(int c)
{
	string retorno;

	switch (_dataset)
	{
	case 1:
		retorno = _dataset_cat_1[c];
		break;
	case 2:
		retorno = _dataset_cat_2[c];
		break;
	case 3:
		retorno = _dataset_cat_3[c];
		break;
	case 4:
		retorno = _dataset_cat_4[c];
		break;
	default:
		retorno = _dataset_cat_1[c];
	}

	return retorno;
}

string vidsrc::vsrc::GetSequence(int s)
{
	string retorno;

	switch (_dataset)
	{
	case 1:
		retorno = _baseline_seq_1[s];
		break;
	case 2:
		retorno = _baseline_seq_2[s];
		break;
	case 3:
		retorno = _baseline_seq_3[s];
		break;
	case 4:
		retorno = _baseline_seq_4[s];
		break;
	default:
		retorno = _baseline_seq_1[s];
	}

	return retorno;
}

string vidsrc::vsrc::GetDatasetPath()
{
	string retorno;

	switch (_dataset)
	{
	case 1:
		retorno = _dataset_path_1;
		break;
	case 2:
		retorno = _dataset_path_2;
		break;
	case 3:
		retorno = _dataset_path_3;
		break;
	case 4:
		retorno = _dataset_path_4;
		break;
	default:
		retorno = _dataset_path_1;
	}

	return retorno;
}

string vidsrc::vsrc::GetImagePath()
{
	string retorno;

	switch (_dataset)
	{
	case 1:
		retorno = _image_path_1;
		break;
	case 2:
		retorno = _image_path_2;
		break;
	case 3:
		retorno = _image_path_3;
		break;
	case 4:
		retorno = _image_path_4;
		break;
	default:
		retorno = _image_path_1;
	}

	return retorno;
}


//void vidsrc::vsrc::CreateResultsDirectories()
//{
//	string makedir_cmd = "mkdir \"" + _project_root_path + "/" + _project_name + "\"";
//	system(makedir_cmd.c_str());
//	makedir_cmd = "mkdir \"" + _results_path + "\"";
//	system(makedir_cmd.c_str());
//
//	switch (_dataset)
//	{
//	case 1:
//		_NumSeqs = sizeof(_baseline_seq_1) / sizeof(_baseline_seq_1[0]);
//		_NumCategs = sizeof(_dataset_cat_1) / sizeof(_dataset_cat_1[0]); 
//		for (int c = 0; c < _NumCategs; c++)
//		{
//			// create directory to store results for category
//			string makedir_cmd = "mkdir \"" + _results_path + "/" + _dataset_cat_1[c] + "\"";
//			system(makedir_cmd.c_str());
//
//			for (int s = 0; s < _NumSeqs; s++)
//			{
//				// create directory to store results for sequence
//				string makedir_cmd = "mkdir \"" + _results_path + "/" + _dataset_cat_1[c] + "/" + _baseline_seq_1[s] + "\"";
//				system(makedir_cmd.c_str());
//			}
//		}
//		break;
//	case 2:
//		_NumSeqs = sizeof(_baseline_seq_2) / sizeof(_baseline_seq_2[0]);
//		_NumCategs = sizeof(_dataset_cat_2) / sizeof(_dataset_cat_2[0]);
//		for (int c = 0; c < _NumCategs; c++)
//		{
//			string makedir_cmd = "mkdir \"" + _results_path + "/" + _dataset_cat_2[c] + "\"";
//			system(makedir_cmd.c_str());
//
//			for (int s = 0; s < _NumSeqs; s++)
//			{
//				// create directory to store results for sequence
//				string makedir_cmd = "mkdir \"" + _results_path + "/" + _dataset_cat_2[c] + "/" + _baseline_seq_2[s] + "\"";
//				system(makedir_cmd.c_str());
//			}
//		}
//		break;
//	case 3:
//		_NumSeqs = sizeof(_baseline_seq_3) / sizeof(_baseline_seq_3[0]);
//		_NumCategs = sizeof(_dataset_cat_3) / sizeof(_dataset_cat_3[0]);
//		for (int c = 0; c < _NumCategs; c++)
//		{
//			string makedir_cmd = "mkdir \"" + _results_path + "/" + _dataset_cat_3[c] + "\"";
//			system(makedir_cmd.c_str());
//
//			for (int s = 0; s < _NumSeqs; s++)
//			{
//				// create directory to store results for sequence
//				string makedir_cmd = "mkdir \"" + _results_path + "/" + _dataset_cat_3[c] + "/" + _baseline_seq_3[s] + "\"";
//				system(makedir_cmd.c_str());
//			}
//		}
//		break;
//	case 4:
//		_NumSeqs = sizeof(_baseline_seq_4) / sizeof(_baseline_seq_4[0]);
//		_NumCategs = sizeof(_dataset_cat_4) / sizeof(_dataset_cat_4[0]);
//		for (int c = 0; c < _NumCategs; c++)
//		{
//			string makedir_cmd = "mkdir \"" + _results_path + "/" + _dataset_cat_4[c] + "\"";
//			system(makedir_cmd.c_str());
//
//			for (int s = 0; s < _NumSeqs; s++)
//			{
//				// create directory to store results for sequence
//				string makedir_cmd = "mkdir \"" + _results_path + "/" + _dataset_cat_4[c] + "/" + _baseline_seq_4[s] + "\"";
//				system(makedir_cmd.c_str());
//			}
//		}
//		break;
//	default:
//		_NumSeqs = sizeof(_baseline_seq_1) / sizeof(_baseline_seq_1[0]);
//		_NumCategs = sizeof(_dataset_cat_1) / sizeof(_dataset_cat_1[0]);
//		for (int c = 0; c < _NumCategs; c++)
//		{
//			string makedir_cmd = "mkdir \"" + _results_path + "/" + _dataset_cat_1[c] + "\"";
//			system(makedir_cmd.c_str());
//
//			for (int s = 0; s < _NumSeqs; s++)
//			{
//				// create directory to store results for sequence
//				string makedir_cmd = "mkdir \"" + _results_path + "/" + _dataset_cat_1[c] + "/" + _baseline_seq_1[s] + "\"";
//				system(makedir_cmd.c_str());
//			}
//		}
//	}
//
//
//
//	
//}
