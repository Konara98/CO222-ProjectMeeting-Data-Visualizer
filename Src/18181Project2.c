#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

//*****************************************************************new data stucture**********************************************************
typedef struct data_{
	char name[80];
	int participants;
	int time;
	int count;
	int rownumber;
	struct data_ *next;
}data;

//********************************************************protocols**************************************************************************
int storedata(char *arr);
void swap(data *one,data *two);
void sortmeeting(data *list);
void sortparticipants(data *list);
void sorttime(data *list);
int numberofrows(int rows, int element);
int maxlenght(data *address,int rows);
void printgraphmeeting(data *address,int rows,int maxlen,int sflag);
void printgraphparticipants(data *address,int rows,int maxlen,int sflag);
void printgraphtime(data *address,int rows,int maxlen,int sflag);
void plotgraph(char *arr,int maxlen,int size,int val);

//*******************************************************global variable**********************************************************************
int element=0;
data *address=NULL;
data *current;

//*******************************************************main funtion*************************************************************************
int main(int argc,char *argv[]){
	int mflag=0,pflag=0,tflag=0,sflag=0,rows=10,maxlen=0;

	//***********************************************error handling***********************************************************************
	static int indicator_m=0,indicator_p=0,indicator_t=0; 
	static int scaled=0;
	static int prevScaled=0;
	
	if(argv[1]==NULL){
		printf("No input files were given\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
		return 0;
	}

	for(int arg=1;arg<argc;arg++){
		int arrlenght=strlen(argv[arg]); //calculate argument length
		prevScaled=scaled;

		//**************************************errors on -m -p -t flags(multiple inputs)*********************************************
		if((argv[arg][0]=='-')&&(argv[arg][1]=='m')){
			if(argv[arg][2]=='\0'){
				//no.of meetings
				if((indicator_p==0)&&(indicator_t==0)){
					//identified -m
					mflag=1;
				}
				else{
					printf("Cannot plot multiple parameters in same graph.\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
					return 0;
				}
				indicator_m++;
			}
			else{
				printf("Invalid options for [%s]\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[arg]);
				return 0;
			}
		}

		else if((argv[arg][0]=='-')&&(argv[arg][1]=='p')){
			if(argv[arg][2]=='\0'){
				//no.of participants
				if((indicator_m==0)&&(indicator_t==0)){
					//identified -p
					pflag=1;
				}
				else{
					printf("Cannot plot multiple parameters in same graph.\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
					return 0;
				}
				indicator_p++;
			}
			else{
				printf("Invalid options for [%s]\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[arg]);
                                return 0;
			}
		}

		else if((argv[arg][0]=='-')&&(argv[arg][1]=='t')){
			if(argv[arg][2]=='\0'){
				//no.of time
				if((indicator_m==0)&&(indicator_p==0)){
					//identified -t
					tflag=1;
				}
				else{
					printf("Cannot plot multiple parameters in same graph.\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
					return 0;
				}
				indicator_t++;
			}
			else{
				printf("Invalid options for [%s]\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[arg]);
                                return 0;
			}
		}

		
		//******************************************************idetify -l flag and errors******************************************************
		else if((argv[arg][0]=='-')&&(argv[arg][1]=='l')){
			if(argv[arg][2]=='\0'){
				int count1,count2,count3=0;
				char numbers[10]={'0','1','2','3','4','5','6','7','8','9'};
				if(argv[arg+1]!=NULL){
					for(count1=0;argv[arg+1][count1]!='\0';count1++){
						for(count2=0;count2<10;count2++){
							if(argv[arg+1][count1]==numbers[count2]){
								count3++;		
							}
						}
					}
				}
				else{
					printf("Not enough options for [%s]\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[arg]);
					return 0;
				}
				if(count1==count3){
					//given nmuber after -l (use as the row numbers)
					rows= atoi(argv[arg+1]);
					arg++;
				}
				else{
					printf("Invalid options for [%s]\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[arg]);
					return 0;
				}
			}
			else{
				printf("Invalid options for [%s]\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[arg]);
                                return 0;
			}
		}
		
		
		//******************************************************identify --scaled******************************************************
		else if((argv[arg][0]=='-')&&(argv[arg][1]=='-')&&(argv[arg][2]=='s')&&(argv[arg][3]=='c')&&(argv[arg][4]=='a')&&(argv[arg][5]=='l')&&(argv[arg][6]=='e')&&(argv[arg][7]=='d')&&(argv[arg][8]=='\0')){
			//call scaled cahrt
			sflag=1;	
			scaled++;	
		}
		
		//******************************************************identify csv files******************************************************		
		else if((argv[arg][arrlenght-4]=='.')&&(argv[arg][arrlenght-3]=='c')&&(argv[arg][arrlenght-2]=='s')&&(argv[arg][arrlenght-1]=='v')){
			if(argv[arg][0]=='-'){					
				//file name cannot starts with -
				printf("Invalid option [%s]\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[arg]);
				return 0;
			}else{
				//read the file
				int valid=storedata(argv[arg]);
				if(valid==0){
					return 0;
				}
			}			
		}

		//******************************************************identify errors like -w -q**********************************************
		else if((argv[arg][0]=='-')&&(argv[arg][1]!='m')&&(argv[arg][1]!='p')&&(argv[arg][1]!='t')&&(argv[arg][1]!='l')){
			if(scaled==prevScaled){
				//errorrs like -q -w 
				printf("Invalid option [%s]\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[arg]);
				return 0;
			}
		}

		else{
			printf("Only .csv files should be given as inputs.\n");
			return 0;
		}

	}
	//**************************************************************************************************************************************
	rows=numberofrows(rows,element);

	//********************************************plot the number of meetings graph*********************************************************
	if(mflag==1){
		//call funtion to sort the data as meetings
		sortmeeting(address);
		maxlen=maxlenght(address,rows);
		printf("\n");
		printgraphmeeting(address,rows,maxlen,sflag);
	}

	//********************************************plot the number of participants graph*****************************************************
	else if(pflag==1){
		//call function to sort the data as participants
		sortparticipants(address);
		maxlen=maxlenght(address,rows);
		printf("\n");
		printgraphparticipants(address,rows,maxlen,sflag);
	}
	//********************************************plot the time graph***********************************************************************
	else if(tflag==1){
		//call function to sort the data as time
		sorttime(address);
		maxlen=maxlenght(address,rows);
		printf("\n");
		printgraphtime(address,rows,maxlen,sflag);
	}
	//********************************************plot default meeting graph****************************************************************
	else{
		//call funtion to sort the data as meetings
		sortmeeting(address);
		maxlen=maxlenght(address,rows);
		printf("\n");
		printgraphmeeting(address,rows,maxlen,sflag);
	}

	//free memory allocations
	free(address);
	free(current);
	return 0;
}

//****************************************************store data as linked list***********************************************************
int storedata(char *arr){
	FILE *filepointer;

	filepointer=fopen(arr,"r");

	if(filepointer==NULL){
		printf("Cannot open one or more given files\n");
		return 0;
	}

	char line[300],*pline;
	char name[80];
	int participant;
	int hour,min,sec;

	while(fgets(line,1024,filepointer)){
		pline=line;
		
		//scan the data in csv file
		sscanf(pline,"%[^,],%d,%2d:%2d:%2d",name,&participant,&hour,&min,&sec);
			
		if(element==0){
			current=(data*)malloc(sizeof(data));															
			strcpy(current->name,name);
			current->participants=participant;
			current->time=hour*60+min;
			current->count=1;
			current->rownumber=element;

			current->next=address;
			address=current;
			element++;
		}
		else{
			int checker=0;
			for(current=address;current!=NULL;current=current->next){
				if(strcmp(current->name,name)==0){
					current->participants+=participant;
                                       	current->time+=hour*60+min;
                                       	current->count+=1;

					checker=1;
					break;	
				}
			}
			if(checker==0){
				current=(data*)malloc(sizeof(data));
				strcpy(current->name,name);
                               	current->participants=participant;
                               	current->time=hour*60+min;
                               	current->count=1;
                               	current->rownumber=element;

                               	current->next=address;
                               	address=current;
				element++;
			}
		}
	}
	
	fclose(filepointer);
	return 1;
}

//******************************************************************sawap two elements****************************************************************
void swap(data *one,data *two){
	data temp;
	
	strcpy(temp.name,one->name);
	temp.count=one->count;
	temp.participants=one->participants;
	temp.time=one->time;
	temp.rownumber=one->rownumber;

	strcpy(one->name,two->name);
        one->count=two->count;
        one->participants=two->participants;
        one->time=two->time;
        one->rownumber=two->rownumber;	

	strcpy(two->name,temp.name);
        two->count=temp.count;
        two->participants=temp.participants;
        two->time=temp.time;
        two->rownumber=temp.rownumber;
}

//*****************************************************************sort data as number of meeting*****************************************************
void sortmeeting(data *list){
	data *temp,*temp2;
	for(temp=list;temp->next!=NULL;temp=temp->next){
		for(temp2=temp->next;temp2!=NULL;temp2=temp2->next){
			if(temp->count<temp2->count){
				swap(temp,temp2);
			}
			else if(temp->count==temp2->count){
				if(temp->rownumber>temp2->rownumber){
					swap(temp,temp2);
				}
			}
		}
	}
}

//****************************************************************sort data as number of participants************************************************
void sortparticipants(data *list){
        data *temp,*temp2;
        for(temp=list;temp->next!=NULL;temp=temp->next){
                for(temp2=temp->next;temp2!=NULL;temp2=temp2->next){
                        if(temp->participants<temp2->participants){
                                swap(temp,temp2);
                        }
                        else if(temp->participants==temp2->participants){
                                if(temp->rownumber>temp2->rownumber){
                                        swap(temp,temp2);
                                }
                        }
		}
	}
}

//***************************************************************sort data as time taken for all of meeting******************************************
void sorttime(data *list){
        data *temp,*temp2;
        for(temp=list;temp->next!=NULL;temp=temp->next){
                for(temp2=temp->next;temp2!=NULL;temp2=temp2->next){
                        if(temp->time<temp2->time){
                                swap(temp,temp2);
                        }
                        else if(temp->time==temp2->time){
                                if(temp->rownumber>temp2->rownumber){
                                        swap(temp,temp2);
                                }
                        }
                }
        }
}          

//****************************************************************number of rows shold print*********************************************************
int numberofrows(int rows, int element){
	if(rows<=element){
		return rows;
	}
	else{
		return element;
	}
}

//***************************************************************name which has maximum lenght*******************************************************
int maxlenght(data *address,int rows){
	int i=0,max=0;
	data *current;

	for(current=address,i=0;i<rows;current=current->next,i++){
		if(max<strlen(current->name)){
			max=strlen(current->name);
		}
	}
	return max;
}

//****************************************************************print the meeting graph************************************************************
void printgraphmeeting(data *address,int rows,int maxlen,int sflag){
	int area,total=0,size,i,j,maxnum;
	data *current,*temp;

	j=address->count;
	maxnum=0;
	while(j!=0){
		j=j/10;
		maxnum++;
	}

	area=80-(maxlen+3+maxnum);
	for(current=address;current!=NULL;current=current->next){
		total=total+current->count;	
	}

	if(sflag==0){
		for(current=address,i=1;i<=rows;current=current->next,i++){
			size=(current->count)*area/total;
			plotgraph(current->name,maxlen,size,current->count);
		}
		for(i=0;i<maxlen+2;i++){
                	printf(" ");
        	}
		printf("\u2514");
		for(i=1;i<=area+maxnum;i++){
			printf("\u2500");
		}
		printf("\n");	
	}
	else{
		temp=address;
		for(current=address,i=1;i<=rows;current=current->next,i++){
			if(i==1){
				size=area;
			}
			else{
                        	size=(current->count)*area/(temp->count);
			}
                        plotgraph(current->name,maxlen,size,current->count);
                }
                for(i=0;i<maxlen+2;i++){
                        printf(" ");
                }
                printf("\u2514");
                for(i=1;i<=area+maxnum;i++){
                        printf("\u2500");
                }
                printf("\n");

	}
}

//****************************************************************print the participants graph************************************************************
void printgraphparticipants(data *address,int rows,int maxlen,int sflag){
        int area,total=0,size,i,j,maxnum;
        data *current,*temp;

	j=address->participants;
        maxnum=0;
        while(j!=0){
                j=j/10;
                maxnum++;
        }
        area=80-(maxlen+3+maxnum);
        for(current=address;current!=NULL;current=current->next){
                total=total+current->participants;
        }

        if(sflag==0){
                for(current=address,i=1;i<=rows;current=current->next,i++){
                        size=(current->participants)*area/total;
                        plotgraph(current->name,maxlen,size,current->participants);
                }
                for(i=0;i<maxlen+2;i++){
                        printf(" ");
                }
                printf("\u2514");
                for(i=1;i<=area+maxnum;i++){
                        printf("\u2500");
                }
                printf("\n");
        }
        else{
                temp=address;
                for(current=address,i=1;i<=rows;current=current->next,i++){
                        if(i==1){
                                size=area;
                        }
                        else{
                                size=(current->participants)*area/(temp->participants);
                        }
                        plotgraph(current->name,maxlen,size,current->participants);
                }
                for(i=0;i<maxlen+2;i++){
                        printf(" ");
                }
                printf("\u2514");
                for(i=1;i<=area+maxnum;i++){
                        printf("\u2500");
                }
                printf("\n");

        }
}

//****************************************************************print the time graph************************************************************
void printgraphtime(data *address,int rows,int maxlen,int sflag){
        int area,total=0,size,i,j,maxnum;
        data *current,*temp;

	j=address->time;
        maxnum=0;
        while(j!=0){
                j=j/10;
                maxnum++;
        }
        area=80-(maxlen+3+maxnum);
        for(current=address;current!=NULL;current=current->next){
                total=total+current->time;
        }

        if(sflag==0){
                for(current=address,i=1;i<=rows;current=current->next,i++){
                        size=(current->time)*area/total;
                        plotgraph(current->name,maxlen,size,current->time);
                }
                for(i=0;i<maxlen+2;i++){
                        printf(" ");
                }
                printf("\u2514");
                for(i=1;i<=area+maxnum;i++){
                        printf("\u2500");
                }
                printf("\n");
        }
        else{
                temp=address;
                for(current=address,i=1;i<=rows;current=current->next,i++){
                        if(i==1){
                                size=area;
                        }
                        else{
                                size=(current->time)*area/(temp->time);
                        }
                        plotgraph(current->name,maxlen,size,current->time);
                }
                for(i=0;i<maxlen+2;i++){
                        printf(" ");
                }
                printf("\u2514");
                for(i=1;i<=area+maxnum;i++){
                        printf("\u2500");
                }
                printf("\n");
        }
}

//**************************************************************plot each row of graph**************************************************************
void plotgraph(char *arr,int maxlen,int size,int val){
	int  num,i;
	num=maxlen+2;

	//first line
	for(i=0;i<maxlen+2;i++){
		printf(" ");
	}
	printf("\u2502");
	for(i=0;i<size;i++){
		printf("\u2591");
	}
	printf("\n");

	//second line
	printf(" %s",arr);
	for(i=0;i<num-strlen(arr)-1;i++){
		printf(" ");
	}
	printf("\u2502");
	for(i=0;i<size;i++){
                printf("\u2591");
        }
	printf("%i\n",val);
        
	//third line
	for(i=0;i<maxlen+2;i++){
                printf(" ");
        }
        printf("\u2502");
        for(i=0;i<size;i++){
                printf("\u2591");
        }
        printf("\n");

	//fourth line
	for(i=0;i<maxlen+2;i++){
                printf(" ");
        }
        printf("\u2502");
	printf("\n");

}
