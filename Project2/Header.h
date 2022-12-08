#include <vector>
#include <iostream>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
class Triangle
{
public:
	Triangle(double x1, double y1, double x2, double y2, double x3, double y3, double mass)
	{
		this->x1 = x1;
		this->x2 = x2;
		this->x3 = x3;
		this->y1 = y1;
		this->y2 = y2;
		this->y3 = y3;
		this->mass = mass;

		cons();
	}
	double x1, y1, x2, y2, x3, y3; // (x1,y1) - ������ ������� ...
	double mass;
	double d12 = sqrt(pow((x2 - x1),2) + (y2 - y1) * (y2 - y1)); // ������ ������� � ��������� 1 � 2
	double d23 = sqrt((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));// ������ ������� � ��������� 2 
	double d13 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));// ������ ������� � ��������� 1 � 3
	double P = d12 + d23 + d13;  // �������� 
	double S = sqrt((P / 2) * ((P / 2) - d12) * ((P / 2) - d23) * ((P / 2) - d13)); // ��������
	double Destiny = mass / S; // ���������
	double x_CenterOfMass = ((x1 + x2 + x3) / 3); //����� ���� ������������ �� �
	double y_CenterOfMass = ((y1 + y2 + y3) / 3); //����� ���� ������������ �� �
	double height = 2 * S / d12; //������ ������ ��������� �� ������� 1,2
	double mInertia = d12 * pow(height, 3) / 12; //������ ������� ������������
	void cons()
	{
		d12 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); // ������ ������� � ��������� 1 � 2
		d23 = sqrt((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));// ������ ������� � ��������� 2 � 3
		d13 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));// ������ ������� � ��������� 1 � 3
		P = d12 + d23 + d13;  // �������� 
		S = sqrt((P / 2) * ((P / 2) - d12) * ((P / 2) - d23) * ((P / 2) - d13)); // ��������
		Destiny = mass / S; // ���������
		x_CenterOfMass = ((x1 + x2 + x3) / 3); //����� ���� ������������ �� �
		y_CenterOfMass = ((y1 + y2 + y3) / 3); //����� ���� ������������ �� �
		height = 2 * S / d12; //������ ������ ��������� �� ������� 1,2
		mInertia = d12 * pow(height, 3) / 12; //������ ������� ������������

	}
};
class Kit_Triangle
{
public:
	double MaxDesteny = 0;
	double mInertia = 0; //������ ������� ����� ����
	double Kit_Mass = 0; // ����� ������
	double SOfAllTriangles = 0; // ������� ���� ������
	double AverageDesttiny = 0; // ��������� ���� ������
	double CenterMassKit_x = 0; // ����� ���� ������ �� ���������� �
	double CenterMassKit_y = 0; // ����� ���� ������ �� ���������� y
	


	std::vector<Triangle> Massive_Of_TRiangle{}; // ������ ������������� �������� � ������
	std::vector<GLfloat > Massive_Of_Dest{};


	Kit_Triangle(Triangle tr) //���������� 
	{
	
	}
	~Kit_Triangle()
	{
		Massive_Of_TRiangle.clear();
	}


	void ChangeMaxDesteny(std::vector<Triangle> Massive_Of_TRiangle, double& MaxDest)
	{
		for (size_t i = 0; i < Massive_Of_TRiangle.size(); ++i)
		{
			if (MaxDest < Massive_Of_TRiangle[i].Destiny) MaxDest = Massive_Of_TRiangle[i].Destiny;
		}
	}
	void add_Triangle(Triangle tr) // ���������� ������������
	{
		if (!Check(Massive_Of_TRiangle, tr)) {
			Massive_Of_TRiangle.push_back(tr); // ��������� ����������� � ����� �������
			SOfAllTriangles += tr.S; //������� ���� ������ + ������� ������������
			AverageDesttiny = Kit_Mass / SOfAllTriangles; //��������� ��� �����/����� �������
			CenterMassKit_x += (tr.x_CenterOfMass * tr.mass + CenterMassKit_x * Kit_Mass) / (Kit_Mass + tr.mass); //�� ������� ���������� �.����
			CenterMassKit_y += (tr.y_CenterOfMass * tr.mass + CenterMassKit_y * Kit_Mass) / (Kit_Mass + tr.mass);
			Kit_Mass += tr.mass;
			mInertia += tr.mInertia + tr.mass * (sqrt((tr.x_CenterOfMass - CenterMassKit_x) * (tr.x_CenterOfMass - CenterMassKit_x)
				- (tr.y_CenterOfMass - CenterMassKit_y) * (tr.y_CenterOfMass - CenterMassKit_y))); //������� ����� ������ �� �-�� ��������-��������
			ChangeMaxDesteny(Massive_Of_TRiangle, MaxDesteny);
			Massive_Of_Dest.push_back(tr.Destiny);
		}
	else throw std::runtime_error("������������");
	}
	float Get_Max_Cord()
	{
		float max = 0;
		std::vector<float> temp = Get_Coords(Massive_Of_TRiangle);
		for(size_t j =0; j<temp.size();++j)
		{
			max = (max < abs(temp[j])) ? abs(temp[j]):max;
		}
		return max;
	}

	  //��� ��� �������� ����������� � ������������ eps
	  typedef std::pair<double, double> TriPoint;

	  inline double Det2D(TriPoint& p1, TriPoint& p2, TriPoint& p3)
	  {
		  return +p1.first * (p2.second - p3.second)
			  + p2.first * (p3.second - p1.second)
			  + p3.first * (p1.second - p2.second);
	  }

	  void CheckTriWinding(TriPoint& p1, TriPoint& p2, TriPoint& p3, bool allowReversed)
	  {
		  double detTri = Det2D(p1, p2, p3);//�������� �� ����������� ����� ���������
		  if (detTri < 0.0)
		  {
			  if (allowReversed)
			  {
				  TriPoint a = p3;
				  p3 = p2;
				  p2 = a;
			  }
			  else throw std::runtime_error("triangle has wrong winding direction");
		  }
	  }

	  bool BoundaryCollideChk(TriPoint& p1, TriPoint& p2, TriPoint& p3, double eps)
	  {
		  return Det2D(p1, p2, p3) < eps;
	  }

	  bool TriTri2D(TriPoint* t1,
		  TriPoint* t2,
		  double eps = 0.3, bool allowReversed = false, bool onBoundary = true)
	  {
		  //Trangles must be expressed anti-clockwise
		  CheckTriWinding(t1[0], t1[1], t1[2], allowReversed);
		  CheckTriWinding(t2[0], t2[1], t2[2], allowReversed);


		  //For edge E of trangle 1,
		  for (int i = 0; i < 3; i++)
		  {
			  int j = (i + 1) % 3;

			  //Check all points of trangle 2 lay on the external side of the edge E. If
			  //they do, the triangles do not collide.
			  if (BoundaryCollideChk(t1[i], t1[j], t2[0], eps) &&
				  BoundaryCollideChk(t1[i], t1[j], t2[1], eps) &&
				  BoundaryCollideChk(t1[i], t1[j], t2[2], eps))
				  return false;
		  }

		  //For edge E of trangle 2,
		  for (int i = 0; i < 3; i++)
		  {
			  int j = (i + 1) % 3;

			  //Check all points of trangle 1 lay on the external side of the edge E. If
			  //they do, the triangles do not collide.
			  if (BoundaryCollideChk(t2[i], t2[j], t1[0], eps) &&
				  BoundaryCollideChk(t2[i], t2[j], t1[1], eps) &&
				  BoundaryCollideChk(t2[i], t2[j], t1[2], eps))
				  return false;
		  }

		  //The triangles collide
		  return true;
	  }


		bool Check(std::vector<Triangle> massiveTr, Triangle Tr)//������� �������� ����������
	  {
		  TriPoint t1[] = { TriPoint(Tr.x1,Tr.y1),TriPoint(Tr.x2,Tr.y2),TriPoint(Tr.x3,Tr.y3) }; //����������� ������� ���������

		  for (size_t i = 0; i < Massive_Of_TRiangle.size(); ++i) // �������� �� ������� �������������
		  {
			  TriPoint Tr[] =
			  {
					  TriPoint(Massive_Of_TRiangle[i].x1,Massive_Of_TRiangle[i].y1),
					  TriPoint(Massive_Of_TRiangle[i].x2,Massive_Of_TRiangle[i].y2),
					  TriPoint(Massive_Of_TRiangle[i].x3,Massive_Of_TRiangle[i].y3)
			  };
			  if (TriTri2D(t1, Tr)) return true;
		  }
		  return false;
	  }


		std::vector<float> Get_Coords(std::vector<Triangle> Massive_tr)
		{
			std::vector<float> Massive_Coords{};
			for (size_t i = 0; i < Massive_tr.size(); ++i)
			{
				Massive_Coords.push_back(Massive_tr[i].x1);
				Massive_Coords.push_back(Massive_tr[i].y1);
				Massive_Coords.push_back(Massive_tr[i].x2);
				Massive_Coords.push_back(Massive_tr[i].y2);
				Massive_Coords.push_back(Massive_tr[i].x3);
				Massive_Coords.push_back(Massive_tr[i].y3);

			}
			return Massive_Coords;

		}
	  double Get_mInertiea() { return  mInertia; }
	  double Get_MaxDest() { return MaxDesteny; }
};


