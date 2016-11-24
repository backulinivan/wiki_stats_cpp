#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class WikiGraph{

    unsigned int pages, n_links;

    string* titles;
    unsigned int* sizes;
    unsigned int* links;
    unsigned int* redirect;
    unsigned int* offset;
    unsigned int* num_of_links_from;
    unsigned int* num_of_links_to;
    unsigned int* num_of_redirects_to;


public:
    WikiGraph(string filename){
        ifstream file(filename);
        file >> pages >> n_links;

        titles = new string[pages];
        sizes = new unsigned int[pages];
        links = new unsigned int[n_links];
        redirect = new unsigned int[pages];
        offset = new unsigned int[pages+1];
        num_of_links_from = new unsigned int[pages];
        num_of_links_to = new unsigned int[pages];
        num_of_redirects_to = new unsigned int[pages];

        offset[0] = 0;

        for(int i = 0; i < pages; i++){
            string title; file >> title;
            titles[i] = title;
            unsigned int size, redir, num_of_links; file >> size >> redir >> num_of_links;
            sizes[i] = size; redirect[i] = redir; num_of_links_from[i] = num_of_links;
            offset[i+1] = offset[i] + num_of_links;
        }

        cout << pages << n_links;

    }
};




int main(){
    WikiGraph graph("wiki_small.txt");
    return 0;
}

