#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <vector>
#include <sys/time.h>
#include <sys/param.h>
#include <algorithm>


//#define THREADS 4
//#define RAND_MAX 1000000000
#define MAX_ARGS 3
#define MAX_THREADS 4
#define LENGHT 20000
#define LINE 15
#define T double


using namespace std;

int MyArrayLenght=0;

class Sort{
public:
	int ID;
	int from,lenght,plenght;
	T *data;
	T *pdata;
	bool type=true;

	Sort(int id, int f, int l,T *mydata):ID(id), from(f), lenght(l), data(mydata){
        this->plenght=l-f;
	    this->pdata = new T [this->plenght];
	    }

	void sort(){
	SubArray();
	BubleSort();
	}

	void BubleSort(){
		if(this->type){
			for (int i=0;i<this->plenght;i++)
			{
				for(int j=0;j<this->plenght;j++){
                    if(j!=this->plenght-1)
					if(this->pdata[j]>this->pdata[j+1]){
						swap(this->pdata[j],this->pdata[j+1]);
					}
				}
			}
		}
		else
		{
			for (int i=0;i<this->plenght;i++)
			{
				for(int j=0;j<this->plenght;j++){
                    if(j!=this->plenght-1)
					if(this->pdata[j]<this->pdata[j+1]){
						swap(this->pdata[j],this->pdata[j+1]);
					}
				}
			}
		}
	}

	void SubArray(){
    int j=0;
	for (int i=this->from;i<this->lenght;i++){
        this->pdata[j]=this->data[i];
        //cout<<"pdata: "<<pdata[j]<<" data: "<<data[i]<<endl;
        j++;
	}
	}

	T* GetData(){
	return this->pdata;
	}

	int GetArrayLenght(){
	return this->plenght;
	}

	void Print(){
	    for(int j=0;j<this->plenght;j++){
                cout<<"["<<j<<"]"<<this->pdata[j]<<"\t";
			if((j+1)%LINE==0)cout<<endl;
	    }
    cout<<endl;
	}

	void SetSort(bool t){
	this->type=t;
	}
};

/*class InserSort :public Sort{
public:


};*/

void *MyThread( void *void_arg )
{
    Sort *sort_task = ( Sort * ) void_arg;

    cout<<"Threat started process "<<sort_task<<" with ID: "<<sort_task->ID<<", from: "<<sort_task->from<<" to "<<sort_task->lenght<<endl;


    sort_task->sort();

    cout<<"Thread process "<<sort_task->ID<<" DONE!"<<endl;

    return NULL;
}

void RNG(T *data, int max){
	srand( ( int ) time( NULL ) );
	for (int i=0;i<LENGHT;i++){
    data[i]=rand()%max;
    }
        cout<<"RNG generated "<<LENGHT<<" numbers in range "<<0<<" to "<< max<<endl;
    }

double GetTime( timeval *before, timeval *after )
{
    timeval res;
    res.tv_sec= after->tv_sec- before->tv_sec;
    res.tv_usec= after->tv_usec- before->tv_usec;
    return (res.tv_sec*1000) + (res.tv_usec / 1000);
}

void Line(int n=LINE*2, char c='x'){
for(int i=0;i<n;i++)cout<<c;
cout<<endl;
}


T* SubArrayAndResult(T *arr1, int s1, T *arr2, int s2){
cout<<"Merging "<<arr1<<" + "<<arr2<<endl;
MyArrayLenght=s1+s2;
T* result=new T[MyArrayLenght];

int i=0,j=0,k=0;

        while(i<s1 && j<s2){
            if(arr1[i] < arr2[j] ){
                result[k]=arr1[i];
                i++;k++;
            }
            else{
                result[k]=arr2[j];
                j++;k++;
            }
        }
        while(i < s1)
            result[k++]=arr1[i++];
        while(j < s2)
            result[k++]=arr2[j++];

return result;
}

T* SubTwoArray(T* myarray, T *arr1, int s1, T *arr2, int s2){
cout<<"Completing "<<arr1<<" + "<<arr2<<endl;
int ss=s1+s2;
T *tmp=new T[ss];

int i=0,j=0,k=0;

        while(i<s1 && j<s2){
            if(arr1[i] < arr2[j] ){
                tmp[k]=arr1[i];
                i++;k++;
            }
            else{
                tmp[k]=arr2[j];
                j++;k++;
            }
        }
        while(i < s1)
            tmp[k++]=arr1[i++];
        while(j < s2)
            tmp[k++]=arr2[j++];

T *tmpdata= new T[MyArrayLenght];
for(int i=0;i<MyArrayLenght;i++){
    tmpdata[i]=myarray[i];
}
return SubArrayAndResult(tmp, ss, tmpdata, MyArrayLenght );

}



void SaveToFile(string name, double time, T* data, int dtl){
 ofstream odf;
    cout<<"Saving results to txt file..["<<name<<"]"<<endl;
    odf.open(name.c_str());
    if (odf.is_open())
    {
        for(int i=0; i<dtl;i++)
        {
            odf<<"["<<i<<"]"<<data[i]<<"\t";
			if((i+1)%LINE==0)odf<<endl;
        }
        odf<<"\nProcess time was: "<<time<<" ms"<<endl;
        odf.close();
    }
    else
    {
    cout<<"IO Error!"<<endl;
    return;
    }
}


int main(int argc, char **argv)
{

    /*cout<<"argc:"<<argc<<endl;
    for(int i=0;i<argc;i++){
       cout<<i<<"->argv:"<<argv[i]<<endl;
    }*/


    int *args;
    if(argc>MAX_ARGS){
        cout<<"Arguments overload..";
        return 1;
    }
    else
    {
       args=new int[argc-1];
           for(int i=1;i<argc;i++){
       args[i]=atoi(argv[i]);
       }
    }

    int THREADS=args[1];
    int MAX=args[2];

    if(THREADS>MAX_THREADS){
        cout<<"Threds overload..";
        return 1;
    }
    if(MAX<=0){
        cout<<"Set RNG error..";
        return 1;
    }

    T data[LENGHT];
    T cdata[LENGHT];

    string fileadress;

    timeval time_before, time_after; //Time records


    cout << "Sorter demo:" << endl;
    Line();
    cout << "\t\t***Using one core...->"<<endl;
    int id=0;

    cout << "Generate RNG...." << endl;
    RNG(data,MAX);

    for(int i=0;i<LENGHT;i++){
      cdata[i]=data[i];
    }

    Line();
    cout << "\t***SORT UP***"<<endl;
    gettimeofday( &time_before, NULL );
    Sort Sorter(id++,0, LENGHT, data);
    cout << "Sort...." << endl;
    Sorter.sort();
    gettimeofday( &time_after, NULL );
    cout << "Done.." << endl;
    //Sorter.Print();

    cout<<"\nThis mode take "<<GetTime(&time_before,&time_after)<<" ms."<<endl;
    cout << "Save to file..."<<endl;
    fileadress="SortUp.txt";
    SaveToFile(fileadress, GetTime(&time_before,&time_after), Sorter.GetData(), Sorter.GetArrayLenght());

    Line();

    cout << "\t***SORT DOWN***"<<endl;
    gettimeofday( &time_before, NULL );
    Sorter.SetSort(false);
    cout << "Sort...." << endl;
    Sorter.sort();
    cout << "Done.." << endl;
    gettimeofday( &time_after, NULL );
    //Sorter.Print();

    cout<<"\nThis mode take "<<GetTime(&time_before,&time_after)<<" ms."<<endl;
    cout << "Save to file..."<<endl;
    fileadress="SortDown.txt";
    SaveToFile(fileadress, GetTime(&time_before,&time_after), Sorter.GetData(), Sorter.GetArrayLenght());

    Line();



    cout << "\t\t***Using multi core...->" <<THREADS<<endl;
    vector<pthread_t*>MyThreads;
    vector<Sort*> MySorters;


    cout << "Create task-parts...." << endl;
    for(int i=0;i<THREADS;i++){
        if(i==0)
            MySorters.push_back(new Sort(i,0,LENGHT/THREADS,cdata));
        else
            MySorters.push_back(new Sort(i,(LENGHT/THREADS)*i,(LENGHT/THREADS)*(i+1),cdata));

    cout << "->Create part and thread "<<i+1<<" ...." << endl;
    MyThreads.push_back(new pthread_t());
    }
    cout << "\t***SORT UP***"<<endl;

    cout << "Starting threads!" << endl;
     gettimeofday( &time_before, NULL );
    for(int i=0;i<THREADS;i++){
       pthread_create( MyThreads.at(i), NULL, MyThread, MySorters.at(i));
    }

    cout << "Geting threads results..." << endl;
    for(int i=0;i<THREADS;i++){
       pthread_join( *MyThreads.at(i), NULL );
    }

    /*for(int i=0;i<THREADS;i++){
    MySorters.at(i)->Print();
    }*/

    Line();
    cout << "Merge..."<<endl;

    T *MyArray;

     for(int i=0; i<THREADS-1;i++){
    MyArray=SubTwoArray(MyArray, MySorters.at(i)->GetData(), MySorters.at(i)->GetArrayLenght(), MySorters.at(i+1)->GetData(), MySorters.at(i+1)->GetArrayLenght());
    }
    gettimeofday( &time_after, NULL );
    cout << "Done!"<<endl;


    cout<<"\nThis mode take "<<GetTime(&time_before,&time_after)<<" ms."<<endl;
    Line();

    /*cout << "Print sort merge data..."<<endl;
    for(int i=0; i<MyArrayLenght;i++)
    {
        cout<<"["<<i<<"]"<<MyArray[i]<<"\t";
			if((i+1)%LINE==0)cout<<endl;
    }
    cout<<endl;*/


    cout << "Save to file..."<<endl;
    fileadress="SortUpWithThread.txt";
    SaveToFile(fileadress, GetTime(&time_before,&time_after), MyArray, MyArrayLenght);


    getchar();




    /* printf( "\nMaximum number search using two threads...\n" );
    pthread_t pt1, pt2;
    task_part tp1( 1, 0, my_length / 2, my_array );
    task_part tp2( 2, my_length / 2, my_length - my_length / 2, my_array );
    timeval time_before, time_after;

    // Time recording before searching
    gettimeofday( &time_before, NULL );


    // Threads starting
    pthread_create( &pt1, NULL, my_thread, &tp1 );
    pthread_create( &pt2, NULL, my_thread, &tp2 );

    // Waiting for threads completion
    pthread_join( pt1, NULL );
    pthread_join( pt2, NULL );

    // Time recording after searching
    gettimeofday( &time_after, NULL );

    printf( "The found maximum: %d\n", MAX( tp1.get_result(), tp2.get_result() ) );
    printf( "The search time: %d [ms]\n", timeval_to_ms( &time_before, &time_after ) );

    printf( "\nMaximum number search using one thread...\n" );

    gettimeofday( &time_before, NULL );*/


    return 0;
}
