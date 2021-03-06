#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stack>
#include <queue>
#include <algorithm>
#include <utility>
#include <windows.h>
using namespace std;

#define edge pair<int,int>
struct direc{
    int x, y;
    direc(int X, int Y) : x(X), y(Y) {}
};
int total = 0;
vector<direc> moves;
vector<edge> steps;
stack<edge> home,record;

int R,C,a,b,rows,cols,pace; 
int **graph,**dist,**visited;
int arr[3], _firstRow,_firstCol;
bool rec = false;bool battery = false;bool out = true; 
bool another =false;bool go = false;

int **allocate_Memory(int rows,int cols);
void distances(int i ,int j);
void find_path(int p,int k,int pace);
void checkBattery();
void anotherTree(int i,int j);
int main(int argc, char *argv[])
{
    // Open the file with the arguments passed in
    string argf =argv[1];
    string arg = "./hw/"+argf+"/floor.data";
    ifstream infile(arg.c_str());

    if(!infile){
        cout<<"Something(Input) wrong"<<endl;
        return 1;
    }

    //allocate memory
    infile >> rows >> cols >> pace;
    graph = allocate_Memory(rows,cols);
    dist = allocate_Memory(rows,cols);
    visited = allocate_Memory(rows,cols);

    //read all value
    string value;
    for (int i = 0; i < rows; i++){
        for(int j =0; j < cols; j++){
            infile >> value;
            if(value == "1") {graph[i][j] = 1;}
            else if(value == "0"){ graph[i][j] = 0; total++;}
            else {graph[i][j] =82; a=i;b=j;}
            dist[i][j] = 0;
            visited[i][j] = 0;
        }
    }

    //calculate the shortest distance from every vertex to R
    moves.push_back(direc(0,1));
    moves.push_back(direc(-1,0));
    moves.push_back(direc(0,-1));
    moves.push_back(direc(1,0));

    distances(a,b);
    
    //find_path
    find_path(a,b,pace);

    //output
    string arg1 = "./hw/"+argf+"/final.path";
    ofstream outfile(arg1.c_str());
    int _size = steps.size();
    outfile << _size << '\n';
    for (int i = 0; i < (_size-1); i++)
        outfile << steps[i].first << ' ' << steps[i].second  << '\n';
        
    outfile << steps[_size-1].first << ' ' << steps[_size-1].second  ;

    infile.close();
    outfile.close();

    delete [] graph;
    delete [] visited;
    delete [] dist;

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
    queue<edge> store;      //BFS
    store.push(edge(i,j));
    while(!store.empty()){
        i = store.front().first; j =store.front().second; store.pop();
        for(unsigned int k = 0; k < moves.size() ;k++){
            R = moves[k].x;
            C = moves[k].y;
            if((i+R)>=0 && (i+R)<rows && (j+C)>=0 &&(j+C)<cols){
                if(graph[i+R][j+C]==0 && (dist[i+R][j+C] > dist[i][j] + 1 || dist[i+R][j+C]==0)){
                    dist[i+R][j+C] = dist[i][j] + 1;
                    store.push(edge(i+R,j+C));
                }
            }
        }
    }
    return; 
}

//find next step
int* next(int i ,int j,int p){
    //if out of battery,back to start point
    if(p<=pace/2) out = false;
    //no enough battery,back to home
    if(!out && home.size()>1){
        home.pop();
        steps.push_back(home.top());
        arr[0] = i = home.top().first;
        arr[1] = j = home.top().second;
        arr[2] = --p;
        if(visited[i][j]==0) total--;
        visited[i][j]++;
        if(rec) record.push(home.top());
        if(!rec){
            for(unsigned int k = 0; k < moves.size() ;k++){
                R = moves[k].x;
                C = moves[k].y;
                if(i==a && j==b) break;
                if((i+R)<0 || (i+R)>rows-1 || (j+C)<0 ||(j+C)>cols-1 || graph[i+R][j+C]!=0){continue;}
                if(dist[i+R][j+C] == dist[i][j]+1 && visited[i+R][j+C]==0){
                    record.push(edge(i+R,j+C));
                    record.push(edge(i,j));
                    rec=true;
                    break;
                }
            }
        }
        if(home.size()==1){
            rec = false; out=true;
            arr[2] = pace;
        }
        return arr;
    }
    //pop out the record path
    if(!record.empty() && out){
        if(record.top().first==a && record.top().second==b) record.pop();
        home.push(record.top());
        steps.push_back(record.top());
        arr[0] = i = record.top().first;
        arr[1] = j = record.top().second;
        arr[2] = --p;record.pop();
        if(visited[i][j]==0 && graph[i][j]==0) total--;
        visited[i][j]++;
        return arr;
    }
    //if this tree is done ,go to another tree
    if(record.empty() && out && visited[_firstRow][_firstCol] !=0 && go){
        if(i==a && j==b){
            anotherTree(_firstRow,_firstCol); 
            out = false;
            home.pop();
            steps.push_back(home.top());
            arr[0] = i = home.top().first;
            arr[1] = j = home.top().second;
            arr[2] = --p;
            visited[i][j]++;
            return arr;
        }
    }
    //find new path
    bool walk = false;
    struct direc visits(0,0);
    for(unsigned int k = 0; k < moves.size() ;k++){
        R = moves[k].x;
        C = moves[k].y;
        if((i+R)<0 || (i+R)>rows-1 || (j+C)<0 ||(j+C)>cols-1 || graph[i+R][j+C]==1){continue;}
        if(dist[i+R][j+C] == dist[i][j]+1){
            if(walk == false && visited[i+R][j+C]==0){
                visited[i+R][j+C] = 1; total--;
                if(dist[i+R][j+C]==1){
                    _firstRow = i+R;
                    _firstCol = j+C;
                    go = true;
                } 
                steps.push_back(edge(i+R,j+C));
                home.push(edge(i+R,j+C));
                arr[0] = i+R;
                arr[1] = j+C;
                arr[2] = --p;
                walk = true;
                return arr;
            }
        }
    }
    if(walk==false){
        home.pop(); out= false;
        steps.push_back(home.top());
        arr[0] = i = home.top().first;
        arr[1] = j = home.top().second; visited[i][j]++;
        arr[2] = --p;
        if(rec) record.push(home.top());
        if(!rec){
            for(unsigned int k = 0; k < moves.size() ;k++){
                R = moves[k].x;
                C = moves[k].y;
                if((i+R)<0 || (i+R)>rows-1 || (j+C)<0 ||(j+C)>cols-1 || graph[i+R][j+C]!=0){continue;}
                if(dist[i+R][j+C] == dist[i][j]+1 && visited[i+R][j+C]==0){
                    record.push(edge(i+R,j+C));
                    record.push(edge(i,j));
                    rec=true;
                    break;
                }
            }
        }
    }
    another = true;
    return arr;
}

//go to another tree
void anotherTree(int i,int j){
    int starti =i; int startj = j;
    for(unsigned int k = 0; k < moves.size() ;k++){
        R = moves[k].x;
        C = moves[k].y;
        if((a+R)<0 || (a+R)>rows-1 || (b+C)<0 ||(b+C)>cols-1 || graph[a+R][b+C]==1){continue;}
        if(dist[a+R][b+C] == dist[a][b]+1 && visited[a+R][b+C]==0){
            _firstRow = a+R;
            _firstCol = b+C;
        }
    }
    queue<edge> store;      //BFS
    int **dist1 = allocate_Memory(rows,cols);
    for(int q=0;q<rows;q++){
        for(int w=0;w<cols;w++){
            dist1[q][w]=0;
        }
    }
    store.push(edge(i,j));dist1[i][j] = 1;
    while(!store.empty()){
        i = store.front().first; j =store.front().second; store.pop();
        for(unsigned int k = 0; k < moves.size() ;k++){
            R = moves[k].x;
            C = moves[k].y;
            if((i+R)>=0 && (i+R)<rows && (j+C)>=0 &&(j+C)<cols){
                if(graph[i+R][j+C]==0 && (dist1[i+R][j+C] > dist1[i][j] + 1 || (dist1[i+R][j+C]==0))){
                    dist1[i+R][j+C] = dist1[i][j] + 1;
                    store.push(edge(i+R,j+C));
                }
            }
        }
        if(dist1[_firstRow][_firstCol]!=0) break;
    }

    home.push(edge(_firstRow,_firstCol));
    i =_firstRow; j = _firstCol;
    while(i!=starti || j!= startj){
        for(unsigned int k = 0; k < moves.size() ;k++){
            R = moves[k].x;
            C = moves[k].y;
            if((i+R)>=0 && (i+R)<rows && (j+C)>=0 &&(j+C)<cols){
                if(dist1[i+R][j+C]==dist1[i][j]-1){
                    home.push(edge(i+R,j+C));
                    i = i+R; j = j+C;
                    break;
                }
            }
        }
    }
    return;
}


void find_path(int i,int j,int p){
    home.push(edge(i,j));
    int* now = next(i,j,p);
    while(now[0]!=i ||now[1]!=j || total>0){
        int* nextone = next(now[0],now[1],now[2]);
        now[0] = nextone[0];
        now[1] = nextone[1];
        now[2] = nextone[2];
    }
    return ;
}
