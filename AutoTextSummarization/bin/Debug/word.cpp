#include<iostream>
using namespace std;
char word[501];
#define mod 1000000007
long long int eulerinverse[]={1,1,500000004,166666668,41666667,808333339,301388891,900198419,487524805,831947206,283194722};
long long int fact[501]={1,1};
int alphabets[125]={0};
int main()
{
    int t,n;
    long long int temp=1;
    scanf("%d",&t);
    while(t--){
               cin>>word;
               memset(alphabets,0,sizeof(alphabets));
               temp=1;
               n=strlen(word);
               for(int i=0;i<n;i++){
                       alphabets[word[i]]++;
                       }
               for(int i=2;i<=n;i++){
                       if(fact[i]==0)
                                     fact[i]=(fact[i-1]*i)%mod;
                       }
               for(int i=0;i<125;i++){
                       temp=(temp*eulerinverse[alphabets[i]])%mod;
                       }
               cout<<(fact[n]*temp)%mod<<endl;
               }
system("pause");
return 0;
}
