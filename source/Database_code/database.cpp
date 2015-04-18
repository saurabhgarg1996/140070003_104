/************************************************************************************
CS 101: LIBRARY HELPMATE FIRE BIRD V BOT
Team ID : 104
Team Members : (I) Saurabh Garg
(II)Shaurya Arya
(III)Pratik Brahma
(IV)Abhin Shah
Filename : DATABASE FOR LIBRARY MANAGEMENT
************************************************************************************/
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include <windows.h>
using namespace std;
HANDLE hPort = CreateFile("COM7", GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); /**to handle port for zigbee*/
DCB dcb; /**Handle is a class predefined in windows.h*/
void delay_ms() /**to wait till user presses any key*/
{
getch();
}
bool writebyte (char* data) /**to write data to Xbee port*/
{
DWORD byteswritten;
if (!GetCommState(hPort, &dcb))
{
cout<<"\n Serial port can't be opened";
return false;
}
dcb.BaudRate = CBR_9600;
dcb.ByteSize = 8;
dcb.Parity = NOPARITY;
dcb.StopBits = ONESTOPBIT;
if (!SetCommState(hPort, &dcb))
return false;
bool retVal = WriteFile(hPort, data, 1, &byteswritten, NULL);
return retVal;
}
int readByte() /**to read echo data from xbee*/
{
int Val;
BYTE byte;
DWORD dwBytesTransferred;
DWORD dwCommModemStatus;
if(!GetCommState(hPort, &dcb))
return 0;
SetCommMask(hPort, EV_RXCHAR|EV_ERR);
WaitCommEvent(hPort, &dwCommModemStatus, 0);
if (dwCommModemStatus& EV_RXCHAR)
ReadFile(hPort, &byte, 1, &dwBytesTransferred, 0);
Val = byte;
return Val;
}
int enter_pass() /**Funtion for password*/
{
a: /**goto statement*/
string pass ="";
char ch;
cout << "Enter pass\n";
ch = _getch();
while(ch != 13)
{ /**character 13 is enter*/
pass.push_back(ch);
cout << '*';
ch = _getch();
}
if(pass == "cs101")
{
cout << "\nAccess granted \n";
return 1;
}
else
{
cout << "\nAccess aborted...\n";
return 0;
}
}
class book
{
int book_no;
char book_colour;
unsigned int no_of_books;
public :
void get_data()
{
cout<<"\n Enter the book number : ";
cin>>book_no;
cout<<"\n Enter book_colour : ";
cin>>book_colour;
cout<<"\n Enter number of books : ";
cin>>no_of_books;
}
int get_booknum()
{
return book_no;
}
void modify()
{
cout<<"\n Enter number of copies";
cin>>no_of_books;
}
void put_data()
{
cout<<"\n Book number : "<<book_no;
cout<<"\n Book colour : ";
cout<<book_colour;
cout<<"\n Number of book : "<<no_of_books;
}
char return_color()
{
return book_colour;
}
int return_number()
{
return no_of_books;
}
void decrement()
{
--no_of_books;
}
};
class destination
{
unsigned int destination_id;
char direction;
public:
int number_of_book_type;
book b[10];
destination()
{
number_of_book_type=0;
}
void get_data()
{
int i,j=0;
cout<<"\n Destination Id : ";
cin>>destination_id;
cout<<"\n Enter number of book type for this destination(max 10)";
cin>>i;
number_of_book_type=i;
while(j<i)
{
cout<<"\n Enter details of book type "<<j;
b[j].get_data();
j++;
}
if(destination_id==1)
direction='4';
else if(destination_id==2)
direction='8';
else
direction='6';
}
void put_data()
{
cout<<"\n Destination id - "<<destination_id<<endl;
for(int i=0;i<number_of_book_type;i++)
{cout<<b[i].get_booknum()<<endl;
cout<<b[i].return_color()<<endl;
cout<<b[i].return_number()<<endl;
}
}
int get_destination_id()
{
return destination_id;
}
void modify()
{ int book_type;
cout<<"\n Enter book type whose number of books to be modified(i.e 1,2,3.......) : ";
cin>>book_type;
b[book_type-1].modify();
}
char return_direction()
{return direction;}
};
char xbee()
{
char color;
cout<<"\n Enter the colour ";
cin>>color;
fstream f;
destination d;
char direction;
f.open("Destination_Database",ios::binary|ios::in|ios::out);
while(f.read((char*)&d,sizeof(d)))
{
for(int i=0;i<d.number_of_book_type;i++)
{
if(d.b[i].return_color()==color)
{
direction=d.return_direction();
d.b[i].decrement();
f.write((char*)&d,sizeof(d));
}
}
}
f.close();
return direction;
}
int main()
{
book lib;
destination des;
fstream file,fio;
//file.open("Library_Database",ios::in|ios::out|ios::binary|ios::app);
// fio.open("Destination_Database",ios::in|ios::out|ios::binary|ios::app);
long pos;
int book_num,destination_num;
int num,flag=1;
label:
cout<<"\n WELCOME TO LIBRARY DATABASE ";
int pass=enter_pass();
if(!pass) /**if password is wrong*/
{
delay_ms();
system("cls");
goto label;
}
else
{
delay_ms();
system("cls");
start:
cout<<"\n WELCOME TO LIBRARY DATABASE ";
cout<<"\n 1. Books Management ";
cout<<"\n 2. Path Management ";
cout<<"\n 3. Send data using XBEE";
cout<<"\n 4. EXIT ";
cout<<"\n Enter corresponding number to continue....... ";
cin>>num;
if(num!=1&&num!=2&&num!=3&&num!=4)
{
cout<<"\n Wrong Choice";
delay_ms();
system("cls");
goto start;
}
if(num==1)
{
system("cls");
lb:
cout<<"\n MENU FOR BOOKS :";
cout<<"\n 1. Add book details ";
cout<<"\n 2. Edit book data ";
cout<<"\n 3. Search book data ";
cout<<"\n 4. Delete book data ";
cout<<"\n 5. Goto main menu ";
cout<<"\n Enter corresponding number to continue....... ";
cin>>num;
char confirm;
if(num!=1&&num!=2&&num!=3&&num!=4&&num!=5)
{
cout<<"\n Wrong choice ";
delay_ms();
system("cls");
goto lb;
}
switch(num)
{
case 1:{
file.open("Library_Database",ios::in|ios::out|ios::binary|ios::app);
lib.get_data();
file.write((char*)&lib,sizeof(book));
file.close();
system("cls") ;
goto lb;
break;
}
case 2:{
flag=1;
file.open("Library_Database",ios::in|ios::out|ios::binary);
cout<<"\n Enter book number to be edited : ";
cin>>book_num;
file.seekp(0,ios::beg);
pos=file.tellp();
while(file.read((char*)&lib,sizeof(book)))
{
if(lib.get_booknum()==book_num)
{ file.seekp(pos);
lib.modify();
file.write((char*)&lib,sizeof(book));
flag=0;
}
pos=file.tellp();
}
file.close();
if(flag==1)
{
cout<<"\n NO SUCH BOOK FOUND";
delay_ms();
system("cls");
goto lb;
}
else
{
cout<<"\n Successfully updated";
delay_ms();
system("cls");
goto lb;
}
break;
}
case 3: {
flag=1;
file.open("Library_Database",ios::in|ios::out|ios::binary);
cout<<"\n Enter book number : " ;
cin>>book_num;
file.seekg(0,ios::beg);
while(file.read((char*)&lib,sizeof(book)))
{
if(lib.get_booknum()==book_num)
{
lib.put_data();
flag=0;
}
} file.close();
if(flag==1)
{
cout<<"\n NO SUCH BOOK FOUND";
delay_ms();
system("cls");
goto lb;
}
else
{
delay_ms();
system("cls");
goto lb;
}
break;
}
case 4 :{
flag=1;
file.open("Library_Database",ios::in|ios::out|ios::binary);
fstream temp;
temp.open("temp",ios::out|ios::in|ios::binary|ios::app);
cout<<"\n Enter book number to be deleted : " ;
cin>>book_num;
file.seekp(0,ios::beg);
while(file.read((char*)&lib,sizeof(book)))
{
if(lib.get_booknum()==book_num)
{ lib.put_data();
cout<<"\n Are you sure, you want to delete this record (y/n)";
cin>>confirm;
if(confirm=='n')
temp.write((char*)&lib,sizeof(book));
flag=0;
}
else
temp.write((char*)&lib,sizeof(book));
}
file.close();
temp.close();
remove("Library_Database");
rename("temp","Library_Database");
if(flag==1)
{
cout<<"\n NO SUCH BOOK FOUND";
delay_ms();
system("cls");
goto lb;
}
else
{ cout<<"DELETED SUCCESSFULLY";
delay_ms();
system("cls");
goto lb;
}
break;
}
case 5 :
{
system("cls");
goto start;
}
}
}
else if(num==2)
{
system("cls");
pb:
cout<<"\n MENU FOR DESTINATION :";
cout<<"\n 1. Add destination details ";
cout<<"\n 2. Edit destination data ";
cout<<"\n 3. Show all details ";
cout<<"\n 4. GO TO MAIN MENU ";
cout<<"\n Enter corresponding number to continue....... ";
cin>>num;
if(num!=1&&num!=2&&num!=3&&num!=4)
{
cout<<"\n Wrong Choice";
delay_ms();
system("cls");
goto pb;
}
switch(num)
{
case 1:
{ fio.open("Destination_Database",ios::in|ios::out|ios::binary|ios::app);
des.get_data();
fio.seekp(0,ios::end);
fio.write((char*)&des,sizeof(destination));
fio.close();
delay_ms();
system("cls");
goto pb;
break;
}
case 2:
{ fio.open("Destination_Database",ios::in|ios::out|ios::binary|ios::app);
cout<<"\n Enter destination id to modified : ";
cin>>destination_num;
fio.seekp(0,ios::beg);
pos=fio.tellp();
while(fio.read((char*)&des,sizeof(destination)))
{
if(des.get_destination_id()==destination_num)
{ fio.seekp(pos);
des.modify();
fio.write((char*)&des,sizeof(destination));
flag=0;
}
pos=fio.tellp();
} fio.close();
if(flag==1)
{
cout<<"\n NO SUCH DESTINATION FOUND";
delay_ms();
system("cls");
goto pb;
}
else
{
cout<<"\n Successfully updated";
delay_ms();
system("cls");
goto pb;
}
break;
}
case 3:{fio.open("Destination_Database",ios::in|ios::out|ios::binary);
destination d;
while(fio.read((char*)&d,sizeof(d)))
{
cout<<"\n";
d.put_data();
}
delay_ms();
system("cls");
goto pb;
fio.close();
}
case 4 :
{
system("cls");
goto start;
}
}
}
else if(num==3)
{
char d;
system("cls");
d=xbee();
char data;
if(d=='4')
{
data=0x34;
}
else if(d=='6')
{
data=0x36;
}
else if(d=='8')
{
data=0x38;
}
if (writebyte(&data)) cout<<"Data Sent";
cout<<"\n Data Received::"<<readByte();
CloseHandle(hPort);
}
else
{
exit(0);
}
}
}
