#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stack>
#include <algorithm>
#include <utility>
#include <windows.h>

using namespace std;

#define edge pair<int,int>
struct direc{
    int x, y;
    direc(int X, int Y) : x(X), y(Y) {}
};
int R,C;
int total = 0;
vector<direc> moves;
vector<edge> steps;
stack<edge> home,record;
int a,b;
int rows,cols,pace;
int **graph;
int **dist;
int **visited;
int arr[3];
bool rec = false;bool battery = false;
int **allocate_Memory(int rows,int cols);
void distances(int i ,int j);
void find_path(int p,int k,int pace);

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
    moves.push_back(direc(1,0));
    moves.push_back(direc(-1,0));
    moves.push_back(direc(0,1));
    moves.push_back(direc(0,-1));
    try{
        distances(a,b);
    }catch(const char* msg){
        cout<<msg<<endl;
        return 0;
    }
    //dist[a][b] = 82;

    //find_path
    find_path(a,b,pace);
    cout<<steps.size();
    for (int i = 0; i < steps.size(); i++){
        cout<<steps[i].first<<"/"<<steps[i].second<<" ";
    }
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
        R = moves[k].x;
        C = moves[k].y;
        if((i+R)<0 || (i+R)>rows-1 || (j+C)<0 ||(j+C)>cols-1 ||
                (dist[i+R][j+C]!=0 &&dist[i+R][j+C] < dist[i][j] + 1)){continue;}
        if(!battery && graph[i+R][j+C]==0){
            dist[i+R][j+C] = dist[i][j] + 1;
            if(dist[i+R][j+C]>pace/2) throw "battery is not enough!";
            distances(i+R,j+C);
            battery = true;
            continue;
        }
        if(graph[i+R][j+C]==0){
            if(i == a && j==b) continue; 
            dist[i+R][j+C] = dist[i][j] + 1;
            if(dist[i+R][j+C]>pace/2) throw "battery is not enough!";
            distances(i+R,j+C);
        }
    }
    return ;
}
int* next(int i ,int j,int p){
    //if out of battery,back to start point
    if(p <= pace/2 && home.size()>1){ 
        home.pop();
        steps.push_back(home.top()); 
        arr[0] = i = home.top().first;
        arr[1] = j = home.top().second; 
        arr[2] = --p;
        visited[i][j]++;
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
        if(home.size()==1){
            rec = false;
            arr[2] = pace;
            cout<<arr[2];
        }
        return arr;
    }
    //pop out the record path
    if(!record.empty() && p> pace/2){
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

    //find new path
    bool walk = false;
    int visit = 1000; struct direc visits(0,0);
    for(unsigned int k = 0; k < moves.size() ;k++){
        R = moves[k].x;
        C = moves[k].y;
        if((i+R)<0 || (i+R)>rows-1 || (j+C)<0 ||(j+C)>cols-1 || graph[i+R][j+C]==1){continue;}
        if(dist[i+R][j+C] == dist[i][j]+1){
            if(walk == false && visited[i+R][j+C]==0){
                visited[i+R][j+C] = 1; total--;
                steps.push_back(edge(i+R,j+C));
                home.push(edge(i+R,j+C));
                arr[0] = i+R;
                arr[1] = j+C;
                arr[2] = --p;
                walk = true;
                return arr;
            }else if(walk == false){
                if(visit > visited[i+R][j+C]){
                    visit = visited[i+R][j+C];
                    visits.x = i+R;
                    visits.y = j+C;
                }
            }
        }
    }
    // if(walk==false && home.size()==1){
    //     visited[visits.x][visits.y] ++;
    //     steps.push_back(edge(visits.x,visits.y));
    //     home.push(edge(visits.x,visits.y));
    //     walk = true;
    //     arr[0] = visits.x;
    //     arr[1] = visits.y;
    //     arr[2] = --p;
    //     return arr;
    // }
    if(walk==false){
        home.pop();
        steps.push_back(home.top());
        arr[0] = i = home.top().first;
        arr[1] = j = home.top().second; visited[i][j]++;
        arr[2] = --p;
        cout<<"---------"<<rec<<"+++";
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
    return arr;
}
void find_path(int i,int j,int p){
    home.push(edge(i,j));
    int* now = next(i,j,p); 
    while(now[0]!=i ||now[1]!=j || total>0){
        Sleep(500);
        int* nextone = next(now[0],now[1],now[2]); 
        now[0] = nextone[0];
        now[1] = nextone[1];
        now[2] = nextone[2];
        cout<<"///"<< total <<"++"<<now[2]<<endl;
        for(int ro = 0;ro<rows;ro++){
            for(int co=0;co<cols;co++){
                cout<< visited[ro][co]<<" ";

            }
            cout<<endl;
        }
        
        cout<<"===================="<<endl;
    }
    
    return ;
}
