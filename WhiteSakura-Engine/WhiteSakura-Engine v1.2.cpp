#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cctype>
#include<limits>
#include<algorithm>
#include<sstream>
#include<thread>
#include<chrono>
#include<map>
using namespace std;

int feelnum = 0;

inline void cleanscreen(){
    cout<<"\033[2J\033[1;1H";
    cout.flush();
}

struct Question {
    string identifier;
    vector<pair<string, int>> answers;
};

struct Chapter {
    char type;
    int number;
    string name;
    int startLine;
};

struct Ending {
    int minFeel;
    int maxFeel;
    int startLine;
    int endLine;
};

struct Data
{
    vector<int> saves;
    vector<int> feelnumSaves;
    int all;
    vector<string> name, idx;
    vector<Question> questions;
    vector<Chapter> chapters;
    vector<Ending> endings;
    map<string, string> nameMap;
};

void run(const Data& data, int start=0);
void loadpage(int from);
void savepage(int current);
void historypage(int current);
void settings();
void jumpscreen(const Data& data, int currentIndex);
bool muitlending(const Data& data, int& currentIndex);

string getDisplayName(const Data& data, const string& abbr){
    auto it = data.nameMap.find(abbr);
    return it != data.nameMap.end() ? it->second : abbr;
}

Data initi()
{
    Data data;
    ifstream ifs("save.wst");
    
    string saveLine;
    getline(ifs, saveLine);
    istringstream saveStream(saveLine);
    for(int i=0;i<15;i++){
        int tmp;
        saveStream>>tmp;
        data.saves.push_back(tmp);
    }
    
    if(getline(ifs, saveLine)){
        istringstream feelStream(saveLine);
        for(int i=0;i<15;i++){
            int tmp;
            feelStream>>tmp;
            data.feelnumSaves.push_back(tmp);
        }
    } else {
        data.feelnumSaves.resize(15, 0);
    }
    ifs.close();

    ifstream nameFile("name.wst");
    if(nameFile){
        int total;
        nameFile >> total;
        nameFile.ignore();
        for(int i=0;i<total;i++){
            string line;
            getline(nameFile, line);
            size_t pos = line.find("  ");
            if(pos != string::npos){
                string full = line.substr(0, pos);
                string abbr = line.substr(pos+2);
                data.nameMap[abbr] = full;
            }
        }
    }

    ifs.open("index.wst");
    ifs>>data.all;
    data.name.resize(data.all);
    data.idx.resize(data.all);
    ifs.ignore(numeric_limits<streamsize>::max(), '\n');
    
    ifstream queFile("indexque.wst");
    vector<Question> allQuestions;
    string queLine;
    while(getline(queFile, queLine)){
        istringstream iss(queLine);
        Question q;
        string tmp;
        iss >> q.identifier;
        for(int i=0;i<3;i++){
            string ans;
            int num;
            iss >> ans >> num;
            q.answers.emplace_back(ans, num);
        }
        allQuestions.push_back(q);
    }

    ifstream conFile("indexcon.wst");
    string conLine;
    while(getline(conFile, conLine)){
        istringstream iss(conLine);
        Chapter ch;
        char typeTemp;
        iss >> typeTemp >> ch.number >> ch.name >> ch.startLine;
        ch.type = typeTemp;
        data.chapters.push_back(ch);
    }
    sort(data.chapters.begin(), data.chapters.end(), [](const Chapter& a, const Chapter& b){
        return a.startLine < b.startLine;
    });

    ifstream endFile("muitlending.wst");
    if(endFile){
        int total;
        endFile >> total;
        for(int i=0;i<total;i++){
            Ending e;
            endFile >> e.minFeel >> e.maxFeel >> e.startLine >> e.endLine;
            data.endings.push_back(e);
        }
    }

    for(int i=0;i<data.all;i++){
        string line;
        getline(ifs, line);
        size_t pos = line.find("  ");
        if(pos == string::npos) continue;
        
        data.name[i] = line.substr(0, pos);
        line.erase(0, pos+2);
        
        if(data.name[i].find('&') != string::npos){
            data.name[i].erase(remove(data.name[i].begin(), data.name[i].end(), '&'), data.name[i].end());
            pos = line.find("  ");
            data.idx[i] = line.substr(0, pos);
            for(auto& q : allQuestions){
                if(q.identifier == data.name[i]){
                    data.questions.push_back(q);
                    break;
                }
            }
        } else {
            data.idx[i] = line;
        }
    }
    ifs.close();
    return data;
}

bool muitlending(const Data& data, int& currentIndex){
    for(const auto& e : data.endings){
        if(feelnum >= e.minFeel && feelnum <= e.maxFeel){
            if(currentIndex == e.endLine){
                cleanscreen();
                cout<<"游戏已结束，感谢您的游玩\n";
                cin.ignore(1024, '\n');
                while(cin.get()!='\n');
                return true;
            }
            if(currentIndex < e.startLine){
                currentIndex = e.startLine;
                return false;
            }
        }
    }
    return false;
}

void jumpscreen(const Data& data, int currentIndex){
    static int lastChapter = -1;
    static int lastAct = -1;
    
    Chapter newChapter{'a', 0, "", -1};
    Chapter newAct{'b', 0, "", -1};
    
    for(const auto& ch : data.chapters){
        if(ch.startLine <= currentIndex+1){
            if(ch.type == 'a') newChapter = ch;
            else if(ch.type == 'b') newAct = ch;
        }
    }
    
    if(newChapter.number != lastChapter || newAct.number != lastAct){
        cleanscreen();
        cout<<"*Next scenario\n";
        cout<<"["<<newChapter.number<<"] "<<newChapter.name<<endl;
        cout<<"["<<newAct.number<<"] "<<newAct.name<<endl;
        lastChapter = newChapter.number;
        lastAct = newAct.number;
        this_thread::sleep_for(chrono::seconds(5));
    }
}

void historypage(int current){
    Data data=initi();
    cleanscreen();
    cout<<"对话历史记录：\n";
    for(int i=max(0,current-4);i<=current;i++){
        if(i<data.all){
            string displayName = getDisplayName(data, data.name[i]);
            if(data.name[i] == "0")
                cout<<">"<<data.idx[i]<<endl;
            else
                cout<<displayName<<">"<<data.idx[i]<<endl;
        }
        else
            cout<<endl;
    }
    cout<<"\n按回车返回...";
    cin.ignore(1024, '\n');
    while(cin.get()!='\n');
}

void savepage(int current){
    Data data=initi();
    int page=0;
    
    while(1){
        cleanscreen();
        cout<<"存档页\n";
        
        for(int i=0;i<5;i++){
            int slot=page*5+i;
            cout<<i+1<<". 存档位"<<slot+1<<": ";
            if(data.saves[slot] != 0){
                int line = data.saves[slot];
                string abbr = (line < data.name.size() && line >=0) ? data.name[line] : "未知";
                string name = getDisplayName(data, abbr);
                string idx = (line < data.idx.size() && line >=0) ? data.idx[line] : "未知";
                cout<<name<<">"<<idx<<" 好感度："<<data.feelnumSaves[slot];
            } else {
                cout<<"空";
            }
            cout<<endl;
        }
        
        cout<<"<["<<page+1<<"/3]>\n";
        cout<<"<00>返回 <0>上一页 <回车>下一页 <1~5>选择存档位\n";
        
        string input;
        getline(cin,input);
        
        if(input=="00")return;
        else if(input=="0")page=page>0?page-1:page;
        else if(input.empty())page=page<2?page+1:page;
        else if(isdigit(input[0])){
            int choice=stoi(input);
            if(choice>=1&&choice<=5){
                int slot=page*5+(choice-1);
                data.saves[slot]=current;
                data.feelnumSaves[slot]=feelnum;
                
                ofstream ofs("save.wst");
                for(int i=0;i<15;i++)
                    ofs<<data.saves[i]<<"  ";
                ofs<<"\n";
                for(int i=0;i<15;i++)
                    ofs<<data.feelnumSaves[i]<<"  ";
                ofs.close();
                
                cout<<"存档成功！";
                cin.ignore(1024, '\n');
                return;
            }
        }
    }
}

void loadpage(int from){
    Data data=initi();
    int page=0;
    vector<int> validSaves;
    for(int i=0;i<15;i++)
        if(data.saves[i]!=0)
            validSaves.push_back(i);
    
    while(1){
        cleanscreen();
        cout<<"读档页\n";
        
        for(int i=0;i<5;i++){
            int globalIdx=page*5+i;
            if(globalIdx>=validSaves.size()){
                cout<<endl;
                continue;
            }
            int saveID=validSaves[globalIdx];
            int line = data.saves[saveID];
            string abbr = (line < data.name.size() && line >=0) ? data.name[line] : "未知";
            string name = getDisplayName(data, abbr);
            string idx = (line < data.idx.size() && line >=0) ? data.idx[line] : "未知";
            cout<<i+1<<". 存档位"<<saveID+1<<": "<<name<<">"<<idx<<" 好感度："<<data.feelnumSaves[saveID]<<endl;
        }
        
        cout<<"<["<<page+1<<"/3]>\n";
        cout<<"<00>返回 <0>上一页 <回车>下一页 <1~5>读档\n";
        
        string input;
        getline(cin,input);
        
        if(input=="00")return;
        else if(input=="0")page=page>0?page-1:page;
        else if(input.empty())page=page<2?page+1:page;
        else if(isdigit(input[0])){
            int choice=stoi(input);
            if(choice>=1&&choice<=5){
                int globalIdx=page*5+(choice-1);
                if(globalIdx<validSaves.size()){
                    feelnum = data.feelnumSaves[validSaves[globalIdx]];
                    run(data,validSaves[globalIdx]);
                    return;
                }
            }
        }
    }
}

void run(const Data& data, int start){
    cleanscreen();
    int currentIndex=start;
    cin.ignore(1024, '\n');
    bool firstLine = true;
    static int lastTriggerLine = -1;
    
    while(currentIndex<data.all){
        if(muitlending(data, currentIndex)) return;
        
        if(currentIndex != lastTriggerLine){
            for(const auto& ch : data.chapters){
                if(currentIndex == ch.startLine-1 && ch.startLine != 0){
                    jumpscreen(data, currentIndex);
                    lastTriggerLine = currentIndex;
                    break;
                }
            }
        }
        
        if(!firstLine) cout<<"\n\n\n\n\n\n";
        firstLine = false;

        bool isQuestion = false;
        Question currentQuestion;
        if(currentIndex < data.questions.size()){
            for(const auto& q : data.questions){
                if(q.identifier == data.name[currentIndex]){
                    isQuestion = true;
                    currentQuestion = q;
                    break;
                }
            }
        }

        if(isQuestion){
            cout<<"问题："<<data.idx[currentIndex]<<endl;
            char choice = 'a';
            for(const auto& ans : currentQuestion.answers){
                cout<<choice++<<". "<<ans.first<<endl;
            }
            
            cout<<"请选择答案(a-c): ";
            char input;
            cin>>input;
            cin.ignore(1024, '\n');
            
            if(input >= 'a' && input <= 'c'){
                int idx = input - 'a';
                feelnum += currentQuestion.answers[idx].second;
                cout<<"好感度变化："<<currentQuestion.answers[idx].second<<endl;
                cout<<"当前好感度："<<feelnum<<endl;
            }
            currentIndex++;
            continue;
        }

        string displayName = getDisplayName(data, data.name[currentIndex]);
        if(data.name[currentIndex] == "0")
            cout<<">"<<data.idx[currentIndex]<<endl;
        else
            cout<<displayName<<">"<<data.idx[currentIndex]<<endl;
        
        cout<<"--------------------\n";
        cout<<"[回车]继续 [1]上一句 [2]存档 [3]对话记录 [4]返回主页";
        
        string input;
        getline(cin, input);
        
        int choice = 0;
        if(input.empty()) {
            choice = 0;
        }
        else if(isdigit(input[0])) {
            choice = stoi(input);
            if(choice <1 || choice >4) {
                cleanscreen();
                cout<<"输入无效，请重新选择！\n";
                continue;
            }
        }
        else {
            cleanscreen();
            cout<<"输入无效，请重新选择！\n";
            continue;
        }

        cleanscreen();
        switch(choice)
        {
            case 0:
                currentIndex++;
                break;
            case 1:
                if(currentIndex>0)currentIndex--;
                else cout<<"已经是第一句了！\n";
                break;
            case 2:
                savepage(currentIndex);
                break;
            case 3:
                historypage(currentIndex);
                break;
            case 4:
                return;
        }
    }
    
    cleanscreen();
    cout<<"游戏已结束，感谢您的游玩\n";
    cin.ignore(1024, '\n');
    while(cin.get()!='\n');
}

void settings(){
    cleanscreen();
    cout<<"关于\n白樱引擎v1.2\n使用CUI C++编写\n开源|跨平台|极简\nPowered by tangaaya |丹花小绫\n";
    cin.ignore(1024, '\n');
    while(cin.get()!='\n');
}

int home(const Data& data){
    int choice;
    while(1){
        cleanscreen();
        cout<<"白樱之诗v1.2\n当前位于<主页>，请选择接下来的操作:\n";
        int index=1;
        bool hasContinue=!data.saves.empty()&&data.saves[0]!=0;
        vector<int> optionMap(6);

        if(hasContinue){
            cout<<index<<". 继续游戏\n";
            optionMap[index++] = 1;
        }
        cout<<index<<". 开始游戏\n";   optionMap[index++] = 2;
        cout<<index<<". 读档页\n";     optionMap[index++] = 3;
        cout<<index<<". 关于引擎\n";    optionMap[index++] = 4;
        cout<<index<<". 退出游戏\n";    optionMap[index++] = 5;

        cin>>choice;
        int maxOpt=index-1;

        if(cin.fail()||choice<1||choice>maxOpt){
            cin.clear();
            cin.ignore(1024,'\n');
            cleanscreen();
            cout<<"输入错误，请重新选择!\n";
            continue;
        }

        cin.ignore(1024, '\n');
        return optionMap[choice];
    }
}

int main(){
    Data data=initi();
    while(1){
        int opt=home(data);
        switch(opt){
            case 1: 
                run(data,data.saves[0]);
                break;
            case 2: 
                run(data,0);
                break;
            case 3: 
                loadpage(1);
                break;
            case 4: 
                settings();
                break;
            case 5: 
                return 0;
        }
    }
}