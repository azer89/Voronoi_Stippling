#ifndef MYPOINT_H
#define MYPOINT_H

    struct MyPoint
    {
    public:
        // x
        double x;

        // y
        double y;

        // Default constructor
        MyPoint()
        {
            this->x = -1;
            this->y = -1;
        }

        // Constructor
        MyPoint(double x, double y)
        {
            this->x = x;
            this->y = y;
        }

        // Scale a point
        MyPoint Resize(double val)
        {
            MyPoint newP;
            newP.x = this->x * val;
            newP.y = this->y * val;
            return newP;
        }

        // if a point is (-1, -1)
        bool Invalid()
        {
            if(((int)x) == -1 && ((int)y) == -1)
                { return true; }
            return false;
        }

        // Normalize
        MyPoint Norm()
        {
            double vlength = std::sqrt( x * x + y * y );
            return MyPoint(this->x / vlength, this->y / vlength);
        }

        // Euclidean distance
        double Distance(MyPoint other)
        {
            double xDist = x - other.x;
            double yDist = y - other.y;
            return sqrt(xDist * xDist + yDist * yDist);
        }

        // Euclidean distance
        double Distance(double otherX, double otherY)
        {
            double xDist = x - otherX;
            double yDist = y - otherY;
            return sqrt(xDist * xDist + yDist * yDist);
        }

        // squared euclidean distance
        double DistanceSquared(MyPoint other)
        {
            double xDist = x - other.x;
            double yDist = y - other.y;
            return (xDist * xDist + yDist * yDist);
        }

        // squared euclidean distance
        double DistanceSquared(double otherX, double otherY)
        {
            double xDist = x - otherX;
            double yDist = y - otherY;
            return (xDist * xDist + yDist * yDist);
        }

        // operator overloading
        MyPoint operator+ (const MyPoint& other) { return MyPoint(x + other.x, y + other.y); }

        // operator overloading
        MyPoint operator- (const MyPoint& other) { return MyPoint(x - other.x, y - other.y); }
        bool operator== (const MyPoint& other)
        { return (abs(this->x - other.x) < M_EPS && abs(this->y - other.y) < M_EPS); }

        // operator overloading
        bool operator!= (const MyPoint& other)
        { return (abs(this->x - other.x) >= M_EPS || abs(this->y - other.y) >= M_EPS); }

        // operator overloading
        MyPoint operator+= (const MyPoint& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        // operator overloading
        MyPoint operator-= (const MyPoint& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        // operator overloading
        MyPoint operator/ (const double& val) { return MyPoint(x / val, y / val); }

        // operator overloading
        MyPoint operator* (const double& val) { return MyPoint(x * val, y * val); }

        // operator overloading
        MyPoint operator*= (const double& val)
        {
            x *= val;
            y *= val;
            return *this;
        }

        // operator overloading
        MyPoint operator/= (const double& val)
        {
            x /= val;
            y /= val;
            return *this;
        }

        // length of a vector
        double Length() { return sqrt(x * x + y * y); }

        // squared length of a vector
        double LengthSquared() { return x * x + y * y; }

        // dot product
        double Dot(MyPoint otherPt) { return x * otherPt.x + y * otherPt.y; }

        // cross product
        MyPoint Cross(MyPoint otherPt)
        {
            //U x V = Ux*Vy-Uy*Vx
            return MyPoint(x * otherPt.y, y * otherPt.x);
        }

        // linear dependency test
        bool IsLinearDependent(MyPoint otherPoint)
        {
            double det = (this->x * otherPoint.y) - (this->y * otherPoint.x);
            if(det > -M_EPS && det < M_EPS) { return true; }
            return false;
        }

        // angle direction
        MyPoint DirectionTo(MyPoint otherPt)
        {
            return MyPoint(otherPt.x - this->x, otherPt.y - this->y);
        }
    };

#endif
