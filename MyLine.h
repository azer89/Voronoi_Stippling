
/**
 *
 * Line representation with two points, start and end points
 * Each points doesn't hold the real position
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __MYLINE_H__
#define __MYLINE_H__

#include "MyPoint.h"

//namespace CVSystem
//{

    struct MyLine
    {
    public:
        double XA;	double YA;	// start
        double XB;	double YB;	// end

        // Constructor #1
        MyLine()
        {
            this->XA = -1;	this->YA = -1;
            this->XB = -1;	this->YB = -1;
        }

        // Constructor #2
        MyLine(double XA, double YA, double XB, double YB)
        {
            this->XA = XA;	this->YA = YA;
            this->XB = XB;	this->YB = YB;
        }

        MyLine(MyPoint pt1, MyPoint pt2)
        {
            this->XA = pt1.x;	this->YA = pt1.y;
            this->XB = pt2.x;	this->YB = pt2.y;
        }


        MyLine Resize(double val)
        {
            MyLine newL;

            newL.XA = this->XA * val;
            newL.YA = this->YA * val;

            newL.XB = this->XB * val;
            newL.YB = this->YB * val;

            return newL;
        }

        // Uninitialized ?
        bool Invalid()
        {
            if(((int)XA) == -1 && ((int)YA) == -1 && ((int)XB) == -1 && ((int)YB) == -1) return true;
            return false;
        }

        // Start point
        MyPoint GetPointA() { return MyPoint(XA, YA); }

        // End point
        MyPoint GetPointB() { return MyPoint(XB, YB); }

        // Direction of the vector
        MyPoint Direction() { return MyPoint(XB - XA, YB - YA);}

        double Magnitude()
        {
            MyPoint dir = Direction();
            return sqrt(dir.x * dir.x + dir.y * dir.y);
        }

        bool LiesHere(MyPoint pt)
        {
            double det = (XB - XA) * (pt.y - YA) - (YB - YA) * (pt.x - XA);
            if(det > -M_EPS && det < M_EPS) return true;
            return false;
        }

        // returns:
        //	 1 : same direction
        //  -1 : opposite direction
        //   0 : other else
        int HasSameDirection(MyLine otherLine)
        {
            double mag1 = Magnitude();
            double mag2 = otherLine.Magnitude();

            MyPoint dir1 = Direction();
            MyPoint dir2 = otherLine.Direction();

            double a_dot_b = dir1.Dot(dir2);
            double a_b_mag = mag1 *  mag2;

            double addValue = a_dot_b + a_b_mag;
            if(addValue > -M_EPS && addValue < M_EPS ) { return -1; }

            double subsValue = a_dot_b - a_b_mag;

            if(subsValue > -M_EPS && subsValue < M_EPS ) { return 1; }

            return 0;
        }
    };
//}

#endif
