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