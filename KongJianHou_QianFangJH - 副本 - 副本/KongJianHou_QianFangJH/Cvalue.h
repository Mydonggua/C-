#pragma once
#include "Matrix.h"

struct PointPHOTO
{
	double X;           //��ά����
	double Y;
};

struct Point
{
	double X;           //��ά����
	double Y;
	double Z;
};

//�ⷽλԪ��
struct WFWYS
{
	double XS, YS, ZS, angle_t, angle_w, angle_k;
};

//����
struct JD
{
	double jdXS, jdYS, jdZS, jdangle_t, jdangle_w, jdangle_k;
};


class Cvalue
{
	int knowPHOTOnum, unknowPointnum; //��֪���Ƶ�������δ֪���������
	PointPHOTO *knowPHOTO1;
	PointPHOTO *knowPHOTO2;
	PointPHOTO *unknowPoint_PHO1;
	PointPHOTO *unknowPoint_PHO2;
	Point *knowGROUND;
	Point *unknowGROUND;
	double f, m;
	double BX, BY, BZ;  //��Ӱ���߷���
	//����
	WFWYS left; 
	WFWYS right;
	JD leftJD;
	JD rightJD;
	double a01, a02;

public:
	Cvalue();
	~Cvalue();

public:
	void getValue(CString &strinput);  //��ȡ����
	void calculateFWYS(PointPHOTO *knowPHOTO, Point *knowGROUND, WFWYS &wfwys, JD &temJD, double &a0);//�����ⷽλԪ��
	void GROUNDunpointCal(WFWYS wfwys1, WFWYS wfwys2, PointPHOTO *unknowPoint_PHO1, PointPHOTO *unknowPoint_PHO2); //������������
	void outputValue(CString &strout);  //������
public:
	int SpliteStringArray(CString str, char split, CStringArray & astr);//�ָ��
};

