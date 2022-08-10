// MyGPS.cpp: implementation of the CMyGPS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyGPS.h"
#include "public.h"


#define Old_Method  1
#define New_Method  0
#define Method  Old_Method


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoGPS::CCoGPS()
{
	m_IsPx = false; //�����в�����Ȩƽ��
}


CCoGPS::~CCoGPS()
{
  
}



/////////////////////////////////////////////////////////////
//        һ��ͬ�����ķ�����,������С����ƽ��
void CCoGPS::CaATPAi(int nk,  //�۲�ֵ������������������
					 int *dir1,
					 int *dir2,
					 double *Pk,
					 double *Lk // ������
					)  
{
     for(int s1=0;s1<nk;s1++)
     {
         int i1=3*dir1[s1/3]+s1%3; //����Vk��ϵ��Ϊ-1��δ֪�����
         int i2=3*dir2[s1/3]+s1%3; //����Vk��ϵ��Ϊ+1��δ֪�����

         for(int s2=0; s2<nk; s2++)
         {
            int j1=3*dir1[s2/3]+s2%3;
            int j2=3*dir2[s2/3]+s2%3;

			double p12 = Pk[ij(s1,s2)];

            double l=Lk[s2];

            ATPL[i1]-=p12*l;
            ATPL[i2]+=p12*l;

            if(i1>=j1)ATPA[ij(i1,j1)]+=p12;
            if(i1>=j2)ATPA[ij(i1,j2)]-=p12;
            if(i2>=j1)ATPA[ij(i2,j1)]-=p12;
            if(i2>=j2)ATPA[ij(i2,j2)]+=p12;
         }
     }
	 
}




//////////////////////////////////////////////////////////////////////////
//    ��ɷ�����ʽ
void CCoGPS::CaATPA()
{
     int t=3*Pnumber;
     int tt = t*(t+1)/2;
     for(int i=0;i<t;i++) ATPL[i]=0.0;
     for(i=0;i<tt;i++) ATPA[i]=0.0;

     double *l=new double [3*Vnumber];//����������
	 CaLV(l); //�������������

     for(i=0;i<Anumber;i++) //����������ɷ�����
     {
         int s=dir0[i];
         int n=dir0[i+1]-s;
         CaATPAi(3*n,dir1+s,dir2+s,P[i],l+3*s);
     }

     delete []l;
}






//////////////////////////////////////////////////////////////////////
//      ����ԭʼ���ݣ��������ꡢGPS������
bool CCoGPS::InputData(char *file)
{
	 FILE *fp=fopen(file,"r"); // ��ԭʼ�����ļ�
	 if(fp==NULL)
	 {
		 MyBreak("ԭʼ�����ļ��򲻿�!");
		 return 0;
	 }

	 fscanf(fp,"%d",&Pnumber); //�ܵ���
	 fscanf(fp,"%d",&Anumber); //ͬ����������������������
	 fscanf(fp,"%d",&Vnumber); //��������
	 
	 Pname = new char* [Pnumber]; //������ַ����
	 IsKnown = new bool[Pnumber]; //��֪/δ֪���־����
	 for(int i=0;i<Pnumber;i++)Pname[i]=NULL;
	 
     dir0=new int [Anumber+1];//��ͬ�������������ܹ۲������е����
     dir1=new int [Vnumber];  //������ʼ���
     dir2=new int [Vnumber];  //�����յ���
	 AreaNumber=new int [Vnumber]; //����������������ı��
	 
     int n=3*Vnumber;  //�۲�ֵ����
     L=new double [n]; //�����۲�ֵ
     V=new double [n]; //�в�
 
	 P=new double* [Anumber]; //�۲�ֵȨ�����ַ
     PV=new double [n]; //Ȩ�����ڲв������ĳ˻�
	 
     int t=3*Pnumber;
     int tt=t*(t+1)/2;
	 XYZ =new double[t];   //��������
     ATPA=new double [tt]; //������ϵ�����󣨻������
     ATPL=new double [t];  //����������������

	 unPnumber=0; //δ֪������
	 for(int k=0;k<Pnumber;k++) //��������
	 {
		 char name[20];
		 fscanf(fp,"%s",name); //����
		 int i=GetStationNumber(name); //����������ɵ��
		 if(i<0)
		 {
			 MyBreak("��ȡ�����ǳ������������ܵ���");  
			 return 0;
		 }
		 
		 fscanf(fp,"%lf%lf%lf",XYZ+3*i,XYZ+3*i+1,XYZ+3*i+2);//����ֵ 
		 fscanf(fp,"%d",IsKnown+i); //��֪���־
		 if(!IsKnown[i])unPnumber++;
	 }

     dir0[0]=0;
     for( i=0;i<=Anumber-1;i++)   //��ͬ����ѭ��
     {
		 int d;
		 fscanf(fp,"%d",&d); //ͬ�������		 
		 if(d!=i+1)
		 {
			 MyBreak("ͬ������Ų�����");
			 return 0;
		 }

         int jd;  //ͬ������������
		 fscanf(fp,"%d",&jd);

		 dir0[i+1]=dir0[i]+jd;
         for(int j=dir0[i];j<dir0[i+1];j++)
         {			 
             char P1[20],P2[20];
			 fscanf(fp,"%s %s",P1,P2); //���������Ķ˵����
             dir1[j]=GetStationNumber(P1);
             dir2[j]=GetStationNumber(P2);
			 if(dir1[j]<0 || dir2[j]<0)
			 {
				 MyBreak("��ȡ�������ݳ������������ܵ���");
				 return 0;
			 }

             //���������۲�ֵ
			 fscanf(fp,"%lf %lf %lf",L+3*j,L+3*j+1,L+3*j+2);
			 AreaNumber[j]=i;
         }

		 //---------------------------------------
		 //     ���뷽��Э�������
		 int ni=3*jd;
         double *Pi=new double [(ni+1)*ni/2];
		 P[i]=Pi;		 
		 
	 	 int s=0;
         for(j=0;j<ni;j++)
		 {
			 int d;
			 fscanf(fp,"%d",&d); //�к�
		     for(int k=0;k<=j;k++)
			 {
				 fscanf(fp,"%lf",Pi+s); //�����ķ���Э����
				 s++;
			 }
		 }
     }
     fclose(fp);

	 return 1;
}


//////////////////////////////////////////////////////////////////////
//       ���ԭʼ����
void CCoGPS::PrintData()
{
	fprintf(resultfp,"\n  �ܵ���: %d",Pnumber);
	fprintf(resultfp,"\n  ����������: %d",Anumber);
	fprintf(resultfp,"\n  ��������: %d\n",Vnumber);

	//�����֪����ͽ�������	
	for(int k=0; k<Pnumber; k++) 
	{
		fprintf(resultfp,"\n %4s",Pname[k]); //�������
		fprintf(resultfp,"  %14.5lf",XYZ[3*k]);
		fprintf(resultfp,"  %14.5lf",XYZ[3*k+1]);
		fprintf(resultfp,"  %14.5lf",XYZ[3*k+2]);
		fprintf(resultfp,"  %d",IsKnown[k]);
	}		
	 
	//��������۲�ֵ���䷽����
	for(int i=0; i<Anumber; i++)
	{
		 int jd=dir0[i+1]-dir0[i]; //������������
         fprintf(resultfp,"\n\n%4d  %d",i+1,jd);//�������ţ�����������

         for(int j=dir0[i];j<dir0[i+1];j++)
         {
             int k1=dir1[j];
             int k2=dir2[j];

             double dx=L[3*j];
             double dy=L[3*j+1];
             double dz=L[3*j+2];
	         
			 fprintf(resultfp,"\n %8s %8s",Pname[k1],Pname[k2]);
			 fprintf(resultfp,"%17.5lf %17.5lf %17.5lf ",dx,dy,dz);
		 }

         int jj=0;
         for(j=0;j<3*jd;j++)
         {
			  fprintf(resultfp,"\n%3d",j+1);
              for(int k=0;k<=j;k++)
			  {
				   if(k%4==0 && k>0)fprintf(resultfp,"\n   "); 
				   fprintf(resultfp," %17.9e",P[i][jj++]);
			  }
		 }
	 }
}


//////////////////////////////////////////////////////////////////////////
//    δ֪������
double CCoGPS::Ca_dX(bool IsPrint)
{
     int t=3*Pnumber;
     if(!inverse(ATPA,t))
	 {
		 MyBreak("�����̲�����!");
		 fclose(resultfp);
		 exit(0);
	 }
     
	 if(IsPrint)
	 {
		 fprintf(resultfp,"\n\n           ===== ��������� =====\n");		 
	 }
	 double max=0.0;
	 for(int i=0;i<t;i++)
	 {
          double xi=0.0;
   	      for(int j=0;j<t;j++)xi-=ATPA[ij(i,j)]*ATPL[j];
		  XYZ[i]+=xi;
		  if(fabs(xi)>fabs(max))max=xi;
		  if(IsPrint)
		  {
			  if(i%3==0) fprintf(resultfp,"\n%10s ",Pname[i/3]);
			  fprintf(resultfp," %8.4lf",xi);
		  }
     }
	 return max;
}


//////////////////////////////////////////////////////////////////////
//   �����������������С���˲в�
void CCoGPS::CaLV(double V[])
{
     for(int i=0;i<Vnumber;i++)
     {
         int k1=dir1[i];
         int k2=dir2[i];
         
		 V[3*i]=XYZ[3*k2]-XYZ[3*k1]-L[3*i];         
         V[3*i+1]=XYZ[3*k2+1]-XYZ[3*k1+1]-L[3*i+1]; 
         V[3*i+2]=XYZ[3*k2+2]-XYZ[3*k1+2]-L[3*i+2]; 
     }
}

//////////////////////////////////////////////////////////////////////
//    ���� PV,VTPV
double CCoGPS::CaVTPV()
{
	 double VTPV=0.0;
	 for(int i=0;i<Anumber;i++) // Anumber������������
	 {
	    double *Pi=P[i];     //i��Ȩ������ʼ��ַ
        int k1=3*dir0[i];    //i���������ڹ۲�ֵ�е����
        int k2=3*dir0[i+1];  //i+1���������ڹ۲�ֵ�е����

		double *PVi=PV+k1;
		double *Vi=V+k1;
		int n=k2-k1;

		double pvv=0.0;
		for(int j=0;j<n;j++)
		{
			double pvj=0.0;
			for(int k=0; k<n; k++) pvj+=Pi[ij(j,k)]*Vi[k];
			PVi[j]=pvj;
			pvv += pvj*Vi[j];
		}
		VTPV+=pvv;
	 }
	 return VTPV;
}



//////////////////////////////////////////////////////////////////////
//    ����в�V,Ҳ����������������������������Ȩ����
void  CCoGPS::PrintLV(char *Title,double *V)
{
	fprintf(resultfp,"\n\n        	         ===== %s =====\n\n",Title);
	fprintf(resultfp,"    ������  �� ��       �� ��"
		"     Vx         Vy         Vz");

	int last_ni = -1;
	for(int i=0;i<Vnumber;i++)
	{
		int ni=AreaNumber[i];
		if(ni!=last_ni)	fprintf(resultfp,"\n");
		last_ni=ni; 
		
		fprintf(resultfp,"\n%8d",ni+1);
		fprintf(resultfp,"%10s %10s",Pname[dir1[i]],Pname[dir2[i]]);
		fprintf(resultfp,"%10.4lf %10.4lf %10.4lf",V[3*i],V[3*i+1],V[3*i+2]);
	}  
}




//////////////////////////////////////////////////////////////////////
//  ��֪�㴦����֪��̶�����֪���Ȩ��
void CCoGPS::Known()
{
	if(!m_IsPx) //��֪����̶�ƽ��
	{
		for(int i=0;i<Pnumber;i++)
		{
			if(IsKnown[i])
			{
				ATPA[ij(3*i,3*i)]+=1.0e30;
				ATPA[ij(3*i+1,3*i+1)]+=1.0e30;
				ATPA[ij(3*i+2,3*i+2)]+=1.0e30;
			}
		}
		return;
	}

    //--------------------------------------
	//  ��֪�����Ȩƽ��
	int kn=Pnumber-unPnumber;
	for(int i=0;i<3*kn;i++) //ϵ������
	{
		int p1=3*KnownPoints[i/3]+i%3;
		for(int j=0;j<=i;j++)
		{
			int p2=3*KnownPoints[j/3]+j%3;
			ATPA[ij(p1,p2)]+=KnownPx[ij(i,j)];
		} 
	} 
		 
	//�������������
	double *dX=new double[3*kn]; //��֪�������������������
	for(i=0;i<kn;i++)
	{
		int k=KnownPoints[i];
		dX[3*i]  =XYZ[3*k]  -KnownXYZ[3*i];
		dX[3*i+1]=XYZ[3*k+1]-KnownXYZ[3*i+1];
		dX[3*i+2]=XYZ[3*k+2]-KnownXYZ[3*i+2];
	}

	for(i=0;i<3*kn;i++)
	{
		double dL=0.0;
		for(int j=0;j<3*kn;j++)
			dL+=KnownPx[ij(i,j)]*dX[j];
		int k=KnownPoints[i/3]; //���
		ATPL[3*k+i%3]+=dL;
	} 
	delete []dX;
	return;
}



//////////////////////////////////////////////////////////////////////
//   ������ƽ���
void CCoGPS::Free()
{
	//���۲�ֵ�ķ���Э����������棬��ΪȨ����
	for(int i=0; i<Anumber; i++)
	{
		int ni = 3*(dir0[i+1]-dir0[i]); //������۲�ֵ����
		inverse(P[i],ni);
	}
	
	CaATPA();     	 
	for( i=0; i<3*Pnumber; i++)
	{
		for(int j=0; j<=i;j++)
		{
			if(i%3==j%3) ATPA[ij(i,j)]+=1.0/Pnumber;
		}
	}
	
	double max=Ca_dX(true);     
	for( i=0; i<3*Pnumber; i++)
	{
		for(int j=0; j<=i;j++)
		{
			if(i%3==j%3) ATPA[ij(i,j)]-=1.0/Pnumber;
		}
	}
	
	CaLV(V);  //����в�
	
	double VTPV=CaVTPV();
	Sigma=sqrt(VTPV/(3*Vnumber-3*(Pnumber-1)));

	PrintXYZ();
	PrintLV("V",V);
	fprintf(resultfp,"\n\n������ƽ��:\n[pvv]=%e",VTPV);
	fprintf(resultfp,"\n��λȨ������=��%e",Sigma);	
	
}




//////////////////////////////////////////////////////////////////////
//  ��С����ƽ���
void CCoGPS::LeastSquares()
{
	//���۲�ֵ�ķ���Э����������棬��ΪȨ����
	for(int i=0; i<Anumber; i++)
	{
		int ni = 3*(dir0[i+1]-dir0[i]); //������۲�ֵ����
		inverse(P[i],ni);
	}
	
	CaATPA();     	 
	Known();
	double max=Ca_dX(true);     
	
	CaLV(V);  //������С���˲в�
	double VTPV=CaVTPV();

	if(m_IsPx)
	{
		double pvv=Ca_VTPV_XYZ()+VTPV;
		Sigma=sqrt(pvv/(3*Vnumber-3*unPnumber));
	}
	else
	{
		Sigma=sqrt(VTPV/(3*Vnumber-3*unPnumber));
	}	
	PrintXYZ();
	PrintLV("V",V);

	fprintf(resultfp,"\n\n��С����ƽ��:\n[pvv]=%e",VTPV);
	fprintf(resultfp,"\n��λȨ������=��%e",Sigma);	
			 
}

//////////////////////////////////////////////////////////////////////
//  ��С����ƽ���
void CCoGPS::Helmert()
{

	for(int kk=1;kk<=2;kk++)
	{
		CaATPA();     	 
		Known();
		double max=Ca_dX(1);     
		
		CaLV(V);  //������С���˲в�
		PrintLV("V",V);
		
		double VTPV=0.0;
		
		for(int i=0;i<Anumber;i++) // Anumber������������
		{
			double *Pi=P[i];     //i��Ȩ������ʼ��ַ
			int k1=3*dir0[i];    //i���������ڹ۲�ֵ�е����
			int k2=3*dir0[i+1];  //i+1���������ڹ۲�ֵ�е����
			
			double *PVi=PV+k1;
			double *Vi=V+k1;
			int n=k2-k1;
			
			double pvv=0.0;
			int d0 = 3*dir0[i];
			for(int j=0;j<n;j++)
			{
				double pvj=0.0;
				for(int k=0; k<n; k++) pvj+=Pi[ij(j,k)]*Vi[k];
				
				PVi[j]=pvj;
				pvv += pvj*Vi[j];
			}
			
			
			fprintf(resultfp,"\n[%d]=%lf  \n",i+1,pvv/n);

			double m2=pvv/n;
//			if(m2<0.1)m2=0.1;
			
			for(j=0; j<(n+1)*n/2; j++)Pi[j]/=m2;
			
			//if(kk==2)inverse(Pi,n);
			VTPV+=pvv;
		}

			
	}
//	PrintData();
//	return;

	double VTPV=CaVTPV();
	if(m_IsPx)
	{
		double pvv=Ca_VTPV_XYZ()+VTPV;
		Sigma=sqrt(pvv/(3*Vnumber-3*unPnumber));
		
	}
	else
	{
		Sigma=sqrt(VTPV/(3*Vnumber-3*unPnumber));
	}	
	PrintXYZ();
	PrintLV("V",V);
	
	fprintf(resultfp,"\n     ��󾫶ȹ���:\n");
	fprintf(resultfp,"\n     [pvv]:    %e",VTPV);
	fprintf(resultfp,"\n     m:        %e(���λȨ�����)",Sigma);
}




//////////////////////////////////////////////////////////////////////
// ����в�V��Ȩ����
void CCoGPS::caQV()
{
	//����۲�ֵƽ��ֵȨ����
	for(int i=0;i<3*Vnumber; i++)
    {
        int k1=3*dir1[i/3]+i%3;  //��վ��δ֪�����
        int k2=3*dir2[i/3]+i%3;  //��׼��δ֪�����
		
		double q=ATPA[ij(k1,k1)]+ATPA[ij(k2,k2)]-2.0*ATPA[ij(k1,k2)];

		QV[i] = QV[i]-q;
    }	
}



//////////////////////////////////////////////////////////////////////
//  ������λ����:   (1)���㵥λȨ�в�; (2)��λ�����㷽������
void CCoGPS::Ca_UintV()
{
	for(int i=0; i<3*Vnumber; i++)
	{
		double mi=sqrt(QV[i]);
		if(fabs(QV[i])<1.0e-15) W[i] = 1.0;
		else W[i]=V[i]/mi;  
	}

    MEDIAN=Median(W,3*Vnumber,true)*1.4826;
	
	//fprintf(resultfp,"\nMEDIAN=%10e\n",MEDIAN);
}






//////////////////////////////////////////////////////////////////////////
//     �������ƽ��ֵ���������
void CCoGPS::PrintXYZ()
{
	double m=Sigma;
	double Maxmx=0.0;
	double Maxmy=0.0;
	double Maxmz=0.0;

	fprintf(resultfp,"\n\n\n STATION       X              Y              Z          RMS_X  RMS_Y  RMS_Z");
	fprintf(resultfp,"\n              (m)            (m)            (m)          (cm)   (cm)   (cm)\n");
	fprintf(resultfp," ----------------------------------------------------------------------------\n");

	for(int k=0;k<Pnumber;k++)
	{
		fprintf(resultfp,"\n%4d %4s",k+1,Pname[k]);

		double X=XYZ[3*k];
		double Y=XYZ[3*k+1];
		double Z=XYZ[3*k+2];				 

		fprintf(resultfp," %14.4lf %14.4lf %14.4lf",X,Y,Z);

		double mx,my,mz;

		mx=sqrt(ATPA[ij(3*k,3*k)])*m*100.0;
		my=sqrt(ATPA[ij(3*k+1,3*k+1)])*m*100.0;
		mz=sqrt(ATPA[ij(3*k+2,3*k+2)])*m*100.0;

		fprintf(resultfp," %6.2lf %6.2lf %6.2lf",mx,my,mz);

		if(mx>Maxmx)Maxmx=mx;
		if(my>Maxmy)Maxmy=my;
		if(mz>Maxmz)Maxmz=mz;

	}
	fprintf(resultfp,"\n\n  Max(RMS_X)=%5.2lf",Maxmx);
	fprintf(resultfp,"\n  Max(RMS_Y)=%5.2lf",Maxmy);
	fprintf(resultfp,"\n  Max(RMS_Z)=%5.2lf",Maxmz);
}




//////////////////////////////////////////////////////////////////////////
//    ��֪��ӣ���أ�Ȩƽ��ʱ������������Ķ�����
double CCoGPS::Ca_VTPV_XYZ()
{
	 int kn=Pnumber-unPnumber; //��֪����
	 double *Vxyz=new double [3*kn];

	 for(int i=0;i<kn;i++)
	 {
		 int k=KnownPoints[i];  //��֪��ĵ��
		 double X=XYZ[3*k];
		 double Y=XYZ[3*k+1];
		 double Z=XYZ[3*k+2];				 

		 Vxyz[3*i]  =X-KnownXYZ[3*i];				 
		 Vxyz[3*i+1]=Y-KnownXYZ[3*i+1];				 
		 Vxyz[3*i+2]=Z-KnownXYZ[3*i+2];
	 }

	 double VTPV_XYZ=0.0;
	 for(i=0;i<3*kn;i++)
	 {
		 for(int j=0;j<3*kn;j++)
			 VTPV_XYZ+=Vxyz[i]*KnownPx[ij(i,j)]*Vxyz[j];
	 }

	 delete []Vxyz;
	 return VTPV_XYZ;
}




//////////////////////////////////////////////////////////////////////////
//	 �������������ص�����Ӧ�ĵ��
int  CCoGPS::GetStationNumber(char *buf)
{
    for(int i=0; i<Pnumber; i++) 
	{
		if(Pname[i]!=NULL)
		{
			//���Ѿ����������ַ����ĵ����Ƚ�
			if(strcmp(buf,Pname[i])==0)return i;
		}
		else
		{
			//���µ����浽�ڴ棬��ַ�ŵ�Pname������
			int len = strlen(buf);
			Pname[i] = new char[len+1];
			strcpy(Pname[i], buf);
			return i;
		}
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//   GPS���������
void CCoGPS::Robust(int fname,double eps)
{
	int n=3*Vnumber;    //�۲�ֵ����
	QV=new double [n];  //�в��Ȩ����
	W =new double [n];  //Ȩ����
	
	//------------------------------------
	//  ����۲�ֵ��Ȩ����
	int ii=0;  // ������
	for(int i=0; i<Anumber; i++)
	{
		int ni = 3*(dir0[i+1]-dir0[i]); //������۲�ֵ����
		for(int j=0;j<ni;j++)
		{
			QV[ii]=P[i][ij(j,j)];
			ii++;
		}
		inverse(P[i],ni);
	}
	//--------------------------------------
	//  ����в��Ȩ����
	CaATPA();   //��ɷ�����
	if(!inverse(ATPA,3*Pnumber))  //������ϵ����������
	{
		MyBreak("������ϵ�����Ƚ��ȣ�");
		exit(0);
	}
	caQV();  //����в��Ȩ����
	
	//--------------------------------------
	//  ���۲�ֵ��Ȩ���ݵ�����PP	
	double **PP= new double *[Anumber]; //�ȼ�Ȩ����
	for(int k=0; k<Anumber; k++)
	{
		int nk=3*(dir0[k+1]-dir0[k]);
		PP[k]=new double[nk*(nk+1)/2];
		for(int i=0;i<nk*(nk+1)/2;i++) PP[k][i]=P[k][i];
	}
	
	//---------------------------------------
	// ������Ƶ���
	for( i=0;i<n;i++) W[i]=1.0;//Ȩ���Ӹ���ֵ
	int No=0; //��������
	while(1)
	{
		No++;
		// ����ȼ�Ȩ
		for(int k=0; k<Anumber; k++) // k����������
		{
			int nk = 3*(dir0[k+1]-dir0[k]); //�۲�ֵ����
			double *Wk=W+3*dir0[k];         //Ȩ���ӵ������ַ
			
			int index = 0;
			for(int i=0;i<nk;i++)
			{
				for(int j=0;j<=i;j++)
				{
					if(Wk[i]<1.0 || Wk[j]<1.0)
						P[k][index] = PP[k][index]*sqrt(Wk[i]*Wk[j]);
					else P[k][index] = PP[k][index];
					index++;
				}
			}
		}

		CaATPA(); //��ɷ�����
		Known();  // ��֪�㴦��
		double maxX=Ca_dX(0);     
		CaLV(V);  //����в�

		//fprintf(resultfp,"\n������ƣ���%d�ε���...\nmax(dX)=%e",No,maxX);
		// 		PrintLV("������Ʋв�",V);
		
		if(fabs(maxX)<eps && No>1)
		{
			break;
		}

		//-----------------------------------------------
		//Ȩ���Ӽ���
		Ca_UintV();  //��׼���в����(����W��)
		for(int i=0;i<3*Vnumber;i++)
		{
			W[i]=Wi(fname,W[i]/MEDIAN,5.0 , 8.0);
		}
	}

	int dd=0; //��Ȩ�۲�ֵ�ĸ���
	for(i=0;i<3*Vnumber;i++)
	{
		if(W[i]<1.0)dd++;
	}
	
	if(dd==0)
	{
		fprintf(resultfp,"\n  ���й۲�ֵ��δ��Ȩ!\n");	
	}
	else
	{   //�����Ȩ����
		fprintf(resultfp,"\n  ����%d���۲�ֵ����Ȩ!\n",dd);	
		fprintf(resultfp,"\n\nȨ����: \n");
		for(int i=0;i<3*Vnumber;i++)
		{
			if(W[i]<1.0)
			{
				int ni=AreaNumber[i/3];
				fprintf(resultfp,"\n%8d",ni+1);
				fprintf(resultfp,"%10s %10s",
					Pname[dir1[i/3]],Pname[dir2[i/3]]);
				fprintf(resultfp,"   d%c %10.4lf",'x'+i%3,W[i]);
			}
		}  
	}
				
	double VTPV=CaVTPV();
	Sigma=sqrt(VTPV/(n-dd-3*unPnumber));
	PrintXYZ();
	PrintLV("V",V);

	fprintf(resultfp,"\n\n�������:\n[pvv]=%e",VTPV);
	fprintf(resultfp,"\n��λȨ������=��%e",Sigma);	
	
	delete []PP;
	//PrintM(resultfp,W,3*Vnumber,3,"%6.3lf ","����Ȩ����");	
}

//////////////////////////////////////////////////////////////////////////
//     ����ƽ��
void CCoGPS::Quasi_Stable(char *file)
{
	//���۲�ֵ�ķ���Э����������棬��ΪȨ����
	for(int i=0; i<Anumber; i++)
	{
		int ni = 3*(dir0[i+1]-dir0[i]); //������۲�ֵ����
		inverse(P[i],ni);
	}
	
	CaATPA();
	
	FILE *fp=fopen(file,"r");
	if(fp==NULL) 
	{
		MyBreak("���ȵ���Ϣ�ļ��򲻿���");
		exit(0);
	}
	
	bool *IsStable = new bool[Pnumber]; //�Ƿ�Ϊ���ȵ�
	for( i=0;i<Pnumber;i++)
	{
		IsStable[i]=false;
	}
	int StableNumber; //���ȵ�����
	fscanf(fp,"%d",	&StableNumber);
	for(i=0;i<StableNumber;i++)
	{
		char buf[50];
		fscanf(fp,"%s",buf);
		int k=GetStationNumber(buf);
		if(k<0)
		{
			MyBreak("���ȵ����ļ��е�������"); 
			exit(0);
		}
		IsStable[k]=true;
	}
	fclose(fp);

	double f=1.0/StableNumber;
	for(i=0; i<3*Pnumber; i++)
	{
		if(!IsStable[i/3]) continue;//��ʾ���ȵ㣬����ȥ
		for(int j=0; j<=i;j++)
		{
			if(i%3==j%3 && IsStable[j/3])
				ATPA[ij(i,j)]+=f;
		}
	}
	
	delete []IsStable;
	Ca_dX(true);
	
	for( i=0; i<3*Pnumber; i++)
	{
		for(int j=0; j<=i;j++)
		{
			if(i%3==j%3) ATPA[ij(i,j)]-=f;
		}
	}
	
	CaLV(V);
	double VTPV=CaVTPV();
	Sigma = sqrt(VTPV/(3*Vnumber-3*(Pnumber-1)));
	
	PrintXYZ();
	fprintf(resultfp,"\n\n����ƽ��:\n[pvv]=%e",VTPV);
	fprintf(resultfp,"\n��λȨ������=��%e",Sigma);	
}

//////////////////////////////////////////////////////////////////////////
//  �Գ�������������(����������Ԫ��)
bool inverse2(double a[],int n)
{
    double *Y=new double[n];
    for(int k=0;k<n;k++)
	{ 
		double a0=a[0];
		if(a0+1.0==1.0)
		{
			delete []Y;  return false;
		}
		int m=n-k-1;
		for(int i=1;i<n;i++)
		{
			double g=a[i*(i+1)/2];
			Y[i]=g/a0;
			if(i<=m)Y[i]=-Y[i];
			for(int j=1;j<=i;j++)
			{
				a[(i-1)*i/2+j-1]=a[i*(i+1)/2+j]+g*Y[j];
			} 
		}
		a[n*(n+1)/2-1]=1.0/a0;
		for(i=1;i<n;i++)
			a[(n-1)*n/2+i-1]=Y[i];
	} 
	delete []Y;
	return true;
}


///////////////////////////////////////////////////////////////
// ����ֲ��ֵ��Ȩ����Qg,�ֲ��ֵgross,���شֲ������VTPVg
double CCoGPS::CaQg(
			int    s,      //�ֲ�����ĸ���
			int    Jint[], //���˴ֲ�����Ĺ۲�ֵ��ţ�����
			double gross[],//�ֲ�����
			double Qg[],   //�ֲ�Ȩ����
			double JTPA[],  //PA���Ӿ���
			double JTPV[] ) //V��������

{
     CaJTPJ( Qg, Jint, s);
     CaJTPA(JTPA,Jint[s-1],s);	 

	 // �ֲ��ֵ��Ȩ�������
     int t=3*Pnumber;   //��������
	 for(int j=0;j<s; j++) 
	 { 
		 for(int i=0; i<=j; i++)
		 {
			 for(int k=0; k<t; k++)
			 {
				 double a=JTPA[j*t+k];
				 if(a+1.0==1.0)continue;
				 for(int l=0;l<t;l++)
				 {
					 Qg[ij(j,i)]-=a*ATPA[ij(k,l)]*JTPA[i*t+l];
				 }
			 }
		 }
	 }  
	 if(!inverse2(Qg,s))return -2.0;
	 
	 // ����ֲ��ֵ
	 JTPV[s-1]=PV[Jint[s-1]];
	 for(int i=0; i<=s-1; i++)
     {
		 gross[i]=0.0;
		 for(int j=0;j<=s-1;j++)
			 gross[i]-=Qg[ij(i,j)]*JTPV[j];
	 }

     // ����ֲ������
	 double VTPVg=0.0;
     for(i=0; i<=s-1; i++)
	 {
		 VTPVg-=JTPV[i]*gross[i];
	 }
     return VTPVg;
}

#include "Probability.h"

//////////////////////////////////////////////////////////////////////////
//    �ֲ�̽�⺯��      2008-08-29
void CCoGPS::DataSnooping(double Arfa,int Max)  // Max-�����������
{
     //  Ϊ���������ڴ�
	 double *JTPA=new double [3*Pnumber*Max];
     double *JTPV=new double [Max];
     double *Qg= new double [Max*(Max+1)/2]; // �ֲ�Ȩ����
     double *gross= new double [Max];  // �ֲ��ֵ
     int    *Jint= new int [Max];  // ���ɹ۲�ֵ���

     CProbability MyProb; //���ʼ�����
	 
	 //-------------------------------------------------
	 //  ��С����ƽ��
	 for(int i=0; i<Anumber; i++)
	 {
		 int ni = 3*(dir0[i+1]-dir0[i]); //������۲�ֵ����
		 inverse(P[i],ni);
	 }
	 
	 CaATPA();    
	 Known();	 
	 Ca_dX(false);     
	 CaLV(V);  //������С���˲в�
	 double VTPV=CaVTPV();

	 //---------------------------------------------------
	 //�ֲ�̽��
     fprintf(resultfp,"\n           ����������ֲ�̽��:\n");

     int maxp;  //��ǰ�����У�ͳ�������Ĺ۲�ֵ���
     int Gn=0;  //��ǰ��������
	 double  new_Omg;      // �����������ֲ������;
     double  last_Omg=0.0; // ǰ���������ֲ������
  	 int r=3*Vnumber-3*unPnumber; // ����۲���		 
	 
	 bool *W=new bool[3*Vnumber];  //�ֲ��־����
	 for( i=0; i<3*Vnumber; i++)W[i]=0;
			 
     while(1)
     {
        if(Gn+1>Max)
		{
			MyBreak("����������������С���������������������¿�ʼ������");
			fclose(resultfp);
			exit(0);
		}

  	    fprintf(resultfp,"\n��%d������:",Gn+1);
		
        double Flimit=MyProb.re_F(1,r-(Gn+1),1.0-Arfa);

        new_Omg=0.0; 
        for(int i=0; i<3*Vnumber; i++)
		{
			 Jint[Gn]=i;

             if(W[i]) continue;	// ���Ź۲�ֵ�Ƿ�Ϊ��֪�ֲ�۲�ֵ

			 double Omg = CaQg(Gn+1,Jint,gross,Qg,JTPA,JTPV); //�ֲ������
             if(Omg<0.0)continue;

             if(Omg>new_Omg && Omg<VTPV)
			 {
				 new_Omg=Omg; 
				 maxp=i;  
			 }
		}

        double F=(new_Omg-last_Omg)/(VTPV-new_Omg)*(r-(Gn+1));
		   
		if(F>Flimit)fprintf(resultfp,"\n   F=%6.3lf(>%6.3lf)",F,Flimit);
		else	    fprintf(resultfp,"\n   F=%6.3lf(<%6.3lf)",F,Flimit);

		Jint[Gn]=maxp;

		double Omg=CaQg(Gn+1,Jint,gross,Qg,JTPA,JTPV);
 
		for(i=0;i<Gn+1;i++)
		{
			int j=Jint[i];
			fprintf(resultfp,"\n   No.%-2d",j);
			fprintf(resultfp,"  ������:%-3d ",AreaNumber[j/3]+1); 
			fprintf(resultfp,"%5s ",Pname[dir1[j/3]]);
			fprintf(resultfp,"%5s ",Pname[dir2[j/3]]);
			fprintf(resultfp," (%c)",'X'+j%3);
			fprintf(resultfp,"%12.4lf",gross[i]);
		}

        if( F<=Flimit || fabs(gross[Gn])<0.01)break;
		Gn++;
        last_Omg=new_Omg;
		W[maxp]=1;
    }

    if(Gn==0)
    {
	      fprintf(resultfp,"\n\n�ֲ�̽������δ���ֲִ�");
    }
	else
	{
		 for(int j=1;j<=Gn;j++)
		 {
			 new_Omg=CaQg(j,Jint,gross,Qg,JTPA,JTPV);
		 }
		 fprintf(resultfp,"\n\n ������������λ:(�ֲ����)\n");
		 fprintf(resultfp," �ֲ�����: %d\n",Gn);
		 
		 for(int i=0;i<Gn;i++)
		 {
			 j=Jint[i];
			fprintf(resultfp,"\n   No.%-2d",j);
			fprintf(resultfp,"  ������:%-3d ",AreaNumber[j/3]+1); 
			fprintf(resultfp,"%5s ",Pname[dir1[j/3]]);
			fprintf(resultfp,"%5s ",Pname[dir2[j/3]]);
			fprintf(resultfp," (%c)",'X'+j%3);
			fprintf(resultfp,"%12.4lf",gross[i]);
		 }
		 
		 fprintf(resultfp,"\n\n�޳��ֲ����С����ƽ����");
		 VTPV=VTPV-new_Omg;
		 Sigma=sqrt(VTPV/(r-Gn));
		 Ca_X_Qx(gross,Qg,JTPA,Gn);
		 
		 PrintXYZ();
		 CaLV(V);//����в�
		 PrintLV("V",V);	  
		 fprintf(resultfp,"\n\n�ֲ�̽��:\n[pvv]=%e",VTPV);
		 fprintf(resultfp,"\n��λȨ������=��%e",Sigma);	
		 
	}


	delete []JTPA;
	delete []JTPV;
	delete []Qg;
	delete []gross;
	delete []Jint;
	delete []W;
}


//////////////////////////////////////////////////////////////////////
//        ����JTPA�����һ��(n0-1)
//         JTPA��n0��t�еľ���
void CCoGPS::CaJTPA(double JTPA[],int ks,int s) 
//s-���ɹ۲�ֵ���� ;  ks-���һ�����ɹ۲�ֵ�ı��
{
     int t=3*Pnumber;
     for(int i=0;i<t;i++)JTPA[(s-1)*t+i]=0.0;

	 int k =AreaNumber[ks/3];  // ��ks�Ź۲�ֵ�����
     int k1=dir0[k];           // k�������������
     int k2=dir0[k+1];         // k+1���������������
	 int nk=3*(k2-k1);

	 int s2=ks-3*k1; // ���ڹ۲�ֵ���
	 int *Dir1=dir1+k1;
	 int *Dir2=dir2+k1;
	 
	 for(int s1=0;s1<nk;s1++)
     {
         int i1=3*Dir1[s1/3]+s1%3; 
         int i2=3*Dir2[s1/3]+s1%3; 
		 
		 double p12 = P[k][ij(s1,s2)];
		 		
		 JTPA[(s-1)*t+i1]-=p12;
         JTPA[(s-1)*t+i2]+=p12;
     }

	 /*   
     int num=AreaNumber[ks/3];  // ��ks�Ź۲�ֵ�����
     int k1=dir0[num];         //  num�������������
     int k2=dir0[num+1];       //  num+1�������������


     int j=ks-3*k1;
     for(int k=3*k1;k<3*k2;k++)
     {
         int i=k-3*k1;
         int ij=(j<=i) ? i*(i+1)/2+j : j*(j+1)/2+i;
         double pij = P[num][ij];

         int i1=3*dir1[k/3]+k%3;
         int i2=3*dir2[k/3]+k%3;
         JTPA[(s-1)*t+i1]-=pij;
         JTPA[(s-1)*t+i2]+=pij;
     }

*/

}


//////////////////////////////////////////////////////////////////////
//	����JTPJ��s-�ֲ�����ĸ���
void CCoGPS::CaJTPJ(double JTPJ[], int Jint[],int s)
{
	for(int k=0; k<s; k++)
	{
		int k1=Jint[k]; //�۲�ֵ���
		int A1=AreaNumber[k1/3]; //�۲�ֵ����ͬ�������
		double *Pk = P[A1];      //��ͬ����Ȩ��������
		int k0 = 3*dir0[A1];     //��ͬ�����׹۲�ֵ�ı��    			

		for(int i=0;i<=k;i++)
		{ 
			int k2 = Jint[i]; //�۲�ֵ���
			if(AreaNumber[k2/3] == A1) //k1,k2����ͬһͬ����
			{
				JTPJ[ij(k,i)]=Pk[ij(k1-k0,k2-k0)];
			}
			else JTPJ[ij(k,i)]=0.0;
		}	  
	}
}


//////////////////////////////////////////////////////////////////////////
//   �����޳�s���ֲ�������ƽ��ֵ��Ȩ����
void CCoGPS::Ca_X_Qx(double gross[],double Qg[],double JTPA[],int s)
// gross[] -�ֲ�����
// Qg[] - �ֲ��ֵȨ����
// J'PA 
// s      �ֲ����
{
	int t=3*Pnumber;

	// M = inverse(N) * A'PJ
	double *M=new double[t*s];
	for(int i=0;i<t*s;i++) M[i]=0.0;
	for(i=0;i<t;i++)
	{
		for(int j=0;j<s;j++)
		{
			for(int k=0; k<t; k++)
				M[i*s+j]+=ATPA[ij(i,k)]*JTPA[j*t+k];
		}
	}

	for(i=0;i<t;i++)
	{
		double xi=0.0;
		for(int j=0;j<s;j++)
			xi+=M[i*s+j]*gross[j];
		XYZ[i]-=xi;
	}

	for(i=0;i<t; i++)
	{
		for(int j=0;j<=i; j++)
		{
			double nij=0.0;
			for(int k=0; k<s; k++)
				for(int h=0; h<s; h++)
					nij+=M[i*s+k]*Qg[ij(k,h)]*M[j*s+h];
			ATPA[ij(i,j)]+=nij;
		}
	}
	delete []M;
	return;
}




//////////////////////////////////////////////////////////////////////////
//   ������֪������ꡢ���Э�������(��֪�����)�������(��֪�����)
bool CCoGPS::InputKnownXYZ(char *file,bool correlative)
{
	m_IsPx = true; //������Ȩƽ��

    FILE *fp=fopen(file,"r");  
	if(fp==NULL )
	{
		MyBreak("��֪�����ļ��򲻿�");
		return 0;
	}
	
	int kn;
	fscanf(fp,"%d",&kn);   //��֪������
	unPnumber=Pnumber-kn;
	
	int tt=(3*kn)*(3*kn+1)/2;   //Ȩ����Ԫ�صĸ���
	KnownPoints=new int [kn];   //��֪����
	KnownXYZ=new double [3*kn]; //��֪������
	KnownPx=new double [tt];    //��֪�������Ȩ����
	for(int i=0;i<tt;i++) KnownPx[i]=0.0;
	for(i=0;i<Pnumber;i++)IsKnown[i]=false; //��ʼֵ��δ֪��

	if(correlative)//��֪��������ص�
	{
		for(i=0;i<kn;i++)
		{
			char name[20];
			fscanf(fp,"%s%lf%lf%lf",name,
				KnownXYZ+3*i,KnownXYZ+3*i+1,KnownXYZ+3*i+2); 
			int k=GetStationNumber(name);
			if(k<0)
			{
				MyBreak("��ȡ��֪�����ļ�������������");  
				return 0;
			}
			
			KnownPoints[i]=k; //���
			IsKnown[k]=true;
		} 
		
		int jj=0;
		for(i=0;i<3*kn;i++)
		{
			int d;
			fscanf(fp,"%d",&d);   // ��ȡ�к�
			for(int j=0;j<=i;j++)
			{ 
				fscanf(fp,"%lf",KnownPx+jj); //����Э����
				jj++;
			}
		} 
		
		fprintf(resultfp,"\n ��֪������:\n");
		for(i=0;i<kn;i++)
		{
			int k=KnownPoints[i];
			fprintf(resultfp,"\n%10s",Pname[k]);
			fprintf(resultfp," %15.5lf %14.5lf %14.5lf",
				KnownXYZ[3*i],KnownXYZ[3*i+1],KnownXYZ[3*i+2]);
		}
		PrintM2(resultfp,KnownPx,3*kn,4,"%18.10e ","��֪������ķ�����");

		bool rn=inverse(KnownPx,3*kn); //����Э����������棬��Ȩ����
		if(!rn) 
		{
			MyBreak("������֪�����ļ��������귽������");
			return 0;
		}
	}
	else //��֪�����Ƕ�����
	{
		for(i=0;i<kn;i++)
		{
			char name[20];
			fscanf(fp,"%s%lf%lf%lf",name,
				KnownXYZ+3*i,KnownXYZ+3*i+1,KnownXYZ+3*i+2); 
			
			int k=GetStationNumber(name);
			if(k<0)
			{
				MyBreak("��ȡ��֪�����ļ�������������");    
				return 0;
			}
			
			KnownPoints[i]=k;
			IsKnown[k]=true;
						
			double mx,my,mz;
			fscanf(fp,"%lf%lf%lf",&mx,&my,&mz);
			KnownPx[ij(3*i,3*i)] = 1.0/(mx*mx);
			KnownPx[ij(3*i+1,3*i+1)] = 1.0/(my*my);
			KnownPx[ij(3*i+2,3*i+2)] = 1.0/(mz*mz);
		} 
		fprintf(resultfp,"\n ��֪�����꼰�������:\n");
		for(i=0;i<kn;i++)
		{
			int k=KnownPoints[i];
			fprintf(resultfp,"\n%10s",Pname[k]);
			fprintf(resultfp," %15.5lf %14.5lf %14.5lf",
				KnownXYZ[3*i],KnownXYZ[3*i+1],KnownXYZ[3*i+2]);

			double mx=KnownPx[ij(3*i,3*i)];
			double my=KnownPx[ij(3*i+1,3*i+1)];
			double mz=KnownPx[ij(3*i+2,3*i+2)];
			fprintf(resultfp," %6.4lf %6.4lf %6.4lf",
							sqrt(1.0/mx),sqrt(1.0/my),sqrt(1.0/mz));
		}
	}
	fclose(fp);	
	unPnumber=Pnumber-kn;
	return 1;
}

