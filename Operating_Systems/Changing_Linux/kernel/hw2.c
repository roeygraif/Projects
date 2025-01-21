#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/syscalls.h>


asmlinkage long sys_hello(void) {
 printk("Hello, World!\n");
 return 0;
}

asmlinkage long sys_set_weight(int weight){
    if(weight < 0){
        return -EINVAL;
    }
    current->weight = weight;
    return 0;
}
asmlinkage long sys_get_weight(void){
    return current->weight;
}

asmlinkage long sys_get_ancestor_sum(void){
    int sum = 0;
    struct task_struct* curr;
    struct task_struct* father;
    curr = current;
    father = current->parent;  /// real parent??
    while(father != curr){
	sum += curr->weight;
	curr = curr->parent;
	father = curr->parent;
    }
    return sum;
}

void heaviest_recursive(struct task_struct *curr, int *heaviest, int *pid){
    struct task_struct *child;
    if(list_empty(&(curr->children))){
        return;
    }
    list_for_each_entry(child, &curr->children, sibling){
        heaviest_recursive(child, heaviest, pid);
        if(child->weight == *heaviest && child->pid < *pid){
            *pid = child->pid;
        }else if(child->weight > *heaviest){
            *heaviest = child->weight;
            *pid = child->pid;
        }
    }
}
asmlinkage long sys_get_heaviest_descendant(void){
    struct task_struct *curr = current;
    int heaviest = 0;
    int pid = curr->pid;
    if(list_empty(&(curr->children)))
        return -ECHILD;
    heaviest_recursive(curr, &heaviest, &pid);
    return pid;
}