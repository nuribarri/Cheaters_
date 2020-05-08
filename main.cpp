#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#define HASH_SIZE 1000000009


using namespace std;

struct HashNode
{
    int fileIndex;
    HashNode *next;
};


int hashFunction(string &chunk, int tableSize);
void cleanChunk(string &chunk);

void hashTable(int key, int &fileIndex, vector<HashNode*> &T);


/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


int main(int argc, char *argv[]) {
    //string dir = string("sm_doc_set");
    string dir = argv[1];
    vector<string> files = vector<string>();

    vector<HashNode *> T(HASH_SIZE);             // declare has table as vector T = hash table

    int chunkSize;
    chunkSize = atoi(argv[2]);
    //ut << chunkSize << endl;
    int threshold;
    threshold = atoi(argv[3]);
    //ut << threshold << endl;



    getdir(dir, files);
    files.erase(files.begin(), files.begin() + 2);

    const int fileCount = files.size();


    vector<string> words;


    for (int i = 0; i < files.size(); i++) {

//cout << i << files[i] << endl;


        int fileIndex = i;

        string fileName = "./";

        fileName = fileName + argv[1] + '/' + files[i];

        //fileName = fileName + "sm_doc_set" + '/' + files[i];

        ifstream inFile;
        inFile.open(fileName.c_str());

        string s;
        inFile >> s;

        while (inFile) {

            words.push_back(s);

            inFile >> s;
        }

        int numWords = words.size();
//cout << numWords << endl;

        int tableSize = HASH_SIZE;

        for (int j = 0; j < numWords - 5; j++) {

            string chunk = "";

            for (int k = 0; k < chunkSize; k++) {


                chunk = chunk + words[k];

            }


            words.erase(words.begin());
            cleanChunk(chunk);

            int key;
            key = hashFunction(chunk, tableSize);

            hashTable(key, fileIndex, T);



//cout << key << endl;
        }
//


//cout << chunk << endl;

    }

    //vector<vector<int> > Collisions(fileCount, vector<int> (fileCount, 0));      //2d array as vectors

    int Collisions[fileCount][fileCount];

    for (int i = 0; i < fileCount; i++) {

        for (int j = 0; j < fileCount; j++) {

            Collisions[i][j] = 0;
        }

    }


    HashNode *first;
    HashNode *traverse;


    for (int i = 0; i < HASH_SIZE; i++) {

        if (T[i] != NULL) {

            for (first = T[i]; first->next != NULL; first = first->next) {

                for (traverse = first->next; traverse != NULL; traverse = traverse->next) {


                    int second = traverse->fileIndex;
                    int frst = first->fileIndex;

                    Collisions[frst][second]++;

                }
            }
        }
    }

    vector<vector<int> > sortColls;

    for (int i = 0; i < fileCount; i++) {
        for (int j = 0 ; j < fileCount; j++) {
            if (Collisions[i][j] >= threshold) {
                vector<int> Colls;
                Colls.push_back(Collisions[i][j]);
                Colls.push_back(i + 2);
                Colls.push_back(j + 2);
                sortColls.push_back(Colls);
            }
        }
    }

    for(int i = 0; i < sortColls.size() - 1; i++){
        for(int j = 0; j < sortColls.size() - 1; j++){
            if(sortColls[j][0] < sortColls[j+1][0]){
                vector<int> swap = sortColls[j];
                sortColls[j] = sortColls[j+1];
                sortColls[j+1] = swap;
            }
        }
    }

    for(int i = 0; i < sortColls.size(); i++){

        cout << sortColls[i][0] << ": " << files[sortColls[i][1]] << ", " << files[sortColls[i][2]] << endl;
    }

    //for(int i = 0; i < fileCount; i++){

    //      delete[] Collisions[i];
//    }


//    for (int i = 0; i < fileCount; i++){
//
//        for (int j = 0; j < fileCount; j++){
//
//            if (Collisions[i][j] > threshold){
//
//                cout << Collisions[i][j] << ": " << files[i] << ", " << files[j] << endl;
//            }
//        }
//    }

    return 0;
}


void cleanChunk(string &chunk){

    for(int i = 0; i < chunk.size(); i++){

        if(chunk[i] >= 65 && chunk[i] <= 90){

            chunk[i] = chunk[i] + 32;

        }

        if(chunk[i] < 97 || chunk[i] > 122){

            chunk.erase(chunk.begin() + i);
            i--;
        }
    }
}

int hashFunction(string &chunk, int tableSize){

    int key = 0;
    int m = tableSize;
    int factor = 1;


    for(int i = 0; i < chunk.size(); i++){

        key = key + ((chunk[i]*31)% m);
        factor = (factor * 7) % m;
    }

    key = key % tableSize;

    return key;
}

void hashTable(int key, int &fileIndex, vector<HashNode*> &T){

    if(T[key] == NULL || T[key]->fileIndex != fileIndex){       //making sure it is empty, ignore if already in file

        //cout << fileIndex << endl;
        HashNode *node = new HashNode;
        node->fileIndex = fileIndex;
        node->next = T[key];

        T[key] = node;

    }
}