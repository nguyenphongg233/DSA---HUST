// 23 - 12 - 23 

#include<bits/stdc++.h>

using namespace std;

#define read() ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define day() time_t now = time(0);char* x = ctime(&now);cerr<<"Right Now Is : "<<x<<"\n"

#define ii pair<int,int>
#define X first
#define Y second 

const long long MAX = (int)1e5 + 5;
const long long INF = (int)1e9;
const long long MOD = (int)1e9 + 7;

struct node{
	node *a[53];
	// 0 -> 25 : a 
	// 26 -> 51 : A
	// 52 -> - 
	int cnt = 0;
	node(){
		for(int i = 0;i < 53;i++)a[i] = NULL;
		cnt = 0;
	}
};
node *root;
int get_hash(char x){
	if((x >= 'a') && x <= 'z')return x - 'a';
	if((x >= 'A') && x <= 'Z')return x - 'A' + 26;
	if(x == '-')return 52;
	return -1;
}
char reverse_hash(int hash){
	if(hash == 52)return '-';
	if(hash < 26)return char('a' + hash);
	return char('A' + hash - 26); 
}
void add(string s){
	int n = s.size();
	node *rt = root;
	
	for(int i = 0;i < n;i++)if(get_hash(s[i]) == -1)return;
	for(int i = 0;i < n;i++){
		if(rt -> a[get_hash(s[i])] == NULL){
			rt -> a[get_hash(s[i])] = new node();
		}
		rt = rt -> a[get_hash(s[i])];
	}
	rt -> cnt++;
}
bool check(string s){
	int n = s.size();
	node *rt = root;
	for(int i = 0;i < n;i++){
		if(rt -> a[get_hash(s[i])] == NULL)return 0;
		rt = rt -> a[get_hash(s[i])];
	}
	return rt -> cnt > 0;
}
signed main(){
	
	read();
	root = new node();
	
	ifstream file("words.txt");
	
	string s;
	while(file >> s){
		if(s == "###")break;
		add(s);
	}
	file.close();
	
	while(cin >> s){
		if(s == "###")break;
		if(!check(s))cout << s << "\n";
	}
	
}