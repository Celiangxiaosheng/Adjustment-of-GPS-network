// CoGPS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyGPS.h"
#include "public.h"

//////////////////////////////////////////////////////////////////////////
//    ��С����ƽ������
void  main1()
{
	CCoGPS coGPS;
	char *rfile="�̲�����\\��С����ƽ��\\r.txt";
	char *dfile ="�̲�����\\��С����ƽ��\\Data.txt";
	coGPS.resultfp=fopen(rfile,"w");
	if(coGPS.resultfp==NULL)
	{
		MyBreak("�򿪽���ļ�ʧ��");
		return;
	}	
	fprintf(coGPS.resultfp," GPS����С����ƽ������:\n");
	
	coGPS.InputData(dfile);
	//coGPS.PrintData();
	
	coGPS.LeastSquares();
	fclose(coGPS.resultfp);
	
	printf("\n\n\n�����ļ���%s",dfile);
	printf("\n����ļ���%s\n\n",rfile);
	
}

//////////////////////////////////////////////////////////////////////////
//  �����������
void main2()
{
	CCoGPS coGPS;
	char *rfile="�̲�����\\robust\\r.txt";
	char *dfile ="�̲�����\\robust\\Data.txt";
	
	coGPS.resultfp=fopen(rfile,"w");
	if(coGPS.resultfp==NULL)
	{
		MyBreak("�򿪽���ļ�ʧ��!");
		return;
	}	
	
	coGPS.InputData(dfile); //�����������ں���������
	// coGPS.PrintData();
	
	coGPS.Robust(IGG3,0.005);
	fclose(coGPS.resultfp);
	
	printf("\n\n\n�����ļ���%s",dfile);
	printf("\n����ļ���%s\n\n",rfile);
	
}
//////////////////////////////////////////////////////////////////////////
//  �ֲ�̽������
void main3()
{
	CCoGPS coGPS;
	char *dfile="�̲�����\\�ֲ�̽��\\Data.txt";
	char *rfile="�̲�����\\�ֲ�̽��\\r.txt";
	
	coGPS.resultfp=fopen(rfile,"w");
	if(coGPS.resultfp==NULL)
	{
		MyBreak("�򿪽���ļ�ʧ��");
		return;
	}	
	
	coGPS.InputData(dfile); //�����������ں���������
	//coGPS.PrintData();
	
	coGPS.DataSnooping(0.001,20);
	fclose(coGPS.resultfp);
	
	printf("\n\n\n�����ļ���%s",dfile);
	printf("\n����ļ���%s\n\n",rfile);
}

//////////////////////////////////////////////////////////////////////////
//    ������ƽ������
void main4()
{
	CCoGPS coGPS;	
	char *dfile="�̲�����\\������ƽ��\\Data.txt";
	char *rfile="�̲�����\\������ƽ��\\r.txt";
	
	coGPS.resultfp=fopen(rfile,"w");
	if(coGPS.resultfp==NULL)
	{
		MyBreak("�򿪽���ļ�ʧ��");
		return;
	}	
	
	coGPS.InputData(dfile); //�����������ں���������
	//coGPS.PrintData();
	
	coGPS.Free();
	fclose(coGPS.resultfp);			
	
	printf("\n\n\n�����ļ���%s",dfile);
	printf("\n����ļ���%s\n\n",rfile);
	
}


//////////////////////////////////////////////////////////////////////////
//     ����ƽ������
void main5()
{
	CCoGPS coGPS;	
	char *dfile="�̲�����\\����ƽ��\\Data.txt";
	char *rfile="�̲�����\\����ƽ��\\r.txt";
	coGPS.resultfp=fopen(rfile,"w");
	if(coGPS.resultfp==NULL)
	{
		MyBreak("�򿪽���ļ�ʧ��");
		return;
	}	
	
	coGPS.InputData(dfile); //�����������ں���������
	//coGPS.PrintData();
	
	coGPS.Quasi_Stable("�̲�����\\����ƽ��\\���ȵ�.txt");
	fclose(coGPS.resultfp);
	
	printf("\n\n\n�����ļ���%s",dfile);
	printf("\n����ļ���%s\n\n",rfile);
	
}

//////////////////////////////////////////////////////////////////////////
//    ��֪�������Ȩƽ������
void  main6()
{
	CCoGPS coGPS;
	char *dfile="�̲�����\\��֪���Ȩƽ��\\Data.txt";
	char *rfile="�̲�����\\��֪���Ȩƽ��\\r.txt";
	coGPS.resultfp=fopen(rfile,"w");
	if(coGPS.resultfp==NULL)
	{
		MyBreak("�򿪽���ļ�ʧ��");
		return;
	}
	
	coGPS.InputData(dfile);
	//coGPS.PrintData();
				
	char *XYZfile="�̲�����\\��֪���Ȩƽ��\\��֪XYZ.txt";
	coGPS.InputKnownXYZ(XYZfile,false);
	
	coGPS.LeastSquares();
	fclose(coGPS.resultfp);
	printf("\n����ļ���%s\n\n",rfile);
	
}

//////////////////////////////////////////////////////////////////////////
//  ��֪����������Ȩƽ������
void main7()
{
	CCoGPS coGPS;	
	char *dfile="�̲�����\\��֪������Ȩƽ��\\Data.txt";
	char *rfile="�̲�����\\��֪������Ȩƽ��\\r.txt";
	char *XYZfile="�̲�����\\��֪������Ȩƽ��\\��֪XYZ.txt";
	
	coGPS.resultfp=fopen(rfile,"w");
	if(coGPS.resultfp==NULL)
	{
		MyBreak("�򿪽���ļ�ʧ��");
	}	
	
	coGPS.InputData(dfile); //�����������ں���������
	//coGPS.PrintData();
	
	coGPS.InputKnownXYZ(XYZfile,true); //true��ʾ���Ȩ
	coGPS.LeastSquares();
	
	fclose(coGPS.resultfp);
	printf("\n����ļ���%s\n\n",rfile);
}


//////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{

	printf("\n  ==== GPS��ƽ����� ====");
	printf("\n  1.��С����ƽ��");
	printf("\n  2.��С����ƽ�������Ȩ��");
	printf("\n  3.��С����ƽ����������Ȩ��");
	
	printf("\n  4.������ƽ��");
	printf("\n  5.����ƽ��\n");
	printf("\n  6.�������");
	printf("\n  7.�ֲ�̽��\n");
	
	int sw=getch()-'0';
	
	switch(sw) {
	case 1: // ��С����ƽ��
		{
			main1();
		}
		break;
	case 2:  //��С����ƽ�������Ȩ��
		{
			main6();
		}
		break;
	case 3: //��С����ƽ����������Ȩ��
		{
			main7();
		}
		break;
	case 4:
		{   // ������ƽ��
			main4();
		}
		break;
	case 5:
		{   // ����ƽ��
			main5();
		}
		break;
	case 6: // �������
		{
			main2();
		}
		break;
	case 7:  // �ֲ�̽��
		{  
			main3();			
		}
		break;
		
	default:
		break;
	}
	return 0;
}
