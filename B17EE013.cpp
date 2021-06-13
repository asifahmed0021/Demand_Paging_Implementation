#include<iostream>
#include<unordered_map>
#include<vector>
#include<bits/stdc++.h>
using namespace std;
int timer=1;

void print_main(vector<pair<int,int> > main_memory){
    cout<<"Main memory"<<endl;
    for(int i=0;i<main_memory.size();i++){
        cout<<main_memory[i].first<<" "<<main_memory[i].second<<endl;
    }
}


struct Page_Table{
    public:
    unordered_map<int,pair<int,int> > entry;
    Page_Table(int m){
        for(int i = 0; i < m; i++){
            entry[i].first = -1;
            entry[i].second = 0;
        }
    }
};

struct TLB{
    public:
    unordered_map<int,int> entry;
    int s;
    int cap;
    TLB(int cap){
        s=0;
        this->cap=cap;
    }

    void add(int page,int frame){
        if(s<cap){
            entry[page]=frame;
            s++;
        }
    }
    void remove(int key){
        entry.erase(key);
        s--;
    }
};

class Node{
    public:
    Node* next;
    int val;
    Node(int val){
        this->val=val;
        this->next=NULL;
    }
};

struct Free_Frame_List{
    public:
    Node* head;
};

void handlePageFault(vector<pair<int,int> > &main_memory,int process_id,int page, TLB &tlb, vector<Page_Table> &page_tables,Free_Frame_List &free_frame_list){
    int min_time=INT_MAX;
    int min_frame_index=0;
    bool isFrameFree=false;
    bool isLocal=false;
    int i=0;
    for(i=0;i<main_memory.size();i++){
        if(main_memory[i].first==-1){
            isFrameFree=true;
            break;
        }
        if(main_memory[i].first==process_id){
            isLocal=true;
            if(main_memory[i].second<min_time){
                min_time=main_memory[i].second;
                min_frame_index=i;
            }
        }
    }
    if(isLocal){
        cout<<"LRU used to replace frame number "<<min_frame_index <<" for Process "<<process_id+1<<" for page number "<<page<<endl;
        main_memory[min_frame_index].first=process_id;
        main_memory[min_frame_index].second=timer++;
        tlb.add(page,min_frame_index);
        tlb.remove(page);
        page_tables[process_id].entry[page].first=min_frame_index;
        page_tables[process_id].entry[page].second=1;
        for(auto pt=page_tables[process_id].entry.begin();pt!=page_tables[process_id].entry.end();pt++){
            if(pt->second.first==min_frame_index) {
                pt->second.first=-1;
                pt->second.second=0;
            }
        }
        return;
    }
    if(isFrameFree){
        cout<<"Free frame used for Process "<<process_id+1<<" for page number "<<page<<" given frame number "<<i<<endl;
        main_memory[i].first=process_id;
        main_memory[i].second=timer++;
        tlb.add(page,i);
        page_tables[process_id].entry[page].first=i;
        page_tables[process_id].entry[page].second=1;
        free_frame_list.head=free_frame_list.head->next;
        return;
    }

    int frame_to_replace=0;
    int max_number=0;
    unordered_map<int,int> mp;
    for(int i=0;i<main_memory.size();i++){
        mp[main_memory[i].first]++;
        if(mp[main_memory[i].first]>max_number) {
            frame_to_replace=i;
            max_number=mp[main_memory[i].first];
        }
    }
    //replace in thrashing
    int pr=main_memory[frame_to_replace].first;
    cout<<"No free frame or local frame found so replacing frame of process P"<<pr+1<<" with process P"<< process_id+1<<" in frame "<<frame_to_replace<<endl; 
    main_memory[frame_to_replace].first=process_id;
    main_memory[frame_to_replace].second=timer++;
    tlb.add(page,frame_to_replace);
    page_tables[process_id].entry[page].first=frame_to_replace;
    page_tables[process_id].entry[page].second=1;
    for(auto pt=page_tables[pr].entry.begin();pt!=page_tables[pr].entry.end();pt++){
        if(pt->second.first==frame_to_replace) {
            pt->second.first=-1;
            pt->second.second=0;
        }
    }


}



int main(){
#ifndef ONLINE_JUDGE
    freopen("Result.txt", "w", stdout);
#endif
    int k,s,max_page_size,f,total_pages;
    cin>>k;
    vector<Page_Table> page_tables;
    cin>>max_page_size;
    cin>>f;
    cin>>s;
    int processes[k];
    int mps[k];
    //initialisng freem frame list
    Free_Frame_List free_frame_list;
    free_frame_list.head=new Node(0);
    Node* it=free_frame_list.head;
    for(int i=1;i<f;i++){
        it->next=new Node(i);
        it=it->next;
    }

    vector<pair<int,int> > main_memory(f);
    //initialising main memory with <-1,-1>
    for(int i=0;i<f;i++){
        main_memory[i].first=-1;
        main_memory[i].second=-1;
    }
    //flling random process ids for execution
    for(int i=0;i<k;i++){
        processes[i]=i;
    }
    random_shuffle(processes,processes+k);

    //filling corresponding number of pages
    for(int i=0;i<k;i++){
        mps[i]=(rand()%(max_page_size)) +1;
        total_pages+=mps[i];
    }

    for(int i = 0; i < k; i++){
        Page_Table curr(mps[i]);
        page_tables.push_back(curr);
    }

    int j=0;
    int curr_allocated=0;
    for(int i=0;i<k;i++){
        curr_allocated+=mps[i]*f/total_pages;
        int u = 0;
        for(;j<curr_allocated;j++){
            main_memory[j].first=i;
            main_memory[j].second=timer++;
            free_frame_list.head=free_frame_list.head->next;
            page_tables[i].entry[u].first=j;
            page_tables[i].entry[u++].second=1;
        }
    }


    for(int i=0;i<k;i++){
        int curr_page_fault_count=0;
        int r=rand()%(8*mps[processes[i]]);
        TLB tlb(s);
        r+=2*mps[processes[i]];
        int reference_string[r];
        for(int l=0;l<r;l++){
            reference_string[l]=rand()%mps[processes[i]];
        }
        for(int l=0;l<r;l++){
            int query_page=reference_string[l];
            if(tlb.entry.find(query_page)==tlb.entry.end()){
                cout<<"Process P" << processes[i]+1 <<":for page reference "<< query_page<<", TLB miss; ";
                if(page_tables[processes[i]].entry[query_page].second){
                    cout<<"page table valid with frame number "<<page_tables[processes[i]].entry[query_page].first<<endl;
                    tlb.add(query_page,page_tables[processes[i]].entry[query_page].first);
                }
                else{
                    curr_page_fault_count++;
                    cout<<"page fault occurred"<<endl;
                    handlePageFault(main_memory,processes[i],query_page,tlb,page_tables,free_frame_list);
                }
            }
            else{
                cout<<"Process P" << processes[i]+1 <<":for page reference "<< query_page<<", TLB hit; with frame number: "<<tlb.entry[query_page]<<endl;
            }
        }
        cout<<"Process P"<<processes[i]+1<<" execution completed with number of page faults= "<<curr_page_fault_count<<endl;
        cout<<".........................................................................................................."<<endl;
    }
    cout<<"All processes executed successfully!"<<endl;

}