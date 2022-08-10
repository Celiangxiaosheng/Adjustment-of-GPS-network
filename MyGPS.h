// MyGPS.h: interface for the CMyGPS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYGPS_H__C7B8ED1B_4EB2_46BF_B706_63D12F6B803D__INCLUDED_)
#define AFX_MYGPS_H__C7B8ED1B_4EB2_46BF_B706_63D12F6B803D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <conio.h>
#include <stdlib.h>


#include "stdio.h"
#include "math.h"
#include "string.h"

class CCoGPS  
{
public:
    CCoGPS();
	virtual ~CCoGPS();
	FILE   *resultfp; //����ļ�ָ��
	bool  m_IsPx;  // �Ƿ������Ȩƽ��	
	
	int    Anumber;      //ͬ������������������
    int    Pnumber;      //�ܵ���
    int    Vnumber;      //��������
    int    unPnumber;    //δ֪����

    double *XYZ;     // ��������	
    char   **Pname;  // ����ָ������
	bool   *IsKnown;//��֪���־����
	
    int    *dir0;        //��ͬ�����������ڹ۲�ֵ�е����
    int    *dir1,*dir2;  //�����˵��

	double *L;  //ȫ���۲�����
	double *ATPA,*ATPL;
	double MEDIAN;  //����λ������ĵ�λȨ�����
	double Sigma;   //��VTPV����ĵ�λȨ�����
	double **P;     //Ȩ�������飻
	double *QV;     //�۲�ֵ�в�Ȩ����
	double *V;      //�в�
    double *PV;     //
	double *W;      //Ȩ����	

    double *MxMyMz;  //��֪������������
	double *KnownXYZ;  //��֪������,������Ȩƽ��ʱ��
	int    *KnownPoints; //����֪��������Ӧ�ĵ��
    double *KnownPx;  //��֪��Ȩ����
	int    *AreaNumber;  //����������������ı��	

public:
	bool  InputData(char *DataFile); //����ԭʼ����
	bool  InputKnownXYZ(char *file,bool corelated); //������֪������
    void  PrintData(); //��ӡԭʼ����
		
	void  PrintXYZ();  //��ӡƽ������
    void  PrintLV(char *Title,double *LV);//��ӡ�۲�ֵ���в�

	void  LeastSquares(); //��С����ƽ�����
	void  Free();         //�ȿ�������ƽ�����
	void  Quasi_Stable(char *file);//����ƽ�����
	void  Robust(int fname,double eps); // ������С����ƽ��	
	void  DataSnooping(double Arfa,int MaxLoop);  // �ֲ�̽�����

	void CCoGPS::Helmert();
		
private:
	void  CaATPA();	// ��ɷ�����
    void  CaATPAi(int n,int *dir1,int *dir2,double *Pi,double *L);  
	void  Known();//��֪�㴦����
	void   CaLV(double V[]);//����в������������
	double CaVTPV(); //����в������
    double Ca_dX(bool IsPrint); //�����̽���,����������������
	int  GetStationNumber(char *buf); //������������ص��
	double Ca_VTPV_XYZ();//��֪������Ȩƽ��ʱ���������������
	
	// �����Ǵֲ�̽����㸨������
	void CaJTPJ(double JTPJ[], int Jint[],int s);
	double CaQg(int s,int Jint[],double gross[],
		         double Qg[],double JTPA[],double JTPV[] );
	void CaJTPA(double JTPA[],int s,int n0);
	void Ca_X_Qx(double gross[],double Qg[],double JTPA[],int s);

	//�����ǿ�����Ƹ�������
    void   Ca_UintV(); 
	void   caQV();
};





#endif // !defined(AFX_MYGPS_H__C7B8ED1B_4EB2_46BF_B706_63D12F6B803D__INCLUDED_)





















