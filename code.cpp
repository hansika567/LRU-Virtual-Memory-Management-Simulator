#include <iostream>
#include <bits/stdc++.h>
using namespace std;
class Process{
public:
    int processID;
    vector<int> pages;
    Process(){}
    Process(int pid, int totalPages){
         processID = pid;
        for (int i = 0; i < totalPages; i++){
            pages.push_back(i);
        }
    }
};
class PageFrame{
public:
    int processID;
    int pageNumber;
    PageFrame *prev;
    PageFrame *next;
    PageFrame(int pid, int page){
        processID = pid;
        pageNumber = page;
        prev = NULL;
        next = NULL;
    }
};
class PageReplacement{
private:
    int capacity;
    int pageHits;
    int pageFaults;
    int replacements;
    PageFrame *head;
    PageFrame *tail;
    unordered_map<string, PageFrame *> pageTable;
    unordered_map<int, Process> processTable;
    vector<PageFrame *> frames;
    string createKey(int pid, int page){
        return to_string(pid) + "_" + to_string(page);
    }
    void insertFront(PageFrame *page){
        PageFrame *temp = head->next;
        head->next = page;
        page->prev = head;
        page->next = temp;
        temp->prev = page;
    }
    void removePage(PageFrame *page){
        page->prev->next = page->next;
        page->next->prev = page->prev;
    }
    void moveToFront(PageFrame *page){
        removePage(page);
        insertFront(page);
    }
    void removeLRUPage(){
        PageFrame *last = tail->prev;
        cout << "\nRAM Full\n";
        cout << "Removing Process "<< last->processID<< " Page "<< last->pageNumber<< endl;
        removePage(last);
        string key = createKey(last->processID,last->pageNumber);
        pageTable.erase(key);
        for (int i = 0; i < frames.size(); i++){
            if (frames[i] == last){
                frames.erase(frames.begin() + i);
                break;
            }
        }
        delete last;
        replacements++;
    }
public:
    PageReplacement(int totalFrames){
        capacity = totalFrames;
        pageHits = 0;
        pageFaults = 0;
        replacements = 0;
        head = new PageFrame(-1, -1);
        tail = new PageFrame(-1, -1);
        head->next = tail;
        tail->prev = head;
    }
    void createProcess(int pid, int totalPages){
        if (processTable.find(pid) != processTable.end()){
            cout << "\nProcess already exists.\n";
            return;
        }
        processTable[pid] = Process(pid, totalPages);
        cout << "\nProcess "<< pid<< " created successfully.\n";
        cout << "Pages : ";
        for (int page : processTable[pid].pages){
            cout << page << " ";
        }
        cout << endl;
    }
    void accessPage(int pid, int page){
        if (processTable.find(pid) == processTable.end()){
            cout << "\nProcess does not exist.\n";
            return;
        }
        if (page < 0 || page >= processTable[pid].pages.size()){
            cout << "\nInvalid Page Number.\n";
            return;
        }
        string key = createKey(pid, page);
        cout << "\nCPU Requested\n";
        cout << "Process : P" << pid << endl;
        cout << "Page    : " << page << endl;
        if (pageTable.find(key) != pageTable.end()){
            cout << "STATUS : PAGE HIT\n";
            pageHits++;
            moveToFront(pageTable[key]);
            return;
        }
        cout << "STATUS : PAGE FAULT\n";
        pageFaults++;
        if ((int)pageTable.size() == capacity){
            removeLRUPage();
        }
        PageFrame *newPage =new PageFrame(pid, page);
        insertFront(newPage);
        pageTable[key] = newPage;
        frames.push_back(newPage);
        cout << "Loaded into RAM.\n";
    }
    void displayRAM(){
        cout << "\ncurrent ram details:\n";
        if(frames.empty()){
            cout << "RAM is Empty.\n";
            return;
        }
        cout << "Frame\tProcess\tPage\n";
        for(int i=0;i<frames.size();i++){
            cout << i << "\tP"<< frames[i]->processID<< "\t"<< frames[i]->pageNumber<< endl;
        }
        cout << "\nFrom Most Recently Used to Least Recently Used:\n";
        PageFrame *temp=head->next;
        while(temp!=tail){
            cout<<"P"<<temp->processID
                <<" Page "
                <<temp->pageNumber
                <<endl;
            temp=temp->next;
        }
    }
    void displayPageTable(){
        cout<<"\nPAGE TABLE:\n";
        if(pageTable.empty()){
            cout<<"No pages in RAM.\n";
            return;
        }
        cout<<"Process\tPage\tStatus\n";
        PageFrame *temp=head->next;
        while(temp!=tail){
            cout<<"P"<<temp->processID <<"\t"<<temp->pageNumber<<"\tIn RAM"<<endl;
            temp=temp->next;
        }
    }
    void displayProcesses(){
        cout<<"\nPROCESSES TABLE:\n";
        if(processTable.empty()){
            cout<<"No processes created.\n";
            return;
        }
        for(auto &p:processTable){
            cout<<"Process P"<<p.first<<" : ";
            for(int page:p.second.pages) {
                cout<<page<<" ";
            }
            cout<<endl;
        }
    }
    void displayStatistics(){
        cout<<"\nSTATISTICS:\n";
        cout<<"Frames           : "<<capacity<<endl;
        cout<<"Page Hits        : "<<pageHits<<endl;
        cout<<"Page Faults      : "<<pageFaults<<endl;
        cout<<"Replacements     : "<<replacements<<endl;
        int totalRequests=pageHits+pageFaults;
        cout<<"Total Requests   : "<<totalRequests<<endl;
    }
    ~PageReplacement(){
        PageFrame *current=head;
        while(current!=NULL){
            PageFrame *next=current->next;
            delete current;
            current=next;
        }
    }
};
int main(){
    int totalFrames;
    cout<<"Enter Number of Frames : ";
    cin>>totalFrames;
    if(totalFrames<=0){
        cout<<"Invalid Number of Frames.\n";
        return 0;
    }
    PageReplacement memory(totalFrames);
    int choice;
    while(true){
        cout<<"\nSELECT:\n";
        cout<<"1. Create Process\n";
        cout<<"2. Access Page\n";
        cout<<"3. Display RAM\n";
        cout<<"4. Display Page Table\n";
        cout<<"5. Display Processes\n";
        cout<<"6. Display Statistics\n";
        cout<<"7. Exit\n";
        cout<<"\nEnter Choice : ";
        cin>>choice;
        switch(choice){
            case 1:{
                int pid;
                int pages;
                cout<<"Enter Process ID : ";
                cin>>pid;
                if(pid<0){
                    cout<<"Invalid Process ID.\n";
                    break;
                }
                cout<<"Enter Number of Pages : ";
                cin>>pages;
                if(pages<=0){
                    cout<<"Invalid Number of Pages.\n";
                    break;
                }
                memory.createProcess(pid,pages);
                break;
            }
            case 2:{
                int pid;
                int page;
                cout<<"Enter Process ID : ";
                cin>>pid;
                if(pid<0){
                    cout<<"Invalid Process ID.\n";
                    break;
                }
                cout<<"Enter Page Number : ";
                cin>>page;
                memory.accessPage(pid,page);
                break;
            }
            case 3:
                memory.displayRAM();
                break;
            case 4:
                memory.displayPageTable();
                break;
            case 5:
                memory.displayProcesses();
                break;
            case 6:
                memory.displayStatistics();
                break;
            case 7:
                cout<<"Exiting...\n";
                return 0;
            default:
                cout<<"Invalid Choice.\n";
        }
    }
}