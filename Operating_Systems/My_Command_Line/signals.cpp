#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"

using namespace std;

void ctrlZHandler(int sig_num) {
	// TODO: Add your implementation
  int curr_pid = SmallShell::getInstance().getCurrPID();
  std::cout << "smash: got ctrl-Z" << std::endl;
  if(curr_pid == 0){
    return;
  }
  /// NEW - to find jobs that were already in jobslist before
  JobsList::JobEntry* job_ptr = SmallShell::getInstance().getJobsList()->getJobByPID(curr_pid);
  if(!job_ptr){
      SmallShell::getInstance().getJobsList()->addJob(SmallShell::getInstance().getCurrCMD(), true, curr_pid);  /// if new to jobslist
  }
  else{ /// if was before
      job_ptr->makeVisible();
      job_ptr->exe_time = time(0);
      job_ptr->stopped = true;
  }
  if(kill(curr_pid, SIGSTOP) != 0) {
      perror("smash error: kill failed");
      return;
  }
  cout << "smash: process " << curr_pid << " was stopped" << std::endl;
  SmallShell::getInstance().setCurrPID(0);
}

void ctrlCHandler(int sig_num) {
  int curr_pid = SmallShell::getInstance().getCurrPID();
  std::cout << "smash: got ctrl-C" << std::endl;
  if(curr_pid == 0){
    return;
  }
  if(kill(curr_pid, SIGINT) != 0){
      perror("smash error: kill failed");
      return;
    }
  cout << "smash: process " << curr_pid << " was killed" << std::endl;
  SmallShell::getInstance().setCurrPID(0);
}

void alarmHandler(int sig_num) {
    std::cout << "smash: got an alarm" << std::endl;
    SmallShell::getInstance().getJobsList()->killAlarmJobs();
}

