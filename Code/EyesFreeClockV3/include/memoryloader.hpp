#ifndef MEMORYLOADER_HPP
#define MEMORYLOADER_HPP


class MemoryLoader{
public:
    void start();
    
private:
    void getSerialInput(char* dst, int bsize);

};


#endif