#ifndef MANAGER_H
#define MANAGER_H
#include <defines.h>
#include <FileSpooler.h>
#include <ShredManager.h>
#include"AVLTree.hpp"
#include "List.h"
#include "Delete.h"
#include "Import.h"
#include "Export.h"

typedef command * create_object_routine ();
class SharedObjectsManager
{
private:
map <string,command *> comm; // A map to store the cloner objects
map <string,void *> dsos; // A map to store all DSO objects
public:
SharedObjectsManager(); // Constructor
GeometricObject * load(string name); // Loading a DSO from file
~SharedObjectsManager(); // Destructor
};

