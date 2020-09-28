#include "stdafx.h"
#include "Value.h"
#include <locale.h>
#include<cmath>
const double EPSILON = 1.0E-12;  //��Сֵ
const double PI = 4.0*atan(1.0);  //PIԲ����


Value::Value()
{
}

//�ͷ��ڴ�
Value::~Value()
{
	if (knowpoint)delete[]knowpoint;
	if (unknowpoint)delete[]unknowpoint;
	if (angleObs)delete[]angleObs;
	if (distantObs)delete[]distantObs;

	if (FW_angle)delete[]FW_angle;
	if (falseUnknowpoint)delete[]falseUnknowpoint;
}

//���ļ�����ȡ�ļ��е���Ϣ
void Value::getvalue(CString filename)
{
	setlocale(LC_ALL, "");                              //������һ��,�������Ի���
	//���ļ�
	CStdioFile rfile;                                   //�����ļ�
	if (!rfile.Open(filename, CFile::modeRead)) return; //���ļ�

	CString strLine;                          //����
	BOOL bEOF = rfile.ReadString(strLine);    //��ȡ�ļ���һ��
	 bEOF = rfile.ReadString(strLine);        //��ȡ�ļ��ڶ���
	knowpointnum = _ttoi(strLine);            //��ȡ��֪�����
	knowpoint = new Point[knowpointnum];      //Ϊ��֪�㿪�ٿռ�
	CStringArray strtemp;                     //�������洢ÿ�зָ�Ľ��
	//��ȡ��֪��
	for (int i = 0; i < knowpointnum; i++)
	{
		bEOF = rfile.ReadString(strLine);               //��ȡ�ļ�
		int n = SpliteStringArray(strLine,',',strtemp); //�ָ��ȡ����һ��
		//����
		double dx = _tstof(strtemp[1]);          
		double dy = _tstof(strtemp[2]); 
		knowpoint[i].setpoint(strtemp[0], dx, dy);      //��ȡ����������
	}

	//��ȡδ֪��
	bEOF = rfile.ReadString(strLine);
	bEOF = rfile.ReadString(strLine);        //��ȡδ֪�����
	unknowpointnum = _ttoi(strLine);         //δ֪�����
	unknowpoint = new Point[unknowpointnum]; //Ϊδ֪�㿪�ٿռ�
	bEOF = rfile.ReadString(strLine);        //��ȡδ֪������������
	int n = SpliteStringArray(strLine, ',', strtemp); //�ָ�
	//δ֪�������
	for (int i = 0; i < unknowpointnum; i++)
	{
		//��ȡδ֪��
		unknowpoint[i].setpoint(strtemp[i], 0, 0);
	}

	//��ȡ�۲�Ƕ�
	bEOF = rfile.ReadString(strLine);
	bEOF = rfile.ReadString(strLine);   //��ȡ�Ƕȹ۲�ֵ���1Ϊ��۲⣬2Ϊ�ҹ۲⣩
	type = _ttoi(strLine);              //��ȡ�۲�Ƕ�����
	bEOF = rfile.ReadString(strLine);   //��ȡ�Ƕȹ۲�ֵ������
	angleObsnum = _ttoi(strLine);       //��ȡ�۲�Ƕȸ���
	angleObs = new Angle[angleObsnum];  //Ϊ�Ƕȹ۲�ֵ���ٿռ�
	//���л�ȡ�Ƕȹ۲�ֵ
	for (int i = 0; i < angleObsnum; i++)
	{
		bEOF = rfile.ReadString(strLine);
		int n = SpliteStringArray(strLine, ',', strtemp);//�ָ�
		Point *Pstart = SearchPointUsingID(strtemp[0]);  //��һ����
		Point *Pmid = SearchPointUsingID(strtemp[1]);    //�Ƕȹ۲�ֵ��Ӧ����
		Point *Pend = SearchPointUsingID(strtemp[2]);    //��һ����
		double dangle = _tstof(strtemp[3]);              //�۲�ֵ
		angleObs[i].setangle(Pstart, Pmid, Pend, dangle);//��ȡ�۲�Ƕ���Ϣ
	}

	//�۲����
	bEOF = rfile.ReadString(strLine);
	bEOF = rfile.ReadString(strLine);        //��ȡ�۲�������
	Obsdistantnum = _ttoi(strLine);          //��ȡ�۲�������
	distantObs = new Distant[Obsdistantnum]; //Ϊ�۲���뿪�ٿռ�
	//���л�ȡ����۲�ֵ
	for (int i = 0; i < Obsdistantnum; i++)
	{
		bEOF = rfile.ReadString(strLine);
		int n = SpliteStringArray(strLine, ',', strtemp);  //�ָ�
		Point *Pstart = SearchPointUsingID(strtemp[0]);   //���
		Point *Pend = SearchPointUsingID(strtemp[1]);     //�յ�
		double dlength = _tstof(strtemp[2]);              //�۲�ֵ
		distantObs[i].setdistant(Pstart, Pend, dlength);  //��ȡ�۲������Ϣ
	}

	rfile.Close();
}

//
void Value::falseUnKnowPoint()
{
	//����·�ߵĵ�һ��δ֪��
	double RadFW_AnglePointC = PI / 2.0;   //������ʼ��λ��Ϊ90��
	unknowpoint[0].X = knowpoint[0].X + distantObs[0].distant*cos(RadFW_AnglePointC);//����·�ߵĵ�һ��δ֪������
	unknowpoint[0].Y = knowpoint[0].Y + distantObs[0].distant*sin(RadFW_AnglePointC);

	FW_angle = new Angle[angleObsnum];  //Ϊ���跽λ�ǿ��ٿռ�
	
	//�ڶ���δ֪��
	
	//����·�ߵĵ�һ��δ֪���Ӧ�ķ�λ��
	FW_angle[0].startID = SearchPointUsingID(unknowpoint[0].pointname);     //����Ͷ�Ӧ���һ������Ϊû����������㣬��Ӱ��
	FW_angle[0].midID = SearchPointUsingID(unknowpoint[0].pointname);       //��Ӧ���
	FW_angle[0].endID = SearchPointUsingID(unknowpoint[1].pointname);       //��һ���
	FW_angle[0].angle = FangWeiAngle(angleObs[0].angle, RadFW_AnglePointC); //ֵ
	//����·�ߵĵڶ���δ֪������
	unknowpoint[1].X = unknowpoint[0].X + distantObs[1].distant*cos(FW_angle[0].angle);
	unknowpoint[1].Y = unknowpoint[0].Y + distantObs[1].distant*sin(FW_angle[0].angle);

	//����·�ߵĵ������㵽���һ��δ֪��
	for (int i = 1; i < angleObsnum - 1; i++)
	{
		//����·�ߵĵڣ�i+1����δ֪���Ӧ�ķ�λ��
		FW_angle[i].startID = SearchPointUsingID(unknowpoint[i].pointname);//����Ͷ�Ӧ���һ������Ϊû����������㣬��Ӱ��
		FW_angle[i].midID = SearchPointUsingID(unknowpoint[i].pointname);
		FW_angle[i].endID = SearchPointUsingID(unknowpoint[i+1].pointname);
		FW_angle[i].angle = FangWeiAngle(angleObs[i].angle, FW_angle[i-1].angle);
		//����·�ߵĵ�(i+2)��δ֪���Ӧ�ķ�λ��
		unknowpoint[i+1].X = unknowpoint[i].X + distantObs[i+1].distant*cos(FW_angle[i].angle);
		unknowpoint[i+1].Y = unknowpoint[i].Y + distantObs[i+1].distant*sin(FW_angle[i].angle);
	}

	//����·�� ���һ���Ƕȹ۲�ֵ��Ӧ�ķ�λ��
	FW_angle[angleObsnum - 1].startID = SearchPointUsingID(unknowpoint[angleObsnum - 1].pointname);//����Ͷ�Ӧ���һ������Ϊû����������㣬��Ӱ��
	FW_angle[angleObsnum - 1].midID = SearchPointUsingID(unknowpoint[angleObsnum - 1].pointname);
	FW_angle[angleObsnum - 1].endID = SearchPointUsingID(knowpoint[1].pointname);
	FW_angle[angleObsnum - 1].angle = FangWeiAngle(angleObs[angleObsnum - 1].angle, FW_angle[angleObsnum - 2].angle);
	//�����C������
	falseknowpoint.X = unknowpoint[angleObsnum - 1].X +
		distantObs[Obsdistantnum - 1].distant*cos(FW_angle[angleObsnum - 1].angle);
	falseknowpoint.Y = unknowpoint[angleObsnum - 1].Y +
		distantObs[Obsdistantnum - 1].distant*sin(FW_angle[angleObsnum - 1].angle);

}

//���㷽λ�ǲ�
void Value::Fw_angleError()
{
	double temp1 = atan2((knowpoint[1].Y - knowpoint[0].Y) ,(knowpoint[1].X - knowpoint[0].X));    //BC��λ��
	double temp2 = atan2((falseknowpoint.Y - knowpoint[0].Y), (falseknowpoint.X - knowpoint[0].X)); //BC'��λ��
	if (temp1 < 0)temp1 += 2 * PI;
	angleError = temp1 - temp2;//��λ�ǲ�
}

//����R,T
void Value::getR_T_value()
{
	//ԭ·��BC�����
	double distantBC = sqrt((knowpoint[1].X - knowpoint[0].X)*(knowpoint[1].X - knowpoint[0].X) 
		+ (knowpoint[1].Y - knowpoint[0].Y)*(knowpoint[1].Y - knowpoint[0].Y));
	//����·��BC�����
	double False_distantBC = sqrt((falseknowpoint.X - knowpoint[0].X)*(falseknowpoint.X - knowpoint[0].X) 
		+ (falseknowpoint.Y - knowpoint[0].Y)*(falseknowpoint.Y - knowpoint[0].Y));
	R = distantBC / False_distantBC;  //����R���պϱ߳��ȱ�
	T = abs((distantBC - False_distantBC) / distantBC);  //����T��ȫ����Ապϲ�
}

//����ÿ�����ԭ·�߽�������
void Value::truePoint()
{
	falseUnknowpoint = new Point[unknowpointnum];
	//����ԭ·�ߵ�һ��δ֪��Ľ�������
	falseUnknowpoint[0].X = knowpoint[0].X + distantObs[0].distant*cos(PI / 2.0 + angleError);
	falseUnknowpoint[0].Y = knowpoint[0].Y + distantObs[0].distant*sin(PI / 2.0 + angleError);
	//���μ���ʣ��δ֪��Ľ�������
	for (int i = 0; i < angleObsnum - 1; i++)
	{
		FW_angle[i].angle += angleError;//ԭ·��ÿ����λ�ǵ��ڼ���·�߷�λ�Ǽӷ�λ�ǲ�
		falseUnknowpoint[i + 1].X = falseUnknowpoint[i].X + distantObs[i + 1].distant*cos(FW_angle[i].angle);
		falseUnknowpoint[i + 1].Y = falseUnknowpoint[i].Y + distantObs[i + 1].distant*sin(FW_angle[i].angle);
	}
	//����ԭͼC��Ľ������꣬������fx,fy
	FW_angle[angleObsnum - 1].angle += angleError;//ԭ·��ÿ����λ�ǵ��ڼ���·�߷�λ�Ǽӷ�λ�ǲ�
	falseknowpoint2.X = falseUnknowpoint[angleObsnum - 1].X +
		distantObs[Obsdistantnum - 1].distant*cos(FW_angle[angleObsnum - 1].angle);
	falseknowpoint2.Y = falseUnknowpoint[angleObsnum - 1].Y +
		distantObs[Obsdistantnum - 1].distant*sin(FW_angle[angleObsnum - 1].angle);


	//����ԭ·�ߵ�һ��δ֪�������
	unknowpoint[0].X = knowpoint[0].X + distantObs[0].distant*R*cos(PI / 2.0 + angleError);
	unknowpoint[0].Y = knowpoint[0].Y + distantObs[0].distant*R*sin(PI / 2.0 + angleError);
	//���μ���ʣ��δ֪�������
	for (int i = 0; i < angleObsnum - 1; i++)
	{
		//FW_angle[i].angle += angleError;//ԭ·��ÿ����λ�ǵ��ڼ���·�߷�λ�Ǽӷ�λ�ǲ�
		unknowpoint[i + 1].X = unknowpoint[i].X + distantObs[i + 1].distant*R*cos(FW_angle[i].angle);
		unknowpoint[i + 1].Y = unknowpoint[i].Y + distantObs[i + 1].distant*R*sin(FW_angle[i].angle);
	}

}

//���
void Value::resultput()
{
	CFileDialog dlgFile(FALSE, _T("txt"), NULL, OFN_EXPLORER,
		_T("(�ı��ļ�)|*.txt"));
	if (dlgFile.DoModal() == IDCANCEL) return;    //����ɹ��򷵻�
	CString strFileName2 = dlgFile.GetPathName();
	setlocale(LC_ALL, "");                         //���û�������

	//���ļ���д��
	CStdioFile wfile;
	if (!wfile.Open(strFileName2, CFile::modeCreate | CFile::modeWrite)) return;

	double fx, fy;  //����պϲ�
	int accuracy;    //�߳��۲⾫��
	fx = falseknowpoint2.X - knowpoint[1].X;
	fy = falseknowpoint2.Y - knowpoint[1].Y;
	accuracy = int(abs(1 / T));
	//������
	CString strout;
	strout.Format(_T("#####�޶����߽���ƽ��������#####\n��֪�������"));
	CString out1;  //����
	CString out2;  //����
	//��֪��
	out1.Format(_T("%d\n"), knowpointnum);
	strout += out1;
	for (int i = 0; i < knowpointnum; i++)
	{
		out1 = knowpoint[i].pointname;
		out2.Format(_T(",%.3f,%.3f\n"), knowpoint[i].X, knowpoint[i].Y);
		strout += out1 + out2;
	}
	//δ֪��
	out2.Format(_T("δ֪�������%d\n"), unknowpointnum);
	strout += out2;
	for (int i = 0; i < unknowpointnum; i++)
	{
		out1 = unknowpoint[i].pointname;
		out2.Format(_T(",%.3f,%.3f\n"), unknowpoint[i].X, unknowpoint[i].Y);
		strout += out1 + out2;
	}
	//�պϲ����
	out2.Format(_T("����պϲfx=%.3f  fy=%.3f\n�߳��۲⾫�ȣ�1/%d"), fx, fy, accuracy);
	strout += out2;

	wfile.WriteString(strout); //���ݶ��뵽wfile
	wfile.Close();              //�ر��ļ�
}


//�ָ��
int Value::SpliteStringArray(CString str, char split, CStringArray & astr)
{
	int startID = 0;
	int ID = str.Find(split, startID);
	astr.RemoveAll();   //�����
	while (-1 != ID)
	{
		CString sTemp = str.Mid(startID, ID - startID);
		astr.Add(sTemp);
		startID = ID + 1;
		ID = str.Find(split, startID);
	}
	CString sTemp = str.Right(str.GetLength() - startID);
	if (!sTemp.IsEmpty())
		astr.Add(sTemp);
	return astr.GetSize();
}

//���ݵ�Ŵ���֪���������ҵ����Ƶ㣬�����ظõ��ָ��
Point* Value::SearchKnownPointUsingID(CString strID)
{
	for (int i = 0; i < knowpointnum; i++)
	{
		if (strID == knowpoint[i].pointname)
		{
			return &knowpoint[i];
		}
	}
	return NULL;
}

//���ݵ�Ŵ�δ֪���������ҵ����Ƶ㣬�����ظõ��ָ��
Point *Value::SearchUnKnownPointUsingID(CString strID)
{
	for (int i = 0; i < unknowpointnum; i++)
	{
		if (strID == unknowpoint[i].pointname)
		{
			return &unknowpoint[i];
		}
	}
	return NULL;
}

//���ݵ�Ŵ�δ֪�����֪���������ҵ����Ƶ㣬�����ظõ��ָ��
Point *Value::SearchPointUsingID(CString strID)
{
	Point  *pCP = NULL;
	pCP = SearchKnownPointUsingID(strID);
	if (pCP == NULL)
	{
		pCP = SearchUnKnownPointUsingID(strID);
	}
	return pCP;
}

//�ȷ���ת����
double  Value::Dms2Rad(double DMS)
{
	int iDeg, iMin;  //���ȣ���
	double dSec;     //��
	iDeg = int(DMS + EPSILON);
	iMin = int((DMS - iDeg) * 100 + EPSILON);
	dSec = ((DMS - iDeg) * 100 - iMin) * 100;
	return ((iDeg + double(iMin) / 60 + dSec / 3600)*PI / 180);  //����
}


//��ȡ����Ϣ
void Point::setpoint(CString str, double dx, double dy)
{
	pointname = str;
	X = dx;
	Y = dy;
}

//��ȡ�Ƕ���Ϣ
void Angle::setangle(Point *Pstart, Point *Pmid, Point *Pend,double dangle)
{
	startID = Pstart;
	midID = Pmid;
	endID = Pend;
	angle = dangle;
}

//��ȡ������Ϣ
void Distant::setdistant(Point *Pstart, Point *Pend, double dlength)
{
	startID = Pstart;
	endID = Pend;
	distant = dlength;
}

//��λ��
double  Value::FangWeiAngle(double DMS, double RadfrontFW_Angle)  //DMS�۲�Ƕȣ�RadfrontFW_Angle��һ����ķ�λ��
{
	double radAngle = Dms2Rad(DMS);  //תΪ����
	double FW_Angle; //��ʱ�������õ㷽λ��
	//�۲�Ƕ�Ϊ���
	if (type == 1)
	{
		FW_Angle = RadfrontFW_Angle + radAngle - PI;
	}
	//�۲�Ƕ�Ϊ�ұ�
	else
	{
		FW_Angle = RadfrontFW_Angle - radAngle + PI;
	}
	//��ֵȷ����0-2*pi
	if (FW_Angle > 2.0*PI)
	{
		FW_Angle -= 2.0*PI;
	}
	else if (FW_Angle < 0)
	{
		FW_Angle += 2.0*PI;
	}
	return FW_Angle;  //���ط�λ��
}
