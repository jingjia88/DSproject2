#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <algorithm>
using namespace std;

#define edge pair<int,int>
struct direc{
    int x, y;
    direc(int X, int Y) : x(X), y(Y) {}
};

vector<direc> moves;
vector<edge> steps;
int rows,cols,pace;
int **graph;
int **dist;

int **allocate_Memory(int rows,int cols);

void distances(int i ,int j);
int main(int argc, char *argv[])
{
    // Open the file with the arguments passed in
    string argf(argv[1]);
    string arg = ".\\"+argf+"\\floor.data";
    ifstream infile(arg.c_str());

    if(!infile){
        cout<<"Something(Input) wrong"<<endl;
        return 1;
    }

    infile >> rows >> cols >> pace;
    graph = allocate_Memory(rows,cols);
    dist = allocate_Memory(rows,cols);
    //read all value
    string value;
    int p=0,k=0;
    for (int i = 0; i < rows; i++){
        for(int j =0; j < cols; j++){
            infile >> value;
            if(value == "1") {graph[i][j] = 1;}
            else if(value == "0"){ graph[i][j] = 0;}
            else {graph[i][j] =82; p=i;k=j;}
            dist[i][j] = 0;
        }
    }

    //calculate the shortest distance from every vertex to R
    moves.push_back(direc(1,0));
    moves.push_back(direc(-1,0));
    moves.push_back(direc(0,1));
    moves.push_back(direc(0,-1));
    distances(p,k);
    for (int i = 0; i < rows; i++)
        for(int j =0; j < cols; j++)
            cout<<dist[i][j]<<" ";
    //output
    // string arg1 = ".\\"+argf+"\\floor.data";
    // ofstream outfile(arg1.c_str());
    // int _size = steps.size();
    // outfile << _size << '\n';
    // for (int i = 0; i < _size; i++)
    //     outfile << steps[i].first << ' ' << steps[i].second  << '\n';

    // infile.close();
    // outfile.close();

    return 0;
}
int **allocate_Memory(int rows,int cols)
{
    int **res = new int *[rows];
    for(int i =0;i<rows;i++){
        res[i] = new int [cols];
    }
    return res;
}

void distances(int i ,int j){
//for(un,down,left,right)
//  if(avaible)
//      dist[i+R][j+C]=dist[i][j] + 1;

    for(unsigned int k = 0; k < moves.size() ;k++){
        int R = moves[k].x;
        int C = moves[k].y;
        if((i+R)<0 || (i+R)>rows-1 || (j+C)<0 ||(j+C)>cols-1 ||
                (dist[i+R][j+C]!=0 &&dist[i+R][j+C] < dist[i][j] + 1)){continue;}
        if(graph[i+R][j+C]==0){
            dist[i+R][j+C] = dist[i][j] + 1;
            distances(i+R,j+C);}
    }
    return ;
}
