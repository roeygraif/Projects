#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"
#include <fcntl.h>
#include <sys/stat.h>
///#include "signals.h"

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

string _ltrim(const std::string& s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
  return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
  FUNC_ENTRY()
  int i = 0;
  std::istringstream iss(_trim(string(cmd_line)).c_str());
  for(std::string s; iss >> s; ) {
    args[i] = (char*)malloc(s.length()+1);
    memset(args[i], 0, s.length()+1);
    strcpy(args[i], s.c_str());
    args[++i] = NULL;
  }
  return i;

  FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
  const string str(cmd_line);
  return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
  const string str(cmd_line);
  // find last character other than spaces
  unsigned int idx = str.find_last_not_of(WHITESPACE);
  // if all characters are spaces then return
  if (idx == string::npos) {
    return;
  }
  // if the command line does not end with & then return
  if (cmd_line[idx] != '&') {
    return;
  }
  // replace the & (background sign) with space and then remove all tailing spaces.
  cmd_line[idx] = ' ';
  // truncate the command line string up to the last non-space character
  cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}
bool isNumber(const char* text){  //HELPER
  try{
    stoi(text);
    return true;
  }catch(...){
    return false;
  }
}

SmallShell::SmallShell() {

    prompt_name = (char*)"smash> ";
    //curr_cwd = getcwd(nullptr,0);
}

SmallShell::~SmallShell() {

}


/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {

  string cmd_s = _trim(string(cmd_line));
  string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

  /*
   FOR DEBUGGING
      if(cmd_s.find("^") != std::string::npos){
      std::string strtst = cmd_s.substr(cmd_s.find("^")+1);
      if(isNumber(strtst.c_str())){
          sleep(stoi(strtst));
          return nullptr;
      }
      else{
          ctrlCHandler(0);
          return nullptr;
      }
  }
   */

  /// checking for IO redirection or pipe
  if (cmd_s.find("|") != std::string::npos){
      return new PipeCommand(cmd_line);
  }
  if (cmd_s.find(">") != std::string::npos){
      return new RedirectionCommand(cmd_line);
  }

  if(firstWord.compare("timeout") == 0){
      return new TimeoutCommand(cmd_line, &SmallShell::getInstance().jobs_list);
  }
  if (firstWord.compare("pwd") == 0) {
    return new GetCurrDirCommand(cmd_line);
  }
  else if (firstWord.compare("showpid") == 0) {
    return new ShowPidCommand(cmd_line);
  }
  else if (firstWord.compare("chprompt") == 0) {
    return new ChpromptCommand(cmd_line);
  }
    else if (firstWord.compare("cd") == 0) {
    return new ChangeDirCommand(cmd_line, getLastCWD());
  }
    else if (firstWord.compare("jobs") == 0) {
    return new JobsCommand(cmd_line, &SmallShell::getInstance().jobs_list);
  }
    else if (firstWord.compare("fg") == 0) {
    return new ForegroundCommand(cmd_line, &SmallShell::getInstance().jobs_list);
  }
    else if (firstWord.compare("bg") == 0) {
    return new BackgroundCommand(cmd_line, &SmallShell::getInstance().jobs_list);
  }
    else if (firstWord.compare("quit") == 0) {
    return new QuitCommand(cmd_line, &SmallShell::getInstance().jobs_list);
  }
    else if (firstWord.compare("kill") == 0) {
    return new KillCommand(cmd_line, &SmallShell::getInstance().jobs_list);
  }
  else if (firstWord.compare("setcore") == 0) {
      return new SetcoreCommand(cmd_line, &SmallShell::getInstance().jobs_list);
  }
  else if (firstWord.compare("getfiletype") == 0) {
      return new GetFileTypeCommand(cmd_line);
  }
  else if (firstWord.compare("chmod") == 0) {
      return new ChmodCommand(cmd_line);
  }
  else {
    return new ExternalCommand(cmd_line, &SmallShell::getInstance().jobs_list);
  }
}

void SmallShell::executeCommand(const char *cmd_line) {
  if(string(cmd_line).empty()){
      return;
  }
  this->jobs_list.removeFinishedJobs();
  this->curr_cmd = CreateCommand(cmd_line);
  if(this->curr_cmd != nullptr){
      this->curr_cmd->execute();
  }
}


//-------------------------------------
//JOBLIST------------------------------
  void JobsList::addJob(Command* cmd, bool isStopped, int pid){
    this->removeFinishedJobs();
    JobEntry new_job = JobEntry(this->generateJobID(), cmd->cmd, isStopped, pid, cmd);
    this->jobs_vector.push_back(new_job);
  }
  void JobsList::printJobsList(){
    removeFinishedJobs();
    for(unsigned int i = 0; i < jobs_vector.size(); i++){
        if(jobs_vector[i].visible){
            std::cout << "[" << jobs_vector[i].job_ID <<"] ";
            if(jobs_vector[i].cmd_st->alarm_length>0){
                std::cout << "timeout " << jobs_vector[i].cmd_st->alarm_length << " ";
            }
             std::cout << std::string(jobs_vector[i].cmd) << " : " <<
                jobs_vector[i].process_ID << " " << difftime(time(NULL),jobs_vector[i].exe_time) << " secs";
            if(jobs_vector[i].stopped){
              std::cout << " (stopped)";
            }
            std::cout << std::endl;
        }
    }
  }
  void JobsList::killAllJobs(){
    for(unsigned int i = 0; i < jobs_vector.size(); i++){
        if(kill(this->jobs_vector[i].process_ID, SIGKILL) != 0){
            perror("smash error: kill failed");
            return;
        }
    }
  }
  void JobsList::removeFinishedJobs(){
    int status;
    for(unsigned int i = 0; i < jobs_vector.size(); i++){
      status = waitpid(this->jobs_vector[i].process_ID, nullptr, WNOHANG);
      if(status != 0){
        this->jobs_vector.erase(this->jobs_vector.begin() + i);
      }
    }
  }
  JobsList::JobEntry * JobsList::getJobById(int jobId){
    for(unsigned int i = 0; i < jobs_vector.size(); i++){
      if(jobs_vector[i].job_ID == jobId){
        return &jobs_vector[i];
      }
    }
    return nullptr;
  }

  JobsList::JobEntry *JobsList::getJobByPID(int pid) {
      for(unsigned int i = 0; i < jobs_vector.size(); i++){
          if(jobs_vector[i].process_ID == pid){
              return &jobs_vector[i];
          }
      }
      return nullptr;
  }
  void JobsList::removeJobById(int jobId){
    for(std::vector<JobsList::JobEntry>::iterator i = jobs_vector.begin(); i != jobs_vector.end(); i++){
      if((*i).job_ID == jobId){
        jobs_vector.erase(i);
        break;
      }
    }
  }
  JobsList::JobEntry * JobsList::getLastJob(int* lastJobId){
    return &jobs_vector.back();
  }

  JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId){
    for(unsigned int i = jobs_vector.size(); i != 0; i--){
        if(jobs_vector[i-1].stopped){
          return &jobs_vector[i-1];
        }
      }
    return NULL;
  }
  int JobsList::generateJobID(){
    removeFinishedJobs();
    if(jobs_vector.empty()){
        return 1;
    }
    return (this->jobs_vector.back().job_ID + 1);
  }

void JobsList::killAlarmJobs() {
    for(unsigned int i = 0; i < jobs_vector.size(); i++){
        if(jobs_vector[i].cmd_st->waiting_for_alarm){
            if(difftime(time(NULL),jobs_vector[i].exe_time) >= (jobs_vector[i].cmd_st->alarm_length)){
                int status;
                waitpid(jobs_vector[i].process_ID,&status, WNOHANG);
                if(WIFEXITED(status)){
                    continue;
                }
                if(kill(this->jobs_vector[i].process_ID, SIGKILL) != 0){
                    perror("smash error: kill failed");
                    return;
                }
                std::cout << "smash: timeout " << jobs_vector[i].cmd_st->alarm_length << " " << jobs_vector[i].cmd << " timed out!" << std::endl;
            }
        }
    }
}

//END OF JOBLIST-----------------------
//-------------------------------------


void GetCurrDirCommand::execute(){
  char* cwd = getcwd(NULL,0);
  if(!cwd){
    perror("smash error: getcwd failed");
    return;
  }else{
    std::cout << cwd << std::endl;
    free(cwd);
  }
}

void ChpromptCommand::execute(){
    char *parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    int num_args = _parseCommandLine(cmdcpy, parse_command);
    char *new_prmpt;
    if (num_args == 1) {
        new_prmpt = (char *) "smash> ";
    } else {
        new_prmpt = (char *) malloc(std::string(parse_command[1]).length() + 2);
        strcpy(new_prmpt, (std::string(parse_command[1]).append("> ")).c_str());
    }
    SmallShell::getInstance().setPrompt(new_prmpt);
}

void ShowPidCommand::execute(){
  std::cout << "smash pid is " << getpid() << std::endl;
}

void ChangeDirCommand::execute() {
    SmallShell& inst = SmallShell::getInstance();
    char *parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    int num_args = _parseCommandLine(cmdcpy, parse_command);
    char* old_cwd = getcwd(NULL, 0);
    if (!old_cwd) {
        perror("smash error: getcwd failed");
        return;
    }
    if (num_args > 2) {
        std::cerr << "smash error: cd: too many arguments" << std::endl;
        return;
    }
    if (num_args == 1) {
        if (chdir(getenv("HOME")) != 0) {
            perror("smash error: chdir failed");
            return;
        }
    } else if (!strcmp(parse_command[1], "-")) {
        if (std::string(plast_pwd).empty()) {
            std::cerr << "smash error: cd: OLDPWD not set" << std::endl;
            return;
        } else {
            if (chdir(plast_pwd.c_str()) != 0) {
                perror("smash error: chdir failed");
                return;
            }
        }
    } else if (chdir(parse_command[1]) != 0) {
        perror("smash error: chdir failed");
        return;
    }
    inst.setLastCWD(old_cwd);
}

void JobsCommand::execute(){
  ptr_jobs->printJobsList();
}

void ForegroundCommand::execute(){
    char* parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    int num_args = _parseCommandLine(cmdcpy, parse_command);
    JobsList::JobEntry* job_to_fg = NULL;
    ptr_jobs->removeFinishedJobs(); 
    if(num_args > 2){
        std::cerr << "smash error: fg: invalid arguments" <<  std::endl;
        return;
    }
    if(num_args == 1){
        if(ptr_jobs->jobs_vector.empty()){
            std::cerr << "smash error: fg: jobs list is empty" <<  std::endl;
            return;
        }
      job_to_fg = &ptr_jobs->jobs_vector.back();
    }
    else if(!isNumber(parse_command[1])){
        std::cerr << "smash error: fg: invalid arguments" <<  std::endl;
        return;
    }
    else{
        job_to_fg = ptr_jobs->getJobById(stoi(parse_command[1]));
        if(!job_to_fg){
            std::cerr << "smash error: fg: job-id " << stoi(parse_command[1]) << " does not exist" << std::endl;
            return;
        }
    }
    if(kill(job_to_fg->process_ID, SIGCONT) != 0){
        perror("smash error: kill failed");
        return;
    }
    SmallShell::getInstance().setCurrPID(job_to_fg->process_ID);
    int status;
    this->cmd = static_cast<const char *>(job_to_fg->cmd);
    int child_pid_for_wait = job_to_fg->process_ID;
    ptr_jobs->getJobById(job_to_fg->job_ID)->makeInvisible();
    std::cout << job_to_fg->cmd << " : " << job_to_fg->process_ID << std::endl;
    if(waitpid(child_pid_for_wait, &status, WUNTRACED) == -1){
        perror("smash error: waitpid failed");
        return;
    }
}

void BackgroundCommand::execute(){
    char* parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    int num_args = _parseCommandLine(cmdcpy, parse_command);
    JobsList::JobEntry* job_to_bg = NULL;
    if(num_args > 2){
        std::cerr << "smash error: bg: invalid arguments" <<  std::endl;
        return;
    }
    if(num_args == 1){
        job_to_bg = ptr_jobs->getLastStoppedJob(NULL);
        if(!job_to_bg){
            std::cerr << "smash error: bg: there is no stopped jobs to resume" <<  std::endl;
            return;
        }
    }
    else if(!isNumber(parse_command[1])){
        std::cerr << "smash error: bg: invalid arguments" <<  std::endl;
        return;
    }
    else{
        job_to_bg = ptr_jobs->getJobById(stoi(parse_command[1]));
        if(!job_to_bg){
            std::cerr << "smash error: bg: job-id " << stoi(parse_command[1]) << " does not exist" << std::endl;
            return;
        }
        if(!(job_to_bg->stopped)){
            std::cerr << "smash error: bg: job-id " << stoi(parse_command[1]) << " is already running in the background" << std::endl;
            return;
        }
    }
    SmallShell::getInstance().setCurrPID(job_to_bg->process_ID);
    this->cmd = static_cast<const char *>(job_to_bg->cmd);
    if(kill(job_to_bg->process_ID, SIGCONT) != 0){
        perror("smash error: kill failed");
        return;
    }
    std::cout << this->cmd << " : " << job_to_bg->process_ID << std::endl;
    job_to_bg->stopped = false;
}

void QuitCommand::execute(){
    ptr_jobs->removeFinishedJobs();
    char* parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    int num_args = _parseCommandLine(cmdcpy, parse_command);
    if(num_args >= 2 && !strcmp(parse_command[1], "kill")){
      std::cout << "smash: sending SIGKILL signal to " << ptr_jobs->jobs_vector.size() << " jobs:" << std::endl;
      for(unsigned int i = 0; i < ptr_jobs->jobs_vector.size(); i++){
        std::cout << ptr_jobs->jobs_vector[i].process_ID << ": "<<ptr_jobs->jobs_vector[i].cmd << std::endl;
      }
      ptr_jobs->killAllJobs();
    }
    exit(0);
}

void KillCommand::execute(){
    char* parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    int num_args = _parseCommandLine(cmdcpy, parse_command);
    if(num_args != 3){
        std::cerr << "smash error: kill: invalid arguments" << std::endl;
        return;
    }
    char* signal_char = (parse_command[1])++;
    if(!isNumber(signal_char) || !isNumber(parse_command[2]) || stoi(signal_char) >= 0 || -(stoi(signal_char))>=NSIG){
        std::cerr << "smash error: kill: invalid arguments" << std::endl;
        return;
    }
    int signal = stoi(signal_char)*-1;
    JobsList::JobEntry* ptr_entry = ptr_jobs->getJobById(stoi(parse_command[2]));
    if(!ptr_entry){
        std::cerr << "smash error: kill: job-id " << parse_command[2] << " does not exist" << std::endl;
        return;
    }
    if(kill(ptr_entry->process_ID, signal) != 0){
        perror("smash error: kill failed");
        return;
    }
    std::cout << "signal number " << signal << " was sent to pid " << ptr_entry->process_ID << std::endl;
}

void PipeCommand::execute() {
    SmallShell& inst = SmallShell::getInstance();
    std::string s_cmd = std::string(cmd);
    std::string first_cmd, second_cmd;
    bool err = false;
    if (s_cmd.find("|&") != std::string::npos){
        err = true;
        first_cmd = _trim(s_cmd.substr(0, s_cmd.find("|&")));
        second_cmd = _trim(s_cmd.substr(s_cmd.find("|&") + 2));
    }
    else{
        first_cmd = _trim(s_cmd.substr(0, s_cmd.find("|")));
        second_cmd = _trim(s_cmd.substr(s_cmd.find("|") + 1));
    }
    /// now we have first and second commands

    int fd[2];
    if (pipe(fd) == -1){
        perror("smash error: pipe failed");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 < 0){
        perror("smash error: fork failed");
        return;
    }
    if (pid1 == 0){ /// child
        if(setpgrp()==-1){
            perror("smash error: setpgrp failed");
            exit(1);
        }

      if(close(fd[0]) == -1){
          perror("smash error: close failed");
          exit(1);
      }

      if(err){
          if(dup2(fd[1],2) == -1){
              perror("smash error: dup2 failed");
              exit(1);
          }
      }
      else{
        if(dup2(fd[1], 1) == -1){
            perror("smash error: dup2 failed");
            exit(1);
        }
      }

    inst.executeCommand(first_cmd.c_str());
    exit(0);
    }
    else{ /// father
        pid_t pid2 = fork();
        if (pid2 < 0){
            perror("smash error: fork failed");
            return;
        }
        if (pid2 == 0) { /// child2
            if(setpgrp()==-1){
                perror("smash error: setpgrp failed");
                exit(1);
            }

            if (close(fd[1]) == -1) {
                perror("smash error: close failed");
                exit(1);
            }
            if (dup2(fd[0], 0) == -1) {
                perror("smash error: dup2 failed");
                exit(1);
            }
            inst.executeCommand(second_cmd.c_str());
            exit(0);
        }
        else{ /// parent still
            if((close(fd[0]) == -1) || (close(fd[1]) == -1)){
                perror("smash error: close failed");
                return;
            }
            wait(nullptr);
            wait(nullptr);
        }
    }
}

void RedirectionCommand::execute() {  
    SmallShell& inst = SmallShell::getInstance();
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    std::string s_cmd = std::string(cmdcpy);
    std::string first_cmd, second_cmd;
    bool flag = false;
    if (s_cmd.find(">>") != std::string::npos){
        flag = true;
        first_cmd = _trim(s_cmd.substr(0, s_cmd.find(">>")));
        second_cmd = _trim(s_cmd.substr(s_cmd.find(">>") + 2));
    }
    else{
        first_cmd = _trim(s_cmd.substr(0, s_cmd.find(">")));
        second_cmd = _trim(s_cmd.substr(s_cmd.find(">") + 1));
    }
    /// now we have command + output file
    int fd = -1;
    if(flag){
        fd = (open(second_cmd.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0655));
    }else{
        fd = (open(second_cmd.c_str(),O_CREAT | O_WRONLY | O_TRUNC, 0655));
    }
    if (fd == -1){
        perror("smash error: open failed");
        return;
    }
    int fd_backup = dup(1);
    if(fd_backup == -1 || dup2(fd, 1) == -1){
        perror("smash error: dup2 failed");
        return;
    }
    inst.executeCommand(first_cmd.c_str());
    if(dup2(fd_backup, 1) == -1){
        perror("smash error: dup2 failed");
        return;
    }
}

void SetcoreCommand::execute() {
    char* parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    int num_args = _parseCommandLine(cmdcpy, parse_command);
    if (num_args != 3){
        std::cerr << "smash error: setcore: invalid arguments" << std::endl;
        return;
    }
    if(!isNumber(parse_command[1]) || !isNumber(parse_command[2])){
        std::cerr << "smash error: setcore: invalid arguments" << std::endl;
        return;
    }
    int jobid = stoi(parse_command[1]);
    int corenum = stoi(parse_command[2]);

    if(corenum < 0){
        std::cerr << "smash error: setcore: invalid core number" << std::endl;
        return;
    }
    if(!ptr_jobs->getJobById(jobid)){
        std::cerr << "smash error: setcore: job-id " << jobid << " does not exist" << std::endl;
        return;
    }
    int jobpid = ptr_jobs->getJobById(jobid)->process_ID;
    cpu_set_t  cpus;
    CPU_ZERO(&cpus);
    CPU_SET(corenum, &cpus);
    if(sched_setaffinity(jobpid,sizeof(cpu_set_t),&cpus) == -1){
        perror("smash error: sched_setaffinity failed");
        return;
    }
}

void GetFileTypeCommand::execute() {
    char* parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    int num_args = _parseCommandLine(cmdcpy, parse_command);
    if(num_args != 2){
        std::cerr << "smash error: gettype: invalid arguments" << std::endl;
        return;
    }
    struct stat info;
    if(stat(parse_command[1], &info) != 0){
        perror("smash error: stat failed");
        return;
    }
    std::string ftype = "";
    if(S_ISREG(info.st_mode)){
        ftype = "regular file";
    }
    else if(S_ISDIR(info.st_mode)){
        ftype = "directory";
    }
    else if(S_ISCHR(info.st_mode)){
        ftype = "character device";
    }
    else if(S_ISBLK(info.st_mode)){
        ftype = "block device";
    }
    else if(S_ISFIFO(info.st_mode)){
        ftype = "FIFO";
    }
    else if(S_ISLNK(info.st_mode)){
        ftype = "symbolic link";
    }
    else if(S_ISSOCK(info.st_mode)){
        ftype = "socket";
    }
    std::cout << std::string(parse_command[1]) << "'s type is \"" << ftype << "\" and takes up " << info.st_size << " bytes" << std::endl;
}

void ChmodCommand::execute() {
    char* parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    int num_args = _parseCommandLine(cmdcpy, parse_command);
    if(num_args != 3){
        std::cerr << "smash error: chmod: invalid arguments" << std::endl;
        return;
    }
    if(!isNumber(parse_command[1])){
        std::cerr << "smash error: chmod: invalid arguments" << std::endl;
        return;
    }
    if(chmod(parse_command[2], stoi(parse_command[1])) != 0){
        perror("smash error: chmod failed");
        return;
    }
}

void ExternalCommand::execute(){
    char* parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, cmd);
    _removeBackgroundSign(cmdcpy);
    _parseCommandLine(cmdcpy, parse_command);
    bool run_in_bg = false;
    if(_isBackgroundComamnd(cmd)){
      run_in_bg = true;
    }

    pid_t pid = fork();
    if(pid < 0){
        perror("smash error: fork failed");
        return;
    }
    if(pid == 0){ //child
        if(setpgrp()==-1){
            perror("smash error: setpgrp failed");
            exit(1);
        }
        if(std::string(cmdcpy).find("*") == std::string::npos && std::string(cmdcpy).find("?") == std::string::npos){
          //simple
            if(execvp(parse_command[0], parse_command) == -1){
                perror("smash error: execvp failed");
                exit(1);
            }
        }
        else{
            if(execl("/bin/bash", "bash", "-c", cmdcpy, NULL) == -1){
                perror("smash error: execl failed");
                exit(1);
            }
        }
        exit(0);
    }
    else{   //father
        if(!run_in_bg && !this->waiting_for_alarm){
          SmallShell::getInstance().setCurrPID(pid);
          waitpid(pid, nullptr, WUNTRACED);
        }
        else{
          this->ptr_jobs->addJob(this, false, pid);
        }
        SmallShell::getInstance().setCurrPID(0);
    }
}

void TimeoutCommand::execute() {
    char* parse_command[COMMAND_MAX_ARGS];
    char cmdcpy[COMMAND_ARGS_MAX_LENGTH];
    strcpy(cmdcpy, this->cmd);
    int num_args = _parseCommandLine(cmdcpy, parse_command);
    std::string cmd_s = std::string(cmdcpy);
    if (num_args < 3){
        std::cerr << "smash error: timeout: invalid arguments" << std::endl;
        return;
    }
    int seconds = stoi(parse_command[1]);
    if(seconds < 1){
        std::cerr << "smash error: timeout: invalid arguments" << std::endl;
        return;
    }
    cmd_s = cmd_s.substr(cmd_s.find(parse_command[2])); /// without "timeout" and seconds
    strcpy(cmdcpy, cmd_s.c_str());

    Command* timed_cmd = SmallShell::getInstance().CreateCommand(cmdcpy);

    timed_cmd->alarm_length = seconds;
    timed_cmd->waiting_for_alarm = true;

    alarm(seconds);
    timed_cmd->execute();
}
