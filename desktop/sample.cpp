#include "bits/stdc++.h"
using namespace std;

int input()
{
	int t=0;
	bool neg=false;
	char ch=getchar_unlocked();
	while(ch!='-' && (ch<'0' || ch >'9'))
	{
		ch=getchar_unlocked();
	}
	if(ch=='-')
	{
		neg=true;
		while(ch<'0' || ch >'9')
		{
			ch=getchar_unlocked();
		}
	}
	while((ch >= '0' && ch <= '9'))
	{
		t = (t<<3) + (t<<1) + ch - '0';
		ch = getchar_unlocked();
	}
	if(neg)
	{
		t= -1*t;
	}
	
	return t;
}
int key(char ch)
{
	switch(ch)
	{
		case 'c':
		return 0;
		case 'e':
		return 1;
		case 'f':
		return 2;
		case 'h':
		return 3;
	}
}
vector<vector<int> > v;
vector<vector<int> > ans;
char s[1000009];
//std::vector<char> s;
void doit()
{
	

	v.clear();
	v.resize(4);
	for (int i = 0; i < v.size(); ++i)
	{
		v[i].resize(strlen(s),0);
	}
	
	ans.clear();
	ans.resize(16);
	for (int i = 0; i < ans.size(); ++i)
	{
		ans[i].resize(strlen(s),0);
	}
	for (int i = 0; i < strlen(s); ++i)
	{
		if(i!=0)
		{
			for (int j = 0; j < v.size(); ++j)
			{
				v[j][i]=v[j][i-1];
			}

			for (int j = 0; j < 16; ++j)
			{
				ans[j][i]=ans[j][i-1];
			}

			for (int j = 0; j < 4; ++j)
			{
				if(key(s[i])==j)
					continue;
				ans[j*4+key(s[i])][i] +=v[j][i-1];
			}
		}
		v[key(s[i])][i] +=1;
	}

}

void find(char a,char b,int x,int y)
{
	x--;
	y--;
	if (x==0)
	{
		cout<<ans[key(a)*4+key(b)][y]<<"\n";
		return;
	}

	int new_second;

	new_second=v[key(b)][y]-v[key(b)][x-1];
	int old_val;
	old_val=v[key(a)][x-1];
	int diff= old_val*new_second;
	diff +=ans[key(a)*4+key(b)][x-1];

	cout<<(ans[key(a)*4+key(b)][y]-diff)<<"\n";
	return ;
}

int main(int argc, char const *argv[])
{

	//memset(s,'\0',1000009);
	//scanf("%s",s);
	//printf("%lu\n", strlen(s));
	//s.clear();
	memset(s,'\0',1000009);
	char ch;
	ch=getchar_unlocked();
	while(ch!='c'&& ch!='h'&& ch!='e'&& ch!='f')
			ch=getchar_unlocked();
	int k=0;
	while(ch=='c'||ch=='h'||ch=='e'||ch=='f')
	{
			s[k]=ch;
			ch=getchar_unlocked();
			k++;
	}
	
	doit();
	int q;
	/*scanf("%d",&q);*/
	q=input();
	while(q--)
	{
		char a,b;
		int x,y;
		/*scanf(" %c",&a);
		scanf(" %c",&b);
		scanf("%d",&x);
		scanf("%d",&y);*/
		ch=getchar_unlocked();
			while(ch!='c'&& ch!='h'&& ch!='e'&& ch!='f')
				ch=getchar_unlocked();
			//lli k=0;
			while(ch=='c'||ch=='h'||ch=='e'||ch=='f')
			{
				//s_inp[k]=ch;
				a=ch;
				ch=getchar_unlocked();
				//k++;
			}
			while(ch!='c'&& ch!='h'&& ch!='e'&& ch!='f')
				ch=getchar_unlocked();
			//lli k=0;
			while(ch=='c'||ch=='h'||ch=='e'||ch=='f')
			{
				//s_inp[k]=ch;
				b=ch;
				ch=getchar_unlocked();
				//k++;
			}
 
		//cin>>x>>y;
		x=input();
		y=input();
		find(a,b,x,y);
	}

	/*doit(s);
	string s1 = s.substr(3);
	cout<<"New string\n";
	cout<<s1<<"\n";
	doit(s1);*/
	return 0;
}