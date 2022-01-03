//
//  main.cpp
//  TetravexFinalSolver
//
//  Created by eddy akiki on 03/01/2022.
//
#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <utility>
#include <thread>
#include <mutex>
#define N 5
using namespace std;
using namespace std::chrono;
struct bloque{
    int haut,droite,bas,gauche;
};
mutex m;
vector<thread> threadVect;
bloque tableau[N][N];
vector<bloque> bloques={};
bloque blqN;
void enleve(int haut,int droite,int bas, int gauche) {
    bloques.erase(
        remove_if(bloques.begin(), bloques.end(), [&](bloque const & bloquee) {
            return bloquee.haut == haut && bloquee.droite == droite && bloquee.bas == bas && bloquee.gauche == gauche ;
        }),
        bloques.end());
}
vector<bloque> cherchePossible(int ligne,int colone){
    vector<bloque> possible = {};
    vector<pair<bloque, vector<bloque>::iterator>> possible1 = {};
    if(ligne==0&&colone==0){
        return bloques;
    }
    for( vector<bloque>::iterator itr= bloques.begin();itr!=bloques.end();itr++){
        if(ligne>0&&colone==0){
            if(tableau[ligne-1][colone].bas==itr->haut){
                possible.emplace_back(*itr);
                
            }
        }
        if(ligne>0&&colone>0){
        if(tableau[ligne][colone-1].droite==itr->gauche&&tableau[ligne-1][colone].bas==itr->haut){
            possible.emplace_back(*itr);
        }
        }
        if(ligne==0){
            if(tableau[ligne][colone-1].droite==itr->gauche){
                possible.emplace_back(*itr);
            }
        }
    }
    return possible;
}
void prochain(int ligne,int colone,int &ligneProchaine,int &coloneProchaine){
    int index = N*N;
    for(int  i = ligne*N+colone+1;i<N*N;i++){
        if(tableau[i/N][i%N].droite==0&&tableau[i/N][i%N].haut==0&&tableau[i/N][i%N].bas==0&&tableau[i/N][i%N].gauche==0){
            index =i;
            break;
        }
    }
    ligneProchaine=index/N;
    coloneProchaine=index%N;
}
bool resoudreTetravex(int ligne,int colone){
    if(ligne>N-1){
        return true;
    }
    vector<bloque> possible = cherchePossible(ligne, colone);
    if(possible.size()==0){
        return false;
    }
    bool etat = false;
    for(int i=0;i<possible.size();i++){
        bloque blq = possible[i];
        enleve( blq.haut, blq.droite, blq.bas, blq.gauche);
        int ligneProchaine,coloneProchaine;
        prochain(ligne, colone, ligneProchaine, coloneProchaine);
        m.lock();
        tableau[ligne][colone]=blq;
        m.unlock();
        if(resoudreTetravex(ligneProchaine, coloneProchaine)){
            etat=true;
            break;
        }
        else{
            m.lock();
            tableau[ligne][colone]=blqN;
            m.unlock();
            bloques.emplace_back(blq);
        }
    }
    return etat;
}
void threadExecute(){
    for(unsigned int x =0;x<3;x++){
        threadVect.emplace_back(resoudreTetravex,0,0);
    }
    for(auto& t : threadVect){
        t.join();
    }
}
int main() {
    ifstream fin;
    ofstream fout;
    bloque blq;
        fin.open("/Users/eddyakiki/input5x5.rtf");
        if(!fin){cerr<<"Error"<<endl; return 1;}
    int x,y,z,k;
        while(fin>>x>>y>>z>>k){
            blq.haut=x;
            blq.droite=y;
            blq.bas=z;
            blq.gauche=k;
            bloques.push_back(blq);
            }
    for(int i =0;i<N;i++){
        for(int j=0;j<N;j++){
            tableau[i][j].droite=0;
            tableau[i][j].bas=0;
            tableau[i][j].gauche=0;
            tableau[i][j].haut=0;
        }
    }
    blqN.haut=0;blqN.bas=0;blqN.droite=0;blqN.gauche=0;
    auto start = high_resolution_clock::now();
    //bool ok = resoudreTetravex(0, 0);
    threadExecute();
    auto stop = high_resolution_clock::now();
    cout<<endl;
    for(int i =0;i<N;i++){
        for(int j=0;j<N;j++){
            cout<<tableau[i][j].haut;
            cout<<tableau[i][j].droite;
            cout<<tableau[i][j].bas;
            cout<<tableau[i][j].gauche;
            cout<<endl;
        }
    }
    
    //cout<<ok;
    auto duration = duration_cast<milliseconds>(stop - start);
     
       cout << "Time taken by function: "
            << duration.count() << " microseconds" << endl;
            fout.open("input5x5.rtf"); // opens the file
               if( !fout ) { // file couldn't be opened
                  cerr << "Error: file could not be opened" << endl;
                  exit(1);
               }
               for(int i =0;i<N;i++){
                   for(int j=0;j<N;j++){
                       fout<<tableau[i][j].haut;
                       fout<<tableau[i][j].droite;
                       fout<<tableau[i][j].bas;
                       fout<<tableau[i][j].gauche;
                       fout<<endl;
                   }
               }
               
    return 0;
}
