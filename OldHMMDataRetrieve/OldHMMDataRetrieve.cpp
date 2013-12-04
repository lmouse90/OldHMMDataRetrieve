// OldHMMDataRetrieve.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <atlstr.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <direct.h>
#include <vector>
using namespace std;

void mergeOldHMMFeatures_train(CString file1,CString file2,CString file3, CString file4, CString dstFile);
void mergeOldHMMFeatures_test(CString file, CString dstFile);
void getOldHMMFeature(CString filePath);
bool cmp(CString str1, CString str2);

bool cmp(CString str1, CString str2)
{
	int index1 = str1.Find("_");
	CString tp1 = str1.Left(index1);
	tp1 = tp1.Right(4);
	int i1 = atoi(tp1);
	int index2 = str2.Find("_");
	CString tp2 = str2.Left(index2);
	tp2 = tp2.Right(4);
	int i2 = atoi(tp2);
	if(i1 == i2)
	{
		return str1[index1+1]<str2[index2+1];
	}
	else
	{
		return i1<i2;
	}
}

void mergeOldHMMFeatures_train(CString file1,CString file2,CString file3, CString file4, CString dstFile)
{
	fstream fw(dstFile,ios::out);
	int frame,feaNums;
	float featureNum;
	fstream f1(file1,ios::in);
	f1 >> frame >> feaNums;
	fw << frame << " " << feaNums << endl;
	int i, j;
	for(i=0; i<frame; i++)
	{
		for(j=0; j<feaNums; j++)
		{
			f1 >> featureNum;
			fw << featureNum << " ";
		}
		fw << endl;
	}
	f1.close();

	fstream f2(file2,ios::in);
	f2 >> frame >> feaNums;
	fw << frame << " " << feaNums << endl;
	for(i=0; i<frame; i++)
	{
		for(j=0; j<feaNums; j++)
		{
			f2 >> featureNum;
			fw << featureNum << " ";
		}
		fw << endl;
	}
	f2.close();

	fstream f3(file3,ios::in);
	f3 >> frame >> feaNums;
	fw << frame << " " << feaNums << endl;
	for(i=0; i<frame; i++)
	{
		for(j=0; j<feaNums; j++)
		{
			f3 >> featureNum;
			fw << featureNum << " ";
		}
		fw << endl;
	}
	f3.close();

	fstream f4(file4,ios::in);
	f4 >> frame >> feaNums;
	fw << frame << " " << feaNums << endl;
	for(i=0; i<frame; i++)
	{
		for(j=0; j<feaNums; j++)
		{
			f4 >> featureNum;
			fw << featureNum << " ";
		}
		fw << endl;
	}
	f4.close();

	fw.close();
}

void mergeOldHMMFeatures_test(CString file, CString dstFile)
{
	fstream fw(dstFile,ios::out);
	int frame,feaNums;
	float featureNum;
	fstream f1(file,ios::in);
	f1 >> frame >> feaNums;
	fw << frame << " " << feaNums << endl;
	int i, j;
	for(i=0; i<frame; i++)
	{
		for(j=0; j<feaNums; j++)
		{
			f1 >> featureNum;
			fw << featureNum << " ";
		}
		fw << endl;
	}
	f1.close();

	fw.close();
}


void getOldHMMFeature(CString filePath)
{
	
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(filePath,&findFileData);

	vector<CString> vFileName;
	CString fileName;
	if(hFind == INVALID_HANDLE_VALUE) return;
	bool bNextFile = true;
	
	while(bNextFile)
	{	
		fileName = findFileData.cFileName;
		vFileName.push_back(fileName);

		bNextFile = FindNextFileA(hFind,&findFileData);
	}

	sort(vFileName.begin(),vFileName.end(),cmp);

	for(int i=0; i<vFileName.size(); )
	{
		CString tmpFilePath = filePath.Left(filePath.GetLength()-5);
		CString trainPath[4], testPath;

		fileName = vFileName[i];
		fileName =fileName.Left(5);

		cout << fileName << endl;

		for(int j=0; j<5; j++)
		{
			testPath = tmpFilePath + vFileName[i+j];
			int trainIndex = 0;
			for(int t=0; t<5; t++)
			{
				if(t != j)
				{
					trainPath[trainIndex++] = tmpFilePath + vFileName[t+i];
				}
			}

			CString dstFilePath1, dstFilePath2;
			dstFilePath1.Format("%s\\train%d",tmpFilePath,j);
			mkdir(dstFilePath1);
			CString dstTrainName = dstFilePath1 + "\\" +  fileName + ".txt";
			mergeOldHMMFeatures_train(trainPath[0],trainPath[1],trainPath[2],trainPath[3],dstTrainName);

			dstFilePath2.Format("%s\\test%d",tmpFilePath,j);
			mkdir(dstFilePath2);
			CString dstTestName = dstFilePath2 + "\\" +  fileName + ".txt";
			mergeOldHMMFeatures_test(testPath,dstTestName);
		}

		i += 5;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	CString filePath = "G:\\Thesis\\Data\\Tra_Hog_withoutInit_51_Format\\*.txt";
	getOldHMMFeature(filePath);
	return 0;
}

