#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

class FileSystem {
  public:
    FileSystem();
    ~FileSystem();
  private:
    string device_;
    string directory_;
    string type_;
    string total_;
    string free_;
    string available_;
    string used_;
};


#endif
