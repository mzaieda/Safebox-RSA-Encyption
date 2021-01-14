#include <ThreadManager.h>


ThreadManager::ThreadManager(){

}
void ThreadManager::operator += (Thread * t){
    threads.push_back(t);
}
void ThreadManager::start ()  //per all theards
{
    //This function needed to be implemented by you
for(auto i=threads.begin();i!=threads.end();i++)
i[0]->start();
}
void ThreadManager::barrier (){
    //This function needed to be implemented by you
for(auto i=threads.begin();i!=threads.end();i++)
i[0]->wait();

}
ThreadManager::~ThreadManager(){
}
