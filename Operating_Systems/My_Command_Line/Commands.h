#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <cstring>
#include <limits.h>


#define COMMAND_ARGS_MAX_LENGTH (80)
#define COMMAND_MAX_ARGS (20)



class Command {
// TODO: Add your data members
 public:
  const char* cmd;
  bool waiting_for_alarm = false;
  time_t alarm_length = 0;
  Command(const char* cmd_line) : cmd(cmd_line){};
  virtual ~Command() = default;
  virtual void execute() = 0;
  //virtual void prepare();
  //virtual void cleanup();
  // TODO: Add your extra methods if needed
};

class BuiltInCommand : public Command {
 public:
  BuiltInCommand(const char* cmd_line) : Command(cmd_line){};
  virtual ~BuiltInCommand() {}
};

class JobsList;
class ExternalCommand : public Command {
 public:
  JobsList* ptr_jobs;
  ExternalCommand(const char* cmd_line, JobsList* jobs) : Command(cmd_line), ptr_jobs(jobs){};
  virtual ~ExternalCommand() {}
  void execute() override;
};

class PipeCommand : public Command {
  // TODO: Add your data members
 public:
  PipeCommand(const char* cmd_line) : Command(cmd_line){};
  virtual ~PipeCommand() {}
  void execute() override;
};

class RedirectionCommand : public Command {
 // TODO: Add your data members
 public:
  RedirectionCommand(const char* cmd_line) : Command(cmd_line){};
  virtual ~RedirectionCommand() {}
  void execute() override;
  //void prepare() override;
  //void cleanup() override;
};

class ChangeDirCommand : public BuiltInCommand {
// TODO: Add your data members public:
public:
  std::string plast_pwd;
  ChangeDirCommand(const char* cmd_line, std::string plastPwd) : BuiltInCommand(cmd_line), plast_pwd(plastPwd){};
  virtual ~ChangeDirCommand() {}
  void execute() override;
};

class GetCurrDirCommand : public BuiltInCommand {
 public:
  explicit GetCurrDirCommand(const char* cmd_line) : BuiltInCommand(cmd_line){};
  virtual ~GetCurrDirCommand() {}
  void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
 public:
  ShowPidCommand(const char* cmd_line) : BuiltInCommand(cmd_line){};
  virtual ~ShowPidCommand() {}
  void execute() override;
};


class QuitCommand : public BuiltInCommand {
// TODO: Add your data members
JobsList* ptr_jobs;
public:
  QuitCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line), ptr_jobs(jobs){};
  virtual ~QuitCommand() {}
  void execute() override;
};

class JobsList {
 public:
  class JobEntry {
   // TODO: Add your data members
   public:
   int job_ID;
   int process_ID;
   char* cmd = (char*)malloc(COMMAND_ARGS_MAX_LENGTH);
   bool stopped;
   bool visible = true; ///// NEW
   time_t exe_time;
   Command* cmd_st;
   JobEntry(int jobid, const char* command, bool is_stopped, int pid, Command* cmd_ST) : job_ID(jobid), process_ID(pid), stopped(is_stopped),exe_time(time(0)) , cmd_st(cmd_ST) { strcpy(cmd, command);};
   ~JobEntry() = default;
   void makeInvisible(){this->visible = false;};
   void makeVisible(){this->visible = true;};
  };
 // TODO: Add your data members
 std::vector<JobEntry> jobs_vector;
 public:
  JobsList() = default;
  ~JobsList() = default;
  void addJob(Command* cmd, bool isStopped = false, int pid = -1);
  void printJobsList();
  void killAllJobs();
  void killAlarmJobs();
  void removeFinishedJobs();
  JobEntry * getJobById(int jobId);
  JobEntry * getJobByPID(int pid);
  void removeJobById(int jobId);
  JobEntry * getLastJob(int* lastJobId);
  JobEntry *getLastStoppedJob(int *jobId);
  int generateJobID();
  // TODO: Add extra methods or modify exisitng ones as needed
};

class ChpromptCommand : public BuiltInCommand {   //TODO our homemade command
 // TODO: Add your data members
 public:
  ChpromptCommand(const char* cmd_line) : BuiltInCommand(cmd_line){};
  virtual ~ChpromptCommand() {}
  void execute() override;
};

class JobsCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  JobsList* ptr_jobs;
  JobsCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line), ptr_jobs(jobs){};
  virtual ~JobsCommand() {}
  void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  JobsList* ptr_jobs;
  ForegroundCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line), ptr_jobs(jobs){};
  virtual ~ForegroundCommand() {}
  void execute() override;
};

class BackgroundCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  JobsList* ptr_jobs;
  BackgroundCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line), ptr_jobs(jobs){};
  virtual ~BackgroundCommand() {}
  void execute() override;
};


class TimeoutCommand : public BuiltInCommand {
/* Bonus */
    JobsList* ptr_jobs;
 public:
  TimeoutCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line){};
  virtual ~TimeoutCommand() {}
  void execute() override;
};

class ChmodCommand : public BuiltInCommand {
  // TODO: Add your data members
 public:
  ChmodCommand(const char* cmd_line): BuiltInCommand(cmd_line){};
  virtual ~ChmodCommand() {}
  void execute() override;
};

class GetFileTypeCommand : public BuiltInCommand {
  // TODO: Add your data members
 public:
  GetFileTypeCommand(const char* cmd_line) : BuiltInCommand(cmd_line){};
  virtual ~GetFileTypeCommand() {}
  void execute() override;
};

class SetcoreCommand : public BuiltInCommand {
  // TODO: Add your data members
 public:
    JobsList* ptr_jobs;
    SetcoreCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line), ptr_jobs(jobs){};
  virtual ~SetcoreCommand() {}
  void execute() override;
};

class KillCommand : public BuiltInCommand {
 // TODO: Add your data members
 JobsList* ptr_jobs;
 public:
  KillCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line), ptr_jobs(jobs){};
  virtual ~KillCommand() {}
  void execute() override;
};

class SmallShell {
 private:
  // TODO: Add your data members
  char* prompt_name = nullptr;
  std::string last_cwd = "";
  char last_command[COMMAND_ARGS_MAX_LENGTH];
  Command* curr_cmd = nullptr;
  JobsList jobs_list;
  int curr_pid = 0;
  SmallShell();
 public:
  Command *CreateCommand(const char* cmd_line);
  SmallShell(SmallShell const&)      = delete; // disable copy ctor
  void operator=(SmallShell const&)  = delete; // disable = operator
  static SmallShell& getInstance() // make SmallShell singleton
  {
    static SmallShell instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
  }
  Command* getCurrCMD(){return curr_cmd;};
  ~SmallShell();
  void executeCommand(const char* cmd_line);
  // TODO: add extra methods as needed
  std::string getPrompt(){return prompt_name;};
  std::string getLastCWD(){return last_cwd;};
  void setLastCWD(std::string lastcwd){last_cwd = lastcwd;};
  char* getLastCommand(){return last_command;};
  void setLastCommand(const char* lastcmd){std::strcpy(last_command, lastcmd);
  }; /// EDIT changed to void
  int getCurrPID(){return curr_pid;};
  void setCurrPID(int pid){curr_pid = pid;};
  JobsList* getJobsList(){return &jobs_list;};
  void setPrompt(char* new_prompt){prompt_name = new_prompt;};
};

#endif //SMASH_COMMAND_H_