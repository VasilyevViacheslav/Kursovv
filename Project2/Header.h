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
	double x1, y1, x2, y2, x3, y3; // (x1,y1) - первая верщина ...
	double mass;
	double d12 = sqrt(pow((x2 - x1),2) + (y2 - y1) * (y2 - y1)); // Длинаа стороны с вершинами 1 и 2
	double d23 = sqrt((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));// Длинаа стороны с вершинами 2 
	double d13 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));// Длинаа стороны с вершинами 1 и 3
	double P = d12 + d23 + d13;  // Периметр 
	double S = sqrt((P / 2) * ((P / 2) - d12) * ((P / 2) - d23) * ((P / 2) - d13)); // АПлощадь
	double Destiny = mass / S; // Плотность
	float x_CenterOfMass = ((x1 + x2 + x3) / 3); //Центр масс треугольника по х
	float y_CenterOfMass = ((y1 + y2 + y3) / 3); //Центр масс треугольника по у
	double Koefd12;
	double heightm;
	double C;
	double mInertia; //Момент инерции треугольника
	void cons()
	{
		d12 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); // Длинаа стороны с вершинами 1 и 2
		d23 = sqrt((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));// Длинаа стороны с вершинами 2 и 3
		d13 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));// Длинаа стороны с вершинами 1 и 3
		P = d12 + d23 + d13;  // Периметр 
		S = sqrt((P / 2) * ((P / 2) - d12) * ((P / 2) - d23) * ((P / 2) - d13)); // АПлощадь
		Destiny = mass / S; // Плотность
		x_CenterOfMass = ((x1 + x2 + x3) / 3); //Центр масс треугольника по х
		y_CenterOfMass = ((y1 + y2 + y3) / 3); //Центр масс треугольника по у
		if ((y2 - y1) == 0) Koefd12 = 0;
		else Koefd12 = ((x2 - x1) / (y2 - y1));
		C = (x1 + x2) / 2;
		// Koef * x - x1/Koef
		heightm = abs(Koefd12 * x_CenterOfMass - y_CenterOfMass - C) / abs(sqrt(Koefd12 * Koefd12 + 1));
		mInertia = (mass / 6) * (d12 * d12 - d12 * d23 + d23 * d23 + 3*heightm*heightm);
	}
	float Get_Center_Tr_x() { return x_CenterOfMass; }
	float Get_Center_Tr_y() { return y_CenterOfMass; }
};
class Kit_Triangle
{
public:
	double MaxDesteny = 0;
	double mInertia = 0; //Момент инерции всего тела
	double Kit_Mass = 0; // Масса фигуры
	double SOfAllTriangles = 0; // Площадь всей фигуры
	double AverageDesttiny = 0; // Плотность всей фигуры
	double CenterMassKit_x = 0; // Центр масс фигуры по координате х
	double CenterMassKit_y = 0; // Центр масс фигуры по координате y
	


	std::vector<Triangle> Massive_Of_TRiangle{}; // Массив треугольников входящих в фигуру
	std::vector<GLfloat > Massive_Of_Dest{};


	Kit_Triangle(Triangle tr) {};
	~Kit_Triangle(){Massive_Of_TRiangle.clear();}


	void ChangeMaxDesteny(std::vector<Triangle> Massive_Of_TRiangle, double& MaxDest)
	{
		for (size_t i = 0; i < Massive_Of_TRiangle.size(); ++i)
		{
			if (MaxDest < Massive_Of_TRiangle[i].Destiny) MaxDest = Massive_Of_TRiangle[i].Destiny;
		}
	}
	void add_Triangle(Triangle tr) // Добавление треугольника
	{
		if (!Check(Massive_Of_TRiangle, tr)) {
			tr.cons();
			Massive_Of_TRiangle.push_back(tr); // Добавляем треугольник в конец массива
			SOfAllTriangles += tr.S; //Площадь всей фигуры + площадь треугольника
			AverageDesttiny = Kit_Mass / SOfAllTriangles; //Плотность общ масса/Общая площадь
			CenterMassKit_x = (tr.x_CenterOfMass * tr.mass + CenterMassKit_x * Kit_Mass) / (Kit_Mass + tr.mass); //По формуле нахождения ц.Масс
			CenterMassKit_y = (tr.y_CenterOfMass * tr.mass + CenterMassKit_y * Kit_Mass) / (Kit_Mass + tr.mass);
			Kit_Mass += tr.mass;
			mInertia += tr.mInertia + tr.mass * (sqrt((tr.x_CenterOfMass - CenterMassKit_x) * (tr.x_CenterOfMass - CenterMassKit_x)
				- (tr.y_CenterOfMass - CenterMassKit_y) * (tr.y_CenterOfMass - CenterMassKit_y))); //Находим общий момент по ф-ле Гюйгейна-Штейнера
			ChangeMaxDesteny(Massive_Of_TRiangle, MaxDesteny);
			Massive_Of_Dest.push_back(tr.Destiny);
		}
	else throw std::runtime_error("Пересекаются");
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

	  //Код для проверки пересечения с погрешностью eps
	  typedef std::pair<double, double> TriPoint;

	  inline double Det2D(TriPoint& p1, TriPoint& p2, TriPoint& p3)
	  {
		  return +p1.first * (p2.second - p3.second)
			  + p2.first * (p3.second - p1.second)
			  + p3.first * (p1.second - p2.second);
	  }

	  void CheckTriWinding(TriPoint& p1, TriPoint& p2, TriPoint& p3, bool allowReversed)
	  {
		  double detTri = Det2D(p1, p2, p3);//ПРоверка на очередность ввода координат
		  if (detTri <= 0.0)
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
		  CheckTriWinding(t1[0], t1[1], t1[2], allowReversed);
		  CheckTriWinding(t2[0], t2[1], t2[2], allowReversed);


		  for (int i = 0; i < 3; i++)
		  {
			  int j = (i + 1) % 3;

			
			  if (BoundaryCollideChk(t1[i], t1[j], t2[0], eps) &&
				  BoundaryCollideChk(t1[i], t1[j], t2[1], eps) &&
				  BoundaryCollideChk(t1[i], t1[j], t2[2], eps))
				  return false;
		  }

		 
		  for (int i = 0; i < 3; i++)
		  {
			  int j = (i + 1) % 3;

			  
			  if (BoundaryCollideChk(t2[i], t2[j], t1[0], eps) &&
				  BoundaryCollideChk(t2[i], t2[j], t1[1], eps) &&
				  BoundaryCollideChk(t2[i], t2[j], t1[2], eps))
				  return false;
		  }

		  //The triangles collide
		  return true;
	  }


		bool Check(std::vector<Triangle> massiveTr, Triangle Tr)//Функция проверки пересечния
	  {
		  TriPoint t1[] = { TriPoint(Tr.x1,Tr.y1),TriPoint(Tr.x2,Tr.y2),TriPoint(Tr.x3,Tr.y3) }; //Треугольник который добавляем

		  for (size_t i = 0; i < Massive_Of_TRiangle.size(); ++i) // Проходим по массиву треугольников
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
	  double Get_Center_Figure_x() { return CenterMassKit_x;}
	  double Get_Center_Figure_y() { return CenterMassKit_y; }
};


