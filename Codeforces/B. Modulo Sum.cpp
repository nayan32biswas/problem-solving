#include<cstdio>
#include<vector>
int n,m,a,s,i,b[1005];
std::vector<int>v;
int f(int p){
    if(!b[p])b[p]=1,v.push_back(p);
}
main(){
	scanf("%d%d",&n,&m);
	while(n--){
		scanf("%d",&a);
		s=v.size();
		for(i=0;i<s;i++)f((v[i]+a)%m);
		f(a%m);
		if(b[0])break;
	}
	printf("%d", n);
 	puts(n<0?"NO":"YES");
}
