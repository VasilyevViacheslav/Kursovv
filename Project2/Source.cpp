#include <GLFW/glfw3.h>
#include "Header.h"
#include <iterator>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
int main(void)
{
    GLFWwindow* window;
    std::vector<Triangle> First;
    Triangle None{ 0,0,0,0,0,0,0 };
    First.push_back(None);
    Kit_Triangle Obs(None);
    float Tr_Info_mass[7];
    size_t i = 0, pos = 0, j = 0;
    std::string Tr_Info_str;
    std::ifstream fout;
    std::string temp;
    fout.open("Текст.txt");
    while (!fout.eof())
    {
        if (fout.is_open())
        {
            std::getline(fout, Tr_Info_str);
            pos = 0;
            while (pos < Tr_Info_str.size()) {
                temp = temp.assign(Tr_Info_str, pos, Tr_Info_str.find(',', pos) - pos);
                if (temp.size() > 0)  // проверка на пустую строку при необходимости
                    Tr_Info_mass[j] = std::stof(temp);
                pos += temp.size() + 1;
                ++j;
               
            }
            if ((j != 7)||(Tr_Info_mass[6]<0)) throw std::runtime_error("Не верно введены координаты и масса");
            j = 0;
           
            First[0] = { Tr_Info_mass[0],Tr_Info_mass[1],Tr_Info_mass[2],
                         Tr_Info_mass[3],Tr_Info_mass[4],Tr_Info_mass[5],Tr_Info_mass[6] };
            Obs.add_Triangle(First[0]);
        }
    }
    std::vector<float> Coords = Obs.Get_Coords(Obs.Massive_Of_TRiangle);

    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "TRIANGLEEEES", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
       
        glBegin(GL_TRIANGLES);
        GLfloat j = 0;
        float max = Obs.Get_Max_Cord();

        for (int i = 0; i < Coords.size(); i += 6) 
        {
                glColor3f(Obs.Massive_Of_Dest[j]/Obs.MaxDesteny, 0, 0);
                glVertex2d((Coords[i]/max), (Coords[i + 1]/max));
                glVertex2d(Coords[i + 2] / max , (Coords[i + 3] / max) );
                glVertex2d(Coords[i + 4] / max , (Coords[i + 5] / max) );
                ++j;
        }


        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glBegin(GL_TRIANGLES);

        float x, y;

        for (size_t i = 0; i < Obs.Massive_Of_TRiangle.size(); i++)
        {
            x = Obs.Massive_Of_TRiangle[i].Get_Center_Tr_x() / max;
            y = Obs.Massive_Of_TRiangle[i].Get_Center_Tr_y() / max;
            glVertex3f(x, y, 0.0f);
            glVertex3f(x, y, 0.0f);
            glVertex3f(x, y, 0.0f);
        }
        

        glVertex3f(Obs.Get_Center_Figure_x() / max, Obs.Get_Center_Figure_x() / max, 0.0f);
        glVertex3f(Obs.Get_Center_Figure_x() / max, Obs.Get_Center_Figure_x() / max, 0.0f);
        glVertex3f(Obs.Get_Center_Figure_x() / max, Obs.Get_Center_Figure_x() / max, 0.0f);
        glEnd();


        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}