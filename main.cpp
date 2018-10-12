#include <iostream>
#include<fstream>
#include <stdlib.h>
#include<vector>
#include <algorithm>
#include<math.h>
using namespace std;

struct Point
{
    double x,y;
};

ostream& operator<<( ostream& os, const Point &p)
{
    os<<p.x<<" "<<p.y<<endl;
}

void sortX(vector<Point>& point,int N)
{
    int flag = 1;
    Point temp;
    for(int i=0;i< N && flag;i++)
    {
         flag = 0;
         for (int j=0;j <N-1;j++)
         {
            if (point[j].x> point[j+1].x )
            {
                temp = point[j];
                point[j] = point[j+1];
                point[j+1] = temp;
                flag = 1;
            }
            else if(point[j].x == point[j+1].x)
            {
                 if (point[j].y < point[j+1].y)
                 {
                    temp = point[j];
                    point[j] = point[j+1];
                    point[j+1] = temp;
                    flag = 1;
                 }
            }
         }
     }
     return;
}

int distance(Point A, Point B, Point C)
{
    int num = (B.y - A.y) * (A.x - C.x)-(B.x - A.x) * (A.y - C.y) ;
    if (num < 0) num = -num;
    return num;
}

int turn(Point A, Point B, Point P)
{
    int num = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
    if (num > 0) return 1;
    else if (num == 0) return 0;
    else return -1;
}

int triangle(Point A, Point B, Point C, Point D)
{
    int num1 = turn(A,B,D);
    int num2 = turn(B,C,D);
    int num3 = turn(C,A,D);
    if (num1 > 0 && num2 > 0 && num3 > 0 ) return 1;
    else if(num1 < 0 && num2 < 0 && num3 < 0 ) return 1;
    else return 0;
}

void QuickHull(Point A, Point B, vector<Point> sets, vector<Point>& output)
{
    if (sets.size() == 0) return ;
    if (sets.size() == 1)
    {
        output.push_back(sets[0]);
        sets.erase (sets.begin());
        return;
    }
    int max_distance = -999;
    Point max_point;
    int index;
    for (int i = 0; i < sets.size(); i++)
    {
        Point point = sets[i];
        if (distance(A, B, point) > max_distance)
        {
            max_distance = distance(A, B, point);
            max_point=point;
            index=i;
        }
    }
    sets.erase (sets.begin()+index);
    /*for (int i = 0; i < sets.size(); i++)
    {
        Point p = sets[i];
        if (triangle(A, B,max_point, p) == 1) sets.erase (sets.begin()+i);
    }*/
    vector<Point> upperSetAP;
    vector<Point> upperSetPB;
    for (int i = 0; i < sets.size(); i++)
    {
        Point M = sets[i];
        if (turn(A, max_point, M) == 1)
        {
            upperSetAP.push_back(M);
        }
        else if (turn(max_point,B, M) == 1)
        {
            upperSetPB.push_back(M);
        }
    }
    QuickHull(A, max_point, upperSetAP, output);
    output.push_back(max_point);
    QuickHull(max_point, B, upperSetPB, output);
    return;
}

void HUll(vector<Point> point,int N,vector<Point>& output)
{
    vector<Point>::iterator i;
    sortX(point,N);
    Point a=point[0];
    Point b=point[point.size()-1];
    point.erase (point.begin());
    point.erase (point.begin()+(point.size()-1));

    vector<Point> upperSet;
    vector<Point> lowerSet;
    for (int i = 0; i < point.size(); i++)
    {
        Point p = point[i];
        if (turn(a, b, p) == -1) lowerSet.push_back(p);
        else if (turn(a, b, p) == 1) upperSet.push_back(p);
    }
    output.push_back(a);
    QuickHull(a, b, upperSet, output);
    output.push_back(b);
    QuickHull(output[1],output[0], lowerSet, output);
    //sortedOutput(output,output.size());
}

double calculateArea(Point A,Point B,Point P,Point Q)
{
    double ab=sqrt((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));
    double h1 = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
    h1=h1/ab;
    if (h1 < 0) h1 = -h1;
    double h2 = (B.x - A.x) * (Q.y - A.y) - (B.y - A.y) * (Q.x - A.x);
    h2=h2/ab;
    if (h2 < 0) h2 = -h2;
    double area1= 0.5 * h1 * ab;
    double area2= 0.5 * h2 * ab;
    return area1+area2;
}

double rectangle(vector<Point> output,vector<Point>& RP)
{
    double area;
    double max_area=0.0;
    Point Final1,Final2,Final3,Final4;
    for (int i = 0; i < output.size(); i++)
    {
        Point a = output[i];
        for (int j = i+2; j < output.size(); j++)
        {
            Point b = output[j];
            vector<Point> rightSet;
            vector<Point> leftSet;

            for (int k = 0; k < output.size(); k++)
            {
                if(k!=i && k!=j)
                {
                    Point p = output[k];
                    if (turn(a, b, p) == -1) rightSet.push_back(p);
                    else if (turn(a, b, p) == 1) leftSet.push_back(p);
                }
            }
            if(rightSet.size()==0 || leftSet.size()==0) break;
            int max_distance1 = -999;
            Point max_point1;
            int max_distance2 = -999;
            Point max_point2;
            for (int k = 0; k < rightSet.size(); k++)
            {
                Point point = rightSet[k];
                if (distance(a, b, point) > max_distance1)
                {
                    max_distance1 = distance(a, b, point);
                    max_point1=point;
                }
            }
            for (int k = 0; k < leftSet.size(); k++)
            {
                Point point = leftSet[k];
                if (distance(a, b, point) > max_distance2)
                {
                    max_distance2 = distance(a, b, point);
                    max_point2=point;
                }
            }
            area=calculateArea(a,b,max_point1,max_point2);
            if(area > max_area)
            {
                max_area = area;
                Final1=a;
                Final2=max_point1;
                Final3=b;
                Final4=max_point2;
            }
        }
    }
    RP.push_back(Final1);
    RP.push_back(Final2);
    RP.push_back(Final3);
    RP.push_back(Final4);
    return max_area;
}

int find(Point a,Point b,vector<Point>& Set,int first,int last)
{
    int index;
    if (last==0) index=first;
    else if (first > last)index = first;
    else if (first == last) index = first ;
    else
    {
        int mid = (first + last)/2;
        Point point1 = Set[mid];
        Point point2 = Set[mid-1];
        Point point3= Set[mid+1];
        if((distance(a, b, point1) > distance(a, b, point2)) && (distance(a, b, point1) > distance(a, b, point3)))
        {
            index=mid;
        }
        else if (distance(a, b, point1) > distance(a, b, point2))
            index = find(a,b,Set, mid+1, last);
        else
            index = find(a,b,Set,first, mid-1);

    }
    return index;

}

double binary(vector<Point> output,vector<Point>& RP)
{
    double area;
    double max_area=0.0;
    Point Final1,Final2,Final3,Final4;
    for (int i = 0; i < output.size(); i++)
    {
        Point a = output[i];
        for (int j = i+2; j < output.size(); j++)
        {
            Point b = output[j];
            vector<Point> rightSet;
            vector<Point> leftSet;

            for (int k = 0; k < output.size(); k++)
            {
                if(k!=i && k!=j)
                {
                    Point p = output[k];
                    if (turn(a, b, p) == -1) rightSet.push_back(p);
                    else if (turn(a, b, p) == 1) leftSet.push_back(p);
                }
            }
            if(rightSet.size()==0 || leftSet.size()==0) break;
            int max_distance1 = -999;
            Point max_point1;
            int max_distance2 = -999;
            Point max_point2;
            vector<Point>::iterator i;
            /*cout<<"right : ";
            for(i=rightSet.begin(); i!= rightSet.end(); ++i)
            {
                cout<<*i;
            }
            cout<<"left : ";
            for(i=leftSet.begin(); i!= leftSet.end(); ++i)
            {
                cout<<*i;
            }*/
            int index = find(a,b,rightSet,0,rightSet.size()-1);
            max_point1=rightSet[index];
            //cout<<"a : "<<a.x<<"  "<<a.y<<endl;
            //cout<<"b : "<<b.x<<"  "<<b.y<<endl;
            //cout<<"max1 : "<<max_point1.x<<"  "<<max_point1.y<<endl;
            index = find(a,b,leftSet,0,leftSet.size()-1);
            max_point2=leftSet[index];
            //cout<<"max2 : "<<max_point2.x<<"  "<<max_point2.y<<endl;
            area=calculateArea(a,b,max_point1,max_point2);
            if(area > max_area)
            {
                max_area = area;
                Final1=a;
                Final2=max_point1;
                Final3=b;
                Final4=max_point2;
            }
        }
    }
    RP.push_back(Final1);
    RP.push_back(Final2);
    RP.push_back(Final3);
    RP.push_back(Final4);
    return max_area;
}


int main()
{
    ifstream fin ("1005081_input.txt");
    cout<<"-------------Quick Hull-------------\n";
    int N;
    fin>>N;
    vector<Point> point;
    vector<Point> output;
    vector<Point> RP;
    vector<Point> BP;
    for(int i=0;i<N;i++)
    {
        Point point1;
        fin>>point1.x;
        fin>>point1.y;
        point.insert(point.begin()+i, point1);
    }
    cout<<"The given points are : "<<endl;
    vector<Point>::iterator i;
    for(i=point.begin(); i!= point.end(); ++i)
    {
        cout<<*i;
    }
    HUll(point,N,output);
    cout<<"The points of Convex hull are : "<<endl;
    for(i=output.begin(); i!= output.end(); ++i)
    {
        cout<<*i;
    }
    cout<<"---------------------------------------"<<endl;
    double area=rectangle(output,RP);
    cout<<"The area of rectangle is: "<<area<<endl;
    cout<<"The points of rectangle are: "<<endl;
    for(i=RP.begin(); i!= RP.end(); ++i)
    {
        cout<<*i;
    }
    cout<<"---------------------------------------"<<endl;
    double Barea=binary(output,BP);
    cout<<"The area of rectangle is: "<<Barea<<endl;
    cout<<"The points of rectangle are: "<<endl;
    for(i=BP.begin(); i!= BP.end(); ++i)
    {
        cout<<*i;
    }
    fin.close();
    return 0;
}
