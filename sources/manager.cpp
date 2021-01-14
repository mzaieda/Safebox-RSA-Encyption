#include "manager.h"

SharedObjectsManager::SharedObjectsManager(){} 
// Constructor
command * SharedObjectsManager::load(string name) { 
// Load object form DSO

command * obj = comm[name]; // Fetch object based on name

if ( obj != NULL ) return obj; // If found return it


// Else load the DSO and fetch the object through external launcher function

string lib_name = "./"+name+".so"; // Compose DSO name

// Loading dynamic object file. RTLD_LAZY: External functions are bound upon call.

void * dso=dlopen (lib_name.c_str(),RTLD_LAZY);

if ( dso ) { // If dso is not NULL then loaded successfully

create_object_routine * my_func = (create_object_routine *) dlsym (dso,"create_object"); 

if ( my_func ) { // If function loaded successfully

comm[name] = my_func(); // Store function in gobj vector


dsos[name] = dso; // Store DSO handler into dsos vectors


obj = comm[name]; // Fetch the external function into obj to be returned

} // Cannot load the external function from the DSO

else cout << "Error fetching external function "<< name << ": " << dlerror() <<"\n";

} // Unable to load then print error message with dlerror()

else cout << "Error loading dynamic object"<< name << ": " << dlerror() <<"\n";

return obj; // Return required object
}

SharedObjectsManager::~SharedObjectsManager()
{
// An iterator loop with a lambda function to delete all objects
for_each (comm.begin(),comm.end(),[](const std::pair<string,command *>& it) -> bool {
command * obj = std::get<1>(it);
delete (obj);
return true;
});
// An iteration loop with a lambda function to close all the dlls
for_each (dsos.begin(),dsos.end(),[](const std::pair<string,void *>& it) -> bool {
void * dso = std::get<1>(it);
dlclose(dso);
return true;
});
}