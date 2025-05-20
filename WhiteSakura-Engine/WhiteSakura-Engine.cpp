#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cctype>
using namespace std;

inline void cleanscreen(){
    cout<<"\033[2J\033[1;1H";
    cout.flush();
}

struct Data
{
    vector<int> saves;
    int all;
    vector<string> name,idx,typ;
};

void run(const Data& data, int start=0);
void loadpage(int from);
void savepage(int current);
void historypage(int current);
void settings();

Data initi()
{
    Data data;
    ifstream ifs("save.wst");
    for(int i=0;i<15;i++)
    {
        int tmp;
        ifs>>tmp;
        data.saves.push_back(tmp);
    }
    ifs.close();
    ifs.open("index.wst");
    ifs>>data.all;
    data.name.resize(data.all);
    data.idx.resize(data.all);
    data.typ.resize(data.all);
    for(int i=0;i<data.all;i++)
        ifs>>data.name[i]>>data.idx[i]>>data.typ[i];
    ifs.close();
    return data;
}

void historypage(int current)
{
    Data data=initi();
    cleanscreen();
    cout<<"对话历史记录：\n";
    for(int i=max(0,current-4);i<=current;i++)
    {
        if(i<data.all){
            if(data.name[i] == "0")
                cout<<">"<<data.idx[i]<<endl;
            else
                cout<<data.name[i]<<">"<<data.idx[i]<<endl;
        }
        else
            cout<<endl;
    }
    cout<<"\n按回车返回...";
    cin.ignore(1024, '\n');
    while(cin.get()!='\n');
}

void savepage(int current)
{
    Data data=initi();
    int page=0;
    
    while(1)
    {
        cleanscreen();
        cout<<"存档页\n";
        
        for(int i=0;i<5;i++)
        {
            int slot=page*5+i;
            cout<<i+1<<". 存档位"<<slot+1<<": ";
            cout<<(data.saves[slot]!=0?"已存":"空")<<endl;
        }
        
        cout<<"<["<<page+1<<"/3]>\n";
        cout<<"<00>返回 <0>上一页 <回车>下一页 <1~5>选择存档位\n";
        
        string input;
        getline(cin,input);
        
        if(input=="00")return;
        else if(input=="0")page=page>0?page-1:page;
        else if(input.empty())page=page<2?page+1:page;
        else if(isdigit(input[0]))
        {
            int choice=stoi(input);
            if(choice>=1&&choice<=5)
            {
                int slot=page*5+(choice-1);
                data.saves[slot]=current;
                
                ofstream ofs("save.wst");
                for(int i=0;i<15;i++)
                    ofs<<data.saves[i]<<" ";
                ofs.close();
                
                cout<<"存档成功！";
                cin.ignore(1024, '\n');
                return;
            }
        }
    }
}

void loadpage(int from)
{
    Data data=initi();
    int page=0;
    vector<int> validSaves;
    for(int i=0;i<15;i++)
        if(data.saves[i]!=0)
            validSaves.push_back(i);
    
    while(1)
    {
        cleanscreen();
        cout<<"读档页\n";
        
        for(int i=0;i<5;i++)
        {
            int globalIdx=page*5+i;
            if(globalIdx>=validSaves.size())
            {
                cout<<endl;
                continue;
            }
            int saveID=validSaves[globalIdx];
            if(data.name[saveID] == "0")
                cout<<i+1<<" "<<data.saves[saveID]<<" >"<<data.idx[saveID]<<endl;
            else
                cout<<i+1<<" "<<data.saves[saveID]<<" "<<data.name[saveID]<<">"<<data.idx[saveID]<<endl;
        }
        
        cout<<"<["<<page+1<<"/3]>\n";
        cout<<"<00>返回 <0>上一页 <回车>下一页 <1~5>读档\n";
        
        string input;
        getline(cin,input);
        
        if(input=="00")return;
        else if(input=="0")page=page>0?page-1:page;
        else if(input.empty())page=page<2?page+1:page;
        else if(isdigit(input[0]))
        {
            int choice=stoi(input);
            if(choice>=1&&choice<=5)
            {
                int globalIdx=page*5+(choice-1);
                if(globalIdx<validSaves.size())
                {
                    run(data,validSaves[globalIdx]);
                    return;
                }
            }
        }
    }
}

void run(const Data& data, int start)
{
    cleanscreen();
    int currentIndex=start;
    cin.ignore(1024, '\n');
    bool firstLine = true;
    
    while(currentIndex<data.all)
    {
        if(!firstLine) cout<<"\n\n\n\n\n\n"; // 六个空行
        firstLine = false;
        
        if(data.name[currentIndex] == "0")
            cout<<">"<<data.idx[currentIndex]<<endl;
        else
            cout<<data.name[currentIndex]<<">"<<data.idx[currentIndex]<<endl;
        
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
    cout<<"剧本播放结束！按回车返回主页";
    cin.ignore(1024, '\n');
    while(cin.get()!='\n');
}

void settings()
{
    cleanscreen();
    cout<<"关于\n白樱引擎v1.0\n使用CUI C++编写\n开源|跨平台|极简\n";
    cin.ignore(1024, '\n');
    while(cin.get()!='\n');
}

int home(const Data& data)
{
    int choice;
    while(1)
    {
        cleanscreen();
        cout<<"白樱之诗v1.0\n当前位于<主页>，请选择接下来的操作:\n";
        int index=1;
        bool hasContinue=!data.saves.empty()&&data.saves[0]!=0;
        vector<int> optionMap(6);

        if(hasContinue)
        {
            cout<<index<<". 继续游戏\n";
            optionMap[index++] = 1;
        }
        cout<<index<<". 开始游戏\n";   optionMap[index++] = 2;
        cout<<index<<". 读档页\n";     optionMap[index++] = 3;
        cout<<index<<". 关于引擎\n";    optionMap[index++] = 4;
        cout<<index<<". 退出游戏\n";    optionMap[index++] = 5;

        cin>>choice;
        int maxOpt=index-1;

        if(cin.fail()||choice<1||choice>maxOpt)
        {
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

int main()
{
    Data data=initi();
    while(1)
    {
        int opt=home(data);
        switch(opt)
        {
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