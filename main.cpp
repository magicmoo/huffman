#include <iostream>
#include <map>
#include <queue>
using namespace std;
struct Node{ //哈夫曼树节点
    Node* l, *r;
    char ch;
}; //l is 0,r is 1
struct Temp{ //临时结构体，便于优先级队列进行排序
    Node* data;
    int freq;
    bool operator<(const Temp temp)const{
        return freq > temp.freq;
    }
};
void cal(string str,map<char,int>& mp){ //计算str中各字符出现的次数，并保存在mp中
    for(int i=0;i<str.length();i++){
        mp[str[i]]++;
    }
}
Node* build(map<char,int>& mp){ //通过mp中保存的各字符出现次数建哈夫曼树，返回树根节点指针
    priority_queue<Temp> q; //通过Temp对Node类指针进行排序
    for(auto i=mp.begin();i!=mp.end();i++){
        Node* node = new Node();
        node->ch = i->first; node->l = node->r = NULL;
        q.push({node,i->second});
    }
    while(q.size() > 1){ //当队列中的元素等于1时，将该节点视为根节点         
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
void dfs(Node* node,string code,map<char,string>& mp){ //通过dfs对哈夫曼树进行中序遍历，保存并输出最终各字符编码
    if(node->r==NULL && node->l==NULL){ //当当前节点为叶节点时，其必定代表一个字符，反之亦然
        cout<<node->ch<<": "<<code<<"\n";
        mp[node->ch] = code;
    }else{
        dfs(node->l,code+"1",mp);
        dfs(node->r,code+"0",mp);
    }
}
void encoding(string str,map<char,string> mp){ //通过mp中保存的各字符编码对str进行加密
    for(int i=0;i<str.length();i++)
        cout<<mp[str[i]];
    cout<<"\n";
}
Node* build(map<char,string>& mp,int now){ //通过mp中保存的各字符编码建成哈夫曼树，now表示当前遍历到的哈夫曼树深度
    Node* root = new Node();
    root->l = root->r = NULL; root->ch = '\0';
    if(mp.size() == 1){ //当前mp中保存的数目只有1时，该节点为叶节点
        root->ch = mp.begin()->first;
        return root;
    }
    map<char,string> mp2;
    for(auto i=mp.begin();i!=mp.end();){
        if(i->second[now] == '0'){ //将当前深度编码为0的字符移除mp，加入mp2
            mp2[i->first] = i->second;
            i = mp.erase(i);
        }else i++;
    }
    root->l = build(mp,now+1); //mp中仅剩当前深度编码为1的字符，表示左子树中存在的字符
    root->r = build(mp2,now+1); //mp2中均为当前深度编码为0的字符，表示右子树中存在的字符
    return root;
}
void decoding(string str,Node* root){ //通过哈夫曼树对str中保存的加密文章进行解密
    Node* temp = root;
    string ans;
    for(int i=0;i<str.length();i++){
        if(str[i] == '1') temp = temp->l;
        else if(str[i] == '0') temp = temp->r;
        if(temp->ch != '\0'){ //当前指针的字符不为'\0'时表示已到达叶节点，存下该节点表示的字符并将temp重置为根节点
            ans.push_back(temp->ch);
            temp = root;
        }
    }
    cout<<ans<<"\n";
}
//以下paper为原文章路径，coding为编码路径，encrypted为加密文章路径
void encoding(string paper,string coding,string encrypted){ //加密总函数
    freopen(paper.c_str(),"r",stdin);
    freopen(coding.c_str(),"w",stdout);
    string str;
    map<char,int> mp;
    while( getline(cin,str) ){ //将文章中的字符出现频率一一计算，并存入mp中
        cal(str,mp);
    }
    Node* root = build(mp); //通过mp中保存的字符出现频率进行建树
    map<char,string> mp2;
    dfs(root,"",mp2); //通过dfs将哈夫曼编码存入coding中
    cin.clear(); //清空输入流，方便再次读入
    freopen(paper.c_str(),"r",stdin);
    freopen(encrypted.c_str(),"w",stdout);
    while( getline(cin,str) ){ //再次读入原文章，并通过哈夫曼编码对其进行加密
        encoding(str,mp2);
    }
    freopen("CON","w",stdout); //关闭输出流重定向
}
void decoding(string paper,string coding,string encrypted){ //解密总函数
    freopen(coding.c_str(),"r",stdin);
    freopen(paper.c_str(),"w",stdout);
    string str;
    map<char,string> mp;
    while( getline(cin,str) ){ //读入加密文章，并将其保存编码存入mp中
        mp[str[0]] = str.substr(3);
    }
    Node* root = build(mp,0); //通过mp总保存的哈夫曼编码进行建树
    cin.clear();
    freopen(encrypted.c_str(),"r",stdin);
    while( getline(cin,str) ){ //通过哈夫曼树对加密文章进行编码
        decoding(str,root);
    }
    freopen("CON","w",stdout); //关闭输出流重定向
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