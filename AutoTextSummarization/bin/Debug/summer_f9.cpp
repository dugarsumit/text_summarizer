/*
basic words has score=0
words in the first line has score=1
nouns and abbrevation has score=2
cue words have score=3
numbers have score=2
non of the above words score=1.5
*/

#include<iostream>
#include<string>
#include<fstream>
#include<map>
#include<ctype.h>

using namespace std;

int total_length=0;//stores the total length of the sentence
int countLine=0;//stores total number of lines in the source text file

/* this structure stores all the info abt a sentence. It is used to make
   linked list of sentences.*/
struct line
{
       char info[1000];//data of the sentence
       float score;//final socre of the sentence
       line* next;//structure pointer pointing to the next node of the list
       int length;//complete length of the sentence
       int linenumber;//line number of the sentence in the source text
       }*start,*ptr,*save;
       
/* used for sorting the linked list of sentences*/      
struct sorter
{
       int linenumber;
       float score;
       };

/* DECLARATIONS FOR THE MAPS */     
map<string,int> basicWord;//declaration for making basic map
map<string,int> firstSentence;//declaration for making first Sentence map
map<string,int> wordFrequency;//declaration for calculating frequency of word in source file
map<string,int> cueWord;//declaration for making cue map
map<string,int> titleWord;//declaration for making title map

/* FUNCTION PROTOTYPES */
void basic_in_map();//prototype of a fun which inserts words into basic map
char* filterword(char extract[]);//removes unwanted symbols from a word and returns a filtered word
line* newnode();//creates and returns a new node of line type
void lineextract(char[]); //extracts a line from a node and stores it in a node
void extraspace(char[]);//adds an extra space at the end of the file...this is necessary to extract the last line
void extractword(line*);//extracts a word from a sentence.
int checknoun(char[]);//returns 1 if the word passed is a noun or abbrevation otherwise returns 0
void first_line_map(char[]);//stores the first line words into a map
void source_in_map(char[]);//stores words in map for calculating frequency
void cue_in_map();//stores cue words in map
void calculateFinalScore(line*,float);//calculates the final socre of the sentence
void sort_fun(sorter[]);//sorts the line_sort_array[]
void print(sorter[],char[],int);//prints the summary
int titlefinder(int,int,line*);//checks whether a word is a title or not
void title_in_map();//stores title words in map
int checkNumeral(char word[]);//calculates the score of numeric characters


/* READS A WORD AND RETURNS 1 IF IT IS NUMBER OTHERWISE RETURNS 0 */
int checkNumeral(char word[])
{
 	char tempword[15];
 	strcpy(tempword,word);
 	int len = strlen(tempword);
	int i=0,flag=0;
 	while(i!=len)
 	{
	 			 if(tempword[i]>=48 && tempword[i]<=57)
	 			 {		
				 		flag = 1;
	 			 		break;
				 }
				 else flag=0;		
     i++;	
 	}
 	return flag;	
}

/* CREATES A MAP OF THE TITLE WORDS PRESENT IN THE FILE titles.txt */
void title_in_map()
{
    char checkarray[15];
    string str;
    ifstream input;
    input.open(".\\titles.txt",ios::in);
    while(!input.eof())
    {
                       checkarray[0]='\0';
                       str.clear();
                       input.getline(checkarray,15);
                       str.insert(0,checkarray);
                       titleWord[str]++;                                   
    }
    input.close();   
}

/* RETURNS 1 IF A TITLE WORD IS FOUND */
int titlefinder(int spacepointer,int charpointer,line* ptr)
{
    //cout<<"!";
    char word[30];
    int i=spacepointer+1,j;
    for(j=0;i<charpointer;j++,i++)
            word[j]=ptr->info[i];
    word[j]='\0';
    map<string,int>::iterator itr=titleWord.find(filterword(word));    
    if(itr!=titleWord.end())
                         return 1;//word found
    else return 0;    
}

/* WRITES THE SUMMARY IN THE OUTPUT FILE 
   line_sort_array : its an array of sorter structure containing the line number of sentences to be written in the summary
   destinationfile : name of the output file */
void print(sorter line_sort_array[],char destinationfile[],int summary_factor)
{
     ofstream output;
     output.open(destinationfile, ios::out);
     line* ptr=new line;
     ptr=start;
     int summary_length=0;
     if(countLine==1)
                     summary_length=countLine;
     else 
                     summary_length=(countLine*summary_factor*10)/100;
     for(int i=0;i<summary_length;i++)
     {
             for(;ptr->next!=NULL;)
             {
                                /* if the line number in the sorted list matches the line number in the complete list of sentence
                                   then write that senetence in the output file */   
                                if(line_sort_array[i].linenumber==ptr->linenumber)
                                {
                                       output<<ptr->info;
                                       //cout<<ptr->info;
                                       ptr=ptr->next;
                                       break;
                                }
                                else
                                ptr=ptr->next;
             }
     }
     output.close();
}

/* SORTS SENTENCES BASED ON THEIR SCORE (IN DESCENDING ORDER) AND THEN TAKE THE TOP 50% SENTENCES AND 
   SORT THEM ON THE BASIS ON THEIR LINE NUMBER (IN ASCENDING ORDER)*/
void sort_fun(sorter line_sort_array[])
{
     for(int i=0;i<countLine;i++)
     {
             for(int j=0;j<i;j++)
             {
                     if(line_sort_array[i].score>line_sort_array[j].score)
                     {
                        sorter temp=line_sort_array[i];
                        line_sort_array[i]=line_sort_array[j];
                        line_sort_array[j]=temp;                                    
                     }
             }
     }
     
      for(int i=0;i<countLine/2;i++)
     {
             for(int j=0;j<i;j++)
             {
                     if(line_sort_array[i].linenumber<line_sort_array[j].linenumber)
                     {
                        sorter temp=line_sort_array[i];
                        line_sort_array[i]=line_sort_array[j];
                        line_sort_array[j]=temp;                                    
                     }
             }
     }
 }

/* CALCULATES THE FINAL SCORE OF EACH SENTENCE BY MULTIPLYING THE AVERAGING FACTOR */
void calculateFinalScore(line* ptr,float average_length)
{
     int currentLength=0;
     currentLength=strlen(ptr->info);
     ptr->score=ptr->score*(average_length/currentLength);
 }
 
/* CREATES A MAP OF THE WORDS PRESENT IN THE SOURCE FILE 
   THIS GIVES THE FREQUENCY OF EACH WORD IN THE FILE*/ 
void source_in_map(char sourcefile[])
{
     ifstream input;
     input.open(sourcefile);
     //ofstream test;
     //test.open("d:\\minor\\test.txt",ios::out);
     //input.open("article.txt");
       char word[30];//stores a word from the sentence
       char ch;
       //int len = strlen(sentence);//stores the length of the sentence
       int charpointer=-1;
       while(!input.eof())
       {
               charpointer++;
               input.get(ch);
               if(ch==' ')
               {
                                word[charpointer]='\0';//to convert word array into string
                                strcpy(word,filterword(word));//filters the word and again stores it into word
         						if((isupper(word[0])))                          
  									word[0]=tolower(word[0]);
  									//test<<word<<endl;
                        			map<string,int>::iterator i= basicWord.find(word);   
                        			if(i!=basicWord.end())
                             		{;}  
                        			else 
                        			{
									 	 string str;
									 	 str.insert(0,word);
									 	 wordFrequency[str]++;
						 			}										                       
                                charpointer=-1;//resets the j for the new word          
               }
               else 
               {
                    word[charpointer]=ch;
               }
} 
//test.close();
 }

/* CREATES A MAP OF WORDS PRESENT IN THE FIRST LINE OF THE SOURCE FILE */
void first_line_map(char sentence[])
{
     //cout<<"!";
        char word[15];//stores a word from the sentence
       int len = strlen(sentence);//stores the length of the sentence
       int j=0;
       for(int i=0;i<len;i++,j++)
       {
               if(sentence[i]==' ' || sentence[i]=='\n') 
               {
                                word[j]='\0';//to convert word array into string
                                strcpy(word,filterword(word));//filters the word and again stores it into word
         						if((isupper(word[0])))                          
  									word[0]=tolower(word[0]);
                        			map<string,int>::iterator i= basicWord.find(word);   
                        			if(i!=basicWord.end())
                             		{}  
                        			else 
                        			{
									 	 string str;
									 	 str.insert(0,word);
									 	 //cout<<str<<endl;
									 	 firstSentence[str]++;
						 			}										                       
                                j=-1;//resets the j for the new word          
               }
               else 
               {
                    word[j]=sentence[i];
               }
} 
 }
 
/* CREATES A MAP OF BASIC WORDS */
void basic_in_map()
{
    char checkarray[15];
    string str;
    ifstream input;
    input.open(".\\common_words.txt",ios::in);
    while(!input.eof())
    {
                       checkarray[0]='\0';
                       str.clear();
                       input.getline(checkarray,15);
                       str.insert(0,checkarray);
                       basicWord[str]++;              
                       
    }
    input.close();   
}

/* CREATES A MAP OF CUE WORDS */
void cue_in_map()
{
    char checkarray[15];
    string str;
    ifstream input;
    input.open(".\\cue_words.txt",ios::in);
    while(!input.eof())
    {
                       checkarray[0]='\0';
                       str.clear();
                       input.getline(checkarray,15);
                       str.insert(0,checkarray);
                       cueWord[str]++;              
                       
    }
    input.close();   
}

/* RETIRNS 1 IF A WORD IS NOUN AND OTHERWISE RETURNS 0 */
int checknoun(char word[])
{
    char tempword[15];
    strcpy(tempword,word);
    if((isupper(tempword[0]))&& strlen(tempword)>3)//greater than 3 for filtering titles
    {                           
                        tempword[0]=tolower(tempword[0]);
                        map<string,int>::iterator i=basicWord.find(tempword); 
                        map<string,int>::iterator i1=cueWord.find(tempword);    
                        if(i!=basicWord.end()|| i1!=cueWord.end())
                             return 0;
                        else return 1;
    }
    else return 0;
}

/* EXTRACTS A WORD AND GIVES A SCORE TO IT */
void extractword(line* ptr)
{
     char sentence[1000];
     strcpy(sentence,ptr->info);
       char word[30];//stores a word from the sentence
       int len = strlen(sentence);//stores the length of the sentence
       int j=0;
       for(int i=0;i<len;i++,j++)
       {
               if(sentence[i]==' '||sentence[i]==char(10))
               {
                                word[j]='\0';//to convert word array into string
                                strcpy(word,filterword(word));//filters the word and again stores it into word
                                j=-1;//resets the j for the new word
                                int noun=checknoun(word);
                                if(isupper(word[0]))//for checking word in other chategories
                                                    word[0]=tolower(word[0]);
                                if(noun==1)
                                          {
                                          //add the noun score to line
                                          map<string,int>::iterator i=wordFrequency.find(word);
                                          float freq=i->second;//freq stores frequency of word in source
                                          
                                          map<string,int>::iterator i1=firstSentence.find(word);
                                          float keyscore=i1->second;//keyscore stores the of word in first sentence
                                          ptr->score=ptr->score+2+(keyscore/10)+(freq/100);//add the noun score to the sentence
                                          }
                                else
                                {
                                //check the word for other category and rate the sentence accordingly
                                //cout<<word<<endl;
                                map<string,int>::iterator i=basicWord.find(word);
                                if(i!=basicWord.end())
             		            {//no change in score coz  word is a basic word
                                }  
            			        else 
                    			{
									 	 map<string,int>::iterator i=cueWord.find(word);
									 	 if(i!=cueWord.end())
									 	 {
                                                             map<string,int>::iterator i=wordFrequency.find(word);
                                                             float freq=i->second;
                                                             ptr->score=ptr->score+2+(freq/100);//if word is a cue word
                                         }
                                         else
                                         {//if the word is a number
                                              int numeral = checkNumeral(word);  
							  		 		    if(numeral)
							  		 		    {   // +2 if numeral
											 		   map<string,int>::iterator i1=firstSentence.find(word);
											 		   float keyscore = i1->second;
											 		   map<string,int>::iterator i=wordFrequency.find(word);
											 		   float freq = i->second;   
											 		   ptr->score=ptr->score+2+(keyscore/10)+(freq/100);   	   		   
								 		   		}
						 		   		     else
						 		   		     {
                                             //if word neither basic nor cue nor noun nor a number
                                             map<string,int>::iterator i1=firstSentence.find(word);
                                             float keyscore=i1->second;//keyscore stores the of word in first sentence
                                             map<string,int>::iterator i=wordFrequency.find(word);
                                             float freq=i->second;
                                             ptr->score=ptr->score+1.5+(keyscore/10)+(freq/100);
                                             }	 	 
						 			}	
                                }
                                 }
                                 }
               else 
               {
                    word[j]=sentence[i];
                }
               }
           
      }

/* CREATES A NEW NODE OF TYPE 'LINE' */
line* newnode()
{
      line* node = new line;
      node->next=NULL;
      node->score=0;
      node->info[0]='\0';
      node->length=0;
      node->linenumber=0;
      return node;
      }
      
/* ADDS AN EXTRA SPACE AT THE END OF THE SOURCE FILE 
   THIS IS DONE TO DETECT THE LAST LINE OF THE FILE */
void extraspace(char sourcefile[])
{
    ofstream inputfile;
    inputfile.open(sourcefile,ios::app);
    //inputfile.open("article.txt",ios::app);
    inputfile.seekp(+1,ios::end);
    inputfile.put(char(32));
    inputfile.seekp(+1,ios::end);
    inputfile.put(char(26));
    inputfile.close(); 
 }
 
/* EXTRACTS A LINE FROM THE SOURCE FILE AND MAKES A LINKED LIST OF THESE LINES. 
   AFTER EXTRACTING THE LINE IT CALLS THE EXTRACT WORD FUNCTION AND DOES THE SCORING */
void lineextract(char sourcefile[])
{
     char heading[1000];
     heading[0]='\0';
     extraspace(sourcefile);
     int line_number=0;//stores the number of lines in a file
     ifstream inputfile;
     inputfile.open(sourcefile,ios::in);
     //inputfile.open("article.txt",ios::in);
     start=NULL;
     ptr=newnode();
     start=ptr;
     char ch;
     int charpointer=-1;//points to the char which is being read fron the sentence
     int spacepointer=0;//points to the previous space
     while(!inputfile.eof())
     {
                            
                            charpointer++;
                            inputfile.get(ch); 
                            if(ch==' ')
                                     {spacepointer=charpointer;}
                            if(ch=='.'||ch=='?'||ch=='!'|| ch=='"')
                            {
                                  ptr->info[charpointer]=ch;
                                  checkspace://goto label
                                  inputfile.get(ch);
                                  if(ch==char(32)||ch==char(10))
                                  {
                                                                
                                                            charpointer++;
                                                           // currentspace=charpointer;
                                                            ptr->info[charpointer]=ch;            
                                                            if(strlen(ptr->info)<10 || titlefinder(spacepointer,charpointer,ptr))
                                                                                    continue;//to include words like hi! , mr. in sentence
                                                          else
                                                          {
                                                              line_number++;
                                                              ptr->linenumber=line_number;//sets line number into its node
                                                              countLine=line_number;//stores count of line
                                                              ptr->length=strlen(ptr->info);//sets line length into its node
                                                              total_length=total_length+ptr->length;//sets the total lenght of the source
                                                              charpointer++;
                                                              ptr->info[charpointer]='\0';//marks the end of the extracted sentence
                                                              //cout<<ptr->info<<"#"<<ptr->linenumber;
                                                              if(line_number==1)
                                                                                {
                                                                                first_line_map(ptr->info);//make a map of words(other than stop words) present in first line
                                                                                }
                                                              charpointer=-1;//resets the char pointer
                                                              save=ptr;
                                                             // cout<<ptr->linenumber<<")"<<ptr->info<<endl;
                                                              ptr=newnode();
                                                              save->next=ptr;
                                                              continue;//after a sentence has been extracted the loop starts again
                                                              }
                                           }
                                  else
                                  {
                                      if(ch=='.'||ch=='?'||ch=='!'|| ch=='"')
                                      {
                                      charpointer++;
                                      ptr->info[charpointer]=ch;
                                      goto checkspace;//goto call if 2 sentence end char come together
                                      }
                                  }                         
                                  }
                            else //if not .,!,?,"
                                 {
                                      //cout<<".";
                                 ptr->info[charpointer]=ch;
                                 //heading[charpointer]=ch;
                                 }
                            if(ch==char(10))
                                 {
                                                 //cout<<"#";
                                                 strcpy(heading,ptr->info);
                                                 heading[charpointer]=' ';
                                                 heading[charpointer+1]='\0';
                                                 //cout<<heading;
                                                 first_line_map(heading);
                                                 cout<<firstSentence["dugar"];
                                                 /*ofstream head;
                                                 head.open("readme.txt");
                                                 head<<heading<<"\n";
                                                 head.close();*/
                                                 }
     }
     inputfile.close();
     float average_length=1;
     average_length=total_length/countLine;
    for(ptr=start;ptr->next!=NULL;)
     {                            
     extractword(ptr);//sentence is passed to extract word
     calculateFinalScore(ptr,average_length);
     ptr=ptr->next;
     }
     //cout<<"1";
}

/* REMOVES UNWANTED CHARACTERS FORM WORDS SO THAT THEY CAN BE SEARCHED IN MAPS */
char* filterword(char extract[])
{
     int len = strlen(extract);
     char temp[50];
     int j=0;
     for(int i=0;i<len;i++){
             if((extract[i]>=48 && extract[i]<=57) || (extract[i]>=65 && extract[i]<=90) || (extract[i]>=97 && extract[i]<=122))
                    {temp[j]=extract[i];j++;}      
                    }
     temp[j]='\0';
     strcpy(extract,temp);
     /*if(extract[0]=='"')
     {
                        for(int i=0;i<len;i++)
                                extract[i]=extract[i+1];  
                        extract[len-1]='\0';                       
     }
     len = strlen(extract);
     if((extract[len-1]>=33 && extract[len-1]<=47) || (extract[len-1]>=58 && extract[len-1]<=64))
                            extract[len-1]='\0';
     if((extract[len-2]>=33 && extract[len-2]<=47) || (extract[len-2]>=58 && extract[len-2]<=64))
                            extract[len-2]='\0';*/
     return extract;
}


int main(int argc,char *argv[])
//int main()
{
      //cout<<"###";
      ofstream log_writer;
    log_writer.open(".\\log.txt",ios::out);
    //system("cls");
    //cout<<argv[1]<<"\n"<<argv[2];
    //cout<<"1";
    //checking if the file is empty
    char ch;
    ifstream filechk;
    //filechk.open("d:\\minor\\article.txt");
    filechk.open(argv[1]);
    filechk.get(ch);
    if(ch==' ')
             {
             filechk.close();
             log_writer<<"2";
             log_writer.close();
             return 0;
             }
    //cout<<"1";
    basic_in_map();
    //cout<<"2";
    title_in_map();
    //cout<<"3";
    //source_in_map("d:\\minor\\article.txt");
    //cout<<"1";
    //lineextract("d:\\minor\\article.txt");
    //cout<<"1";
    source_in_map(argv[1]);
    //cout<<"4";
    lineextract(argv[1]);
    //cout<<"5";
    line* p = new line;
    int i;
    sorter line_sort_array[countLine];
    for(p=start,i=0;p->next!=NULL;i++)
    {
            //cout<<p->linenumber<<p->info<<p->score<<endl;
            line_sort_array[i].linenumber=p->linenumber;
            line_sort_array[i].score=p->score;
            p=p->next;
            }
    sort_fun(line_sort_array);
    //print(line_sort_array,"d:\\minor\\article_summary.txt");
    print(line_sort_array,argv[2],atoi(argv[3]));
    //changes the value of log file
    log_writer<<"1";
    log_writer.close();
    delete p;
    delete ptr;
    delete save;
    start=NULL;
    delete start;
    //cout<<endl<<"lines="<<countLine;
    system("pause");
    return 0;             
}


