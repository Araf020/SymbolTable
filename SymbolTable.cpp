#include <bits/stdc++.h>
#include <fstream>

using namespace std;
#define global 1
ofstream  outputfile;


///-------------------SymbolInfo-------------------
class SymbolInfo
{
public:
    string key;      // name
    string value; // value
    SymbolInfo *next;
    string position;
    string tableId;



    SymbolInfo() : key(0), value(""), next(nullptr), position(""){};
    SymbolInfo(string Key, string Value) : key(Key), value(Value), next(nullptr), position(""){};
    SymbolInfo(SymbolInfo const &data) : key(data.key), value(data.value), next(data.next), position(data.position),tableId(data.tableId){};


    const string &getKey() const {
        return key;
    }

    void setKey(const string &key) {
        SymbolInfo::key = key;
    }

     string &getValue()  {
        return value;
    }

    void setValue(const string &value) {
       this->value = value;
    }

     string &getCursorAt()  {
        return position;
    }

    void setCursorAt(const string &cursorAt) {
        this->position = cursorAt;
    }

      string &getTableId()  {
        return this->tableId;
    }

    void setTableId(const string &tableId) {
       this->tableId = tableId;
    }
    void show(){
        cout<<"tableid: "<<tableId<<endl;
        cout<<"postion: "<<position<<endl;  ///not necessary
    }
};


///scopeTable
class scopeTable
{
private:
    int size,      // size: size of table, count: number of data
    count;
    SymbolInfo **table;    ///using double pointer

    int hashFunction(string key);
    void tableDoubling();
    void tableShrinking();
    void reHashing(int size_orig);

public:
    string scopeTable_id;
    int child_serial;
    int *cursorList;

    scopeTable *next;
    scopeTable *parent;
    scopeTable(){};

    ///constructor with size
    scopeTable(int m) : size(m), count(0)
    {
       ///initilizing the text file...
        // cout<<"table  created."<<endl;
        cursorList = new int [size];
        table = new SymbolInfo *[size]; /// allocate the first demension of table
        for (int i = 0; i < size; i++) {

            table[i] = nullptr; /// every bucket points to nullptr
            cursorList[i] = 0;
        }
        parent = nullptr;
        next = nullptr;
        scopeTable_id ="";
        child_serial = 1;
       ///cursor for each bucket

    }

    void setscope_Id(string parent_id, string current_id){
        //cout<<"scopeidset clled"<<endl;

        // cout<<"parent now: "<<parent_id<<endl;

        if (current_id != "") {
            parent_id.append(".");

            parent_id.append(current_id);
        }
        // cout<<"current now: "<<parent_id<<endl;

        this->scopeTable_id= parent_id;
        //cout<<"scopeTable_id: "<<scopeTable_id;
    }

    void setSize(int s){
        size  = s;
    }

    int getSize(){
        return this->size;
    }
    ~scopeTable();

    void Insert(SymbolInfo data); /// consider hashtableShrinking()
    bool Delete(string key);   /// consider hashtableShrinking()
    SymbolInfo* Search(string key);
    void displayTable();
};

void scopeTable::Insert(SymbolInfo data)
{
    count++;
    if (count > size)
    {
        tableDoubling(); ///double the size of table


        int *newcursorList = new int [this->size];
        for (int i = 0; i < count-1 ; ++i) { //double the cursorList
            newcursorList[i] = this->cursorList[i];

        }
        this->cursorList = newcursorList;
       // delete [] newcursorList;

    }

    int index = hashFunction(data.key); // get index of slot
    SymbolInfo *newNode = new SymbolInfo(data);     // create new node to store data

    // push_front()
    if (table[index] == NULL)
    {
        table[index] = newNode; // eg: list: 4->NULL
        cout<<"Inserted in ScopeTable# "<< this->scopeTable_id<<" at position "<<index<< "," <<cursorList[index]<<endl;
        outputfile<<"Inserted in ScopeTable# "<< this->scopeTable_id<<" at position "<<index<< "," <<cursorList[index]<<endl;



         newNode->setTableId(this->scopeTable_id);  ///setting the table_id of that symbol ;-;
        string symbol_position = "";
        // symbol_position.append("-->");
         symbol_position.append(to_string(index));        ///creating symbol position consists of
         symbol_position.append(",");
         symbol_position.append(to_string(cursorList[index]));
         newNode->setCursorAt(symbol_position);

          cursorList[index] = cursorList[index]+1;
    }

    else
    {                                    // eg: list: 5->9->NULL  , add 4
        SymbolInfo *next = table[index]->next; //     list: 5->4->9->NULL
        table[index]->next = newNode;
         cout<<"Inserted in ScopeTable# "<< this->scopeTable_id<<" at position "<<index<< "," <<cursorList[index]<<endl;

         outputfile<<"Inserted in ScopeTable# "<< this->scopeTable_id<<" at position "<<index<< "," <<cursorList[index]<<endl;


        newNode->setTableId(this->scopeTable_id);  ///setting the table_id of that symbol ;-;
        string symbol_position = "";
        // symbol_position.append("-->");
         symbol_position.append(to_string(index));        ///creating symbol position consists of
         symbol_position.append(",");
         symbol_position.append(to_string(cursorList[index]));
         newNode->setCursorAt(symbol_position); ///setting position at the the table

         cursorList[index] = cursorList[index]+1;
        newNode->next = next;

    }

}

bool scopeTable::Delete(string key)
{
    int index = hashFunction(key); // get index of slot
    SymbolInfo *current = table[index],  // use two pointer for traversal in list
    *previous = NULL;

    while (current != NULL && current->key != key)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
    {
        cout << "Data not found!\n\n";
        return false;
    }
    else
    {
        if (previous == NULL)
        {
            table[index] = current->next;
        }

        else
        {
            previous->next = current->next;
        }
        delete current;
        current = 0;
    }

    count--;
    if (count < size / 4)  ///considering load factor as 4
    {
        tableShrinking();
    }
    return true;
}


///searching...........
SymbolInfo* scopeTable::Search(string key)
{
    int index = hashFunction(key);                               /// get index of slot
    SymbolInfo *current = table[index];

    while (current != nullptr)
    { // traversal in list
       // cout<<"searching..."<<endl;
        if (current->key == key){
            return current;
        }
        current = current->next;
    }
    return nullptr;

}

int scopeTable::hashFunction(string key)
{
    int sum = 0;
    for(auto a: key){
        sum+=int (a);
    }
    return sum % this->size; ///sum_ascii mod bucket_size;

}

void scopeTable::tableDoubling()
{
    int size_orig = size; // size_orig represents the original size of table
    size *= 2;            /// double the size of table
    reHashing(size_orig); // create new table with new larger size
}



///---------------remarks: not that neccessary for this offline----------------------------///


void scopeTable::tableShrinking()
{
    int size_orig = size; ///size_orig represents the original size of table
    size /= 2;            /// shrinking the size of table
    reHashing(size_orig); /// creating new table with new smaller size
}

void scopeTable::reHashing(int size_orig)
{
    SymbolInfo **newtable = new SymbolInfo *[size]; /// allocating memory for new table
    for (int i = 0; i < size; i++)
    {                    /// initializetion
        newtable[i] = 0; /// ensuring every node in slot points to nullptr
    }

    for (int i = 0; i < size_orig; i++)
    { // visit every node in the original table

        SymbolInfo *curr_orig = table[i], // curr_orig: current node in original table
        *prev_orig = nullptr;      // prev_orig: following curr_orig

        while (curr_orig != nullptr)
        {
            /// traversal in list of each bucket in  scope_table
            prev_orig = curr_orig->next;

            int index = hashFunction(curr_orig->key); // get index of slot in new table

            if (newtable[index] == nullptr)
            {
                newtable[index] = curr_orig;
                newtable[index]->next = 0; // equivalent to curr_orig->next = 0;
            }

            else
            {
                SymbolInfo *next = newtable[index]->next;
                newtable[index]->next = curr_orig;
                curr_orig->next = next;
            }
            curr_orig = prev_orig; // visit the next node in list in previous
        }
    }
    delete[] table;
    this->table = newtable;
}

scopeTable::~scopeTable()
{
    for (int i = 0; i < size; i++)
    {
        SymbolInfo *current = table[i]; /// point *current to first node in list
        while (current != NULL)
        { // traversal in list
            SymbolInfo *previous = current;
            current = current->next;
            delete previous;
            previous = 0;
        }
    }
    delete[] table;
}


void scopeTable::displayTable()
{

    if(this == nullptr){

        return;    ///if symbol table is empty
    }
    cout<<"ScopeTable # "<<this->scopeTable_id<<endl;
    outputfile<<"ScopeTable # "<<this->scopeTable_id<<endl;


    for (int i = 0; i < size; i++)
    { // visit every node in table

        cout <<i << " --> ";
        outputfile <<i << " --> ";
        SymbolInfo *current = table[i];
        while (current != nullptr)
        {
            cout << "<" << current->key << " : " << current->value << " > ";
            outputfile << "<" << current->key << " : " << current->value << " > ";
            current = current->next;
        }
        cout << endl;
        outputfile << "\n";

    }
    cout << endl;
    outputfile<<"\n";
}



///-------------------------------------symbolTable---------------------------------------------
class SymbolTable {

private:
    int scopetable_size;
     SymbolInfo* search_here(string name);

public:
    scopeTable *top;
    int scopetableSerial;
    int cursor_at;
    int new_Scope ;

    int insertCounter;
    string parent_id;
    SymbolTable(int scopeTableSize) {
        scopetableSerial = 1;
        scopetable_size = scopeTableSize;
        top = nullptr;
        cursor_at = global;
        parent_id = "";
        new_Scope = 1;
        insertCounter = 0;
        //parent_id.append(to_string(cursor_at));

    }

    bool push(SymbolInfo symbolInfo);

    bool pop();

    scopeTable* getTop();

    void search_globally(string name);

    bool Delete_from_current(string key);

    void EnterScope();
    void printCurrent();
    void printAllScopeTable();


};

bool SymbolTable::push(SymbolInfo symbolInfo) {
    //if(top== nullptr){
    //cout<<"no scop table in SymbolTable\n";
    /// return false;
    //}
    top->Insert(symbolInfo);
    top->next = nullptr;
    insertCounter++;
    return true;

}


bool SymbolTable::pop() {
    if (top== nullptr){
        cout<<"Symbol table is empty\n";
        outputfile<<"Symbol table is empty\n";

        return false;
    }
    cout<<"\nScopeTable with id "<<top->scopeTable_id<< " removed\n";
    outputfile<<"\nScopeTable with id "<<top->scopeTable_id<< " removed\n";


    top= top->parent;
    top->child_serial++;

    if(top==nullptr){
        cursor_at ++;
        return true;
    }
    top->next = nullptr;
    return  true;
}

scopeTable* SymbolTable::getTop() {
    return top;
}

bool SymbolTable::Delete_from_current(string key) {

    SymbolInfo* sym = search_here(key);
    if(sym==nullptr){

          ///deleting from current scope_table
        cout<<key<<" not found\n";
        outputfile<<key<<" not found\n";
        return false;
    }
    else
    {
        string tableid = sym->tableId;
        string position = sym->position;

        cout<<"\nDeleted Entry 3, 0 from current ScopeTable\n";
        outputfile<<"\nDeleted Entry "<< position <<" from current ScopeTable\n";

        return false;
    }


}

void SymbolTable::EnterScope() {
    //cout<<"enterscope calld\n";
    scopeTable *newScope = new scopeTable(scopetable_size);
    //  scopeTable *previous = top;
    newScope->parent = top;
    string current_id;
    if(top!=nullptr){
        parent_id = top->scopeTable_id;
        current_id= to_string(top->child_serial);
    }
    else{
        parent_id = to_string(global);
        current_id="";
    }
    top = newScope;
//    scopetableSerial++;
    top->setscope_Id(parent_id,current_id);
    cout<<"\nNew ScopeTable with id "<<top->scopeTable_id <<" created\n";
    outputfile<<"\nNew ScopeTable with id "<<top->scopeTable_id <<" created\n";
//    cursor_at++;

}

void SymbolTable::printAllScopeTable() {

    scopeTable* cur = this->top;
    while (cur != nullptr){

        cur->displayTable();
        cur  = cur->parent;
        cout<<endl;
        outputfile<<"\n";
    }

}

 void SymbolTable::search_globally(string name)
 {
    /// search_here(this->top,name);
    scopeTable* curr = this->top;
    while(curr!=nullptr){
        if(curr->Search(name)==nullptr){
            curr = curr->parent;
        }
        else{
            string tableid = curr->Search(name)->tableId;
            string position =  curr->Search(name)->position;
            //curr->Search(name)->show();
            cout<<"Found in ScopeTable# "<<tableid<<" at position  "<<position<<"\n";
            outputfile<<"Found in ScopeTable# "<<tableid<<" at position  "<<position<<"\n";
            return;
        }
    }
    cout<<"Not found"<<endl;
    outputfile<<"Not found\n";

 }


SymbolInfo* SymbolTable::search_here(string name){

        SymbolInfo* sym = this->top->Search(name);
        if(sym==nullptr){
            cout<<"Not found\n";
            outputfile<<"Not found\n";
            return nullptr;
        }
        string tableid = sym->tableId;
        string position = sym->position;
        cout<<"Found in ScopeTable# "<<tableid<<" at position  "<<position<<"\n";
        outputfile<<"Found in ScopeTable# "<<tableid<<" at position  "<<position<<"\n";
        return sym;


    }

void SymbolTable::printCurrent() {

    this->top->displayTable();

}



int main()
{

    int buckets = 2; ///intialized as 2 by default
    ifstream file("input.txt");
    outputfile.open("output.txt");
    string   line;
    getline(file,line);
    stringstream  linestreams(line);
    linestreams>>buckets;

    //cout<<buckets<<endl;

    ///intialized with initial bucket size of each scopetable
    SymbolTable st(buckets);

    st.EnterScope();
    while(std::getline(file, line))
    {
        stringstream linestream(line);
        string   data;
        char  command;
        string  val2;
        string  val3;



        linestream >> command >> val2>>val3;
        cout<<command<<" "<<val2<<"  "<<val3<<endl;
        outputfile<<command<<" "<<val2<<"  "<<val3<<"\n\n";
        if(command=='I'){
            //cout<<val2<<" "<<val3<<endl;
            SymbolInfo sym(val2,val3);

            st.push(sym);

        }
        if(command=='P'&&val2=="C"){
            st.printCurrent();
        }
        if (command=='P' && val2=="A"){
            st.printAllScopeTable();
        }
        if (command == 'S'){
            st.EnterScope();
        }
        if(command=='E'){
            st.pop();
        }

        if(command=='L'){
            st.search_globally(val2);
        }
        if(command=='D'){
            st.Delete_from_current(val2);
        }
    }
    return 0;
}
