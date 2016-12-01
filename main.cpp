#include <iostream>
#include <fstream>
#include <queue>


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
    bool* fired_list;

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
        fired_list = new bool[pages];

        offset[0] = 0;
        unsigned int curr_link = 0;

        for(int i = 0; i < pages; i++){
            string title; file >> title;
            titles[i] = title;
            fired_list[i] = false;
            unsigned int size, redir, num_of_links; file >> size >> redir >> num_of_links;
            sizes[i] = size; redirect[i] = redir; num_of_links_from[i] = num_of_links;
            offset[i+1] = offset[i] + num_of_links;

            for(int j = curr_link; j < curr_link + num_of_links; j++){
                unsigned int link; file >> link;
                links[j] = link; num_of_links_to[links[j]] += 1;
                if (redirect[i]){
                    num_of_redirects_to[links[j]] += 1;
                }
            }

            curr_link += num_of_links;
        }

        cout << "Graph has been uploaded." << endl;
    }

    /*unsigned int get_number_of_liks_to(unsigned int id){
        return num_of_links_to[id];
    }

    unsigned int get_number_of_redirects(unsigned int id){
        return num_of_redirects_to[id];
    }

    bool is_redirect(unsigned int id){
        return redirect[id] == 1;
    }

    unsigned int get_page_size(unsigned int id){
        return sizes[id];
    }*/

    string get_title(int id){
        return titles[id];
    }

    int get_id(string title){
        for(int i = 0; i < pages; i++)
            if (title == get_title(i))
                return i;
        cerr << "Page not found!" << endl;
        exit(13);
        return -1;
    }

    unsigned int get_number_of_links_from(unsigned int id){
        return num_of_links_from[id];
    }

    bool check_fired(int id){
        return fired_list[id];
    }

    void set_fired(int id){
        fired_list[id] = true;
    }

    int* get_links_from(int id){
        int link_num = num_of_links_from[id];
        int* links_from = new int[link_num];
        for(int i = 0; i < link_num; i++){
            links_from[i] = links[offset[id] + i];
        }
        return links_from;
    }

    void shortest_path(string start_page, string finish_page){

        int start_id = get_id(start_page);
        int finish_id = get_id(finish_page);
        queue<int> bfs_queue;
        int* shortest_from = new int[pages];

        for(int i = 0; i < pages; i++)
            shortest_from[i] = -1;

        set_fired(start_id);
        bfs_queue.push(start_id);

        while(bfs_queue.size() != 0){
            int current = bfs_queue.front(); bfs_queue.pop();
            int* current_links = get_links_from(current);
            for(int i = 0; i < num_of_links_from[current]; i++){
                int neighbour = current_links[i];
                if(check_fired(neighbour)){
                    set_fired(neighbour);
                    bfs_queue.push(neighbour);
                    shortest_from[neighbour] = current;
                }
                if(current == finish_id)
                    break;
            }
            delete current_links;
        }

        string path = finish_page;
        while(shortest_from[finish_id] != -1){
            path = get_title(shortest_from[finish_id]) + " ---> " + path;
            finish_id = shortest_from[finish_id];
        }

        cout << path;
        delete shortest_from;
    }

};


int main(){
    WikiGraph graph("wiki_small.txt");
    string page1, page2;
    cout << graph.get_title(161);
    cin >> page1 >> page2;
    graph.shortest_path(page1, page2);
    return 0;
}

