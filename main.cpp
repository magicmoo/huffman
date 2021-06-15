#include <iostream>
#include <map>
#include <queue>
using namespace std;
struct Node{ //���������ڵ�
    Node* l, *r;
    char ch;
}; //l is 0,r is 1
struct Temp{ //��ʱ�ṹ�壬�������ȼ����н�������
    Node* data;
    int freq;
    bool operator<(const Temp temp)const{
        return freq > temp.freq;
    }
};
void cal(string str,map<char,int>& mp){ //����str�и��ַ����ֵĴ�������������mp��
    for(int i=0;i<str.length();i++){
        mp[str[i]]++;
    }
}
Node* build(map<char,int>& mp){ //ͨ��mp�б���ĸ��ַ����ִ������������������������ڵ�ָ��
    priority_queue<Temp> q; //ͨ��Temp��Node��ָ���������
    for(auto i=mp.begin();i!=mp.end();i++){
        Node* node = new Node();
        node->ch = i->first; node->l = node->r = NULL;
        q.push({node,i->second});
    }
    while(q.size() > 1){ //�������е�Ԫ�ص���1ʱ�����ýڵ���Ϊ���ڵ�         
        Temp t1 = q.top(); q.pop();
        Temp t2 = q.top(); q.pop();
        Node* node = new Node();
        node->l = t1.data;
        node->r = t2.data;
        node->ch = '\0';
        q.push({node,t1.freq+t2.freq});
    }
    return q.top().data;
}
void dfs(Node* node,string code,map<char,string>& mp){ //ͨ��dfs�Թ�����������������������沢������ո��ַ�����
    if(node->r==NULL && node->l==NULL){ //����ǰ�ڵ�ΪҶ�ڵ�ʱ����ض�����һ���ַ�����֮��Ȼ
        cout<<node->ch<<": "<<code<<"\n";
        mp[node->ch] = code;
    }else{
        dfs(node->l,code+"1",mp);
        dfs(node->r,code+"0",mp);
    }
}
void encoding(string str,map<char,string> mp){ //ͨ��mp�б���ĸ��ַ������str���м���
    for(int i=0;i<str.length();i++)
        cout<<mp[str[i]];
    cout<<"\n";
}
Node* build(map<char,string>& mp,int now){ //ͨ��mp�б���ĸ��ַ����뽨�ɹ���������now��ʾ��ǰ�������Ĺ����������
    Node* root = new Node();
    root->l = root->r = NULL; root->ch = '\0';
    if(mp.size() == 1){ //��ǰmp�б������Ŀֻ��1ʱ���ýڵ�ΪҶ�ڵ�
        root->ch = mp.begin()->first;
        return root;
    }
    map<char,string> mp2;
    for(auto i=mp.begin();i!=mp.end();){
        if(i->second[now] == '0'){ //����ǰ��ȱ���Ϊ0���ַ��Ƴ�mp������mp2
            mp2[i->first] = i->second;
            i = mp.erase(i);
        }else i++;
    }
    root->l = build(mp,now+1); //mp�н�ʣ��ǰ��ȱ���Ϊ1���ַ�����ʾ�������д��ڵ��ַ�
    root->r = build(mp2,now+1); //mp2�о�Ϊ��ǰ��ȱ���Ϊ0���ַ�����ʾ�������д��ڵ��ַ�
    return root;
}
void decoding(string str,Node* root){ //ͨ������������str�б���ļ������½��н���
    Node* temp = root;
    string ans;
    for(int i=0;i<str.length();i++){
        if(str[i] == '1') temp = temp->l;
        else if(str[i] == '0') temp = temp->r;
        if(temp->ch != '\0'){ //��ǰָ����ַ���Ϊ'\0'ʱ��ʾ�ѵ���Ҷ�ڵ㣬���¸ýڵ��ʾ���ַ�����temp����Ϊ���ڵ�
            ans.push_back(temp->ch);
            temp = root;
        }
    }
    cout<<ans<<"\n";
}
//����paperΪԭ����·����codingΪ����·����encryptedΪ��������·��
void encoding(string paper,string coding,string encrypted){ //�����ܺ���
    freopen(paper.c_str(),"r",stdin);
    freopen(coding.c_str(),"w",stdout);
    string str;
    map<char,int> mp;
    while( getline(cin,str) ){ //�������е��ַ�����Ƶ��һһ���㣬������mp��
        cal(str,mp);
    }
    Node* root = build(mp); //ͨ��mp�б�����ַ�����Ƶ�ʽ��н���
    map<char,string> mp2;
    dfs(root,"",mp2); //ͨ��dfs���������������coding��
    cin.clear(); //����������������ٴζ���
    freopen(paper.c_str(),"r",stdin);
    freopen(encrypted.c_str(),"w",stdout);
    while( getline(cin,str) ){ //�ٴζ���ԭ���£���ͨ�����������������м���
        encoding(str,mp2);
    }
    freopen("CON","w",stdout); //�ر�������ض���
}
void decoding(string paper,string coding,string encrypted){ //�����ܺ���
    freopen(coding.c_str(),"r",stdin);
    freopen(paper.c_str(),"w",stdout);
    string str;
    map<char,string> mp;
    while( getline(cin,str) ){ //����������£������䱣��������mp��
        mp[str[0]] = str.substr(3);
    }
    Node* root = build(mp,0); //ͨ��mp�ܱ���Ĺ�����������н���
    cin.clear();
    freopen(encrypted.c_str(),"r",stdin);
    while( getline(cin,str) ){ //ͨ�����������Լ������½��б���
        decoding(str,root);
    }
    freopen("CON","w",stdout); //�ر�������ض���
}
int main(){
    cout<<"1.encoding...\n";
    cout<<"2.decoding...\n";
    int index;
    cin>>index;
    if(index == 1){
        encoding("./paper.txt","./coding.txt","./encrypted.txt");
        cout<<"success\n";
    }else if(index == 2){
        decoding("./paper.txt","./coding.txt","./encrypted.txt");
        cout<<"success\n";
    }else cout<<"input error!\n";
    return 0;
}