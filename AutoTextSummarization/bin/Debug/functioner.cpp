#include<iostream>
#include<map>
#include<string>
#include<fstream>

using namespace std;
map<string,int> prefix;
map<string,int> suffix;
void prefix_in_map()
{
 	 char checkarray[15];
    string str;
    ifstream input;
    input.open(".\\prefix.txt",ios::in);
    while(!input.eof())
    {
                       checkarray[0]='\0';
                       str.clear();
                       input.getline(checkarray,15);
                       str.insert(0,checkarray);
                       prefix[str]++;              
                       
    }
    input.close();
}
void suffix_in_map()
{
 	 char checkarray[15];
    string str;
    ifstream input;
    input.open(".\\suffix.txt",ios::in);
    while(!input.eof())
    {
                       checkarray[0]='\0';
                       str.clear();
                       input.getline(checkarray,15);
                       str.insert(0,checkarray);
                       suffix[str]++;              
                       
    }
    input.close();
}
void stemmer(char word[])
{
 	 char prefixS[6],suffixS[5];
 	 string temp=word;
	 int i=0,j;
	 for(;i<5;i++)
	 {
  	     		  prefixS[i] = word[i];
  	     		  prefixS[i+1] = '\0';
  	     		  map<string,int>::iterator itr=prefix.find(prefixS);    
    			  if(itr!=prefix.end())
                   {    
				   		temp.erase(0,i+1);
				   		break;//word found]
     				}
  	 }
  	 cout<<temp;
  	 int len;
     len = (temp.length())-1; 	
     j= len;
	 for(i=0;j>=0;j--,i++)
	 {
  	     		  suffixS[i] = temp[j];
  	     		  suffixS[i+1] = '\0';
  	     		  map<string,int>::iterator itr=suffix.find(suffixS);    
    			  if(itr!=suffix.end())
                  {
				   					   temp.erase(len-i);
									   break;//word found
				  }
  	 }
	   cout<<endl; 	   
  	 cout<<temp;
}
int main()
{
 	char word[30];
 	prefix_in_map();
 	suffix_in_map();
 	cin>>word;
 	stemmer(word);
 	system("pause");
 	return 0;
}
