#ifndef CIC_CORE_POINT_H
#define CIC_CORE_POINT_H

#include <cmath>

namespace cIcCore{

class Point{

public:
    int x;
    int y;

    Point(){
      x =0;
      y = 0;

    }

    Point(int xa, int ya){
      x = xa;
      y = ya;
    }

    void setPoint(Point p){
      x = p.x;
      y = p.y;

    }

    void setPoint(int xa, int ya){
      x  = xa;
      y = ya;
    }

    void rotate(int org_x,int org_y, int angle){
      double angle_ = angle * 2 * asin(1) / 180.0;
      int x1,y1;
      
      x1 = cos(angle_)*(x - org_x) - sin(angle_)*(y - org_y) + org_x;
      y1 = sin(angle_) *(x - org_x) + cos(angle_) * (y- org_y) + org_y;

      x = x1;
      y = y1;
    }

    void translate(int dx, int dy){
      x = x + dx;
      y = y + dy;
    }

    bool leftOf(const Point& p){
      if(p.x < x){
          return true;
        }
      return false;
    }

    bool over(const Point& p){
      if(p.y > y){
          return true;
        }
      return false;
    }

    int swapX(int x2){
      int xx = x;
      x = x2;
      return xx;
    }

    int swapY(int y2){
      y = y2;
      return y;
    }

    QString toString()
    {
        QString s = "x=%1, y=%2";
        return s.arg(x).arg(y);

    }
    


    bool operator==(Point p ){
    if(p.x == x && p.y == y){
        return true;
      }else{
        return false;
      }


    }

};

}

#endif // POINT_H
