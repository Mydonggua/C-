#pragma once
//��
class Point
{
public:
	CString pointname;  //����
	double X;           //����
	double Y;
public:
	Point() {};
	~Point() {};
	void setpoint(CString str, double dx, double dy);
};
//�Ƕ�
class Angle
{
public:
	Point *startID, *midID, *endID;//��һ�㣬���ڵ㣬��һ�㣺��
	double angle;   //ֵ
public:
	Angle() {};
	~Angle() {};
	void setangle(Point *Pstart, Point *Pmid, Point *Pend,double dangle);
};
//����
class Distant
{
public:
	Point *startID, *endID;    //��ʼ�㣬�յ�
	double distant;  //ֵ
public:
	~Distant() {};
	Distant() {};
	void setdistant(Point *Pstart, Point *Pend, double dlength);
};


class Value
{
private:  //�ļ��������������
	int knowpointnum;   //��֪�����
	int unknowpointnum; //δ֪�����
	int angleObsnum;    //�Ƕȹ۲�ֵ����
	int Obsdistantnum;  //�۲�������
	int type;           //���߹۲⽻����
	Point *knowpoint;   //��֪��
	Point *unknowpoint; //δ֪��
	Angle *angleObs;    // �Ƕȹ۲�ֵ
	Distant *distantObs;//�۲����

public:  //�������������
	Value();
	~Value();

public:  //�����ݽ��в����ĺ���
	void getvalue(CString filename); //���ļ�����ȡ�ļ��е���Ϣ
	void falseUnKnowPoint();         //���������ʼ��λ��Ϊ90�ȵ�����
	void Fw_angleError();            //����ǶȲ�
	void getR_T_value();             //����R,T 
	void truePoint();                //���������۲��Ľ�������

	void resultput();                //������

private:  //�²����ı�������������
	Angle *FW_angle;         //��λ��
	Point *falseUnknowpoint; //����δ֪��
	Point falseknowpoint;    //�����
	Point falseknowpoint2;   //����C��
	
	double angleError;  //�ǶȲ�
	double R, T;        //R�պϱ߳��ȱ�,Tȫ����Ապϲ�

public:  //�������㺯��
	int SpliteStringArray(CString str, char split, CStringArray& astr); //�ָ��ַ���

	Point * SearchKnownPointUsingID(CString strID);  //������֪����

	Point * SearchUnKnownPointUsingID(CString strID); //����δ֪����

	Point * SearchPointUsingID(CString strID);  //�������е���

	double Dms2Rad(double DMS);  //�ȷ���ת������

	double FangWeiAngle(double DMS, double RadfrontFW_Angle); //���㷽λ��

};



