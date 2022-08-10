// Chapter_7.h: interface for the CChapter_7 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAPTER_7_H__5C9643A2_4275_4A63_8EB0_B37808D051E1__INCLUDED_)
#define AFX_CHAPTER_7_H__5C9643A2_4275_4A63_8EB0_B37808D051E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////
//���ʼ�����
class CProbability
{
public:
	//      ٤�꺯������Ȼ����ֵ
	double lngam(double x);
	
	//  t�ֲ��ķֲ�����ֵ�������x�Ļ���ֵ��  //n ���ɶ�
	double t(int n, double x);
	
	//  t�ֲ��ķֲ��ܶ�ֵ�������ۼƺ���ֵʱ��Ҫ�����������
	double ff(int n,double x);
	
    //      F�ֲ�����ֵ:����(0,x)�ϵĸ���p, f-�ܶ�ֵ
	double Fdist(int n1,int n2, double x,double *f);
	
	//��̬�ֲ��ķֲ�����ֵ: p(-��,u)
    double norm(double u);
	
    //��̬�ֲ��ķ�����, p(-��,u)=p ; ��֪p, ����u
	double re_norm(double q);
    
	//  chi2�ֲ��ķֲ�������p(0,x)
    double chi2(int n,double x,double *f);
	//chi2�ֲ��ķ�������p=chi2(0,x),��֪x������p
	double re_chi2(int n,double p);
    
    //  F�ֲ��ķ�������p=F(0,x), ��֪p,����x
	double re_F(int n1,int n2,double p);
	
	//  t�ֲ��ķ�����: p(-��,u)=p; ��֪p, ����u
	double re_t(int n,double p); 
		
};


#endif // !defined(AFX_CHAPTER_7_H__5C9643A2_4275_4A63_8EB0_B37808D051E1__INCLUDED_)
