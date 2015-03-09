#include "bits/stdc++.h"
using namespace std;
#define lli long long int

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
vector<vector<lli> > v;
vector<vector<lli> > ans;

char s[1000009];

void doit()
{
	
	int n = strlen(s);
	v.clear();
	v.resize(4);
	for (int i = 0; i < v.size(); ++i)
	{
		v[i].resize(n,0);
	}
	
	ans.clear();
	ans.resize(16);
	for (int i = 0; i < ans.size(); ++i)
	{
		ans[i].resize(n,0);
	}
	for (int i = 0; i < n; ++i)
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
		printf("%lld\n",ans[key(a)*4+key(b)][y]);
		return;
	}

	lli new_second;

	new_second=v[key(b)][y]-v[key(b)][x-1];
	lli old_val;
	old_val=v[key(a)][x-1];
	lli diff= old_val*new_second;
	diff +=ans[key(a)*4+key(b)][x-1];

	printf("%lld\n",(ans[key(a)*4+key(b)][y]-diff));
	return ;
}

int main(int argc, char const *argv[])
{

	memset(s,'\0',1000009);
	scanf("%s",s);
	doit();
	int q;
	scanf("%d",&q);
	//cout<<s<<" "<<q<<" ";
	while(q--)
	{
		char a,b;
		int x,y;
		scanf(" %c",&a);
		scanf(" %c",&b);
		scanf("%d",&x);
		scanf("%d",&y);
		//cout<<a<<" "<<b<<" "<<x<<" "<<y<<"\n";
		find(a,b,x,y);
	}

	/*doit(s);
	string s1 = s.substr(3);
	cout<<"New string\n";
	cout<<s1<<"\n";
	doit(s1);*/
	return 0;
}
