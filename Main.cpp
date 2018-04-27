#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<ctime>
#include<unistd.h>
#include<fstream>
#include<string.h>
#include<sstream>
using namespace std;

const double DIFF=1814400;

int getStringValue(string str){
	stringstream stream(str);
	int ret=0;
	stream>>ret;
	return ret;
}

struct customer{
	string name;
	int id;
	time_t time;
};

class operations{
	customer *users;
	int size;
	int lastIndex;
 public:
	operations(customer *users,int size){
		this->users=users;
		this->size=size;
		this->lastIndex=size-1;;
		setUser();
	}
	
	void setUser(){
		cout<<"**********Welcome to LPG gas service**********\n";
		cout<<"  Enter your User Status"<<endl;
		cout<<"\tEnter 1 for existing user\n";
		cout<<"\tEnter 2 for new user\n";
		cout<<"Your choice : ";
		string us;
		int i=0;
		do{
			if(i!=0){
				cout<<"You have left with "<<3-i<<" choices\n";
				cout<<"Press Ctrl+C to exit \n";
				cout<<"Please enter a valid code : ";
			}
			cin>>us;
			i++;
			if(i==3){
				return;
			}	
		}while(i<3&&(us!="1"&&us!="2"));
		if(us=="1"){
			setRequest();
		}
		else{
			addUser();
			setRequest();
		}
		
	}
	void addUser(){
		cout<<"\n\n------Welcome New User------\n\tEnter your name : ";
		char gar[1];
		string name;
		cin.getline(gar,1);
		getline(cin,name);
		int newId;
		if(size==0){ newId=1111; }
		else{	newId=users[lastIndex].id+1; }
		lastIndex++;
		cout<<name<<", this is your new id "<<newId<<endl;
		ofstream out;
		out.open("record",ios::app);
		out<<newId<<" ";
		out<<0<<" ";
		out<<name<<endl;
		users[lastIndex].id=newId;
		users[lastIndex].name=name;
		users[lastIndex].time=0;
		size++;
		out.close();
	}

	void setRequest(){
		cout<<"\n\n----------------------------------------------\n";
		int id,i=0;
		string name,idStr;
            	char isC;
		string invalid="Invalid Id";
		do{
			if(i>0){
				cout<<"Try again, you have left with "<<3-i<<" chances \n";
				cout<<"Press Ctrl+Z to exit\n";
			}
			cout<<"Enter your id : ";
			cin>>idStr;
			id=getStringValue(idStr);
                	name=getName(id);
			if(name==invalid){
				cout<<name<<endl;
				isC='n';
			}
			else{
                		cout<<"Check your name please "<<name<<endl;
				cout<<"Enter any character if its your account or 'n' if its not : ";
				cin>>isC;
			}
			
			i++; 
		}while(i<3&&(isC=='n'||isC=='N'));
		
		//setting request
		int re=0;
		cout<<"\n\n----Enter your Request----"<<endl;
                cout<<"\tEnter 1 for Booking\n";
                cout<<"\tEnter 2 for Complaints\n";
                cout<<"Your choice : ";
                string us;
                i=0;
                do{
                        if(i!=0){
                                cout<<"You have left with "<<3-i<<" choices\n";
                                cout<<"Press Ctrl+C to exit \n";
                                cout<<"Please enter a valid code : ";
                        }
                        cin>>us;
                        i++;
                        if(i==3){
                                return;
                        }
                }while(i<3&&(us!="1"&&us!="2"));
			
		int ind=getIndex(id);
		if(us=="1"){
			setBooking(ind);
		}
		else{
			setComplaint(ind);
		}
		
	}

	void setBooking(int ind){
		cout<<"-----------Welcome to Booking Section-----------"<<endl<<endl;
		time_t now=time(0);
		time_t prev=users[ind].time;
		string ti;
		double diff=difftime(now,prev);
		diff=diff<0?-diff:diff;
		if(diff<DIFF){
			ti=ctime(&prev);
			cout<<"Sorry, you cannot book a gas. You have already booked one in recent 21 days on : "<<ti;
			return;
		}
		cout<<"Redirecting you to Payments Section..."<<endl;
		usleep(5000000);
		setTime(ind,now);
		setPayment(now);
	}

	void setPayment(time_t now){
		cout<<"\n\n**********Welcome to Payments**********\n Enter 1 for cash, 2 for card: ";
		string us,ti;
                int i=0;
                do{
                        if(i!=0){
                                cout<<"You have left with "<<3-i<<" choices\n";
                                cout<<"Press Ctrl+C to exit \n";
                                cout<<"Please enter a valid code : ";
                        }
                        cin>>us;
                        i++;
                        if(i==3){
                                return;
                        }
                }while(i<3&&(us!="1"&&us!="2"));

		now+=(2*86400);
                ti=ctime(&now);
                srand(time(NULL));

		if(us=="1"){
			cout<<"You can pay the cash when the gas is delivered to your house to the delivery man\n--------Thank you for using our service----------\n";
			cout<<"Successfully booked a gas, you will be receiving it by "<<ti;
			cout<<"Your booking ID: "<<rand()<<endl<<endl;
			return;
		}
		long long cardNumber;
		cout<<"\n***********Welcome to card transactions***********\n";
		i=0;
		do{
			if(i!=0){
				cout<<"Invalid number, try again\n";
			}
			cout<<"Enter your 16 digit card number: ";
			cin>>cardNumber;
			i++;
		}while(getNumberOfDigits(cardNumber)!=16);
		
		i=0;
		string expiry;
		int month,year;
		do{
			if(i!=0){
				cout<<"Invalid date, try again\n";
			}
			cout<<"Enter the month and year of card expiry in the format \"mm/yyyy\": ";
			cin>>expiry;
			int delim=expiry.find("/");
			month=getStringValue(expiry.substr(0,delim));
			expiry.erase(0,delim+1);
			year=getStringValue(expiry);
			i++;
		}while(!isValidDate(month,year)); 
		
		i=0;
		int cvv;
		do{
			if(i!=0){
				cout<<"Invalid CVV, try again\n";
			}
			cout<<"Enter the 3 digit CVV number of your card: ";
			cin>>cvv;
			i++;
		}while(getNumberOfDigits(cvv)!=3);
		
		cout<<"*********Payment Successful*********\n";
		cout<<"Successfully booked a gas, you will be receiving it by "<<ti;
                cout<<"Your booking ID: "<<rand()<<endl<<endl;
		cout<<"--------Thank you for using our service--------\n";
	}
	
	bool isValidDate(int month,int year){
		if(month<1||month>12){ return false; }
		if(getNumberOfDigits(year)!=4){ return false; }
		time_t now=time(0);
		tm *ltm=localtime(&now);
		int presentYear=1900+ltm->tm_year;
		if(presentYear<year){
			return true;
		}else if(presentYear==year){
			return (ltm->tm_mon<month);
		}
		return false;		
	}	
	int getNumberOfDigits(long long number){
		int a=0;
		while(number>0){
			number=number/10;
			a++;
		}
		return a;
	}

	void setTime(int ind,time_t now){
		ofstream out;
		out.open("datebuffer");
		ifstream in;
		in.open("record");
		int i=-1;
		int id,ti;
		string name;
		while(i<size){
			i++;
			in>>id;
			in>>ti;
			getline(in,name);
			out<<id<<" ";
			if(i!=ind){
				out<<ti<<"";
			}
			else{
				out<<now<<"";
			}
			out<<name<<endl;
		}
		out.close();
		in.close();
		out.open("record");
		in.open("datebuffer");
		i=0;
		while(i<size){
			in>>id;
			in>>ti;
			getline(in,name);
			out<<id<<" ";
			out<<ti<<"";
			out<<name<<endl;
			i++;
		}
		in.close();
		out.close();
		users[ind].time=now;
	}	

	void setComplaint(int ind){
		string complaint;
		cout<<"\n\n--------Welcome to complaints section--------\n";
		cout<<"Please enter your complaint briefly:\n";
		char wa;
		cin>>wa;
		getline(cin,complaint);
		cout<<users[ind].name<<" sorry for the inconvinence, we will clear your request asap.\n";
		cout<<"************Thank you for using our service************\n";
	}


	int getIndex(int id){
		int i=-1;
		bool invalidId=true;
		while(i<size){
			i++;
			if(id==users[i].id){
				invalidId=false;
				break;
			}
		}
		return invalidId?-1:i;
	}

	string getName(int id){
		int i=getIndex(id);
		string name;
		if(i==-1){
                        return "Invalid Id";
                }
		return users[i].name;
	}
};

int main(){
	customer c[100];
	ifstream in("record");
	int i=0;
	while(in>>c[i].id){
		in>>c[i].time;
		getline(in,c[i].name);
		i++;
	}
	in.close();
	operations op(c,i);
}
