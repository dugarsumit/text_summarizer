#include<iostream>
#include<math.h>
using namespace std;

int lis(int array[],int n){
        if(n==1) return 1;
        int max=0;
        for(int i=1;i<n;i++){
                int temp=lis(array,i);
                if(array[i-1]<array[n-1]) temp++;
                if(temp>max) {max=temp;cout<<"#"<<max<<endl;}
                }
        return max;
        }
             
int main()
{
    int t,n;
    scanf("%d",&t);
    while(t--){
               scanf("%d",&n);
               int array[n];
               int l[n];
               memset(l,0,sizeof(l));
               for(int i=0;i<n;i++){
                       scanf("%d",&array[i]);
                       }
               int max=0;
              /* max=lis(array,n);
               cout<<max<<endl; */
               //-------------------------
               for(int i=1;i<n;i++){
                       for(int j=0;j<i;j++){
                               if(array[i]>array[j] && l[i]<l[j]+1)
                                                    l[i]=l[j]+1;
                               }
                       }
               for(int i=0;i<n;i++){
                       if(l[i]>max) max=l[i];
                       }
               cout<<max+1<<endl;
               //--------------------------
               
               }
system("pause");
return 0;
}

