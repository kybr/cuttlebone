#ifndef CB_FILE_WATCHER__
#define CB_FILE_WATCHER__

class FileWatcherImpl;
class FileWatcher {
  FileWatcherImpl* impl;

 public:
  FileWatcher();
  virtual ~FileWatcher();
  virtual void start();
  virtual void stop();
  void watch(const char* filePath);
  virtual void onModify(const char* filePath) = 0;
};

#endif
