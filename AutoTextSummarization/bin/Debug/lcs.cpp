#include<iostream>
#include<math.h>
using namespace std;
char a[100];
char b[100];

void lcs(int m,int n){
         int c[m+1][n+1];
         int i,j;
         for(i=-1;i<m;i++){
             for(j=-1;j<n;j++){
                     if(i==-1) c[0][j+1]=0;
                     else if(j==-1) c[i+1][0]=0;
                     else if(a[i+1]==b[j+1]) c[i+1][j+1]=c[i][j]+1;
                     else c[i+1][j+1]=max(c[i][j+1],c[i+1][j]);
                     }
             }
         cout<<c[i][j];
        }
        
        
int main()
{
    scanf("%s%s",a,b);
     int m=strlen(a);
     int n=strlen(b);
     lcs(m,n);
system("pause");
return 0;
}

