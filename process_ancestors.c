//Name:Jaiveer Dhanju
//Purpose: Creating my call system call
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/sched.h>

#define ANCESTOR_NAME_LEN 16
struct process_info {
	long pid; /* Process ID */
	char name[ANCESTOR_NAME_LEN]; /* Program name of process */
	long state; /* Current process state */
	long uid; /* User ID of process owner */
	long nvcsw; /* # voluntary context switches */
	long nivcsw; /* # involuntary context switches */
	long num_children; /* # children process has */
	long num_siblings; /* # sibling process has */
};
SYSCALL_DEFINE3(process_ancestors, /* syscall name for macro */
		struct process_info *,
		info_array, /* array of process info strct */
		long, size, /* size of the array */
		long *, num_filled) /* # elements written to array */
{
	// your code here…
    if(size <= 0){
        return -EINVAL;

    }

	long pid; /* Process ID */
	//char name[ANCESTOR_NAME_LEN]; /* Program name of process */
	long state; /* Current process state */
	long uid; /* User ID of process owner */
	long nvcsw; /* # voluntary context switches */
	long nivcsw; /* # involuntary context switches */
	long num_children; /* # children process has */
	long num_siblings; /* # sibling process has */
	int i = 0;

	struct task_struct *curr_task = current;
	//struct task_struct *startingPoint = current;
	struct list_head *headOfChildList = &(curr_task->children);
	struct list_head *childernLoop = &(curr_task->children);
	struct list_head *headOfSibList = &(curr_task->sibling);
	struct list_head *siblingsLoop = &(curr_task->sibling);
	int check = 0;
	pid = curr_task->pid;
	state = curr_task->state;
	nvcsw = curr_task->nvcsw;
	nivcsw = curr_task->nivcsw;
    uid = curr_task->cred->uid.val;
	num_children = 0;
	num_siblings = 0;

	for (i = 0; i < size; i++) {
		//copy pid
		if (copy_to_user(&info_array[i].pid, &pid, sizeof(long)) != 0) {
			return -EFAULT;
		}

		//copy the process name
		if (copy_to_user(&info_array[i].name, &curr_task->comm,
				 sizeof(char) * 16) != 0) {
			return -EFAULT;
		}

		//copy the state of the process
		if (copy_to_user(&info_array[i].state, &state, sizeof(long)) !=
		    0) {
			return -EFAULT;
		}

		//copy the nvcsw of the process
		if (copy_to_user(&info_array[i].nvcsw, &nvcsw, sizeof(long)) !=
		    0) {
			return -EFAULT;
		}

		//copy the nivcsw of the process
		if (copy_to_user(&info_array[i].nivcsw, &nivcsw,
				 sizeof(long)) != 0) {
			return -EFAULT;
		}


		//finding the number of children
		while (childernLoop->next != headOfChildList) {
			num_children++;
			childernLoop = childernLoop->next;
		}

		if (copy_to_user(&info_array[i].num_children, &num_children,
				 sizeof(long)) != 0) {
			return -EFAULT;
		}

		//reset check
		check = 0;

		//get the number of siblings
		// if (siblingsLoop->next != headOfSibList) {
		// 	num_siblings++;
		// }
		// siblingsLoop = siblingsLoop->next;

		while (siblingsLoop->next != headOfSibList) {
			num_siblings++;
			siblingsLoop = siblingsLoop->next;
		}

		if (copy_to_user(&info_array[i].num_siblings, &num_siblings,
				 sizeof(long)) != 0) {
			return -EFAULT;
		}

		//copy uid
		if (copy_to_user(&info_array[i].uid, &uid, sizeof(long)) !=
		    0) {
			return -EFAULT;
		}

		//need to fix this
		if (pid == 0) {
			i++;
			if (copy_to_user(num_filled, &i, sizeof(int)) != 0) {
				return -EFAULT;
			}
			//printk("Number of cycles: %d\n", i);
			return 0;
		}

		curr_task = curr_task->parent;
		pid = curr_task->pid;
		state = curr_task->state;
		nvcsw = curr_task->nvcsw;
		nivcsw = curr_task->nivcsw;
		num_children = 0;
		headOfChildList = &(curr_task->children);
		childernLoop = &(curr_task->children);
		headOfSibList = &(curr_task->sibling);
		siblingsLoop = &(curr_task->sibling);
		num_siblings = 0;
		check = 0;
	}

	// printk("pid is: %d\n", current->pid);
	// pid = current->pid;
	// if (copy_to_user(&info_array[0].pid, &pid, sizeof(long)) !=
	//     0) {
	// 	return -EFAULT;
	// }

	// if (copy_to_user(&info_array[0].name, &current->comm, sizeof(char)*16) !=
	//     0) {
	// 	return -EFAULT;
	// }
	//     struct task_struct *curr_task = current->parent;
	//     pid = curr_task->pid;

	// 	if (copy_to_user(&info_array[1].pid, &pid, sizeof(long)) !=
	//     0) {
	// 	return -EFAULT;
	// }

	// if (copy_to_user(&info_array[1].name, &curr_task->comm, sizeof(char)*16) !=
	//     0) {
	// 	return -EFAULT;
	// }

	return 0;
}