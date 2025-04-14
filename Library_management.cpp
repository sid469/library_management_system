#include<iostream>
#include<vector>
#include<map>
#include<string>
using namespace std;
class Book{
    private:
        string isbn;
        int copiesAvaliable;
        int totalCopies;
    public:
        string title;
        string author;
        Book(string title="UnknownTitle",string author="UnknownAuthor",string isbn="ISBN",int copiesAvaliable=0,int totalCopies=5){
            this->title=title;
            this->author=author;
            this->isbn=isbn;
            this->totalCopies=totalCopies;
            this->copiesAvaliable=copiesAvaliable;
        }
        Book(Book&B,string newIsbn){
            this->title=B.title;
            this->isbn=newIsbn;
            this->author=B.author;
            this->totalCopies=B.totalCopies;
            this->copiesAvaliable=B.copiesAvaliable;
        }

        //getter functions
        string getIsbn(){
            return isbn;
        }
        int getCopiesAvaliable(){
            return copiesAvaliable;
        }
        int getTotalCopies(){
            return totalCopies;
        }

        //setter function
        void updateCopies(int count){
            if(copiesAvaliable+count<0||totalCopies+count<0){
                cout<<"Invalid request! Count becomes negative\n";
                return;
            }
            copiesAvaliable+=count;
            totalCopies+=count;
        }

        //method functions
        bool borrowBook(){
            if(copiesAvaliable>0){
                copiesAvaliable--;
                return true;
            }
            cout<<"Invalid request! Copy of book not available\n";
            return false;
        }
        bool returnBook(){
            if(copiesAvaliable<totalCopies){
                copiesAvaliable++;
                return true;
            }
            cout<<"Invalid request! Copy of book exceeds total copies\n";
            return false;
        }
        void printDetails(){
            cout<<title<<" "<<author<<" "<<'\n';
        }
};
class Member{
    private:
        string memberID;
        map<string,int>borrowedBooks;
        int borrowLimit;
        int he_has;
    public:
        string name;
        Member(string memberID="",string name="",int borrowLimit=3){
            this->memberID=memberID;
            this->name=name;
            this->borrowLimit=borrowLimit;
            he_has=0;
        }
        //getter
        string getId(){
            return memberID;
        }

        //utility methods
        bool borrowBook(string isbn){
            if(he_has<borrowLimit){
                he_has++;
                borrowedBooks[isbn]++;
                return true;
            }
            cout<<"Invalid request! Borrow limit exceeded\n";
            return false;
        }
        bool returnBook(string isbn){
            if(borrowedBooks.find(isbn)==borrowedBooks.end()){
                cout<<"Invalid request! Book not borrowed\n";
                return false;
            }
            borrowedBooks[isbn]--;
            if(borrowedBooks[isbn]==0)borrowedBooks.erase(isbn);
            he_has--;
            return true;
        }
        void printDetails(){
            if(he_has==0)return;
            auto it=borrowedBooks.begin();
            while(it!=borrowedBooks.end()){
                cout<<memberID<<' '<<name<<' '<<it->first<<' '<<it->second<<'\n';
                it++;
            }
        }
};
class Library{
    private:
        vector<Book>books;
        vector<Member>members;
    public:
        //utility methods
        void update_count(string isbn,int count){
            for(Book&b:books){
                if(b.getIsbn()==isbn){
                    b.updateCopies(count);
                    return;
                }
            }
        }
        void change_isbn_number(string isbn1,string isbn2){
            for(Book&b:books){
                Book*new_one;
                if(b.getIsbn()==isbn1){
                    new_one=new Book(b,isbn2);
                    books.push_back(*new_one);
                    return;
                }
            }
        }
        bool addBook(Book&new_book){
            for(Book&b:books){
                if(b.getIsbn()==new_book.getIsbn()){
                    cout<<"Invalid request! Book with same isbn already exists\n";
                    return false;
                }
            }
            books.push_back(new_book);
            // if(books.size()==0)cout<<"avale";
            // else cout<<"ha "<<new_book.title<<'\n';
            return true;
        }
        bool registerMember(Member&new_member){
            for(Member&m:members){
                if(m.getId()==new_member.getId()){
                    cout<<"Invalid request! Member with same id already exists\n";
                    return false;
                }
            }
            members.push_back(new_member);
            // if(members.size()==0)cout<<"avale";
            // else cout<<"ha "<<new_member.name<<'\n';
            return true;
        }
        bool borrowBook(string memberID,string isbn){
            Book*borrowee=new Book;
            Member*borrower=new Member;
            bool book_there=false;
            bool member_there=false;
            for(Book&b:books){
                if(b.getIsbn()==isbn){
                    borrowee=&b;
                    book_there=true;
                }
            }
            if(!book_there){
                cout<<"Invalid request! Copy of book not available\n";
                return false;
            }
            for(Member&m:members){
                if(m.getId()==memberID){
                    borrower=&m;
                    member_there=true;
                }
            }
            if(!member_there){
                return false;
            }
            //check if it can be borrowed
            bool can_it_borrowed=borrowee->borrowBook();
            if(can_it_borrowed){
                bool can_he_borrow=borrower->borrowBook(isbn);
                if(can_he_borrow){return true;}
                else {borrowee->returnBook();return false;}
            }
            return false;
        }
        bool returnBook(string memberID,string isbn){
            Book*borrowee=new Book;
            Member*borrower=new Member;
            bool book_there=false;
            bool member_there=false;
            for(Book&b:books){
                if(b.getIsbn()==isbn){
                    borrowee=&b;
                    book_there=true;
                }
            }
            if(!book_there){
                cout<<"Invalid request! Copy of book not available\n";
                return false;
            }
            for(Member&m:members){
                if(m.getId()==memberID){
                    borrower=&m;
                    member_there=true;
                }
            }
            if(!member_there){
                return false;
            }
            //can it be returned
            bool can_it_returned=borrowee->returnBook();
            if(can_it_returned){
                bool can_he_return=borrower->returnBook(isbn);
                if(can_he_return){return true;}
                else {borrowee->borrowBook();return false;}
            }
            return false;
        }
        void printLibraryDetails(){
            for(Book&b:books){
                cout<<b.title<<" "<<b.author<<" "<<b.getCopiesAvaliable()<<'\n';
            }
            for(Member&m:members){
                cout<<m.getId()<<" "<<m.name<<'\n';
            }
        }
        void printBook(string isbn){
            for(Book&b:books){
                if(b.getIsbn()==isbn){
                    b.printDetails();
                    return;
                }
            }
        }
        void printMember(string id){
            for(Member&m:members){
                if(m.getId()==id){
                    m.printDetails();
                    return;}
            }
        }
};
int main(){
    string s="";
    Library IITM;
    cin>>s;
    while(s!="Done"){
        Book*b=new Book;
        Member*m=new Member;

        //adding book
        if(s=="Book"){
            string sub;
            cin>>sub;
            //if it is case two
            if(sub=="None"){
                IITM.addBook(*b);
            }//if it is case three
            else if(sub=="ExistingBook"){
                string isbn1,isbn2;
                cin>>isbn1>>isbn2;
                IITM.change_isbn_number(isbn1,isbn2);
            }//if it is case one
            else {
                string title=sub;
                string author,isbn;
                int copies_ava,total;
                cin>>author>>isbn>>copies_ava>>total;
                b=new Book(title,author,isbn,copies_ava,total);
                IITM.addBook(*b);
            }
        }

        //increasing count
        if(s=="UpdateCopiesCount"){
            string isbn;
            int count;
            cin>>isbn>>count;
            IITM.update_count(isbn,count);
        }
        
        //adding member
        if(s=="Member"){
            string sub;
            cin>>sub;
            //if it is case one
            if(sub=="NoBorrowLimit"){
                string id,name;
                cin>>id>>name;
                m=new Member(id,name);
                IITM.registerMember(*m);
            }//if it is case two
            else {
                string name;
                int borrowlimit;
                cin>>name>>borrowlimit;
                m=new Member(sub,name,borrowlimit);
                IITM.registerMember(*m);
            }
        }

        //borrow
        if(s=="Borrow"){
            string id,isbn;
            cin>>id>>isbn;
            IITM.borrowBook(id,isbn);
        }
        
        //return
        if(s=="Return"){
            string id,isbn;
            cin>>id>>isbn;
            IITM.returnBook(id,isbn);
        }

        //print book
        if(s=="PrintBook"){
            string isbn;
            cin>>isbn;
            IITM.printBook(isbn);
        }

        //print member
        if(s=="PrintMember"){
            string id;
            cin>>id;
            IITM.printMember(id);
        }

        //print details of library
        if(s=="PrintLibrary"){
            IITM.printLibraryDetails();
        }

        cin>>s;
    }
}
